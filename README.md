# BlazeScript

BlazeScript is a statically typed, compiled programming language written in C++. It has a handwritten frontend and uses LLVM for code generation.

## Prerequisites

Before building BlazeScript, make sure you have:

* **LLVM 22**
* **CMake** (3.20 or higher)
* A **C++17-compatible compiler** (GCC, Clang, or Apple Clang)

## Building

Clone the repository and build it using CMake:

```bash
git clone https://github.com/kashyap404/BlazeScript.git
cd BlazeScript
cmake -S . -B build
cmake --build build
```

The compiler will be available at:

```bash
./build/blazescript
```

## Usage

```bash
./build/blazescript [options] <file>
```

If no flag is provided, semantic analysis is run by default.

### Command-Line Flags

| Flag               | Description             |
| ------------------ | ----------------------- |
| `-t`, `--tokens`   | Print tokens            |
| `-ast`, `--ast`    | Print the AST           |
| `-s`, `--semantic` | Run semantic analysis   |
| `-ir`, `--ir`      | Generate LLVM IR        |
| `-o <file>`        | Write LLVM IR to a file |
| `-h`, `--help`     | Show the help menu      |

## Overview

BlazeScript has compiler that first reads the source code and breaks it down into smaller parts called **tokens**. The **parser** then uses these tokens to understand the structure of the program and builds an **Abstract Syntax Tree (AST)**. After that, **semantic analysis** checks the program for things like incorrect types, undeclared variables, and invalid function usage.

Once the program passes these checks, **code generation** begins. The compiler walks through the AST and uses the **LLVM backend** to generate **LLVM Intermediate Representation (IR)** for the program. This IR can then be further processed by LLVM's tools.

## Currently Supported Language Features

BlazeScript currently supports:

* Variable declarations using `let`
* Type annotations
* Functions and function calls
* Assignments
* `if` / `else`
* `while` loops
* `return` statements
* Arithmetic operators: `+`, `-`, `*`, `/`, `%`
* Comparison operators: `==`, `!=`, `<`, `<=`, `>`, `>=`
* Boolean operators: `and`, `or`
* Unary operators: `-`, `!`
* Type checking
* Scope checking
* LLVM IR generation

The compiler currently supports `i32` and `bool`. Other integer types and floating-point types are not yet supported for parsing and code generation.

## Syntax Example

```blz
fn main() -> i32 {
    let x: i32 = 10;
    let y: i32 = 20;
    let is_valid: bool = true;

    if (is_valid and x < y) {
        return x + y;
    }

    return 0;
}
```

## Language Reference

The language syntax is documented in [SYNTAX.md](SYNTAX.md).


