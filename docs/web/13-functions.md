# Functions

## Mastering Functions in KLang

Functions are reusable blocks of code that perform specific tasks. KLang supports multiple function declaration styles, closures, and advanced functional programming patterns.

## Function Basics

### Function Declaration

Define named functions with the `fn` keyword:

```klang
# Basic function
fn greet() {
    println("Hello, World!")
}

greet()  # Call the function
# Output: Hello, World!

# Function with parameters
fn greetPerson(name) {
    println("Hello, " + name + "!")
}

greetPerson("Alice")  # Hello, Alice!
greetPerson("Bob")    # Hello, Bob!

# Function with return value
fn add(a, b) {
    return a + b
}

let sum = add(5, 3)
println("Sum: " + str(sum))  # Sum: 8
```

### Function Parameters

Functions can accept zero or more parameters:

```klang
# No parameters
fn sayHello() {
    return "Hello!"
}

# One parameter
fn square(x) {
    return x * x
}

# Multiple parameters
fn multiply(a, b) {
    return a * b
}

# Many parameters
fn createUser(name, age, email, city) {
    return {
        name: name,
        age: age,
        email: email,
        city: city
    }
}

let user = createUser("Alice", 25, "alice@example.com", "NYC")
println(user.name)  # Alice
```

### Return Values

Functions can return values or return nothing:

```klang
# Return a value
fn getDouble(x) {
    return x * 2
}

let result = getDouble(5)
println(result)  # 10

# Return early
fn divide(a, b) {
    if b == 0 {
        return null  # Early return
    }
    return a / b
}

println(divide(10, 2))  # 5
println(divide(10, 0))  # null

# No explicit return (returns null/undefined)
fn logMessage(msg) {
    println("LOG: " + msg)
    # No return statement
}

let x = logMessage("Hello")  # x is null/undefined

# Multiple return points
fn getGrade(score) {
    if score >= 90 {
        return "A"
    } else if score >= 80 {
        return "B"
    } else if score >= 70 {
        return "C"
    } else if score >= 60 {
        return "D"
    } else {
        return "F"
    }
}
```

### Type Annotations (Optional)

Add type hints for better documentation:

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

# Mixed types
fn formatPrice(amount: float) -> string {
    return "$" + str(amount)
}

println(formatPrice(19.99))  # $19.99
```

## Arrow Functions

Concise syntax for simple functions:

### Basic Arrow Functions

```klang
# Single parameter (parentheses optional)
let square = x => x * x
println(square(5))  # 25

# Multiple parameters (parentheses required)
let add = (a, b) => a + b
println(add(3, 7))  # 10

# No parameters (empty parentheses required)
let greet = () => "Hello!"
println(greet())  # Hello!

# Single expression (implicit return)
let double = x => x * 2
let isPositive = x => x > 0
let negate = x => -x

println(double(21))      # 42
println(isPositive(5))   # true
println(negate(10))      # -10
```

### Arrow Functions with Block Body

```klang
# Multi-line arrow function (explicit return needed)
let complex = x => {
    let doubled = x * 2
    let result = doubled + 1
    return result
}

println(complex(10))  # 21

# Arrow function with multiple statements
let processValue = x => {
    if x < 0 {
        return 0
    }
    if x > 100 {
        return 100
    }
    return x
}

println(processValue(-5))   # 0
println(processValue(50))   # 50
println(processValue(150))  # 100
```

### Arrow Functions in Context

```klang
# Array methods
let numbers = [1, 2, 3, 4, 5]

let doubled = numbers.map(x => x * 2)
println(doubled)  # [2, 4, 6, 8, 10]

let evens = numbers.filter(x => x % 2 == 0)
println(evens)  # [2, 4]

let sum = numbers.reduce((acc, x) => acc + x, 0)
println(sum)  # 15

# Inline arrow functions
let scores = [85, 92, 78, 95, 88]
let passed = scores.filter(score => score >= 80)
println("Passed: " + str(passed))  # [85, 92, 95, 88]
```

## Closures

Functions that capture variables from their outer scope:

### Basic Closures

```klang
# Inner function accesses outer variable
fn outer() {
    let x = 10
    fn inner() {
        return x + 5  # Accesses x from outer scope
    }
    return inner
}

let myFunc = outer()
println(myFunc())  # 15

# Closure with parameters
fn makeAdder(x) {
    fn adder(y) {
        return x + y  # Captures x
    }
    return adder
}

let add5 = makeAdder(5)
let add10 = makeAdder(10)

println(add5(3))   # 8
println(add10(3))  # 13
```

### Closures with State

```klang
# Counter closure
fn makeCounter() {
    let count = 0
    fn increment() {
        count = count + 1
        return count
    }
    return increment
}

let counter1 = makeCounter()
println(counter1())  # 1
println(counter1())  # 2
println(counter1())  # 3

let counter2 = makeCounter()
println(counter2())  # 1 (separate state)

# Private state pattern
fn createBankAccount(initialBalance) {
    let balance = initialBalance
    
    return {
        deposit: amount => {
            balance = balance + amount
            return balance
        },
        withdraw: amount => {
            if amount > balance {
                return "Insufficient funds"
            }
            balance = balance - amount
            return balance
        },
        getBalance: () => balance
    }
}

let account = createBankAccount(100)
println(account.deposit(50))       # 150
println(account.withdraw(30))      # 120
println(account.getBalance())      # 120
```

### Nested Closures

```klang
# Multiple levels of nesting
fn level1() {
    let a = 1
    fn level2() {
        let b = 2
        fn level3() {
            return a + b + 3  # Accesses both a and b
        }
        return level3
    }
    return level2()
}

let deepFunc = level1()
println(deepFunc())  # 6

# Closure factory
fn multiplierFactory(factor) {
    return x => x * factor
}

let double = multiplierFactory(2)
let triple = multiplierFactory(3)
let quadruple = multiplierFactory(4)

println(double(5))     # 10
println(triple(5))     # 15
println(quadruple(5))  # 20
```

## First-Class Functions

Functions are values that can be passed around:

### Functions as Arguments

```klang
# Higher-order function: accepts function as parameter
fn applyOperation(x, y, operation) {
    return operation(x, y)
}

fn add(a, b) {
    return a + b
}

fn multiply(a, b) {
    return a * b
}

println(applyOperation(5, 3, add))       # 8
println(applyOperation(5, 3, multiply))  # 15

# With arrow functions
let result1 = applyOperation(10, 2, (a, b) => a - b)  # 8
let result2 = applyOperation(10, 2, (a, b) => a / b)  # 5
```

### Functions as Return Values

```klang
# Return different functions based on condition
fn getOperation(op) {
    if op == "add" {
        return (a, b) => a + b
    } else if op == "multiply" {
        return (a, b) => a * b
    } else {
        return (a, b) => 0
    }
}

let addFunc = getOperation("add")
let multFunc = getOperation("multiply")

println(addFunc(5, 3))   # 8
println(multFunc(5, 3))  # 15
```

### Storing Functions

```klang
# Functions in arrays
let operations = [
    x => x + 1,
    x => x * 2,
    x => x * x
]

let value = 5
for op of operations {
    println(op(value))
}
# Output: 6, 10, 25

# Functions in objects
let math = {
    add: (a, b) => a + b,
    subtract: (a, b) => a - b,
    multiply: (a, b) => a * b,
    divide: (a, b) => b != 0 ? a / b : null
}

println(math.add(10, 5))      # 15
println(math.subtract(10, 5)) # 5
println(math.multiply(10, 5)) # 50
println(math.divide(10, 5))   # 2
```

## Functional Patterns

### Map, Filter, Reduce

```klang
# Map: transform each element
let numbers = [1, 2, 3, 4, 5]
let squared = numbers.map(x => x * x)
println(squared)  # [1, 4, 9, 16, 25]

# Filter: select elements
let evens = numbers.filter(x => x % 2 == 0)
println(evens)  # [2, 4]

# Reduce: combine elements
let sum = numbers.reduce((acc, x) => acc + x, 0)
println(sum)  # 15

# Chaining operations
let result = numbers
    .filter(x => x > 2)
    .map(x => x * 2)
    .reduce((acc, x) => acc + x, 0)
println(result)  # 24 (3*2 + 4*2 + 5*2 = 6 + 8 + 10)
```

### Composition

```klang
# Function composition
fn compose(f, g) {
    return x => f(g(x))
}

let addOne = x => x + 1
let double = x => x * 2

let addOneThenDouble = compose(double, addOne)
println(addOneThenDouble(5))  # 12 ((5 + 1) * 2)

let doubleThenAddOne = compose(addOne, double)
println(doubleThenAddOne(5))  # 11 ((5 * 2) + 1)
```

### Partial Application

```klang
# Create specialized functions from general ones
fn multiply(a, b) {
    return a * b
}

fn partial(func, x) {
    return y => func(x, y)
}

let multiplyBy5 = partial(multiply, 5)
let multiplyBy10 = partial(multiply, 10)

println(multiplyBy5(3))   # 15
println(multiplyBy10(3))  # 30
```

### Currying

```klang
# Transform multi-parameter function to chain of single-parameter functions
fn curriedAdd(a) {
    return b => a + b
}

let add5 = curriedAdd(5)
println(add5(10))  # 15

# More complex example
fn curriedMultiply(a) {
    return b => {
        return c => a * b * c
    }
}

let mult2 = curriedMultiply(2)
let mult2By3 = mult2(3)
println(mult2By3(4))  # 24 (2 * 3 * 4)

# Or call in sequence
println(curriedMultiply(2)(3)(4))  # 24
```

## Recursion

Functions that call themselves:

### Basic Recursion

```klang
# Factorial
fn factorial(n) {
    if n <= 1 {
        return 1
    }
    return n * factorial(n - 1)
}

println(factorial(5))  # 120 (5 * 4 * 3 * 2 * 1)

# Fibonacci
fn fibonacci(n) {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

println(fibonacci(7))  # 13 (0, 1, 1, 2, 3, 5, 8, 13)

# Countdown
fn countdown(n) {
    if n <= 0 {
        println("Done!")
        return
    }
    println("Count: " + str(n))
    countdown(n - 1)
}

countdown(5)
# Output: 5, 4, 3, 2, 1, Done!
```

### Tail Recursion

```klang
# Tail-recursive factorial (accumulator pattern)
fn factorialTail(n, acc) {
    if n <= 1 {
        return acc
    }
    return factorialTail(n - 1, n * acc)
}

fn factorial(n) {
    return factorialTail(n, 1)
}

println(factorial(5))  # 120

# Tail-recursive sum
fn sumList(arr, index, acc) {
    if index >= len(arr) {
        return acc
    }
    return sumList(arr, index + 1, acc + arr[index])
}

let numbers = [1, 2, 3, 4, 5]
println(sumList(numbers, 0, 0))  # 15
```

### Recursive Array Processing

```klang
# Sum array recursively
fn sumArray(arr, index) {
    if index >= len(arr) {
        return 0
    }
    return arr[index] + sumArray(arr, index + 1)
}

let nums = [1, 2, 3, 4, 5]
println(sumArray(nums, 0))  # 15

# Find maximum recursively
fn findMax(arr, index, currentMax) {
    if index >= len(arr) {
        return currentMax
    }
    let newMax = arr[index] > currentMax ? arr[index] : currentMax
    return findMax(arr, index + 1, newMax)
}

let values = [3, 7, 2, 9, 4]
println(findMax(values, 1, values[0]))  # 9
```

## Common Patterns

### Pattern 1: Callback Functions

```klang
# Execute callback after operation
fn processData(data, callback) {
    let result = data * 2
    callback(result)
}

processData(5, result => println("Result: " + str(result)))
# Output: Result: 10

# With validation
fn fetchUser(id, onSuccess, onError) {
    if id > 0 {
        let user = {id: id, name: "User" + str(id)}
        onSuccess(user)
    } else {
        onError("Invalid ID")
    }
}

fetchUser(123, 
    user => println("Found: " + user.name),
    error => println("Error: " + error)
)
```

### Pattern 2: Factory Functions

```klang
# Create objects with methods
fn createCounter(start) {
    let count = start
    return {
        increment: () => {
            count = count + 1
            return count
        },
        decrement: () => {
            count = count - 1
            return count
        },
        reset: () => {
            count = start
            return count
        },
        getValue: () => count
    }
}

let counter = createCounter(10)
println(counter.increment())  # 11
println(counter.increment())  # 12
println(counter.decrement())  # 11
println(counter.reset())      # 10
```

### Pattern 3: Decorator/Wrapper Functions

```klang
# Add logging to any function
fn withLogging(func) {
    return x => {
        println("Calling with: " + str(x))
        let result = func(x)
        println("Result: " + str(result))
        return result
    }
}

let square = x => x * x
let loggedSquare = withLogging(square)

loggedSquare(5)
# Output:
# Calling with: 5
# Result: 25

# Add timing
fn withTiming(func, name) {
    return x => {
        println("Starting " + name + "...")
        let result = func(x)
        println("Finished " + name)
        return result
    }
}
```

### Pattern 4: Memoization

```klang
# Cache function results
fn memoize(func) {
    let cache = {}
    return x => {
        # Simplified - proper implementation would use object/map
        # This is conceptual
        if cache[str(x)] != null {
            return cache[str(x)]
        }
        let result = func(x)
        cache[str(x)] = result
        return result
    }
}

# Without proper memoization support, use recursive pattern
fn fibonacci(n) {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}
```

### Pattern 5: Pipeline

```klang
# Chain function calls
fn pipeline(value, ...funcs) {
    # Conceptual - apply each function in sequence
    let result = value
    for func of funcs {
        result = func(result)
    }
    return result
}

# Or simpler version
fn pipe(value, f1, f2, f3) {
    return f3(f2(f1(value)))
}

let addOne = x => x + 1
let double = x => x * 2
let square = x => x * x

let result = pipe(5, addOne, double, square)
# 5 -> 6 -> 12 -> 144
println(result)
```

## Best Practices

### 1. Single Responsibility

```klang
# ✅ Good: Each function does one thing
fn isValidEmail(email) {
    return email.includes("@")
}

fn sendEmail(email, message) {
    if !isValidEmail(email) {
        return false
    }
    # Send email logic
    return true
}

# ❌ Avoid: Function does too much
fn sendEmailWithValidation(email, message, logToFile, notifyAdmin) {
    # Too many responsibilities
}
```

### 2. Descriptive Names

```klang
# ✅ Good: Clear names
fn calculateTotalPrice(items) { }
fn isUserAuthenticated(user) { }
fn fetchUserById(id) { }

# ❌ Avoid: Unclear names
fn calc(x) { }
fn check(u) { }
fn get(i) { }
```

### 3. Limit Parameters

```klang
# ✅ Good: Few parameters
fn createUser(name, email) { }

# ✅ Better: Use object for many parameters
fn createUser(userData) {
    # userData = {name, email, age, city, ...}
}

# ❌ Avoid: Too many parameters
fn createUser(name, email, age, city, country, phone, address) { }
```

### 4. Pure Functions When Possible

```klang
# ✅ Good: Pure function (no side effects)
fn add(a, b) {
    return a + b
}

# ⚠️ Impure: Has side effects
let globalCount = 0
fn incrementGlobal() {
    globalCount = globalCount + 1  # Modifies external state
}

# ✅ Better: Return new value
fn increment(count) {
    return count + 1
}
```

### 5. Early Returns

```klang
# ✅ Good: Guard clauses
fn processUser(user) {
    if user == null {
        return
    }
    if !user.active {
        return
    }
    # Process user
}

# ❌ Avoid: Deep nesting
fn processUser(user) {
    if user != null {
        if user.active {
            # Process user
        }
    }
}
```

## Common Mistakes

### Mistake 1: Forgetting Return

```klang
# ❌ Wrong: No return statement
fn add(a, b) {
    a + b  # Result not returned
}

# ✅ Correct: Explicit return
fn add(a, b) {
    return a + b
}
```

### Mistake 2: Modifying Parameters

```klang
# ⚠️ Caution: Modifying array parameter
fn addItem(arr, item) {
    arr.push(item)  # Modifies original
    return arr
}

# ✅ Better: Return new array
fn addItem(arr, item) {
    let newArr = [...arr]  # If spread operator supported
    newArr.push(item)
    return newArr
}
```

### Mistake 3: Infinite Recursion

```klang
# ❌ Wrong: Missing base case
fn countdown(n) {
    println(n)
    countdown(n - 1)  # Never stops!
}

# ✅ Correct: Has base case
fn countdown(n) {
    if n <= 0 {
        return
    }
    println(n)
    countdown(n - 1)
}
```

## Summary

### Function Types
- **Named functions**: `fn name() { }`
- **Arrow functions**: `x => x * 2`
- **Anonymous functions**: Passed directly as values

### Key Concepts
- **Parameters**: Input values
- **Return values**: Output from function
- **Closures**: Functions capturing outer scope
- **Recursion**: Functions calling themselves
- **Higher-order functions**: Functions that accept/return functions

### Best Practices
1. Single responsibility per function
2. Descriptive function names
3. Limit number of parameters
4. Use pure functions when possible
5. Implement early returns (guard clauses)

## Next Steps

Master functions, then explore:

- **[Arrays](14-arrays.md)** - Array methods using functions
- **[Objects](15-objects.md)** - Methods and object-oriented patterns
- **[Control Flow](12-control-flow.md)** - Using functions with loops
- **[Closures & Advanced](16-advanced-functions.md)** - Deep dive into functional programming

---

**Write clean, reusable, and powerful functions!** 🚀
