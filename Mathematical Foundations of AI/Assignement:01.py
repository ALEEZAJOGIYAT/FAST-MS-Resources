"""
Assignment 1 - Q4
Student ID: 26K-7618
Binary vectors over F2 (addition mod 2)
"""

import itertools
import numpy as np

# Given vectors
v1 = np.array([1, 0, 1, 1, 0])
v2 = np.array([0, 1, 1, 0, 1])
v3 = np.array([1, 1, 0, 1, 1])

def add_mod2(a, b):
    return (a + b) % 2

# a) v1 + v2 over F2
sum_v1_v2 = add_mod2(v1, v2)
print("a) v1 + v2 =", sum_v1_v2)

# b) Check linear independence over F2
# Build matrix and check rank over GF(2) using Gaussian elimination mod 2
def rank_mod2(matrix):
    M = matrix.copy() % 2
    rows, cols = M.shape
    rank = 0
    for col in range(cols):
        pivot_row = None
        for r in range(rank, rows):
            if M[r, col] == 1:
                pivot_row = r
                break
        if pivot_row is None:
            continue
        M[[rank, pivot_row]] = M[[pivot_row, rank]]
        for r in range(rows):
            if r != rank and M[r, col] == 1:
                M[r] = add_mod2(M[r], M[rank])
        rank += 1
        if rank == rows:
            break
    return rank

M = np.array([v1, v2, v3])
r = rank_mod2(M)
print(f"b) Rank of [v1, v2, v3] over F2 = {r}")
if r < 3:
    print("   => v1, v2, v3 are LINEARLY DEPENDENT over F2")
    print(f"   Check: v1 + v2 + v3 = {add_mod2(add_mod2(v1, v2), v3)} (should be all zeros)")
else:
    print("   => v1, v2, v3 are linearly independent over F2")

# c) Generate all vectors in the span of v1, v2, v3 over F2
vectors = [v1, v2, v3]
span_set = set()
n = len(vectors)
for coeffs in itertools.product([0, 1], repeat=n):
    combo = np.zeros(len(v1), dtype=int)
    for c, v in zip(coeffs, vectors):
        if c == 1:
            combo = add_mod2(combo, v)
    span_set.add(tuple(combo))

print(f"\nc) All vectors in span(v1, v2, v3) [{len(span_set)} total, duplicates removed]:")
for vec in sorted(span_set):
    print("   ", tuple(int(x) for x in vec))

# d) Dimension of the resulting subspace
dimension = r  # rank = dimension of span
print(f"\nd) Dimension of the subspace = {dimension}")
print(f"   (Confirmed: |span| = 2^dim = 2^{dimension} = {2**dimension}, "
      f"matches {len(span_set)} unique vectors found)")

# e) Explanation (printed as comment/output)
explanation = """
e) Relevance to error detection/correction in data transmission:
   Binary vectors over F2 form the basis of linear (parity-check) codes used
   in communication systems. A set of independent codewords (here, a basis
   of dimension {dim}) spans a subspace of valid transmitted messages. Since
   v3 = v1 + v2 (linearly dependent), v3 does not add new information/redundancy
   beyond v1 and v2 - it is a codeword already reachable via linear combination.
   In real coding schemes, such redundancy is used deliberately: extra
   (redundant) bits are added so that if noise flips some bits during
   transmission, the received vector will fall OUTSIDE the valid subspace,
   allowing the receiver to detect an error. With enough redundancy (a
   sufficiently large code distance), the receiver can even identify the
   nearest valid codeword and correct the error, which is exactly how
   Hamming codes and other linear error-correcting codes work.
""".format(dim=dimension)
print(explanation)