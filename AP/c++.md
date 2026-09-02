# C++ — quick grasp

## 1. Create & run a file

You already have Apple's compiler (`g++` / `clang++` — on macOS `g++` *is* clang). No install needed.

**Create** `hello.cpp`:

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, C++!" << std::endl;
    return 0;
}
```

**Run it** (two steps — C++ compiles to a binary first, unlike Python):

```bash
g++ -std=c++20 -Wall hello.cpp -o hello   # compile -> creates ./hello
./hello                                    # run
```

One-liner while iterating:

```bash
g++ -std=c++20 -Wall hello.cpp -o /tmp/hello && /tmp/hello
```

Flags worth knowing:

| Flag | Meaning |
|---|---|
| `-o NAME` | name of the output binary (default is `a.out`) |
| `-std=c++20` | which language version — always set it |
| `-Wall -Wextra` | turn on warnings; fix them, they're real bugs |
| `-g` | include debug info (for `lldb` / debugger) |
| `-O2` | optimize (for real builds, not while debugging) |
| `-fsanitize=address` | catch memory bugs at runtime (great while learning) |

Multiple files: list them all — `g++ -std=c++20 main.cpp utils.cpp -o app`.
In VS Code: `Cmd+Shift+B` to build, or just run the compile command in the terminal.

Working examples in this folder: [hello.cpp](hello.cpp) and [tour.cpp](tour.cpp) (a commented tour of everything below).

```bash
g++ -std=c++20 -Wall AP/tour.cpp -o /tmp/tour && /tmp/tour
```

---

## 2. Anatomy of a program

```cpp
#include <iostream>   // pull in a library (here: input/output)

int main() {          // execution starts here, exactly one main per program
    ...
    return 0;         // 0 = success, non-zero = error
}
```

- `std::` is the *namespace* of the standard library. `std::cout`, `std::string`, `std::vector`.
- Every statement ends with `;`.
- `#include <...>` for standard headers, `#include "myfile.h"` for your own.

---

## 3. The core concepts

### Types are fixed and checked at compile time

```cpp
int    n = 42;
double d = 2.5;
bool   ok = true;
char   c = 'a';
std::string s = "text";     // not char*, use std::string
const int kMax = 100;       // can't change
auto   x = 3.14;            // compiler infers the type — still static, just inferred
```

### Value vs reference vs pointer — *the* C++ concept

```cpp
void byValue(int n)        { n = 99; }   // gets a COPY, caller unaffected
void byRef(int& n)         { n = 99; }   // alias to the caller's variable — edits it
void byConstRef(const std::string& s);   // no copy, can't edit — default for big objects
void byPointer(int* p)     { *p = 99; }  // address; can be null; deref with *
```

Rule of thumb: pass small things (`int`, `double`) by value, everything else by `const&`, use `&` when you intend to modify.

### Control flow

```cpp
if (a > b) { ... } else if (...) { ... } else { ... }
while (cond) { ... }
for (int i = 0; i < 10; i++) { ... }
for (int x : vec) { ... }              // range-based for — prefer this
switch (v) { case 1: ...; break; default: ...; }
```

### Structs & classes

Same thing, different default access: `struct` = public (plain data), `class` = private (data + behavior).

```cpp
struct Point { double x = 0, y = 0; };

class Counter {
public:
    Counter(std::string name) : name_(std::move(name)) {}  // ctor + member init list
    void bump() { count_++; }
    int count() const { return count_; }   // const = this method won't mutate the object
private:
    std::string name_;
    int count_ = 0;                        // default member initializer
};
```

- Trailing `_` on members is a common convention.
- `const` on a method is a promise, enforced by the compiler. Mark every non-mutating method `const`.

### Inheritance & polymorphism

```cpp
class Shape {
public:
    virtual ~Shape() = default;         // ALWAYS virtual dtor on a base class
    virtual double area() const = 0;    // = 0 -> pure virtual -> Shape is abstract
};

class Circle : public Shape {
public:
    double area() const override { return 3.14159 * r_ * r_; }   // `override` = checked
private:
    double r_;
};
```

`virtual` means "dispatch on the real runtime type". Without it, calling through a `Shape*` would run `Shape`'s version. `override` makes the compiler verify you actually overrode something.

### Memory & RAII — the mental model that matters

C++ has no garbage collector. Instead:

- **Stack objects** (`Point p;`) are destroyed automatically when the scope ends.
- **RAII**: an object's destructor releases whatever it holds (memory, file, lock). So "cleanup" = "let it go out of scope".
- **Never write `new`/`delete` by hand.** Use smart pointers:

```cpp
auto p = std::make_unique<Circle>(1.0);   // sole owner, freed automatically
auto q = std::make_shared<Circle>(1.0);   // shared ownership, ref-counted
Circle* raw = p.get();                    // non-owning view, don't delete it
```

`unique_ptr` is the default. Reach for `shared_ptr` only when ownership is genuinely shared.

### The standard library does the work

```cpp
std::vector<int> v{3, 1, 2};        // dynamic array — your default container
v.push_back(4);  v.size();  v[0];  v.at(0);   // .at() bounds-checks

std::map<std::string, int> m;        // sorted key->value
m["a"] = 1;
std::unordered_map<std::string,int> um;   // hash map, faster lookup

#include <algorithm>
std::sort(v.begin(), v.end());
std::find(v.begin(), v.end(), 2);
auto it = std::max_element(v.begin(), v.end());

for (const auto& [key, value] : m) { ... }   // structured binding (C++17)
```

Prefer `std::vector` / `std::string` over C arrays and `char*`, always.

### Templates (generics)

```cpp
template <typename T>
T largest(const std::vector<T>& v) { ... }

largest(nums);                        // T deduced as int
largest<std::string>({"a", "b"});     // T given explicitly
```

Templates are compiled per-type, so they're zero-cost at runtime. This is why they live in headers.

### Lambdas

```cpp
auto add = [](int x, int y) { return x + y; };
int offset = 10;
auto f = [offset](int x) { return x + offset; };   // [=] by value, [&] by reference
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });  // descending
```

### Headers vs source

```cpp
// utils.h  — declarations (the interface)
#pragma once
int square(int n);

// utils.cpp — definitions (the implementation)
#include "utils.h"
int square(int n) { return n * n; }
```

Compile together: `g++ -std=c++20 main.cpp utils.cpp -o app`. `#pragma once` prevents double inclusion.

### Errors

```cpp
#include <stdexcept>
if (n < 0) throw std::invalid_argument("n must be >= 0");

try { risky(); }
catch (const std::exception& e) { std::cerr << e.what() << "\n"; }
```

---

## 4. Coming from Python/JS — the mindset shifts

| Idea | What's different in C++ |
|---|---|
| Types | Declared and checked before the program runs |
| Running | Compile to a binary, then execute |
| Memory | You own object lifetimes; RAII + smart pointers do it for you |
| Assignment | `b = a` **copies** the object by default (not a reference) |
| Undefined behavior | Out-of-bounds / use-after-free doesn't throw — it silently corrupts. Use `-fsanitize=address` while learning |
| Performance | No runtime, no GC — abstractions compile away ("zero-cost") |

## 5. Practice loop

1. Edit `tour.cpp`, break something, read the compiler error — errors *are* the tutorial.
2. `g++ -std=c++20 -Wall -fsanitize=address AP/tour.cpp -o /tmp/tour && /tmp/tour`
3. Look things up on [cppreference.com](https://en.cppreference.com/w/cpp) (the authoritative reference).
