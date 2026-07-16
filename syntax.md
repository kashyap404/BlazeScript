# BlazeScript Syntax
This document describes the lexical syntax currently supported by the BlazeScript compiler.
---
# Variables

Variables are declared using the `let` keyword.

let age = 20;
let name = "Pranay";
let pi = 3.14;

---

# Type Annotations

Variables may optionally specify an explicit type.

let age: i32 = 20;
let distance: f64 = 15.75;
let flag: bool = true;

Supported types:

- i32
- i64
- u32
- u64
- f32
- f64
- bool
- string

---

# Functions

Functions are declared using the `fn` keyword.

fn greet() {
    print("Hello");
}

Functions may specify an explicit return type using `->`.

fn add(a: i32, b: i32) -> i32 {
    return a + b;
}

---

# Boolean Literals

true
false

---

# Null Literal

null

---

# Operators

## Arithmetic

```text
+
-
*
/
```

Example:

let x = 10 + 5;
---

## Comparison

```text
==
!=
<
<=
>
>=
```

Example:

if (age >= 18) {
}

---

# Delimiters

```text
(
)

{
}

,

.

:

;
```

---

# Comments

Single-line comments are supported.

// This is a comment
let x = 10;

---

# Keywords

```
let
const
fn
if
else
while
for
return
true
false
null
and
or
```

---

# Literals

The scanner currently recognizes:

- Identifiers
- Integer numbers
- Floating-point numbers
- String literals

Examples:

42
3.1415
"Hello"
variableName

---

# Example Program

fn main() -> i32 {
    let age: i32 = 20;

    if (age >= 18) {
        print("Adult");
    } else {
        print("Minor");
    }

    return 0;
}

---

# Notes

- Every statement ends with a semicolon (`;`).
- Blocks are enclosed in `{}`.
- Function return types use the `->` syntax.
- The current compiler stage performs lexical analysis only. Parsing and semantic analysis will be added in future stages.