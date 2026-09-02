// ============================================================
// LESSON 2 — EXERCISES
// Run: g++ -std=c++20 -Wall AP/lessons/02_exercise.cpp -o /tmp/e2 && /tmp/e2
// ============================================================

#include <iostream>
#include <string>

int main() {
    // --- Exercise 1: if / else if -----------------------------------
    // Given temp, print:
    //   > 30        -> "hot"
    //   20 to 30    -> "warm"
    //   10 to 19    -> "cool"
    //   below 10    -> "cold"
    int temp = 24;
    // TODO

    // --- Exercise 2: ternary ----------------------------------------
    // Using the ternary operator, print "even" or "odd" for this number.
    int num = 17;
    // TODO

    // --- Exercise 3: while loop -------------------------------------
    // Print the powers of 2 that are <= 1000 (1 2 4 8 ... 512).
    // TODO

    // --- Exercise 4: for loop + accumulator -------------------------
    // Sum every number from 1 to 100 and print the total.
    // Then print the sum of only the EVEN numbers from 1 to 100.
    // TODO

    // --- Exercise 5: switch -----------------------------------------
    // Given a char op ('+', '-', '*', '/') and two ints x and y,
    // use a switch to print the result of that operation.
    // Careful: handle division by zero, and remember int division truncates.
    char op = '*';
    int x = 12, y = 5;
    // TODO

    // --- Exercise 6: nested loops -----------------------------------
    // Print this triangle using nested loops (5 rows):
    // *
    // **
    // ***
    // ****
    // *****
    // TODO

    // --- Exercise 7: break / continue -------------------------------
    // Loop from 1 to 50. Skip multiples of 3. Stop entirely once the
    // running total exceeds 100. Print the total and where you stopped.
    // TODO

    // --- Exercise 8 (challenge): is it prime? -----------------------
    // Decide whether `candidate` is prime and print the answer.
    // Hint: loop i from 2 while i * i <= candidate; if candidate % i == 0
    //       it's not prime, so break out with a flag.
    int candidate = 97;
    // TODO

    // --- Bonus: spot the bugs ---------------------------------------
    // Each of these compiles but is WRONG. Predict the behaviour, then try it.
    //
    // int k = 5;
    // if (k = 3) std::cout << "k is three\n";        // why does this always run?
    //
    // for (int i = 0; i < 5; i++);                    // note the trailing semicolon
    //     std::cout << "loop body?\n";
    //
    // int j = 0; while (j < 5) { std::cout << j; }    // what happens here?

    return 0;
}
