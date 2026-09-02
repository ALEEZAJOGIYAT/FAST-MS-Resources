# C++ Course — syllabus

How each lesson works:
1. Read the lesson file (`NN_topic.cpp`) — it's a commented, runnable program.
2. Run it, then **change things and break it** on purpose.
3. Do the exercise file (`NN_exercise.cpp`) — fill in the `TODO`s.
4. Say "next" and we move on.

Build & run any lesson:

```bash
g++ -std=c++20 -Wall AP/lessons/01_basics.cpp -o /tmp/l && /tmp/l
```

## Part 1 — Basics (move fast)

| # | Topic | Status |
|---|---|---|
| 01 | Output, variables, types, input | ✅ ready |
| 02 | Operators & control flow (`if`, loops, `switch`) | ✅ ready |
| 03 | Functions, scope, value vs reference | ✅ ready |
| 04 | Arrays, `std::vector`, `std::string` | |

## Part 2 — Core C++

| # | Topic | Status |
|---|---|---|
| 05 | Pointers, references, memory model | |
| 06 | Structs, classes, constructors, `const` | |
| 07 | RAII & smart pointers | |
| 08 | Inheritance, `virtual`, polymorphism | |

## Part 3 — Real C++

| # | Topic | Status |
|---|---|---|
| 09 | The standard library: containers & algorithms | |
| 10 | Templates & generic programming | |
| 11 | Lambdas, `std::function`, functional style | |
| 12 | Move semantics, copy vs move, rule of five | |
| 13 | Error handling: exceptions, `optional`, `expected` | |
| 14 | Multi-file projects, headers, build systems | |

## Part 4 — Advanced

| # | Topic | Status |
|---|---|---|
| 15 | `constexpr`, compile-time computation | |
| 16 | Concepts & template constraints (C++20) | |
| 17 | Concurrency: threads, mutexes, atomics | |
| 18 | Performance: cache, inlining, profiling, UB | |

Reference notes: [../c++.md](../c++.md) · Lookups: [cppreference.com](https://en.cppreference.com/w/cpp)
