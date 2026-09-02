// ============================================================
// LESSON 3 — Functions, scope, and value vs reference
// Run: g++ -std=c++20 -Wall AP/lessons/03_functions.cpp -o /tmp/l3 && /tmp/l3
// ============================================================


// functions is all about what and why?
// we have return statements, parameter and name of function 
// don't repeat urself, to tackle repetition, 



#include <iostream>
#include <string>
#include <vector>

// ---------- 1. Anatomy of a function ----------
//
//  returnType  name  (parameters)
//
int add(int a, int b) {
    return a + b;
}

// `void` = returns nothing. A bare `return;` (or falling off the end) exits.
void greet(std::string who) {
    if (who.empty()) return;              // early return: a guard clause
    std::cout << "Hello, " << who << "!\n";
}

// A function must be DECLARED before it is used. Two ways:
//   (a) define it above main   (what we're doing here)
//   (b) declare a prototype up top, define it lower down:
double average(const std::vector<int>& v);     // <- prototype (no body, ends with ;)

// ---------- 2. PASS BY VALUE — the function gets a copy ----------
void tryToDouble(int n) {
    n = n * 2;                 // modifies the LOCAL copy only
    std::cout << "  inside tryToDouble, n = " << n << "\n";
}

// ---------- 3. PASS BY REFERENCE (&) — the function gets the caller's variable ----------
void reallyDouble(int& n) {    // `int&` = "reference to an int"
    n = n * 2;                 // modifies the caller's variable
}

void appendBang(std::string& s) {
    s += "!";
}

// ---------- 4. PASS BY CONST REFERENCE — no copy, but read-only ----------
// This is the DEFAULT for anything bigger than a couple of bytes.
double average(const std::vector<int>& v) {     // definition of the prototype above
    if (v.empty()) return 0.0;
    int total = 0;
    for (int x : v) total += x;
    return static_cast<double>(total) / v.size();   // cast so it isn't int division
}

// A reference can also be a RETURN type — but only to something that outlives the call.
int& biggest(int& a, int& b) {
    return (a > b) ? a : b;    // returns an alias to whichever is larger
}
// NEVER do this:  int& bad() { int x = 5; return x; }   // x dies at the closing brace
// That's a dangling reference — undefined behaviour, not a compile error.

// ---------- 5. Multiple return values via out-parameters ----------
// (Lesson 9 shows the nicer way: return a struct or std::pair.)
void divide(int a, int b, int& quotient, int& remainder) {
    quotient  = a / b;
    remainder = a % b;
}

// ---------- 6. Default arguments ----------
// Defaults go on the DECLARATION, and must be the trailing parameters.
void logMsg(const std::string& msg, const std::string& level = "INFO") {
    std::cout << "  [" << level << "] " << msg << "\n";
}

// ---------- 7. Overloading — same name, different parameter types ----------
// The compiler picks by the arguments you pass. (Return type alone can't overload.)
int    area(int side)               { return side * side; }
double area(double w, double h)     { return w * h; }
std::string area(const std::string& shape) { return "no idea about a " + shape; }

// ---------- 8. Recursion — a function calling itself ----------
long long factorial(int n) {
    if (n <= 1) return 1;              // base case — without one you get a stack overflow
    return n * factorial(n - 1);       // recursive case, moves toward the base
}

// ---------- 9. constexpr — computed at COMPILE time when it can be ----------
constexpr int cube(int n) { return n * n * n; }

int main() {
    // ---------- calling functions ----------
    std::cout << "-- basics --\n";
    std::cout << "add(2, 3) = " << add(2, 3) << "\n";
    greet("Aleeza");
    greet("");                                     // guard clause returns early

    // ---------- value vs reference: the key demo ----------
    std::cout << "\n-- value vs reference --\n";
    int x = 10;
    std::cout << "before: x = " << x << "\n";
    tryToDouble(x);
    std::cout << "after tryToDouble:  x = " << x << "   <-- unchanged (a copy was made)\n";
    reallyDouble(x);
    std::cout << "after reallyDouble: x = " << x << "   <-- changed (reference)\n";

    std::string s = "hi";
    appendBang(s);
    std::cout << "appendBang: " << s << "\n";

    // ---------- const reference: no copy, no modification ----------
    std::cout << "\n-- const& --\n";
    std::vector<int> nums{4, 8, 15, 16, 23, 42};
    std::cout << "average = " << average(nums) << "\n";
    // `nums` has 6 elements here; if average took it BY VALUE the whole vector
    // would be copied on every call. With a million elements that's a real cost.

    // ---------- returning a reference ----------
    int p = 3, q = 9;
    biggest(p, q) = 100;                  // assigning THROUGH the returned reference
    std::cout << "p = " << p << ", q = " << q << "   <-- q was the bigger one\n";

    // ---------- out-parameters ----------
    std::cout << "\n-- out params --\n";
    int quo = 0, rem = 0;
    divide(17, 5, quo, rem);
    std::cout << "17 / 5 = " << quo << " remainder " << rem << "\n";

    // ---------- defaults & overloads ----------
    std::cout << "\n-- defaults & overloads --\n";
    logMsg("starting up");                // uses level = "INFO"
    logMsg("disk almost full", "WARN");
    std::cout << "area(4)        = " << area(4) << "\n";          // int version
    std::cout << "area(2.5, 4.0) = " << area(2.5, 4.0) << "\n";   // double version
    std::cout << "area(\"blob\")   = " << area(std::string("blob")) << "\n";

    // ---------- recursion ----------
    std::cout << "\n-- recursion --\n";
    for (int i = 1; i <= 10; i++) std::cout << factorial(i) << " ";
    std::cout << "\n";

    // ---------- constexpr ----------
    constexpr int kVolume = cube(3);      // computed during compilation, costs 0 at runtime
    int runtimeSide = 4;
    std::cout << "\ncube(3) at compile time = " << kVolume
              << ", cube(runtimeSide) = " << cube(runtimeSide) << "\n";

    // ---------- scope & shadowing ----------
    std::cout << "\n-- scope --\n";
    int value = 1;
    {
        int value = 2;    // SHADOWS the outer one — legal, and a common source of bugs
        std::cout << "inner value = " << value << "\n";
    }
    std::cout << "outer value = " << value << "\n";
    // Compile with -Wshadow to be warned about this:
    //   g++ -std=c++20 -Wall -Wshadow AP/lessons/03_functions.cpp

    return 0;
}

