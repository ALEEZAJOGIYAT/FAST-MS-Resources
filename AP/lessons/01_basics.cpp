// ============================================================
// LESSON 1 — Output, variables, types, input
// Run: g++ -std=c++20 -Wall AP/lessons/01_basics.cpp -o /tmp/l1 && /tmp/l1
// ============================================================

#include <iostream>   // std::cout, std::cin  — input/output
#include <string>     // std::string

int main() {
    // ---------- 1. Printing ----------
    std::cout << "Hello!" << "\n";          // << chains: send each piece to output
    std::cout << "A" << 1 << 2.5 << '\n';   // any type works, no format specifiers
    std::cout << "no newline here...";
    std::cout << " ...same line\n";
    // "\n" is a newline. std::endl also flushes the buffer — slower, rarely needed.

    // ---------- 2. Variables: type NAME = VALUE; ----------
    int         count   = 42;          // whole number
    double      price   = 19.99;       // decimal (use double, not float)
    bool        active  = true;        // true / false
    char        grade   = 'A';         // ONE character, single quotes
    std::string name    = "Aleeza";    // text, double quotes

    std::cout << "\n-- variables --\n";
    std::cout << count << " " << price << " " << active << " "
              << grade << " " << name << "\n";
    // note: a bool prints as 1/0, not true/false

    // ---------- 3. The type is fixed forever ----------
    count = 100;        // fine — new int value
    // count = "text";  // ERROR: uncomment this and read what the compiler says.
    // This is the big difference from Python: types are checked before it runs.

    // ---------- 4. auto — let the compiler figure out the type ----------
    auto n = 10;        // int
    auto d = 2.5;       // double
    auto s = std::string("hi");   // std::string
    // auto t = "hi";   // careful: this is a C-string (const char*), NOT std::string
    std::cout << "\n-- auto --\n" << n << " " << d << " " << s << "\n";

    // ---------- 5. const — a value that cannot change ----------
    const double kTaxRate = 0.15;   // convention: k prefix for constants
    // kTaxRate = 0.2;              // ERROR — that's the point. Use const by default.
    std::cout << "tax on " << price << " = " << price * kTaxRate << "\n";

    // ---------- 6. Arithmetic, and the classic integer-division trap ----------
    std::cout << "\n-- arithmetic --\n";
    std::cout << "7 + 2  = " << 7 + 2 << "\n";
    std::cout << "7 / 2  = " << 7 / 2 << "   <-- 3, NOT 3.5! int / int = int\n";
    std::cout << "7.0/2  = " << 7.0 / 2 << "   <-- one double makes it a double\n";
    std::cout << "7 % 2  = " << 7 % 2 << "   <-- remainder (modulo)\n";

    int x = 5;
    x += 3;    std::cout << "x += 3  -> " << x << "\n";   // 8
    x++;       std::cout << "x++     -> " << x << "\n";   // 9

    // ---------- 7. Strings ----------
    std::cout << "\n-- strings --\n";
    std::string first = "Ada";
    std::string last  = "Lovelace";
    std::string full  = first + " " + last;        // + concatenates
    std::cout << full << "\n";
    std::cout << "length: " << full.length() << "\n";
    std::cout << "first char: " << full[0] << "\n";        // 0-indexed
    std::cout << "uppercase A? " << (full[0] == 'A') << "\n";

    // ---------- 8. Reading input ----------
    // std::cin >> stops at whitespace; std::getline reads a whole line.
    std::cout << "\n-- input --\n";
    std::cout << "Type your name and press enter: ";
    std::string who;
    std::getline(std::cin, who);
    if (who.empty()) who = "stranger";           // handles piped/empty input
    std::cout << "Nice to meet you, " << who << "!\n";

    // For a number:
    //   int age;  std::cin >> age;
    // If the user types letters, cin "fails" and age is left at 0 — real programs check:
    //   if (!(std::cin >> age)) { /* handle bad input */ }

    return 0;   // 0 = success
}
