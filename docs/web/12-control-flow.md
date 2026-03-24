# Control Flow

## Mastering Program Flow Control in KLang

Control flow statements determine the order in which code executes. KLang provides powerful control structures for building complex logic.

## Conditional Statements

### If Statement

Execute code based on conditions:

```klang
# Simple if
let age = 25
if age >= 18 {
    println("You are an adult")
}

# If with comparison
let score = 85
if score >= 60 {
    println("You passed!")
}

# Multiple statements in block
let temperature = 30
if temperature > 25 {
    println("It's hot outside")
    println("Stay hydrated!")
}
```

### If-Else Statement

Provide an alternative when condition is false:

```klang
let age = 15
if age >= 18 {
    println("You can vote")
} else {
    println("You cannot vote yet")
}

# With expressions
let score = 55
if score >= 60 {
    println("Pass")
} else {
    println("Fail")
}

# Complex conditions
let hasTicket = true
let isVIP = false

if hasTicket && isVIP {
    println("VIP entrance")
} else {
    println("Regular entrance")
}
```

### If-Else If-Else Chain

Handle multiple conditions:

```klang
# Grade calculation
let score = 85

if score >= 90 {
    println("Grade: A")
} else if score >= 80 {
    println("Grade: B")
} else if score >= 70 {
    println("Grade: C")
} else if score >= 60 {
    println("Grade: D")
} else {
    println("Grade: F")
}

# Traffic light logic
let light = "yellow"

if light == "red" {
    println("Stop")
} else if light == "yellow" {
    println("Slow down")
} else if light == "green" {
    println("Go")
} else {
    println("Invalid light color")
}

# Range checking
let value = 50

if value < 0 {
    println("Negative")
} else if value == 0 {
    println("Zero")
} else if value <= 100 {
    println("Small positive")
} else {
    println("Large positive")
}
```

### Nested If Statements

If statements inside other if statements:

```klang
let age = 25
let hasLicense = true
let hasInsurance = true

if age >= 18 {
    println("Age requirement met")
    
    if hasLicense {
        println("License verified")
        
        if hasInsurance {
            println("Approved to drive!")
        } else {
            println("Need insurance")
        }
    } else {
        println("Need license")
    }
} else {
    println("Too young to drive")
}

# Login validation
let username = "alice"
let password = "secret123"

if len(username) > 0 {
    if len(password) >= 8 {
        if username == "alice" && password == "secret123" {
            println("Login successful")
        } else {
            println("Invalid credentials")
        }
    } else {
        println("Password too short")
    }
} else {
    println("Username required")
}
```

### Ternary Operator (Conditional Expression)

Shorthand for simple if-else:

```klang
# Basic ternary
let age = 20
let status = age >= 18 ? "adult" : "minor"
println(status)  # adult

# In expressions
let score = 85
println("Result: " + (score >= 60 ? "Pass" : "Fail"))

# Finding maximum
let a = 10
let b = 20
let max = a > b ? a : b
println("Max: " + str(max))  # Max: 20

# Nested ternary (use sparingly)
let score = 85
let grade = score >= 90 ? "A" :
            score >= 80 ? "B" :
            score >= 70 ? "C" :
            score >= 60 ? "D" : "F"
println(grade)  # B
```

## Loops

### For Loop (C-Style)

Traditional for loop with initialization, condition, and increment:

```klang
# Basic for loop
for (let i = 0; i < 5; i = i + 1) {
    println("Count: " + str(i))
}
# Output: 0, 1, 2, 3, 4

# Counting down
for (let i = 5; i > 0; i = i - 1) {
    println("Countdown: " + str(i))
}
# Output: 5, 4, 3, 2, 1

# Custom increment
for (let i = 0; i < 10; i = i + 2) {
    println("Even: " + str(i))
}
# Output: 0, 2, 4, 6, 8

# Multiple variables (if supported)
for (let i = 0; i < 3; i = i + 1) {
    for (let j = 0; j < 3; j = j + 1) {
        println(str(i) + "," + str(j))
    }
}
```

### For-In Loop (Range-Based)

Iterate over a range of numbers:

```klang
# Basic range
for i in 0..5 {
    println("Index: " + str(i))
}
# Output: 0, 1, 2, 3, 4

# Using range variable
for count in 1..11 {
    println("Count: " + str(count))
}
# Output: 1, 2, 3, ... 10

# With array indices
let fruits = ["apple", "banana", "cherry"]
for i in 0..len(fruits) {
    println(str(i) + ": " + fruits[i])
}
# Output:
# 0: apple
# 1: banana
# 2: cherry

# Nested ranges
for i in 0..3 {
    for j in 0..3 {
        println("(" + str(i) + ", " + str(j) + ")")
    }
}
```

### For-Of Loop (Array Iteration)

Iterate over array elements directly:

```klang
# Iterate over array elements
let colors = ["red", "green", "blue"]
for color of colors {
    println("Color: " + color)
}
# Output: Color: red, Color: green, Color: blue

# With numbers
let numbers = [10, 20, 30, 40, 50]
for num of numbers {
    println("Number: " + str(num))
}

# Processing each element
let prices = [10.99, 25.50, 5.00]
for price of prices {
    let withTax = price * 1.1
    println("Price with tax: $" + str(withTax))
}

# Building results
let scores = [85, 92, 78, 95]
let total = 0
for score of scores {
    total = total + score
}
let average = total / len(scores)
println("Average: " + str(average))
```

### While Loop

Repeat while condition is true:

```klang
# Basic while loop
let count = 0
while count < 5 {
    println("Count: " + str(count))
    count = count + 1
}
# Output: 0, 1, 2, 3, 4

# Countdown
let timer = 5
while timer > 0 {
    println("Time remaining: " + str(timer))
    timer = timer - 1
}
println("Time's up!")

# User input simulation (conceptual)
let password = ""
let attempts = 3
while attempts > 0 && password != "secret" {
    println("Enter password (attempts remaining: " + str(attempts) + ")")
    # password = input()  # If input function exists
    attempts = attempts - 1
}

# Accumulator pattern
let sum = 0
let i = 1
while i <= 10 {
    sum = sum + i
    i = i + 1
}
println("Sum: " + str(sum))  # 55
```

### Do-While Loop (If Supported)

Execute at least once, then repeat while condition is true:

```klang
# Note: Check if KLang supports do-while
# If not supported, use while with initial execution

# Pattern (if supported):
# do {
#     # Code here
# } while (condition)

# Alternative using while:
let count = 0
count = count + 1  # Execute once
println("Count: " + str(count))

while count < 5 {
    count = count + 1
    println("Count: " + str(count))
}
```

## Loop Control

### Break Statement

Exit loop immediately:

```klang
# Break on condition
for i in 0..10 {
    if i == 5 {
        println("Breaking at " + str(i))
        break
    }
    println("i = " + str(i))
}
# Output: 0, 1, 2, 3, 4, Breaking at 5

# Search pattern
let numbers = [10, 25, 30, 45, 50]
let target = 30
let found = false

for num of numbers {
    if num == target {
        println("Found " + str(target) + "!")
        found = true
        break
    }
}

if !found {
    println("Not found")
}

# C-style for with break
for (let i = 0; i < 100; i = i + 1) {
    if i * i > 50 {
        println("Stopping at i = " + str(i))
        break
    }
    println(str(i) + " squared = " + str(i * i))
}
```

### Continue Statement

Skip to next iteration:

```klang
# Skip even numbers
for i in 0..10 {
    if i % 2 == 0 {
        continue  # Skip even numbers
    }
    println("Odd: " + str(i))
}
# Output: 1, 3, 5, 7, 9

# Skip specific values
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
for num of numbers {
    if num == 5 {
        println("Skipping 5")
        continue
    }
    println("Processing: " + str(num))
}

# Filter while iterating
for i in 1..21 {
    if i % 3 != 0 {
        continue  # Skip non-multiples of 3
    }
    println("Multiple of 3: " + str(i))
}
# Output: 3, 6, 9, 12, 15, 18

# C-style with continue
for (let i = 0; i < 10; i = i + 1) {
    if i == 2 || i == 7 {
        continue
    }
    println("Value: " + str(i))
}
```

### Nested Loop Control

```klang
# Break from nested loop
let found = false
for i in 0..5 {
    for j in 0..5 {
        if i == 2 && j == 3 {
            println("Found at (" + str(i) + ", " + str(j) + ")")
            found = true
            break  # Breaks inner loop only
        }
    }
    if found {
        break  # Breaks outer loop
    }
}

# Continue in nested loops
for i in 0..3 {
    for j in 0..3 {
        if j == 1 {
            continue  # Skip j=1, continues inner loop
        }
        println("i=" + str(i) + ", j=" + str(j))
    }
}
```

## Pattern Matching (If Supported)

Some languages support switch/match statements. Here's the equivalent pattern in KLang:

### Switch-Like Pattern with If-Else

```klang
let day = "Monday"

if day == "Monday" {
    println("Start of work week")
} else if day == "Tuesday" || day == "Wednesday" || day == "Thursday" {
    println("Midweek")
} else if day == "Friday" {
    println("TGIF!")
} else if day == "Saturday" || day == "Sunday" {
    println("Weekend!")
} else {
    println("Invalid day")
}

# Using a function
fn getDayType(day) {
    if day == "Saturday" || day == "Sunday" {
        return "weekend"
    } else if day == "Monday" || day == "Tuesday" || day == "Wednesday" || day == "Thursday" || day == "Friday" {
        return "weekday"
    } else {
        return "invalid"
    }
}

println(getDayType("Monday"))    # weekday
println(getDayType("Sunday"))    # weekend
```

### Object-Based Dispatch

```klang
# Simulate switch with object lookup
let actions = {
    start: "Starting...",
    stop: "Stopping...",
    pause: "Pausing...",
    resume: "Resuming..."
}

let command = "start"
let message = actions[command]  # Lookup
if message != null {
    println(message)
} else {
    println("Unknown command")
}
```

## Common Patterns

### Pattern 1: Guard Clauses

Early return to simplify logic:

```klang
fn processUser(user) {
    # Guard: check if user exists
    if user == null {
        return "Error: No user"
    }
    
    # Guard: check if user has name
    if type(user.name) != "string" {
        return "Error: Invalid name"
    }
    
    # Guard: check if user is active
    if !user.active {
        return "Error: Inactive user"
    }
    
    # Main logic
    return "Processing: " + user.name
}

# Example with numbers
fn divide(a, b) {
    if b == 0 {
        return null  # Guard against division by zero
    }
    return a / b
}
```

### Pattern 2: Loop Accumulator

Collect values while iterating:

```klang
# Sum array
let numbers = [1, 2, 3, 4, 5]
let sum = 0
for num of numbers {
    sum = sum + num
}
println("Sum: " + str(sum))  # 15

# Build result array
let values = [1, 2, 3, 4, 5]
let doubled = []
for val of values {
    doubled.push(val * 2)
}
println(doubled)  # [2, 4, 6, 8, 10]

# Count matches
let words = ["apple", "banana", "apricot", "cherry"]
let aCount = 0
for word of words {
    if word[0] == "a" {  # Assuming string indexing
        aCount = aCount + 1
    }
}
println("Words starting with 'a': " + str(aCount))  # 2
```

### Pattern 3: Search and Find

Find element in collection:

```klang
# Linear search
let numbers = [10, 25, 30, 45, 50]
let target = 30
let foundIndex = -1

for i in 0..len(numbers) {
    if numbers[i] == target {
        foundIndex = i
        break
    }
}

if foundIndex != -1 {
    println("Found at index: " + str(foundIndex))
} else {
    println("Not found")
}

# Find first matching condition
let ages = [15, 22, 17, 30, 25]
let firstAdult = -1

for i in 0..len(ages) {
    if ages[i] >= 18 {
        firstAdult = ages[i]
        break
    }
}

println("First adult age: " + str(firstAdult))  # 22
```

### Pattern 4: Validation Loop

Validate all items:

```klang
# Check if all items meet condition
let scores = [85, 92, 78, 95, 88]
let allPassed = true

for score of scores {
    if score < 60 {
        allPassed = false
        break
    }
}

println("All passed: " + str(allPassed))  # true

# Check if any item meets condition
let numbers = [2, 4, 6, 7, 8]
let hasOdd = false

for num of numbers {
    if num % 2 != 0 {
        hasOdd = true
        break
    }
}

println("Has odd number: " + str(hasOdd))  # true
```

### Pattern 5: Two-Pointer Pattern

Process from both ends:

```klang
# Reverse array
let arr = [1, 2, 3, 4, 5]
let reversed = []

for i in 0..len(arr) {
    let reverseIndex = len(arr) - 1 - i
    reversed.push(arr[reverseIndex])
}

println(reversed)  # [5, 4, 3, 2, 1]

# Check palindrome (with string)
fn isPalindrome(text) {
    let len = len(text)
    for i in 0..(len / 2) {
        if text[i] != text[len - 1 - i] {
            return false
        }
    }
    return true
}
```

### Pattern 6: Nested Iteration

Process 2D structures:

```klang
# Multiplication table
for i in 1..11 {
    for j in 1..11 {
        let product = i * j
        print(str(product) + "\t")
    }
    println("")  # New line after each row
}

# Grid traversal
let grid = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
]

for row of grid {
    for cell of row {
        print(str(cell) + " ")
    }
    println("")
}
# Output:
# 1 2 3
# 4 5 6
# 7 8 9
```

### Pattern 7: State Machine

Track state through iterations:

```klang
# Simple state machine
let state = "idle"
let count = 0

while count < 10 {
    if state == "idle" {
        println("State: idle")
        state = "running"
    } else if state == "running" {
        println("State: running")
        if count >= 5 {
            state = "done"
        }
    } else if state == "done" {
        println("State: done")
        break
    }
    
    count = count + 1
}
```

## Advanced Control Flow

### Loop with Multiple Conditions

```klang
# Multiple exit conditions
let count = 0
let maxAttempts = 10
let found = false

while count < maxAttempts && !found {
    println("Attempt " + str(count + 1))
    
    # Simulate search
    if count == 5 {
        found = true
        println("Found!")
    }
    
    count = count + 1
}

if !found {
    println("Not found after " + str(maxAttempts) + " attempts")
}
```

### Infinite Loop with Break

```klang
# Infinite loop pattern (use with caution)
let counter = 0
while true {
    println("Iteration: " + str(counter))
    counter = counter + 1
    
    if counter >= 5 {
        println("Exiting loop")
        break
    }
}

# Menu-like pattern
let running = true
let choice = 1

while running {
    println("Menu: 1=Continue, 2=Exit")
    
    if choice == 1 {
        println("Processing...")
        choice = 2  # Change for demo
    } else if choice == 2 {
        println("Exiting...")
        running = false
    }
}
```

## Best Practices

### 1. Prefer For-Of Over Indices

```klang
let items = ["a", "b", "c"]

# ✅ Good: Direct iteration
for item of items {
    println(item)
}

# ❌ Less ideal: Manual indexing (when not needed)
for i in 0..len(items) {
    println(items[i])
}

# ✅ Good: When you need index
for i in 0..len(items) {
    println(str(i) + ": " + items[i])
}
```

### 2. Use Guard Clauses

```klang
# ✅ Good: Early returns
fn processItem(item) {
    if item == null {
        return
    }
    if !item.valid {
        return
    }
    
    # Process item
    println("Processing: " + item.name)
}

# ❌ Avoid: Deep nesting
fn processItem(item) {
    if item != null {
        if item.valid {
            # Process item
            println("Processing: " + item.name)
        }
    }
}
```

### 3. Limit Loop Nesting

```klang
# ✅ Good: Extract to function
fn processRow(row, rowIndex) {
    for colIndex in 0..len(row) {
        println("(" + str(rowIndex) + "," + str(colIndex) + "): " + str(row[colIndex]))
    }
}

let grid = [[1, 2], [3, 4]]
for i in 0..len(grid) {
    processRow(grid[i], i)
}

# ❌ Avoid: Deep nesting (when extractable)
for i in 0..len(grid) {
    for j in 0..len(grid[i]) {
        for k in 0..len(grid[i][j]) {  # Too deep!
            # ...
        }
    }
}
```

### 4. Clear Loop Boundaries

```klang
# ✅ Good: Clear intent
const MAX_ATTEMPTS = 5
for attempt in 0..MAX_ATTEMPTS {
    println("Attempt " + str(attempt + 1))
}

# ❌ Avoid: Magic numbers
for attempt in 0..5 {
    println("Attempt " + str(attempt + 1))
}
```

### 5. Avoid Complex Conditions

```klang
# ✅ Good: Break into parts
let isWeekday = day != "Saturday" && day != "Sunday"
let isBusinessHour = hour >= 9 && hour <= 17

if isWeekday && isBusinessHour {
    println("Office is open")
}

# ❌ Avoid: Long compound conditions
if (day != "Saturday" && day != "Sunday") && (hour >= 9 && hour <= 17) && (temp > 0) {
    # Hard to read
}
```

## Common Mistakes

### Mistake 1: Off-By-One Errors

```klang
let arr = [1, 2, 3, 4, 5]

# ❌ Wrong: Misses last element
for i in 0..len(arr) - 1 {
    println(arr[i])
}

# ✅ Correct: Includes all elements
for i in 0..len(arr) {
    println(arr[i])
}
```

### Mistake 2: Infinite Loops

```klang
# ❌ Wrong: Never increments
let i = 0
while i < 10 {
    println(i)
    # Forgot: i = i + 1
}

# ✅ Correct: Increments counter
let i = 0
while i < 10 {
    println(i)
    i = i + 1
}
```

### Mistake 3: Modifying Loop Variable

```klang
# ⚠️ Caution: Modifying loop variable
for i in 0..10 {
    println(i)
    # i = i + 1  # Don't modify range variable
}

# ✅ Use C-style for if you need custom control
for (let i = 0; i < 10; i = i + 1) {
    if i % 2 == 0 {
        i = i + 1  # Now it's OK (though unusual)
    }
    println(i)
}
```

### Mistake 4: Wrong Break Scope

```klang
# ⚠️ Breaks only inner loop
for i in 0..5 {
    for j in 0..5 {
        if j == 2 {
            break  # Only breaks inner loop
        }
    }
}

# ✅ Use flag to break outer loop
let shouldBreak = false
for i in 0..5 {
    for j in 0..5 {
        if j == 2 {
            shouldBreak = true
            break
        }
    }
    if shouldBreak {
        break
    }
}
```

## Summary

### Conditional Statements
- **if**: Execute code when condition is true
- **if-else**: Provide alternative path
- **if-else if-else**: Handle multiple conditions
- **ternary**: Shorthand conditional expression

### Loops
- **for (C-style)**: Traditional loop with full control
- **for-in**: Iterate over range
- **for-of**: Iterate over array elements
- **while**: Loop while condition is true

### Loop Control
- **break**: Exit loop immediately
- **continue**: Skip to next iteration

### Best Practices
1. Use guard clauses for early returns
2. Prefer for-of when you don't need indices
3. Extract nested loops to functions
4. Use clear variable names and constants
5. Avoid infinite loops and off-by-one errors

## Next Steps

Master control flow, then explore:

- **[Functions](13-functions.md)** - Build reusable logic
- **[Arrays](14-arrays.md)** - Array iteration and methods
- **[Operators](11-operators.md)** - Review comparison and logical operators
- **[Variables & Types](10-variables-types.md)** - Variable scoping in loops

---

**Control the flow of your programs with confidence!** 🚀
