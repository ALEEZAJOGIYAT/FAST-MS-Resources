"""
WisdomNet implementation
=========================
Reproduces the method from:
  Tran, T.X. & Aygun, R.S. "WisdomNet: trustable machine learning toward
  error-free classification." Neural Computing and Applications 33,
  2719-2734 (2021). https://doi.org/10.1007/s00521-020-05147-4

Idea
----
A normally-trained classifier N always has some misclassification error.
WisdomNet W is built FROM the trained N by adding one extra output neuron --
the "conjugate neuron" -- that learns to fire "abstain" (i.e. "I can't read
this safely") instead of forcing a possibly-wrong decision. W is:
  1. an exact clone of N (Theorem 1: identical outputs at construction, since
     the conjugate neuron's weights and bias start at zero), then
  2. fine-tuned using ONLY the samples N got wrong during training, all
     relabeled as the "abstain" class.

The result: the digits W does commit to reading have (ideally) 0% error, at
the cost of abstaining on the hard ones.

Case study: handwritten digit OCR for automated mail / cheque sorting
---------------------------------------------------------------------
An automatic sorter reads handwritten digits off envelopes and cheques. A
misread digit is expensive -- a letter goes to the wrong city, or a payment is
made for the wrong amount -- while an *abstention* is cheap: the item is
simply diverted to a human operator. This is the textbook setting for a reject
option, so it is exactly the trade-off WisdomNet is designed to buy.

Data: sklearn's `load_digits` (1797 8x8 grayscale digits, 10 classes).
"""

import copy

import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
from sklearn.datasets import load_digits
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

SEED = 0
torch.manual_seed(SEED)
np.random.seed(SEED)

N_DIGITS = 10  # classes 0-9
ABSTAIN = N_DIGITS  # index of the conjugate ("send to a human") neuron


# ---------------------------------------------------------------------------
# 1. Base network N: the plain OCR reader
# ---------------------------------------------------------------------------
class DigitReader(nn.Module):
    """Standard MLP with one output neuron per digit (Fig. 1a / Eq. 1)."""

    def __init__(self, n_pixels, hidden_sizes=(32, 16)):
        super().__init__()
        layers = []
        prev = n_pixels
        for h in hidden_sizes:
            layers += [nn.Linear(prev, h), nn.ReLU()]
            prev = h
        self.hidden = nn.Sequential(*layers)
        self.output = nn.Linear(prev, N_DIGITS)

    def forward(self, x):
        return self.output(self.hidden(x))  # raw logits


# ---------------------------------------------------------------------------
# 2. WisdomNet W: the reader plus an "abstain" neuron
# ---------------------------------------------------------------------------
class CautiousReader(nn.Module):
    """
    Built from a *trained* DigitReader, following Sect. 3.3 / Table 1:
    every inherited weight is kept, and one conjugate neuron is added to the
    output layer with w^c = 0 and b^c = 0. So at construction W reads every
    digit exactly as N does (Theorem 1); the abstain neuron only comes alive
    once fine-tuned.
    """

    def __init__(self, reader: DigitReader):
        super().__init__()
        # Deep-copy so the original reader survives for side-by-side scoring.
        self.hidden = copy.deepcopy(reader.hidden)
        self.output = nn.Linear(reader.output.in_features, N_DIGITS + 1)

        with torch.no_grad():
            self.output.weight[:N_DIGITS] = reader.output.weight.clone()
            self.output.bias[:N_DIGITS] = reader.output.bias.clone()
            self.output.weight[ABSTAIN] = 0.0  # conjugate neuron: w^c = 0
            self.output.bias[ABSTAIN] = 0.0  # conjugate neuron: b^c = 0

    def forward(self, x):
        return self.output(self.hidden(x))


# ---------------------------------------------------------------------------
# 3. Scoring
# ---------------------------------------------------------------------------
def score_reader(reader, X, y):
    """Plain accuracy of the base network -- it reads every digit, no opt-out."""
    reader.eval()
    with torch.no_grad():
        preds = reader(X).argmax(dim=1)
    return {"accuracy": (preds == y).float().mean().item()}


def score_cautious(reader, X, y):
    """Split the predictions into 'read' vs 'sent to a human' and score both."""
    reader.eval()
    with torch.no_grad():
        preds = reader(X).argmax(dim=1)
    read = preds != ABSTAIN
    n_read = int(read.sum())
    n_wrong = int((preds[read] != y[read]).sum())
    return {
        "abstain_rate": 1 - n_read / len(y),
        "n_read": n_read,
        "n_total": len(y),
        "error_on_read": n_wrong / n_read if n_read else 0.0,
        "overall_error": n_wrong / len(y),
    }


# ---------------------------------------------------------------------------
# 4. Training
# ---------------------------------------------------------------------------
def train_reader(reader, X, y, epochs=300, lr=1e-3):
    """Step 1: train the base OCR network the ordinary way."""
    optimizer = optim.Adam(reader.parameters(), lr=lr)
    loss_fn = nn.CrossEntropyLoss()
    reader.train()
    for _ in range(epochs):
        optimizer.zero_grad()
        loss_fn(reader(X), y).backward()
        optimizer.step()
    return reader


def collect_misreads(reader, X, y):
    """Step 2: the digits the trained reader gets wrong -- the paper's S^r."""
    reader.eval()
    with torch.no_grad():
        preds = reader(X).argmax(dim=1)
    return X[preds != y]


def teach_abstention(cautious, X_hard, X_train, y_train, max_steps=2000, lr=1e-2):
    """
    Step 4: train W on S^r alone, every sample relabeled "abstain". Returns
    the step it stopped at, or None if it never reached 0% error.

    Implementation note not spelled out in the paper's Algorithm 1: S^r holds
    only POSITIVE abstain examples -- nothing tells the conjugate neuron
    "don't abstain here". Trained to convergence on such one-sided data the
    neuron's bias just drifts upward, and the abstain rate creeps toward 100%
    (verified empirically: given enough steps it abstains on everything,
    including digits the base net read correctly and confidently). Because the
    abstain rate grows ~monotonically here, early stopping is the fix: stop
    the moment there is 0% error among the digits W still commits to reading
    on the training set -- the paper's stated goal, and also the cheapest
    abstain rate that achieves it.
    """
    if len(X_hard) == 0:
        return 0

    optimizer = optim.SGD(cautious.parameters(), lr=lr)
    loss_fn = nn.CrossEntropyLoss()
    abstain_labels = torch.full((len(X_hard),), ABSTAIN, dtype=torch.long)

    # Fallback if 0% error is never reached: the checkpoint that abstained least.
    snapshot, best_rate = copy.deepcopy(cautious.state_dict()), 1.0

    for step in range(1, max_steps + 1):
        cautious.train()
        optimizer.zero_grad()
        loss_fn(cautious(X_hard), abstain_labels).backward()
        optimizer.step()

        report = score_cautious(cautious, X_train, y_train)
        if report["error_on_read"] == 0.0:
            return step
        if report["abstain_rate"] < best_rate:
            snapshot, best_rate = copy.deepcopy(cautious.state_dict()), report["abstain_rate"]

    cautious.load_state_dict(snapshot)
    return None


# ---------------------------------------------------------------------------
# 5. End-to-end demo: OCR with a human-review escape hatch
# ---------------------------------------------------------------------------
def load_mail_digits():
    """8x8 handwritten digit scans, flattened to 64 pixel features and scaled."""
    X, y = load_digits(return_X_y=True)
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.3, random_state=SEED, stratify=y
    )
    scaler = StandardScaler().fit(X_train)
    as_x = lambda a: torch.tensor(scaler.transform(a), dtype=torch.float32)
    as_y = lambda a: torch.tensor(a, dtype=torch.long)
    return as_x(X_train), as_y(y_train), as_x(X_test), as_y(y_test)


def main():
    X_train, y_train, X_test, y_test = load_mail_digits()

    # Step 1: the ordinary reader.
    reader = train_reader(DigitReader(n_pixels=X_train.shape[1]), X_train, y_train)
    train_acc = score_reader(reader, X_train, y_train)["accuracy"]
    test_acc = score_reader(reader, X_test, y_test)["accuracy"]

    # Steps 2-3: misread digits, then clone the reader with an abstain neuron.
    X_hard = collect_misreads(reader, X_train, y_train)
    cautious = CautiousReader(reader)

    # Step 4: teach it to abstain on those.
    stopped_at = teach_abstention(cautious, X_hard, X_train, y_train)

    train_report = score_cautious(cautious, X_train, y_train)
    test_report = score_cautious(cautious, X_test, y_test)

    print("=" * 68)
    print("Case study: handwritten digit OCR for automated mail sorting")
    print("-" * 68)
    print("Base reader N (plain MLP, reads every digit)")
    print(f"  train accuracy: {train_acc:.4f}")
    print(f"  test  accuracy: {test_acc:.4f}")
    print(f"  misread train digits -> abstain set S^r: {len(X_hard)}")
    if len(X_hard) == 0:
        print("  (100% train accuracy -> nothing to fine-tune on; shrink "
              "hidden_sizes or epochs to see WisdomNet do something)")
    print("-" * 68)
    print(f"WisdomNet W (N + conjugate neuron), fine-tuning stopped at step: {stopped_at}")
    for name, r in (("train", train_report), ("test ", test_report)):
        print(f"  {name}: sent to human={r['abstain_rate']:.4f}  "
              f"error among digits read={r['error_on_read']:.4f}  "
              f"overall error={r['overall_error']:.4f}  "
              f"({r['n_read']}/{r['n_total']} read automatically)")
    print("=" * 68)


if __name__ == "__main__":
    main()
