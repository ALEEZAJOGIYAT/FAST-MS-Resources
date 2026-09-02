// A one-file tour of the C++ concepts you actually use day to day.
// Build: g++ -std=c++20 -Wall AP/tour.cpp -o /tmp/tour && /tmp/tour

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

// ---------- 1. Functions, parameters, references ----------

int square(int n) { return n * n; }          // by value: gets a copy

void shout(std::string& s) { s += "!"; }     // by reference: edits the caller's variable

int sum(const std::vector<int>& v) {         // const reference: no copy, no edits
    int total = 0;
    for (int x : v) total += x;               // range-based for loop
    return total;
}

// ---------- 2. Structs and classes ----------

struct Point {          // struct: members public by default, plain data
    double x = 0;
    double y = 0;
};

class Counter {         // class: members private by default, data + behavior
public:
    Counter(std::string name) : name_(std::move(name)) {}   // constructor + init list

    void bump() { count_++; }
    int count() const { return count_; }                   // const = doesn't mutate
    const std::string& name() const { return name_; }

private:
    std::string name_;
    int count_ = 0;
};

// ---------- 3. Inheritance and virtual functions (polymorphism) ----------

class Shape {
public:
    virtual ~Shape() = default;            // virtual destructor: required for base classes
    virtual double area() const = 0;       // = 0 makes this abstract (pure virtual)
    virtual std::string kind() const { return "shape"; }
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159265 * r_ * r_; }
    std::string kind() const override { return "circle"; }
private:
    double r_;
};

class Rect : public Shape {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    std::string kind() const override { return "rect"; }
private:
    double w_, h_;
};

// ---------- 4. Templates (generics) ----------

template <typename T>
T largest(const std::vector<T>& v) {
    T best = v.at(0);
    for (const T& x : v) if (x > best) best = x;
    return best;
}

int main() {
    // --- basics ---
    int a = 7;                    // fixed type, checked at compile time
    auto b = 2.5;                 // auto = compiler infers the type (double)
    const int kMax = 100;         // const = cannot change
    std::string name = "world";

    std::cout << "square(" << a << ") = " << square(a) << "\n";
    std::cout << "a + b = " << a + b << ", kMax = " << kMax << "\n";

    // --- control flow ---
    if (a % 2 == 0) std::cout << a << " is even\n";
    else            std::cout << a << " is odd\n";

    for (int i = 0; i < 3; i++) std::cout << "i=" << i << " ";
    std::cout << "\n";

    // --- references ---
    shout(name);                                  // modifies name in place
    std::cout << "name is now: " << name << "\n";

    // --- containers (the standard library does the heavy lifting) ---
    std::vector<int> nums = {5, 3, 9, 1};         // dynamic array
    nums.push_back(7);
    std::sort(nums.begin(), nums.end());          // <algorithm>
    std::cout << "sorted:";
    for (int n : nums) std::cout << " " << n;
    std::cout << "  (sum=" << sum(nums) << ")\n";

    std::map<std::string, int> ages{{"ann", 31}, {"bo", 24}};   // key -> value
    ages["cy"] = 45;
    for (const auto& [key, value] : ages)                        // structured binding
        std::cout << key << "=" << value << " ";
    std::cout << "\n";

    // --- structs and classes ---
    Point p{1.0, 2.0};
    std::cout << "point: (" << p.x << ", " << p.y << ")\n";

    Counter clicks("clicks");
    clicks.bump();
    clicks.bump();
    std::cout << clicks.name() << " = " << clicks.count() << "\n";

    // --- polymorphism through smart pointers ---
    // unique_ptr owns the object and frees it automatically (RAII, no manual delete)
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rect>(2.0, 3.0));
    for (const auto& s : shapes)
        std::cout << s->kind() << " area = " << s->area() << "\n";
    // no cleanup code needed: leaving main destroys the vector and its shapes

    // --- templates + lambdas ---
    std::cout << "largest int:    " << largest(nums) << "\n";
    std::cout << "largest string: " << largest<std::string>({"pear", "apple", "zebra"}) << "\n";

    auto add = [](int x, int y) { return x + y; };            // lambda = inline function
    int offset = 10;
    auto add_offset = [offset](int x) { return x + offset; }; // captures offset by value
    std::cout << "add(2,3)=" << add(2, 3) << " add_offset(5)=" << add_offset(5) << "\n";

    return 0;   // 0 means success
}
