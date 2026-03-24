# Variables and Types

## Understanding Variables and Type System in KLang

KLang features a **dynamic type system** with **optional type annotations** and **powerful type inference**. This guide covers everything you need to know about variables and types.

## Variable Declarations

KLang provides three ways to declare variables, each with specific semantics:

### `const` - Immutable Variables

Constants cannot be reassigned after initialization:

```klang
const PI = 3.14159
const MAX_USERS = 100
const APP_NAME = "MyApp"

println(PI)  # 3.14159

# This will cause an error:
# PI = 3.14  # Error: cannot assign to const variable 'PI'
```

**Key Points:**
- Must be initialized at declaration
- Cannot be reassigned
- Convention: Use UPPER_CASE for true constants
- Use for values that never change

**When to use `const`:**
- Mathematical constants (PI, E)
- Configuration values
- API endpoints
- Any value that shouldn't change

### `let` - Block-Scoped Mutable Variables

The default choice for most variables:

```klang
let count = 0
let name = "Alice"
let scores = [85, 90, 78]

# Can be reassigned
count = count + 1
name = "Bob"
scores.push(95)

println(count)  # 1
println(name)   # Bob
```

**Key Points:**
- Block-scoped (only exists in its block)
- Can be reassigned
- Cannot be redeclared in the same scope
- Can be declared without initialization

**When to use `let`:**
- Loop counters
- Accumulator variables
- Working variables in functions
- Any value that needs to change

### `var` - Allows Redeclaration

Similar to `let` but allows redeclaration:

```klang
var x = 10
println(x)  # 10

# Can be redeclared
var x = 20
println(x)  # 20

# Can be reassigned
x = 30
println(x)  # 30
```

**Key Points:**
- Allows redeclaration in the same scope
- Use sparingly (prefer `let`)
- Useful for REPL experimentation

**When to use `var`:**
- REPL sessions for quick testing
- Legacy code compatibility
- Rare cases needing redeclaration

## Type System Overview

KLang supports these core types:

| Type | Description | Example |
|------|-------------|---------|
| **int** | Integer numbers | `42`, `-17`, `0` |
| **float** | Floating-point numbers | `3.14`, `-0.5`, `2.0` |
| **string** | Text | `"hello"`, `'world'` |
| **bool** | Boolean values | `true`, `false` |
| **array** | Ordered collections | `[1, 2, 3]` |
| **object** | Key-value pairs | `{name: "Alice"}` |
| **function** | Callable functions | `fn() { }` |
| **null** | Absence of value | `null` |

## Type Inference

KLang automatically infers types from values:

```klang
# Type is inferred from the value
let age = 25              # int
let price = 19.99         # float
let name = "Alice"        # string
let isActive = true       # bool
let items = [1, 2, 3]     # array
let person = {name: "Bob"}  # object

# Check types with type()
println(type(age))        # int
println(type(price))      # float
println(type(name))       # string
println(type(isActive))   # bool
println(type(items))      # array
println(type(person))     # object
```

## Type Annotations (Optional)

You can optionally specify types for clarity and documentation:

```klang
# Function with type annotations
fn add(a: int, b: int) -> int {
    return a + b
}

fn greet(name: string) -> string {
    return "Hello, " + name
}

fn divide(x: float, y: float) -> float {
    return x / y
}

# Variables with type annotations
let count: int = 0
let price: float = 9.99
let message: string = "Hello"
```

**Benefits of type annotations:**
- Documentation for code readers
- Better IDE support
- Catch type errors early
- Self-documenting code

## Numbers

### Integers

```klang
# Integer literals
let positive = 42
let negative = -17
let zero = 0

# Arithmetic operations
let sum = 10 + 5      # 15
let diff = 10 - 5     # 5
let product = 10 * 5  # 50
let quotient = 10 / 5 # 2
let remainder = 10 % 3 # 1

# Integer operations
println(10 + 20)  # 30
println(10 - 3)   # 7
println(5 * 4)    # 20
println(20 / 4)   # 5
println(17 % 5)   # 2 (modulo)
```

### Floating-Point Numbers

```klang
# Float literals
let pi = 3.14159
let negative = -0.5
let scientific = 1.5e10  # 15000000000.0

# Float operations
let a = 10.5
let b = 2.5
println(a + b)  # 13.0
println(a - b)  # 8.0
println(a * b)  # 26.25
println(a / b)  # 4.2
```

### Number Type Conversion

```klang
# String to number
let age = int("25")           # 25
let price = float("19.99")    # 19.99

# Number to string
let ageStr = str(25)          # "25"
let priceStr = str(19.99)     # "19.99"

# Int to float
let x = 10
let y = float(x)              # 10.0

# Float to int (truncates)
let pi = 3.14159
let piInt = int(pi)           # 3
```

### Number Operations

```klang
# Mathematical operations
let x = 10
let y = 3

println(x + y)     # 13 (addition)
println(x - y)     # 7 (subtraction)
println(x * y)     # 30 (multiplication)
println(x / y)     # 3 (division)
println(x % y)     # 1 (modulo)

# Compound operations
let count = 10
count = count + 5  # 15
count = count * 2  # 30
count = count / 3  # 10

# Negative numbers
let neg = -42
println(-neg)      # 42
```

## Strings

### String Literals

```klang
# Single or double quotes
let name1 = "Alice"
let name2 = 'Bob'

# Escape sequences
let withQuote = "He said \"Hello\""
let withNewline = "Line 1\nLine 2"
let withTab = "Column1\tColumn2"

# Multi-line strings
let multiline = "This is
a multi-line
string"
```

### String Concatenation

```klang
# Using + operator
let first = "Hello"
let last = "World"
let greeting = first + " " + last
println(greeting)  # Hello World

# Concatenating with numbers (need str())
let age = 25
let message = "Age: " + str(age)
println(message)  # Age: 25
```

### String Methods

```klang
let text = "Hello, World!"

# Case conversion
let upper = uppercase(text)   # "HELLO, WORLD!"
let lower = lowercase(text)   # "hello, world!"

# Length
let length = len(text)        # 13

# String operations
let words = text.split(", ")  # ["Hello", "World!"]
let joined = words.join("-")  # "Hello-World!"

# Substring and search
let contains = text.includes("World")  # true
let index = text.indexOf("World")      # 7
```

### String Examples

```klang
# Building messages
let name = "Alice"
let age = 25
let city = "New York"

let bio = "Name: " + name + ", Age: " + str(age) + ", City: " + city
println(bio)  # Name: Alice, Age: 25, City: New York

# String manipulation
let email = "user@example.com"
let parts = email.split("@")
println("Username: " + parts[0])  # user
println("Domain: " + parts[1])    # example.com

# Case transformation
let command = "SAVE"
let normalized = lowercase(command)
println(normalized)  # save
```

## Booleans

### Boolean Values

```klang
let isActive = true
let isComplete = false

# Boolean from comparisons
let isAdult = age >= 18
let isEmpty = len(items) == 0
let isPositive = value > 0
```

### Boolean Operations

```klang
# Logical AND
let a = true
let b = false
println(a && b)  # false
println(a && true)  # true

# Logical OR
println(a || b)  # true
println(b || false)  # false

# Logical NOT
println(!a)  # false
println(!b)  # true

# Complex conditions
let age = 25
let hasLicense = true
let canDrive = age >= 18 && hasLicense
println(canDrive)  # true
```

### Truthy and Falsy Values

```klang
# Falsy values: false, 0, "", null, undefined
# Truthy values: everything else

if 0 {
    println("0 is falsy")  # Won't print
}

if 1 {
    println("1 is truthy")  # Prints
}

if "" {
    println("Empty string is falsy")  # Won't print
}

if "hello" {
    println("Non-empty string is truthy")  # Prints
}
```

## Null and Undefined

### Null Values

```klang
# Explicit null
let value = null
println(type(value))  # null

# Uninitialized variables
let x
println(x)  # null or undefined

# Checking for null
if value == null {
    println("Value is null")
}

# Null coalescing pattern
fn getValue(x) {
    if x == null {
        return "default"
    }
    return x
}
```

## Arrays

Arrays are covered in detail in [Arrays documentation](14-arrays.md), but here's a quick overview:

```klang
# Array creation
let numbers = [1, 2, 3, 4, 5]
let mixed = [1, "hello", true, [1, 2]]
let empty = []

# Array access
println(numbers[0])  # 1
println(numbers[4])  # 5

# Array methods
numbers.push(6)         # Add to end
let first = numbers[0]  # Get first element
let length = len(numbers)  # 6

# Type
println(type(numbers))  # array
```

## Objects

Objects are covered in detail in [Objects documentation](15-objects.md), but here's a quick overview:

```klang
# Object creation
let person = {
    name: "Alice",
    age: 25,
    city: "New York"
}

# Property access
println(person.name)  # Alice
println(person.age)   # 25

# Property modification
person.age = 26
person.city = "Boston"

# Type
println(type(person))  # object
```

## Variable Scope

### Block Scope

```klang
let outer = "I'm outside"

{
    let inner = "I'm inside"
    println(outer)  # Accessible: I'm outside
    println(inner)  # Accessible: I'm inside
}

# println(inner)  # Error: inner not defined
println(outer)  # Still accessible
```

### Function Scope

```klang
let global = "global"

fn myFunction() {
    let local = "local"
    println(global)  # Can access global
    println(local)   # Can access local
}

myFunction()
# println(local)  # Error: local not defined
```

### Loop Scope

```klang
# Loop variables are scoped to the loop
for i in 0..5 {
    let loopVar = i * 2
    println(loopVar)
}

# println(i)  # Error: i not defined
# println(loopVar)  # Error: loopVar not defined
```

### Shadowing

```klang
let x = 10
println(x)  # 10

{
    let x = 20  # Shadows outer x
    println(x)  # 20
}

println(x)  # 10 (outer x unchanged)
```

## Type Conversion

### Explicit Conversion

```klang
# To integer
let a = int("42")        # String to int: 42
let b = int(3.14)        # Float to int: 3
let c = int(true)        # Bool to int: 1

# To float
let d = float("3.14")    # String to float: 3.14
let e = float(42)        # Int to float: 42.0

# To string
let f = str(42)          # Int to string: "42"
let g = str(3.14)        # Float to string: "3.14"
let h = str(true)        # Bool to string: "true"
let i = str([1, 2, 3])   # Array to string: "[1, 2, 3]"

# To boolean (logical conversion)
let j = bool(1)          # true
let k = bool(0)          # false
let m = bool("")         # false
let n = bool("hello")    # true
```

### Implicit Conversion

```klang
# String concatenation converts to string
let message = "Count: " + str(42)  # Need explicit str()

# Arithmetic converts to numbers
let sum = 10 + 5      # Both ints: 15
let mixed = 10 + 5.5  # Int + float: 15.5
```

## Type Checking

### Using `type()` Function

```klang
let x = 42
println(type(x))  # int

let y = "hello"
println(type(y))  # string

let z = [1, 2, 3]
println(type(z))  # array

# Conditional based on type
fn processValue(val) {
    if type(val) == "int" {
        println("Processing integer: " + str(val))
    } else if type(val) == "string" {
        println("Processing string: " + val)
    } else {
        println("Unknown type: " + type(val))
    }
}

processValue(42)       # Processing integer: 42
processValue("hello")  # Processing string: hello
```

## Common Patterns

### Pattern 1: Default Values

```klang
fn greet(name) {
    # Provide default if name is null or undefined
    if name == null {
        name = "Guest"
    }
    return "Hello, " + name
}

println(greet("Alice"))  # Hello, Alice
println(greet(null))     # Hello, Guest
```

### Pattern 2: Type Guards

```klang
fn safeAdd(a, b) {
    # Ensure both are numbers
    if type(a) != "int" || type(b) != "int" {
        return null
    }
    return a + b
}

println(safeAdd(5, 10))      # 15
println(safeAdd(5, "hello")) # null
```

### Pattern 3: Type Conversion with Validation

```klang
fn parseInteger(str) {
    # Try to convert, return null on failure
    let result = int(str)
    if type(result) != "int" {
        return null
    }
    return result
}

let valid = parseInteger("42")     # 42
let invalid = parseInteger("abc")  # null or error
```

### Pattern 4: Flexible Function Parameters

```klang
fn display(value) {
    let t = type(value)
    
    if t == "string" {
        println("String: " + value)
    } else if t == "int" || t == "float" {
        println("Number: " + str(value))
    } else if t == "array" {
        println("Array with " + str(len(value)) + " items")
    } else {
        println("Other type: " + t)
    }
}

display("hello")      # String: hello
display(42)           # Number: 42
display([1, 2, 3])    # Array with 3 items
```

### Pattern 5: Configuration Objects

```klang
# Default configuration
const DEFAULT_CONFIG = {
    timeout: 30,
    retries: 3,
    debug: false
}

fn createConfig(options) {
    # Merge with defaults
    let config = {
        timeout: options.timeout != null ? options.timeout : DEFAULT_CONFIG.timeout,
        retries: options.retries != null ? options.retries : DEFAULT_CONFIG.retries,
        debug: options.debug != null ? options.debug : DEFAULT_CONFIG.debug
    }
    return config
}

let config1 = createConfig({timeout: 60})
# Result: {timeout: 60, retries: 3, debug: false}
```

## Best Practices

### 1. Use `const` by Default

```klang
# ✅ Good: Values that don't change
const MAX_SIZE = 100
const API_URL = "https://api.example.com"

# ❌ Avoid: Using let for constants
let MAX_SIZE = 100  # Should be const
```

### 2. Use `let` for Variables That Change

```klang
# ✅ Good: Value changes
let count = 0
for i in 0..10 {
    count = count + i
}

# ❌ Avoid: Using var unnecessarily
var count = 0  # Use let instead
```

### 3. Avoid `var` Except in REPL

```klang
# ✅ Good: In REPL for experimentation
# >>> var x = 10
# >>> var x = 20  # OK for testing

# ❌ Avoid: In production code
var x = 10  # Use let or const
```

### 4. Explicit Type Conversion

```klang
# ✅ Good: Explicit conversion
let age = int("25")
let message = "Age: " + str(age)

# ❌ Avoid: Relying on implicit conversion
# Some operations may not work as expected
```

### 5. Initialize Variables

```klang
# ✅ Good: Initialize with a value
let count = 0
let items = []
let name = ""

# ⚠️ Caution: Uninitialized (null/undefined)
let x
println(x)  # May be null
```

### 6. Meaningful Variable Names

```klang
# ✅ Good: Descriptive names
let userCount = 10
let isAuthenticated = true
let maxRetryAttempts = 3

# ❌ Avoid: Cryptic names
let x = 10
let flag = true
let n = 3
```

### 7. Use Type Annotations for Public APIs

```klang
# ✅ Good: Clear interface
fn calculateTotal(price: float, quantity: int) -> float {
    return price * quantity
}

# ⚠️ OK but less clear
fn calculateTotal(price, quantity) {
    return price * quantity
}
```

## Common Mistakes

### Mistake 1: Modifying Constants

```klang
const MAX_SIZE = 100
# MAX_SIZE = 200  # ❌ Error: cannot assign to const
```

### Mistake 2: Forgetting Type Conversion

```klang
let count = 42
# println("Count: " + count)  # ❌ May not work
println("Count: " + str(count))  # ✅ Correct
```

### Mistake 3: Unintended Shadowing

```klang
let value = 10

fn process() {
    let value = 20  # ⚠️ Shadows outer value
    println(value)  # 20
}

process()
println(value)  # Still 10
```

### Mistake 4: Assuming Type Coercion

```klang
let a = "10"
let b = 20
# let sum = a + b  # ❌ May not work as expected
let sum = int(a) + b  # ✅ Correct: 30
```

## Summary

### Variable Declarations

- **`const`**: Immutable, must initialize, cannot reassign
- **`let`**: Mutable, block-scoped, can reassign
- **`var`**: Mutable, allows redeclaration (use sparingly)

### Core Types

- **Numbers**: `int`, `float`
- **Text**: `string`
- **Logic**: `bool`
- **Collections**: `array`, `object`
- **Special**: `null`, `function`

### Key Functions

- **Type checking**: `type(value)`
- **Conversion**: `int()`, `float()`, `str()`, `bool()`
- **String**: `uppercase()`, `lowercase()`, `len()`
- **Utility**: `println()`, `print()`

## Next Steps

Now that you understand variables and types, continue learning:

- **[Operators](11-operators.md)** - Arithmetic, comparison, and logical operators
- **[Control Flow](12-control-flow.md)** - Conditionals and loops
- **[Functions](13-functions.md)** - Function declarations and patterns
- **[Arrays](14-arrays.md)** - Array manipulation and methods
- **[Objects](15-objects.md)** - Object-oriented programming

---

**Master variables and types to build solid KLang programs!** 🚀
