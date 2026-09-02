// ============================================================
// LESSON 3 — EXERCISES
// Run: g++ -std=c++20 -Wall AP/lessons/03_exercise.cpp -o /tmp/e3 && /tmp/e3
// ============================================================

#include <iostream>
#include <string>
#include <vector>

// --- Exercise 1: write a function --------------------------------
// Write `bool isEven(int n)` that returns true when n is even.
// TODO


// --- Exercise 2: value vs reference ------------------------------
// Write TWO functions:
//   int   withValue(int n)   -> returns n + 10, leaves the caller's variable alone
//   void  withRef(int& n)    -> adds 10 to the caller's variable directly
// Then in main, prove the difference by printing before/after for each.
// TODO


// --- Exercise 3: const reference ---------------------------------
// Write `int longestLength(const std::vector<std::string>& words)` that returns
// the length of the longest string. Why is const& the right choice here?
// TODO


// --- Exercise 4: swap --------------------------------------------
// Write `void swapValues(int& a, int& b)` that exchanges the two values.
// You'll need a temporary variable. (std::swap already exists — write it yourself.)
// TODO


// --- Exercise 5: out-parameters ----------------------------------
// Write `void minMax(const std::vector<int>& v, int& outMin, int& outMax)`
// that finds both the smallest and largest value in one pass.
// TODO


// --- Exercise 6: default arguments -------------------------------
// Write `std::string repeat(const std::string& s, int times = 3)` that returns
// s concatenated `times` times. Call it both with and without the second argument.
// TODO


// --- Exercise 7: overloading -------------------------------------
// Write three overloads of `describe`:
//   describe(int)          -> "the number N"
//   describe(double)       -> "the decimal N"
//   describe(bool)         -> "the truth value true/false"
// Then call describe(5), describe(5.0), describe(true) and see which runs.
// Surprise ahead: try describe('c') and describe("text") and explain the result.
// TODO


// --- Exercise 8: recursion ---------------------------------------
// Write `int fib(int n)` returning the nth Fibonacci number
// (fib(0)=0, fib(1)=1, fib(n)=fib(n-1)+fib(n-2)).
// Print fib(0) through fib(15).
// Then: why does fib(45) take forever? (Answer involves how many calls happen.)
// TODO


// --- Exercise 9 (challenge): a reference return ------------------
// Write `int& elementAt(std::vector<int>& v, int index)` returning a reference
// to that element, so the caller can write:  elementAt(v, 2) = 99;
// TODO


int main() {
    std::cout << "fill in the exercises above and call them here\n";

    // --- Bonus: find the bug ------------------------------------
    // This function looks fine and is badly broken. Explain why,
    // then compile with -fsanitize=address and run it to see.
    //
    // int& brokenMax(int a, int b) {
    //     int result = (a > b) ? a : b;
    //     return result;
    // }
    //
    // Hint: what happens to `result` at the closing brace?

    return 0;
}
