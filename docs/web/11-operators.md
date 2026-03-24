# Operators

## Complete Guide to KLang Operators

Operators are symbols that perform operations on values and variables. KLang supports a rich set of operators similar to JavaScript and Python.

## Arithmetic Operators

Perform mathematical calculations on numbers.

### Basic Arithmetic

```klang
let a = 10
let b = 3

# Addition
println(a + b)  # 13

# Subtraction
println(a - b)  # 7

# Multiplication
println(a * b)  # 30

# Division
println(a / b)  # 3 (integer division)
println(10.0 / 3.0)  # 3.333... (float division)

# Modulo (remainder)
println(a % b)  # 1
println(10 % 2)  # 0
```

### Unary Operators

```klang
let x = 10

# Unary plus (no effect on numbers)
let positive = +x  # 10

# Unary minus (negation)
let negative = -x  # -10

# Double negation
let y = -(-x)  # 10
```

### Arithmetic Examples

```klang
# Calculate circle area
const PI = 3.14159
let radius = 5.0
let area = PI * radius * radius
println("Area: " + str(area))  # Area: 78.53975

# Temperature conversion
let celsius = 25.0
let fahrenheit = (celsius * 9.0 / 5.0) + 32.0
println(str(celsius) + "°C = " + str(fahrenheit) + "°F")  # 25°C = 77°F

# Calculate average
let scores = [85, 92, 78, 95, 88]
let sum = scores.reduce((acc, n) => acc + n, 0)
let average = sum / len(scores)
println("Average: " + str(average))  # Average: 87.6

# Modulo for even/odd check
let num = 17
if num % 2 == 0 {
    println("Even")
} else {
    println("Odd")  # This prints
}
```

## Assignment Operators

Assign values to variables.

### Basic Assignment

```klang
# Simple assignment
let x = 10
let name = "Alice"
let items = [1, 2, 3]

# Multiple assignments
let a = 1
let b = 2
let c = 3

# Assignment with expression
let result = 5 + 10  # 15
```

### Compound Assignment

```klang
let count = 10

# Addition assignment
count = count + 5  # 15
# Note: KLang may support +=, but safest to use explicit form

# Subtraction
count = count - 3  # 12

# Multiplication
count = count * 2  # 24

# Division
count = count / 4  # 6

# Modulo
count = count % 5  # 1
```

### Assignment Patterns

```klang
# Accumulator pattern
let sum = 0
for i in 0..10 {
    sum = sum + i
}
println(sum)  # 45

# Counter pattern
let count = 0
while count < 5 {
    println(count)
    count = count + 1
}

# Array building
let results = []
for i in 0..5 {
    results.push(i * i)
}
println(results)  # [0, 1, 4, 9, 16]
```

## Comparison Operators

Compare two values and return a boolean.

### Equality Operators

```klang
# Equal to
println(5 == 5)    # true
println(5 == 10)   # false
println("hi" == "hi")  # true

# Not equal to
println(5 != 10)   # true
println(5 != 5)    # false
println("hi" != "bye")  # true
```

### Relational Operators

```klang
let a = 10
let b = 20

# Greater than
println(a > b)   # false
println(b > a)   # true

# Less than
println(a < b)   # true
println(b < a)   # false

# Greater than or equal to
println(a >= 10)  # true
println(a >= 11)  # false

# Less than or equal to
println(b <= 20)  # true
println(b <= 19)  # false
```

### Comparison Examples

```klang
# Age validation
let age = 25
if age >= 18 {
    println("Adult")
} else {
    println("Minor")
}

# Password strength
let password = "secret123"
if len(password) >= 8 {
    println("Password is strong enough")
} else {
    println("Password too short")
}

# Grade calculation
let score = 85
let grade = score >= 90 ? "A" : score >= 80 ? "B" : score >= 70 ? "C" : score >= 60 ? "D" : "F"
println("Grade: " + grade)  # Grade: B

# Range checking
let value = 50
if value >= 0 && value <= 100 {
    println("Value is in valid range")
}

# String comparison
let status1 = "active"
let status2 = "inactive"
if status1 == "active" {
    println("User is active")
}
```

## Logical Operators

Combine boolean expressions.

### AND Operator (&&)

Returns true only if both operands are true:

```klang
# Basic AND
println(true && true)    # true
println(true && false)   # false
println(false && true)   # false
println(false && false)  # false

# Practical examples
let age = 25
let hasLicense = true

if age >= 18 && hasLicense {
    println("Can drive")  # Prints
}

# Multiple conditions
let score = 85
let attendance = 90
if score >= 80 && attendance >= 85 {
    println("Qualifies for honors")  # Prints
}

# Range checking
let value = 50
if value > 0 && value < 100 {
    println("Value is in range")  # Prints
}
```

### OR Operator (||)

Returns true if at least one operand is true:

```klang
# Basic OR
println(true || true)    # true
println(true || false)   # true
println(false || true)   # true
println(false || false)  # false

# Practical examples
let isWeekend = true
let isHoliday = false

if isWeekend || isHoliday {
    println("No work today!")  # Prints
}

# Multiple options
let role = "admin"
if role == "admin" || role == "moderator" {
    println("Has elevated privileges")  # Prints
}

# Validation with fallback
let input = ""
if len(input) > 0 || input == null {
    println("Input is empty or null")
}
```

### NOT Operator (!)

Inverts a boolean value:

```klang
# Basic NOT
println(!true)   # false
println(!false)  # true

# Double negation
println(!!true)   # true
println(!!false)  # false

# Practical examples
let isLoggedIn = false
if !isLoggedIn {
    println("Please log in")  # Prints
}

# Inverting conditions
let isEmpty = len(items) == 0
if !isEmpty {
    println("Has items")
}

# With comparisons
let age = 15
if !(age >= 18) {
    println("Under 18")  # Prints
}
```

### Logical Operator Precedence

```klang
# AND has higher precedence than OR
# a || b && c  is evaluated as  a || (b && c)

let a = true
let b = false
let c = true

println(a || b && c)   # true (same as: a || (b && c))
println((a || b) && c) # true (different grouping)

# Use parentheses for clarity
let age = 25
let hasLicense = true
let hasInsurance = false

# Complex condition
if (age >= 18 && hasLicense) && hasInsurance {
    println("Can drive legally")
}

# Clearer version with parentheses
if age >= 18 && (hasLicense || hasInsurance) {
    println("Some driving authorization")
}
```

### Short-Circuit Evaluation

Logical operators use short-circuit evaluation:

```klang
# AND: if first is false, second is not evaluated
let a = false
let b = true
if a && (b = false) {
    # b = false is not executed because a is false
}
println(b)  # true (b wasn't changed)

# OR: if first is true, second is not evaluated
let x = true
let y = false
if x || (y = true) {
    # y = true is not executed because x is true
}
println(y)  # false (y wasn't changed)
```

## Ternary Operator

Conditional expression: `condition ? valueIfTrue : valueIfFalse`

### Basic Ternary

```klang
let age = 20
let status = age >= 18 ? "adult" : "minor"
println(status)  # adult

# With numbers
let a = 10
let b = 20
let max = a > b ? a : b
println(max)  # 20

# With strings
let score = 85
let grade = score >= 60 ? "Pass" : "Fail"
println(grade)  # Pass
```

### Nested Ternary

```klang
# Grade calculation
let score = 85
let grade = score >= 90 ? "A" :
            score >= 80 ? "B" :
            score >= 70 ? "C" :
            score >= 60 ? "D" : "F"
println(grade)  # B

# Category assignment
let value = 15
let category = value > 20 ? "high" :
               value > 10 ? "medium" : "low"
println(category)  # medium

# Sign detection
let num = -5
let sign = num > 0 ? "positive" :
           num < 0 ? "negative" : "zero"
println(sign)  # negative
```

### Ternary in Expressions

```klang
# In function calls
let age = 25
println("You are " + (age >= 18 ? "an adult" : "a minor"))

# In calculations
let basePrice = 100
let discount = 0.1
let finalPrice = basePrice * (discount > 0 ? (1 - discount) : 1)
println(finalPrice)  # 90

# In array operations
let numbers = [1, 2, 3, 4, 5]
let processed = numbers.map(n => n > 3 ? n * 2 : n)
println(processed)  # [1, 2, 3, 8, 10]

# As function argument
fn greet(name, formal) {
    return (formal ? "Good day, " : "Hey ") + name
}
println(greet("Alice", true))   # Good day, Alice
println(greet("Bob", false))    # Hey Bob
```

## String Operators

### Concatenation

```klang
# String + String
let greeting = "Hello" + " " + "World"
println(greeting)  # Hello World

# String + Number (need str())
let age = 25
let message = "Age: " + str(age)
println(message)  # Age: 25

# Building complex strings
let firstName = "Alice"
let lastName = "Smith"
let fullName = firstName + " " + lastName
println(fullName)  # Alice Smith

# URL building
let base = "https://api.example.com"
let endpoint = "/users"
let id = 123
let url = base + endpoint + "/" + str(id)
println(url)  # https://api.example.com/users/123
```

### String Comparison

```klang
# Equality
println("hello" == "hello")  # true
println("hello" == "Hello")  # false

# Inequality
println("cat" != "dog")  # true

# Ordering (lexicographic)
println("apple" < "banana")  # true
println("zebra" > "alpha")   # true
```

## Operator Precedence

Operators are evaluated in this order (highest to lowest):

1. **Parentheses**: `()`
2. **Unary**: `!`, `-`, `+`
3. **Multiplicative**: `*`, `/`, `%`
4. **Additive**: `+`, `-`
5. **Relational**: `<`, `<=`, `>`, `>=`
6. **Equality**: `==`, `!=`
7. **Logical AND**: `&&`
8. **Logical OR**: `||`
9. **Ternary**: `? :`
10. **Assignment**: `=`

### Precedence Examples

```klang
# Multiplication before addition
println(2 + 3 * 4)    # 14, not 20

# Use parentheses to change order
println((2 + 3) * 4)  # 20

# Comparison before logical AND
let x = 5
println(x > 3 && x < 10)  # true

# Logical AND before OR
println(true || false && false)  # true (same as: true || (false && false))

# Complex expression
let a = 10
let b = 20
let c = 5
let result = a + b * c - 10  # 10 + 100 - 10 = 100
println(result)
```

### Clarity with Parentheses

```klang
# ❌ Hard to read
let result = a + b * c / d - e % f

# ✅ Clear with parentheses
let result = a + ((b * c) / d) - (e % f)

# ❌ Ambiguous
if x > 5 && y < 10 || z == 0 {
    # ...
}

# ✅ Clear intent
if (x > 5 && y < 10) || (z == 0) {
    # ...
}
```

## Type Operators

### Type Checking

```klang
# Get type as string
let x = 42
println(type(x))  # int

let name = "Alice"
println(type(name))  # string

let items = [1, 2, 3]
println(type(items))  # array

# Type checking in conditions
fn process(value) {
    if type(value) == "int" {
        println("Processing integer")
    } else if type(value) == "string" {
        println("Processing string")
    } else {
        println("Unknown type")
    }
}

process(42)       # Processing integer
process("hello")  # Processing string
```

## Common Patterns

### Pattern 1: Range Checking

```klang
# Check if value is in range
fn inRange(value, min, max) {
    return value >= min && value <= max
}

println(inRange(50, 0, 100))   # true
println(inRange(150, 0, 100))  # false

# Clamp value to range
fn clamp(value, min, max) {
    return value < min ? min : (value > max ? max : value)
}

println(clamp(-10, 0, 100))  # 0
println(clamp(50, 0, 100))   # 50
println(clamp(150, 0, 100))  # 100
```

### Pattern 2: Default Values

```klang
# Use OR for default values (if falsy)
fn greet(name) {
    # If name is null/empty, use default
    let displayName = name != null && len(name) > 0 ? name : "Guest"
    return "Hello, " + displayName
}

println(greet("Alice"))  # Hello, Alice
println(greet(""))       # Hello, Guest

# With ternary operator
fn createUser(name, age) {
    return {
        name: name != null ? name : "Unknown",
        age: age != null ? age : 0
    }
}
```

### Pattern 3: Validation

```klang
# Multiple validation checks
fn validateEmail(email) {
    if email == null {
        return false
    }
    if len(email) < 5 {
        return false
    }
    if !email.includes("@") {
        return false
    }
    return true
}

println(validateEmail("test@example.com"))  # true
println(validateEmail("invalid"))            # false

# Combined validation
fn validatePassword(password) {
    return password != null &&
           len(password) >= 8 &&
           len(password) <= 128
}
```

### Pattern 4: Comparison Chains

```klang
# Check if value is between two others
fn isBetween(value, a, b) {
    return a <= value && value <= b
}

# Sorting comparison
fn compareNumbers(a, b) {
    return a < b ? -1 : (a > b ? 1 : 0)
}

println(compareNumbers(5, 10))   # -1 (less than)
println(compareNumbers(10, 5))   # 1 (greater than)
println(compareNumbers(5, 5))    # 0 (equal)
```

### Pattern 5: Boolean Flags

```klang
# Combining multiple flags
let canView = true
let canEdit = false
let canDelete = false

# Check permissions
if canView && !canEdit {
    println("Read-only access")
}

# Set flags based on role
fn getPermissions(role) {
    return {
        canView: role == "admin" || role == "user",
        canEdit: role == "admin",
        canDelete: role == "admin"
    }
}

let adminPerms = getPermissions("admin")
println(adminPerms.canDelete)  # true

let userPerms = getPermissions("user")
println(userPerms.canDelete)   # false
```

### Pattern 6: Guard Clauses

```klang
# Early return for invalid input
fn divide(a, b) {
    if b == 0 {
        return null  # Guard against division by zero
    }
    return a / b
}

# Multiple guards
fn processUser(user) {
    if user == null {
        return "Error: No user provided"
    }
    if type(user.name) != "string" {
        return "Error: Invalid name"
    }
    if type(user.age) != "int" {
        return "Error: Invalid age"
    }
    
    return "Processing user: " + user.name
}
```

## Best Practices

### 1. Use Parentheses for Clarity

```klang
# ✅ Good: Clear intent
if (age >= 18 && hasLicense) || isSupervised {
    println("Can drive")
}

# ❌ Avoid: Ambiguous
if age >= 18 && hasLicense || isSupervised {
    println("Can drive")
}
```

### 2. Avoid Complex Nested Ternaries

```klang
# ❌ Hard to read
let x = a ? b ? c : d : e ? f : g

# ✅ Better: Use if-else
let x
if a {
    x = b ? c : d
} else {
    x = e ? f : g
}

# ✅ Or extract logic
fn getX() {
    if a {
        return b ? c : d
    } else {
        return e ? f : g
    }
}
```

### 3. Explicit Type Conversion

```klang
# ✅ Good: Explicit conversion
let count = 42
let message = "Count: " + str(count)

# ❌ Avoid: Assuming implicit conversion
# let message = "Count: " + count  # May not work
```

### 4. Short-Circuit for Null Checks

```klang
# ✅ Good: Safe property access
if user != null && user.name == "Alice" {
    println("Hello Alice!")
}

# ❌ Avoid: May error if user is null
# if user.name == "Alice" {  # Error if user is null
```

### 5. Comparison with Constants

```klang
# ✅ Good: Constant on left (Yoda conditions)
if 18 <= age {  # Or: if age >= 18
    println("Adult")
}

# Both are fine, choose what's readable
```

## Common Mistakes

### Mistake 1: Assignment in Condition

```klang
let x = 10
# ❌ Wrong: Assignment instead of comparison
# if x = 5 {  # This assigns 5 to x!

# ✅ Correct: Use comparison operator
if x == 5 {
    println("x is 5")
}
```

### Mistake 2: String Concatenation with Numbers

```klang
let age = 25
# ❌ May not work
# println("Age: " + age)

# ✅ Correct: Convert to string
println("Age: " + str(age))
```

### Mistake 3: Incorrect Logical Operators

```klang
# ❌ Wrong: Using & or | (bitwise, if supported)
# if age >= 18 & hasLicense { }

# ✅ Correct: Use && and ||
if age >= 18 && hasLicense { }
```

### Mistake 4: Division by Zero

```klang
# ❌ Unchecked division
# let result = a / b  # Error if b is 0

# ✅ Safe division
let result = b != 0 ? a / b : 0
```

## Summary

### Operator Categories

- **Arithmetic**: `+`, `-`, `*`, `/`, `%`
- **Assignment**: `=`
- **Comparison**: `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Logical**: `&&`, `||`, `!`
- **Ternary**: `? :`
- **String**: `+` (concatenation)
- **Type**: `type()`

### Key Takeaways

1. Use parentheses for clarity
2. Remember operator precedence
3. Explicit type conversion for safety
4. Short-circuit evaluation for null checks
5. Ternary for simple conditionals, if-else for complex logic

## Next Steps

Now that you understand operators, continue with:

- **[Control Flow](12-control-flow.md)** - Use operators in conditionals and loops
- **[Functions](13-functions.md)** - Build logic with functions
- **[Arrays](14-arrays.md)** - Array manipulation with operators
- **[Variables & Types](10-variables-types.md)** - Review type system basics

---

**Master operators to write expressive and efficient KLang code!** 🚀
