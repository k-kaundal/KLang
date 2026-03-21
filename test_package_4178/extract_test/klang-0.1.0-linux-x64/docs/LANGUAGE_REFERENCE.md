# KLang Language Reference

## Complete Syntax and Semantics Reference

---

## Table of Contents
1. [Lexical Structure](#lexical-structure)
2. [Data Types](#data-types)
3. [Variables](#variables)
4. [Operators](#operators)
5. [Control Flow](#control-flow)
6. [Functions](#functions)
7. [Classes and Objects](#classes-and-objects)
8. [Built-in Functions](#built-in-functions)
9. [Comments](#comments)
10. [Keywords](#keywords)

---

## Lexical Structure

### Identifiers
- Must start with a letter (a-z, A-Z) or underscore (_)
- Can contain letters, digits, and underscores
- Case-sensitive

**Valid identifiers:**
```klang
name
userName
_private
value123
MAX_SIZE
```

**Invalid identifiers:**
```klang
123value    # Cannot start with digit
user-name   # Hyphen not allowed
class       # Reserved keyword
```

### Keywords (Reserved Words)
```
fn          let         if          else        while
for         return      break       continue    in
true        false       class       new         this
extends     super       null
```

---

## Data Types

### Primitive Types

#### Integer (int)
```klang
let age: int = 25
let count = 100        # Type inferred
let negative = -42
```

#### Float (float)
```klang
let pi: float = 3.14159
let price = 99.99
let scientific = 1.5e10
```

#### String (string)
```klang
let name: string = "Alice"
let message = "Hello, World!"
let multiline = "Line 1\nLine 2"

# String concatenation
let full = "Hello" + " " + "World"
```

**Escape sequences:**
- `\n` - Newline
- `\t` - Tab
- `\\` - Backslash
- `\"` - Double quote

#### Boolean (bool)
```klang
let isActive: bool = true
let isDone = false
let result = x > 10    # Boolean expression
```

#### Null
```klang
let value = null
```

### Collection Types

#### Lists (Arrays)
```klang
# List literals
let numbers = [1, 2, 3, 4, 5]
let names = ["Alice", "Bob", "Charlie"]
let mixed = [1, "two", 3.0, true]

# Indexing (0-based)
let first = numbers[0]    # 1
let last = numbers[4]     # 5

# Modification
numbers[0] = 10

# Length
let count = len(numbers)  # 5
```

---

## Variables

### Declaration

#### With Type Annotation
```klang
let name: string = "Alice"
let age: int = 30
let price: float = 19.99
let active: bool = true
```

#### With Type Inference
```klang
let name = "Alice"        # string
let age = 30              # int
let price = 19.99         # float
let active = true         # bool
```

### Assignment
```klang
let x = 10
x = 20        # Reassignment

let y: int = 5
y = y + 1     # Increment
```

### Scope
```klang
let global = "I'm global"

fn test() {
    let local = "I'm local"
    println(global)    # OK: can access global
    println(local)     # OK: in scope
}

# println(local)       # Error: out of scope
```

---

## Operators

### Arithmetic Operators
```klang
let a = 10
let b = 3

a + b     # 13 (addition)
a - b     # 7  (subtraction)
a * b     # 30 (multiplication)
a / b     # 3  (division)
a % b     # 1  (modulo/remainder)
```

### Comparison Operators
```klang
a == b    # false (equal to)
a != b    # true  (not equal to)
a > b     # true  (greater than)
a < b     # false (less than)
a >= b    # true  (greater than or equal)
a <= b    # false (less than or equal)
```

### Logical Operators
```klang
true && false    # false (logical AND)
true || false    # true  (logical OR)
!true            # false (logical NOT)

# Short-circuit evaluation
x > 0 && y < 10
a == 0 || b == 0
```

### Assignment Operators
```klang
x = 10       # Simple assignment
x = x + 5    # Compound operation

# Note: +=, -=, etc. not yet supported
```

### String Operators
```klang
"Hello" + " " + "World"    # "Hello World" (concatenation)
```

---

## Control Flow

### If Statement
```klang
# Simple if
if x > 10 {
    println("x is greater than 10")
}

# If-else
if x > 10 {
    println("Big")
} else {
    println("Small")
}

# If-else-if
if x > 100 {
    println("Very big")
} else if x > 10 {
    println("Big")
} else {
    println("Small")
}

# Nested if
if x > 0 {
    if x < 10 {
        println("Between 0 and 10")
    }
}
```

### While Loop
```klang
# Basic while loop
let i = 0
while i < 10 {
    println(i)
    i = i + 1
}

# Infinite loop with break
while true {
    if condition {
        break
    }
}

# Continue statement
let i = 0
while i < 10 {
    i = i + 1
    if i % 2 == 0 {
        continue    # Skip even numbers
    }
    println(i)
}
```

### For Loop (Range-based)
```klang
# Basic for loop
for i in 0 .. 10 {
    println(i)    # Prints 0 to 9
}

# For loop with list (future feature)
# for item in items {
#     println(item)
# }
```

### Break and Continue
```klang
# Break: exit loop
for i in 0 .. 100 {
    if i > 10 {
        break
    }
    println(i)
}

# Continue: skip iteration
for i in 0 .. 10 {
    if i % 2 == 0 {
        continue
    }
    println(i)    # Only prints odd numbers
}
```

---

## Functions

### Function Definition
```klang
# Function with no parameters
fn greet() {
    println("Hello!")
}

# Function with parameters
fn add(a: int, b: int) {
    return a + b
}

# Function with return type
fn multiply(a: int, b: int) -> int {
    return a * b
}

# Function with multiple statements
fn calculate(x: int) -> int {
    let y = x * 2
    let z = y + 10
    return z
}
```

### Function Calls
```klang
greet()                    # No arguments
let sum = add(5, 3)        # With arguments
let result = multiply(4, 7)
```

### Return Statement
```klang
fn max(a: int, b: int) -> int {
    if a > b {
        return a
    }
    return b
}

# Early return
fn check(value: int) -> bool {
    if value < 0 {
        return false    # Early exit
    }
    return true
}
```

### Recursive Functions
```klang
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1
    }
    return n * factorial(n - 1)
}

println(factorial(5))    # 120
```

---

## Classes and Objects

### Class Definition
```klang
class ClassName {
    # Fields (instance variables)
    let field1: type = default_value
    let field2: type = default_value
    
    # Constructor
    fn init(param1: type, param2: type) {
        this.field1 = param1
        this.field2 = param2
    }
    
    # Methods
    fn method_name(param: type) -> return_type {
        # Method body
        return value
    }
}
```

### Object Creation
```klang
# Using new keyword
let obj = new ClassName(arg1, arg2)

# Without constructor arguments
class Simple {
    let value: int = 0
}
let s = new Simple()
```

### The `this` Keyword
```klang
class Counter {
    let count: int = 0
    
    fn increment() {
        this.count = this.count + 1
    }
    
    fn get_count() -> int {
        return this.count
    }
}

let c = new Counter()
c.increment()
println(c.get_count())    # 1
```

### Inheritance
```klang
# Parent class
class Animal {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
    }
    
    fn speak() {
        println("Animal speaks")
    }
}

# Child class
class Dog extends Animal {
    let breed: string = ""
    
    fn init(n: string, b: string) {
        super.init(n)      # Call parent constructor
        this.breed = b
    }
    
    fn speak() {
        println("Woof!")   # Override parent method
    }
}

let dog = new Dog("Buddy", "Golden Retriever")
dog.speak()    # "Woof!"
```

### The `super` Keyword
```klang
class Parent {
    fn greet() {
        println("Hello from Parent")
    }
}

class Child extends Parent {
    fn greet() {
        super.greet()              # Call parent method
        println("Hello from Child")
    }
}

let c = new Child()
c.greet()
# Output:
# Hello from Parent
# Hello from Child
```

### Method Overriding
```klang
class Shape {
    fn area() -> float {
        return 0.0
    }
}

class Circle extends Shape {
    let radius: float = 0.0
    
    fn init(r: float) {
        this.radius = r
    }
    
    # Override parent method
    fn area() -> float {
        return 3.14159 * this.radius * this.radius
    }
}
```

---

## Built-in Functions

### I/O Functions

#### `println(value)`
Print value followed by newline
```klang
println("Hello")
println(42)
println(3.14)
```

#### `print(value)` (if available)
Print value without newline
```klang
print("Hello")
print(" ")
print("World")
```

### Type Functions

#### `type(value)` (if available)
Get type of a value
```klang
type(42)         # "int"
type("hello")    # "string"
type(true)       # "bool"
```

### Collection Functions

#### `len(list)`
Get length of a list
```klang
let nums = [1, 2, 3, 4, 5]
let count = len(nums)    # 5
```

---

## Comments

### Single-line Comments
```klang
# This is a single-line comment

let x = 10    # Comment after code
```

### Multi-line Comments (Not yet supported)
```klang
# Multi-line comments not yet available
# Use multiple single-line comments instead:
# Line 1
# Line 2
# Line 3
```

---

## Keywords Reference

| Keyword | Purpose | Example |
|---------|---------|---------|
| `fn` | Define function | `fn add(a, b) { }` |
| `let` | Declare variable | `let x = 10` |
| `if` | Conditional | `if x > 0 { }` |
| `else` | Alternative | `if x > 0 { } else { }` |
| `while` | Loop | `while x < 10 { }` |
| `for` | Iteration | `for i in 0..10 { }` |
| `in` | Range operator | `for i in 0..10` |
| `return` | Return value | `return x + y` |
| `break` | Exit loop | `break` |
| `continue` | Skip iteration | `continue` |
| `class` | Define class | `class Point { }` |
| `new` | Create object | `new Point()` |
| `this` | Current object | `this.field` |
| `extends` | Inheritance | `class Dog extends Animal` |
| `super` | Parent reference | `super.init()` |
| `true` | Boolean true | `let x = true` |
| `false` | Boolean false | `let x = false` |
| `null` | Null value | `let x = null` |

---

## Type Annotations

### Variable Type Annotations
```klang
let name: string = "Alice"
let age: int = 30
let price: float = 19.99
let active: bool = true
```

### Function Parameter Types
```klang
fn greet(name: string, age: int) {
    println(name)
}
```

### Function Return Types
```klang
fn add(a: int, b: int) -> int {
    return a + b
}

fn get_name() -> string {
    return "Alice"
}
```

---

## Best Practices

### Naming Conventions
- **Variables**: `camelCase` or `snake_case`
- **Functions**: `camelCase` or `snake_case`
- **Classes**: `PascalCase`
- **Constants**: `UPPER_SNAKE_CASE` (convention)

```klang
let userName = "Alice"
let user_count = 10

fn calculateTotal() { }
fn get_user_name() { }

class UserAccount { }
class PaymentProcessor { }

let MAX_SIZE = 100
```

### Code Style
```klang
# Good: Clear and readable
fn calculate_average(numbers: list) -> float {
    let sum = 0
    for num in numbers {
        sum = sum + num
    }
    return sum / len(numbers)
}

# Use meaningful names
let user_age = 25
let total_price = 199.99

# Add spaces around operators
let result = a + b * c

# Use blank lines to separate logical sections
```

---

## Error Handling (Future Feature)

Error handling is planned but not yet implemented:
```klang
# Future syntax (not yet available)
# try {
#     let result = risky_operation()
# } catch (error) {
#     println("Error: " + error)
# }
```

---

## Limitations and Future Features

### Current Limitations
- No module/import system yet
- No exception handling
- No generics
- No operator overloading
- No default parameter values
- No variadic functions
- Multi-level inheritance (3+ levels) has stability issues

### Planned Features
- Module system
- Exception handling
- Generics
- Async/await
- Pattern matching
- More collection types (maps, sets)
- String interpolation
- Method chaining improvements

---

## Examples

See the `examples/` directory for complete working examples:
- `hello.kl` - Hello World
- `factorial.kl` - Recursive functions
- `fibonacci.kl` - Fibonacci sequence
- `oop_demo.kl` - Object-oriented programming
- `point_class.kl` - Class with methods
- `calculator_class.kl` - Method chaining
- `inheritance_demo.kl` - Inheritance

---

## References

- [Getting Started Guide](GETTING_STARTED.md)
- [OOP Language Specification](OOP_LANGUAGE_SPEC.md)
- [Tutorial](TUTORIAL.md) (coming soon)
- [API Reference](API_REFERENCE.md) (coming soon)

---

**Questions?** Open an issue on [GitHub](https://github.com/k-kaundal/KLang/issues)
