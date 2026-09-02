"""
WisdomNet implementation
=========================
Reproduces the method from:
  Tran, T.X. & Aygun, R.S. "WisdomNet: trustable machine learning toward
  error-free classification." Neural Computing and Applications 33,
  2719-2734 (2021). https://doi.org/10.1007/s00521-020-05147-4

Idea
----
A standard classifier N is trained normally and will always have some
misclassification error. WisdomNet W is built FROM the trained N by adding
one extra output neuron -- the "conjugate neuron" -- that learns to fire
"reject" (i.e. "I can't safely classify this") instead of forcing a
possibly-wrong decision. W is:
  1. an exact clone of N (Theorem 1: identical outputs at construction,
     since the conjugate neuron's weights/bias start at zero), then
  2. fine-tuned using ONLY the samples N misclassified during training,
     relabeled as class "reject".

The result: the decided predictions (class 0 / class 1) that W does make
have (ideally) 0% error, at the cost of a small "reject" rate on hard
samples that get deferred instead of guessed.

This script applies WisdomNet to a real binary classification problem
(the sklearn breast-cancer diagnostic dataset -- malignant vs. benign,
which mirrors the "misclassification is costly" medical scenario the
paper itself motivates with) using an MLP base network in PyTorch.
"""

import copy

import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
from sklearn.datasets import load_breast_cancer
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

SEED = 0
torch.manual_seed(SEED)
np.random.seed(SEED)


# ---------------------------------------------------------------------------
# 1. Base network N: a standard MLP binary classifier
# ---------------------------------------------------------------------------
class BaseMLP(nn.Module):
    """Standard multilayer perceptron with 2 output neurons (one per class),
    matching Fig. 1a / Eq. (1) of the paper."""

    def __init__(self, n_input, hidden_sizes=(16, 8)):
        super().__init__()
        layers = []
        prev = n_input
        for h in hidden_sizes:
            layers.append(nn.Linear(prev, h))
            layers.append(nn.ReLU())
            prev = h
        self.hidden = nn.Sequential(*layers)
        self.output = nn.Linear(prev, 2)  # decided-0, decided-1

    def forward(self, x):
        return self.output(self.hidden(x))  # raw logits


# ---------------------------------------------------------------------------
# 2. WisdomNet W: N + a conjugate ("reject") neuron
# ---------------------------------------------------------------------------
class WisdomNet(nn.Module):
    """
    Built from a *trained* BaseMLP, following Sect. 3.3 / Table 1 of the
    paper:
      - keeps every neuron and weight inherited from the base network
      - adds one new "conjugate neuron" to the output layer, wired to the
        last hidden layer, with weight vector w^c = 0 and bias b^c = 0
    At construction time W is therefore mathematically identical to the
    base network (Theorem 1) -- the reject neuron only starts producing
    non-zero activations once fine-tuned in step 2.
    """

    def __init__(self, base: BaseMLP):
        super().__init__()
        # Deep-copy so the original base network is left untouched and can
        # still be evaluated separately for comparison.
        self.hidden = copy.deepcopy(base.hidden)

        last_hidden_size = base.output.in_features
        self.output = nn.Linear(last_hidden_size, 3)  # decided-0, decided-1, reject

        with torch.no_grad():
            self.output.weight[:2] = base.output.weight.clone()
            self.output.bias[:2] = base.output.bias.clone()
            self.output.weight[2] = 0.0  # conjugate neuron: w^c = 0
            self.output.bias[2] = 0.0  # conjugate neuron: b^c = 0

    def forward(self, x):
        return self.output(self.hidden(x))


REJECT_CLASS = 2


# ---------------------------------------------------------------------------
# 3. Training helpers
# ---------------------------------------------------------------------------
def train_base_network(model, X_train, y_train, epochs=120, lr=1e-3):
    optimizer = optim.Adam(model.parameters(), lr=lr)
    criterion = nn.CrossEntropyLoss()
    model.train()
    for _ in range(epochs):
        optimizer.zero_grad()
        loss = criterion(model(X_train), y_train)
        loss.backward()
        optimizer.step()
    return model


def build_reject_set(base_model, X_train, y_train):
    """Step 2 of the paper: run the trained base net on the training data
    and collect every sample it gets wrong -- these form S^r."""
    base_model.eval()
    with torch.no_grad():
        preds = base_model(X_train).argmax(dim=1)
    misclassified_mask = preds != y_train
    return X_train[misclassified_mask], misclassified_mask


def fine_tune_wisdomnet(wnet, X_reject, X_train_full, y_train_full, max_epochs=2000, lr=1e-2):
    """
    Step 4: train W using ONLY S^r, all relabeled as 'reject'.

    IMPORTANT implementation note not spelled out in the paper's Algorithm 1:
    S^r contains only POSITIVE "reject" examples -- there are no counter-
    examples telling the conjugate neuron "don't reject this one". Training
    to convergence on one-sided data like this is degenerate: the conjugate
    neuron's bias has nothing pulling it back down, so it drifts upward
    every epoch and the reject rate creeps toward 100% the longer you train
    (verified empirically -- with enough epochs this WILL reject everything,
    even samples the base network got right and confidently). Since reject
    rate grows ~monotonically with training time in this regime, the fix is
    early stopping: stop the instant the network reaches 0% error on the
    DECIDED training predictions (the paper's actual stated goal), which
    also gives the lowest reject rate that achieves it. We check after every
    epoch and stop at the first epoch satisfying that condition.
    """
    if X_reject.shape[0] == 0:
        return wnet, {"stopped_epoch": 0, "note": "no misclassified training samples; nothing to fine-tune"}

    optimizer = optim.SGD(wnet.parameters(), lr=lr)
    criterion = nn.CrossEntropyLoss()
    reject_labels = torch.full((X_reject.shape[0],), REJECT_CLASS, dtype=torch.long)

    best_state = copy.deepcopy(wnet.state_dict())
    best_reject_rate = 1.0

    for epoch in range(1, max_epochs + 1):
        wnet.train()
        optimizer.zero_grad()
        loss = criterion(wnet(X_reject), reject_labels)
        loss.backward()
        optimizer.step()

        metrics = evaluate_wisdomnet(wnet, X_train_full, y_train_full)
        if metrics["misclass_on_decided"] == 0.0:
            # first epoch to hit 0% decided-error also has the lowest reject
            # rate among all such epochs, since reject rate only grows from
            # here -- so stop immediately.
            best_state = copy.deepcopy(wnet.state_dict())
            best_reject_rate = metrics["reject_rate"]
            wnet.load_state_dict(best_state)
            return wnet, {"stopped_epoch": epoch, "train_reject_rate": best_reject_rate}

        # fallback if we never reach 0% error: keep the checkpoint with the
        # lowest reject rate seen so far (early epochs), rather than the
        # fully-collapsed final one.
        if metrics["reject_rate"] < best_reject_rate:
            best_reject_rate = metrics["reject_rate"]
            best_state = copy.deepcopy(wnet.state_dict())

    wnet.load_state_dict(best_state)
    return wnet, {
        "stopped_epoch": None,
        "train_reject_rate": best_reject_rate,
        "note": "did not reach 0% training error within max_epochs; returning lowest-reject-rate checkpoint",
    }


# ---------------------------------------------------------------------------
# 4. Evaluation
# ---------------------------------------------------------------------------
def evaluate_base(model, X, y):
    model.eval()
    with torch.no_grad():
        preds = model(X).argmax(dim=1)
    error_rate = (preds != y).float().mean().item()
    return {"accuracy": 1 - error_rate, "error_rate": error_rate}


def evaluate_wisdomnet(wnet, X, y):
    wnet.eval()
    with torch.no_grad():
        preds = wnet(X).argmax(dim=1)
    n_total = len(y)
    reject_mask = preds == REJECT_CLASS
    decided_mask = ~reject_mask
    n_decided = int(decided_mask.sum().item())
    n_wrong = int((preds[decided_mask] != y[decided_mask]).sum().item())
    return {
        "reject_rate": reject_mask.float().mean().item(),
        "n_decided": n_decided,
        "n_total": n_total,
        "misclass_on_decided": (n_wrong / n_decided) if n_decided else 0.0,
        "overall_error_rate": n_wrong / n_total,  # errors as a fraction of ALL samples
    }


# ---------------------------------------------------------------------------
# 5. End-to-end demo on a real binary classification dataset
# ---------------------------------------------------------------------------
def main():
    data = load_breast_cancer()
    X, y = data.data, data.target  # 0 = malignant, 1 = benign

    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.3, random_state=SEED, stratify=y
    )
    scaler = StandardScaler().fit(X_train)
    X_train = scaler.transform(X_train)
    X_test = scaler.transform(X_test)

    X_train_t = torch.tensor(X_train, dtype=torch.float32)
    y_train_t = torch.tensor(y_train, dtype=torch.long)
    X_test_t = torch.tensor(X_test, dtype=torch.float32)
    y_test_t = torch.tensor(y_test, dtype=torch.long)

    # --- Step 1: pre-train the base network N -----------------------------
    base = BaseMLP(n_input=X_train.shape[1])
    train_base_network(base, X_train_t, y_train_t)

    base_train_metrics = evaluate_base(base, X_train_t, y_train_t)
    base_test_metrics = evaluate_base(base, X_test_t, y_test_t)

    # --- Steps 2-3: build the reject set + WisdomNet -----------------------
    X_reject, _ = build_reject_set(base, X_train_t, y_train_t)
    wnet = WisdomNet(base)

    # --- Step 4: fine-tune WisdomNet on the reject set ----------------------
    _, fine_tune_info = fine_tune_wisdomnet(wnet, X_reject, X_train_t, y_train_t)

    wnet_train_metrics = evaluate_wisdomnet(wnet, X_train_t, y_train_t)
    wnet_test_metrics = evaluate_wisdomnet(wnet, X_test_t, y_test_t)

    print("=" * 60)
    print("Base network N (standard MLP)")
    print(f"  train accuracy: {base_train_metrics['accuracy']:.4f}")
    print(f"  test  accuracy: {base_test_metrics['accuracy']:.4f}")
    print(f"  # misclassified train samples -> reject set size: {X_reject.shape[0]}")
    if X_reject.shape[0] == 0:
        print("  (base net got 100% train accuracy -> nothing to fine-tune; "
              "reduce hidden_sizes/epochs to see WisdomNet actually do something)")
    print("=" * 60)
    print(f"Fine-tuning stopped: {fine_tune_info}")
    print("WisdomNet W (base + conjugate/reject neuron, fine-tuned)")
    print(f"  train: reject_rate={wnet_train_metrics['reject_rate']:.4f}, "
          f"error_on_decided={wnet_train_metrics['misclass_on_decided']:.4f}, "
          f"overall_error={wnet_train_metrics['overall_error_rate']:.4f}")
    print(f"  test : reject_rate={wnet_test_metrics['reject_rate']:.4f}, "
          f"error_on_decided={wnet_test_metrics['misclass_on_decided']:.4f}, "
          f"overall_error={wnet_test_metrics['overall_error_rate']:.4f}")
    print("=" * 60)


if __name__ == "__main__":
    main()