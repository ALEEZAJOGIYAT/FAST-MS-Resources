// ============================================================
// LESSON 1 — EXERCISES
// Fill in the TODOs, then run:
//   g++ -std=c++20 -Wall AP/lessons/01_exercise.cpp -o /tmp/e1 && /tmp/e1
// ============================================================

#include <iostream>
#include <string>

int main() {
    // --- Exercise 1: declare variables -------------------------------
    // Declare: an int called year set to 2026,
    //          a double called pi set to 3.14159,
    //          a std::string called lang set to "C++",
    //          a bool called learning set to true.
    // Then print them all on one line separated by spaces.

    // TODO

    // --- Exercise 2: the division trap -------------------------------
    // You have 7 pizzas and 2 people.
    // Print how many WHOLE pizzas each person gets (integer division),
    // and how many are left over (modulo),
    // and the exact decimal amount each person gets (make it a double!).

    int pizzas = 7;
    int people = 2;
    // TODO

    // --- Exercise 3: a const and a calculation -----------------------
    // Make a const double kVat = 0.20.
    // Given a price of 250.0, print the tax amount and the total.

    // TODO

    // --- Exercise 4: strings -----------------------------------------
    // Build a full name from these two, print it, print its length,
    // and print its first and last characters.
    // Hint for the last char: name[name.length() - 1]

    std::string first = "Grace";
    std::string last  = "Hopper";
    // TODO

    // --- Exercise 5: input ------------------------------------------
    // Ask the user for their favourite number as an int, then print
    // that number doubled, and whether it is even (use % 2).

    // TODO

    // --- Bonus: break it on purpose ---------------------------------
    // Uncomment each line one at a time, compile, and read the error.
    // Learning to read compiler errors is 50% of learning C++.
    //
    // int a = "hello";
    // const int b = 1; b = 2;
    // std::cout << undefinedVariable;
    // int c = 5   // missing semicolon

    return 0;
}
