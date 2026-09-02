// ============================================================
// LESSON 2 — Operators & control flow
// Run: g++ -std=c++20 -Wall AP/lessons/02_control_flow.cpp -o /tmp/l2 && /tmp/l2
// ============================================================

#include <iostream>
#include <string>

int main() {
    // ---------- 1. Comparison operators -> they produce a bool ----------
    std::cout << "-- comparisons --\n";
    int a = 5, b = 8;
    std::cout << "a == b : " << (a == b) << "\n";   // equal      (TWO equals signs)
    std::cout << "a != b : " << (a != b) << "\n";   // not equal
    std::cout << "a <  b : " << (a <  b) << "\n";
    std::cout << "a >= b : " << (a >= b) << "\n";
    // `=` assigns, `==` compares. `if (a = b)` compiles and is almost always a bug.
    std::cout << std::boolalpha;                    // now bools print as true/false
    std::cout << "with boolalpha: " << (a < b) << "\n";
    std::cout << std::noboolalpha;

    // ---------- 2. Logical operators ----------
    std::cout << "\n-- logic --\n";
    bool inRange = (a > 0 && a < 10);    // AND — both must be true
    bool either  = (a > 100 || b > 5);   // OR  — at least one true
    bool nope    = !inRange;             // NOT — flips it
    std::cout << inRange << " " << either << " " << nope << "\n";

    // Short-circuiting: if the left side decides the answer, the right side is
    // never evaluated. This is a feature — it's how you guard a check:
    //   if (ptr != nullptr && ptr->value > 0)   // safe: never derefs a null ptr

    // ---------- 3. if / else if / else ----------
    std::cout << "\n-- if --\n";
    int score = 73;
    if (score >= 90) {
        std::cout << "grade A\n";
    } else if (score >= 80) {
        std::cout << "grade B\n";
    } else if (score >= 70) {
        std::cout << "grade C\n";
    } else {
        std::cout << "grade F\n";
    }
    // Only the FIRST matching branch runs. Order matters: if you tested
    // `score >= 70` first, a 95 would also print "grade C".

    // Braces are optional for a single statement, but always use them.
    // The classic bug:  if (x) doOne(); doTwo();   <- doTwo() ALWAYS runs.

    // ---------- 4. The ternary operator: a compact if/else that has a value ----------
    std::string verdict = (score >= 70) ? "pass" : "fail";
    std::cout << "verdict: " << verdict << "\n";
    // Great for picking a value. Don't nest them — that's what if/else is for.

    // ---------- 5. switch — for comparing one value against fixed cases ----------
    std::cout << "\n-- switch --\n";
    char cmd = 'b';
    switch (cmd) {
        case 'a':
            std::cout << "add\n";
            break;              // WITHOUT break, execution falls through to the next case
        case 'b':
        case 'B':               // deliberate fallthrough: both letters do the same thing
            std::cout << "back\n";
            break;
        case 'q':
            std::cout << "quit\n";
            break;
        default:                // like `else` — always include one
            std::cout << "unknown command\n";
            break;
    }
    // switch only works on integer-like values (int, char, enum) — not strings.

    // ---------- 6. while — repeat while a condition holds ----------
    std::cout << "\n-- while --\n";
    int n = 1;
    while (n <= 5) {
        std::cout << n << " ";
        n++;                    // FORGET THIS and you have an infinite loop
    }
    std::cout << "\n";

    // do-while runs the body at least once, then checks. Rare but occasionally right.
    int tries = 0;
    do {
        tries++;
    } while (tries < 3);
    std::cout << "tries = " << tries << "\n";

    // ---------- 7. for — when you know the shape of the iteration ----------
    std::cout << "\n-- for --\n";
    // for (initialise; condition; step)
    for (int i = 0; i < 5; i++) std::cout << i << " ";
    std::cout << "\n";

    for (int i = 10; i > 0; i -= 2) std::cout << i << " ";   // countdown by 2
    std::cout << "\n";

    // `i` only exists inside the loop — that's deliberate, keep scopes tight.

    // ---------- 8. break and continue ----------
    std::cout << "\n-- break / continue --\n";
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) continue;   // skip the rest of THIS iteration
        if (i > 7)      break;      // leave the loop entirely
        std::cout << i << " ";
    }
    std::cout << "\n";

    // ---------- 9. Nested loops ----------
    std::cout << "\n-- nested --\n";
    for (int row = 1; row <= 3; row++) {
        for (int col = 1; col <= 3; col++) {
            std::cout << row * col << "\t";
        }
        std::cout << "\n";
    }
    // Note: `break` only exits the INNER loop. There is no labelled break in C++;
    // use a flag, or extract the loops into a function and `return`.

    // ---------- 10. Scope: { } creates a new lifetime ----------
    std::cout << "\n-- scope --\n";
    int outer = 1;
    {
        int inner = 2;              // exists only inside these braces
        std::cout << outer + inner << "\n";
    }
    // std::cout << inner;          // ERROR: `inner` no longer exists
    std::cout << "outer still here: " << outer << "\n";
    // This is the foundation of C++ memory management — we'll build on it in Lesson 7.

    // ---------- 11. Putting it together: a small loop with logic ----------
    std::cout << "\n-- FizzBuzz 1..15 --\n";
    for (int i = 1; i <= 15; i++) {
        if      (i % 15 == 0) std::cout << "FizzBuzz ";
        else if (i % 3  == 0) std::cout << "Fizz ";
        else if (i % 5  == 0) std::cout << "Buzz ";
        else                  std::cout << i << " ";
    }
    std::cout << "\n";

    return 0;
}
