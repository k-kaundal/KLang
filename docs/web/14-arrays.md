# Arrays

## Complete Guide to Arrays in KLang

Arrays are ordered collections that can hold multiple values. KLang provides powerful array methods inspired by JavaScript, making data manipulation intuitive and expressive.

## Creating Arrays

### Array Literals

```klang
# Empty array
let empty = []

# Array with numbers
let numbers = [1, 2, 3, 4, 5]

# Array with strings
let fruits = ["apple", "banana", "cherry"]

# Mixed types
let mixed = [1, "hello", true, [1, 2], {name: "Alice"}]

# Multi-line arrays
let config = [
    "option1",
    "option2",
    "option3"
]
```

### Array Size

```klang
let items = [1, 2, 3, 4, 5]
let count = len(items)
println("Array has " + str(count) + " elements")  # 5

# Empty array check
let empty = []
if len(empty) == 0 {
    println("Array is empty")
}
```

## Accessing Elements

### Indexing

```klang
let fruits = ["apple", "banana", "cherry", "date"]

# Access by index (0-based)
println(fruits[0])  # apple
println(fruits[1])  # banana
println(fruits[3])  # date

# Last element
let lastIndex = len(fruits) - 1
println(fruits[lastIndex])  # date

# First and last
let first = fruits[0]
let last = fruits[len(fruits) - 1]
println("First: " + first + ", Last: " + last)
```

### Modifying Elements

```klang
let numbers = [1, 2, 3, 4, 5]

# Change element at index
numbers[0] = 10
numbers[2] = 30
println(numbers)  # [10, 2, 30, 4, 5]

# Update based on current value
numbers[1] = numbers[1] * 2
println(numbers)  # [10, 4, 30, 4, 5]
```

## Array Methods

### push() - Add to End

```klang
let fruits = ["apple", "banana"]

# Add single element
fruits.push("cherry")
println(fruits)  # ["apple", "banana", "cherry"]

# Add multiple elements
fruits.push("date")
fruits.push("elderberry")
println(fruits)  # ["apple", "banana", "cherry", "date", "elderberry"]
```

### pop() - Remove from End

```klang
let fruits = ["apple", "banana", "cherry"]

# Remove last element
let last = fruits.pop()
println("Removed: " + last)  # cherry
println(fruits)  # ["apple", "banana"]
```

### unshift() - Add to Beginning

```klang
let numbers = [2, 3, 4]

# Add to beginning
numbers.unshift(1)
println(numbers)  # [1, 2, 3, 4]

numbers.unshift(0)
println(numbers)  # [0, 1, 2, 3, 4]
```

### shift() - Remove from Beginning

```klang
let numbers = [1, 2, 3, 4, 5]

# Remove first element
let first = numbers.shift()
println("Removed: " + str(first))  # 1
println(numbers)  # [2, 3, 4, 5]
```

## Transformation Methods

### map() - Transform Each Element

Apply a function to each element and return new array:

```klang
let numbers = [1, 2, 3, 4, 5]

# Double each number
let doubled = numbers.map(x => x * 2)
println(doubled)  # [2, 4, 6, 8, 10]

# Square each number
let squared = numbers.map(x => x * x)
println(squared)  # [1, 4, 9, 16, 25]

# With named function
fn addTen(x) {
    return x + 10
}
let increased = numbers.map(addTen)
println(increased)  # [11, 12, 13, 14, 15]

# Transform strings
let words = ["hello", "world"]
let capitalized = words.map(w => uppercase(w))
println(capitalized)  # ["HELLO", "WORLD"]

# Complex transformation
let prices = [10, 20, 30]
let withTax = prices.map(price => price * 1.1)
println(withTax)  # [11, 22, 33]
```

### filter() - Select Elements

Keep only elements that pass a test:

```klang
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

# Get even numbers
let evens = numbers.filter(x => x % 2 == 0)
println(evens)  # [2, 4, 6, 8, 10]

# Get odd numbers
let odds = numbers.filter(x => x % 2 != 0)
println(odds)  # [1, 3, 5, 7, 9]

# Get numbers greater than 5
let bigNumbers = numbers.filter(x => x > 5)
println(bigNumbers)  # [6, 7, 8, 9, 10]

# Filter strings by length
let words = ["hi", "hello", "hey", "goodbye"]
let longWords = words.filter(w => len(w) > 3)
println(longWords)  # ["hello", "goodbye"]

# Multiple conditions
let scores = [45, 67, 89, 92, 55, 78]
let passedWell = scores.filter(s => s >= 60 && s < 90)
println(passedWell)  # [67, 78]
```

### reduce() - Combine Elements

Reduce array to single value:

```klang
let numbers = [1, 2, 3, 4, 5]

# Sum all numbers
let sum = numbers.reduce((acc, x) => acc + x, 0)
println(sum)  # 15

# Product of all numbers
let product = numbers.reduce((acc, x) => acc * x, 1)
println(product)  # 120

# Find maximum
let values = [3, 7, 2, 9, 4]
let max = values.reduce((acc, x) => x > acc ? x : acc, values[0])
println(max)  # 9

# Find minimum
let min = values.reduce((acc, x) => x < acc ? x : acc, values[0])
println(min)  # 2

# Count occurrences (conceptual)
let items = [1, 2, 3, 2, 1, 3, 2]
let count = items.reduce((acc, x) => {
    # Conceptual - would need object support
    return acc + (x == 2 ? 1 : 0)
}, 0)
println("Number of 2s: " + str(count))  # 3

# Calculate average
let scores = [85, 92, 78, 95, 88]
let total = scores.reduce((sum, score) => sum + score, 0)
let average = total / len(scores)
println("Average: " + str(average))  # 87.6
```

### forEach() - Execute for Each

Run function for each element (no return value):

```klang
let fruits = ["apple", "banana", "cherry"]

# Print each element
fruits.forEach(fruit => println(fruit))
# Output:
# apple
# banana
# cherry

# With index (if supported)
fruits.forEach((fruit, index) => {
    println(str(index + 1) + ". " + fruit)
})
# Output:
# 1. apple
# 2. banana
# 3. cherry

# Side effects
let sum = 0
let numbers = [1, 2, 3, 4, 5]
numbers.forEach(n => {
    sum = sum + n
})
println("Total: " + str(sum))  # 15
```

## Search and Test Methods

### find() - Find First Match

```klang
let numbers = [5, 12, 8, 130, 44]

# Find first number > 10
let found = numbers.find(x => x > 10)
println(found)  # 12

# Find first even number
let firstEven = numbers.find(x => x % 2 == 0)
println(firstEven)  # 12

# No match returns undefined/null
let notFound = numbers.find(x => x > 1000)
println(notFound)  # null or undefined
```

### findIndex() - Find Index

```klang
let numbers = [5, 12, 8, 130, 44]

# Find index of first number > 10
let index = numbers.findIndex(x => x > 10)
println(index)  # 1

# Not found returns -1
let notFound = numbers.findIndex(x => x > 1000)
println(notFound)  # -1
```

### includes() - Check if Contains

```klang
let fruits = ["apple", "banana", "cherry"]

# Check if array includes value
println(fruits.includes("banana"))  # true
println(fruits.includes("grape"))   # false

let numbers = [1, 2, 3, 4, 5]
println(numbers.includes(3))  # true
println(numbers.includes(10)) # false
```

### indexOf() - Find Index of Value

```klang
let fruits = ["apple", "banana", "cherry", "banana"]

# Find first occurrence
let index = fruits.indexOf("banana")
println(index)  # 1

# Not found returns -1
let notFound = fruits.indexOf("grape")
println(notFound)  # -1

# With numbers
let numbers = [10, 20, 30, 40]
println(numbers.indexOf(30))  # 2
```

### some() - Test If Any Match

```klang
let numbers = [1, 2, 3, 4, 5]

# Check if any element is even
let hasEven = numbers.some(x => x % 2 == 0)
println(hasEven)  # true

# Check if any element > 10
let hasBig = numbers.some(x => x > 10)
println(hasBig)  # false

let ages = [15, 17, 14, 16]
let hasAdult = ages.some(age => age >= 18)
println(hasAdult)  # false
```

### every() - Test If All Match

```klang
let numbers = [2, 4, 6, 8, 10]

# Check if all elements are even
let allEven = numbers.every(x => x % 2 == 0)
println(allEven)  # true

# Check if all elements > 0
let allPositive = numbers.every(x => x > 0)
println(allPositive)  # true

let ages = [25, 30, 35, 40]
let allAdults = ages.every(age => age >= 18)
println(allAdults)  # true
```

## Array Manipulation

### slice() - Extract Subarray

```klang
let fruits = ["apple", "banana", "cherry", "date", "elderberry"]

# Get elements from index 1 to 3 (exclusive)
let sliced = fruits.slice(1, 3)
println(sliced)  # ["banana", "cherry"]

# From index 2 to end
let fromTwo = fruits.slice(2)
println(fromTwo)  # ["cherry", "date", "elderberry"]

# First 3 elements
let firstThree = fruits.slice(0, 3)
println(firstThree)  # ["apple", "banana", "cherry"]
```

### concat() - Join Arrays

```klang
let arr1 = [1, 2, 3]
let arr2 = [4, 5, 6]

# Combine arrays
let combined = arr1.concat(arr2)
println(combined)  # [1, 2, 3, 4, 5, 6]

# Concat multiple arrays
let arr3 = [7, 8, 9]
let all = arr1.concat(arr2).concat(arr3)
println(all)  # [1, 2, 3, 4, 5, 6, 7, 8, 9]

# Original arrays unchanged
println(arr1)  # [1, 2, 3]
```

### join() - Convert to String

```klang
let words = ["Hello", "World", "KLang"]

# Join with space
let sentence = words.join(" ")
println(sentence)  # "Hello World KLang"

# Join with comma
let csv = words.join(",")
println(csv)  # "Hello,World,KLang"

# Join with custom separator
let dashed = words.join(" - ")
println(dashed)  # "Hello - World - KLang"

# Numbers
let numbers = [1, 2, 3, 4, 5]
let str = numbers.join(", ")
println(str)  # "1, 2, 3, 4, 5"
```

### reverse() - Reverse Order

```klang
let numbers = [1, 2, 3, 4, 5]

# Reverse the array (modifies original)
numbers.reverse()
println(numbers)  # [5, 4, 3, 2, 1]

# Reverse strings
let words = ["first", "second", "third"]
words.reverse()
println(words)  # ["third", "second", "first"]
```

### sort() - Sort Elements

```klang
# Sort numbers
let numbers = [3, 1, 4, 1, 5, 9, 2]
numbers.sort()
println(numbers)  # [1, 1, 2, 3, 4, 5, 9]

# Sort strings
let fruits = ["cherry", "apple", "banana"]
fruits.sort()
println(fruits)  # ["apple", "banana", "cherry"]

# Custom sort (if supported)
let nums = [1, 2, 10, 20]
# nums.sort((a, b) => a - b)  # Numeric sort
```

## Advanced Patterns

### Chaining Methods

```klang
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

# Chain multiple operations
let result = numbers
    .filter(x => x % 2 == 0)        # [2, 4, 6, 8, 10]
    .map(x => x * 2)                # [4, 8, 12, 16, 20]
    .filter(x => x > 10)            # [12, 16, 20]
    .reduce((sum, x) => sum + x, 0) # 48

println(result)  # 48

# Complex data pipeline
let scores = [45, 67, 89, 92, 55, 78, 95]
let topScoresAverage = scores
    .filter(s => s >= 80)           # Get high scores
    .map(s => s * 1.1)              # Add 10% bonus
    .reduce((sum, s) => sum + s, 0) # Sum them
    / 3                             # Average of 3 top scores

println("Top scores avg: " + str(topScoresAverage))
```

### Array Flattening

```klang
# Manual flattening of nested arrays
let nested = [[1, 2], [3, 4], [5, 6]]
let flat = []

for arr of nested {
    for item of arr {
        flat.push(item)
    }
}

println(flat)  # [1, 2, 3, 4, 5, 6]

# Using reduce
let flattened = nested.reduce((acc, arr) => {
    for item of arr {
        acc.push(item)
    }
    return acc
}, [])
```

### Removing Duplicates

```klang
# Remove duplicates (manual)
let numbers = [1, 2, 3, 2, 4, 1, 5, 3]
let unique = []

for num of numbers {
    if !unique.includes(num) {
        unique.push(num)
    }
}

println(unique)  # [1, 2, 3, 4, 5]
```

### Grouping/Partitioning

```klang
# Partition into even and odd
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

let evens = numbers.filter(x => x % 2 == 0)
let odds = numbers.filter(x => x % 2 != 0)

println("Evens: " + str(evens))  # [2, 4, 6, 8, 10]
println("Odds: " + str(odds))    # [1, 3, 5, 7, 9]

# Group by condition
let scores = [45, 67, 89, 92, 55, 78, 95]
let passed = scores.filter(s => s >= 60)
let failed = scores.filter(s => s < 60)

println("Passed: " + str(passed))  # [67, 89, 92, 78, 95]
println("Failed: " + str(failed))  # [45, 55]
```

## Common Patterns

### Pattern 1: Array as Stack

```klang
# Stack operations (LIFO - Last In First Out)
let stack = []

# Push to stack
stack.push(1)
stack.push(2)
stack.push(3)
println(stack)  # [1, 2, 3]

# Pop from stack
let top = stack.pop()
println("Popped: " + str(top))  # 3
println(stack)  # [1, 2]
```

### Pattern 2: Array as Queue

```klang
# Queue operations (FIFO - First In First Out)
let queue = []

# Enqueue (add to end)
queue.push(1)
queue.push(2)
queue.push(3)
println(queue)  # [1, 2, 3]

# Dequeue (remove from front)
let first = queue.shift()
println("Dequeued: " + str(first))  # 1
println(queue)  # [2, 3]
```

### Pattern 3: Array Building

```klang
# Build array with loop
let squares = []
for i in 1..11 {
    squares.push(i * i)
}
println(squares)  # [1, 4, 9, 16, 25, 36, 49, 64, 81, 100]

# Build with condition
let evenSquares = []
for i in 1..11 {
    if i % 2 == 0 {
        evenSquares.push(i * i)
    }
}
println(evenSquares)  # [4, 16, 36, 64, 100]
```

### Pattern 4: Array Aggregation

```klang
# Calculate statistics
let numbers = [12, 45, 23, 67, 34, 89, 15, 56]

# Sum
let sum = numbers.reduce((acc, x) => acc + x, 0)
println("Sum: " + str(sum))

# Average
let avg = sum / len(numbers)
println("Average: " + str(avg))

# Max
let max = numbers.reduce((acc, x) => x > acc ? x : acc, numbers[0])
println("Max: " + str(max))

# Min
let min = numbers.reduce((acc, x) => x < acc ? x : acc, numbers[0])
println("Min: " + str(min))
```

### Pattern 5: Array Transformation Pipeline

```klang
# Transform data through multiple steps
let rawData = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

let processed = rawData
    .filter(x => x > 3)           # Remove small values
    .map(x => x * 10)             # Scale up
    .map(x => ({value: x}))       # Wrap in objects (conceptual)
    .slice(0, 3)                  # Take first 3

println("Processed count: " + str(len(processed)))
```

### Pattern 6: Array Comparison

```klang
# Check if two arrays are equal
fn arraysEqual(arr1, arr2) {
    if len(arr1) != len(arr2) {
        return false
    }
    
    for i in 0..len(arr1) {
        if arr1[i] != arr2[i] {
            return false
        }
    }
    
    return true
}

let a = [1, 2, 3]
let b = [1, 2, 3]
let c = [1, 2, 4]

println(arraysEqual(a, b))  # true
println(arraysEqual(a, c))  # false
```

## Best Practices

### 1. Use Appropriate Methods

```klang
# ✅ Good: Use built-in methods
let doubled = numbers.map(x => x * 2)

# ❌ Avoid: Manual iteration when not needed
let doubled = []
for num of numbers {
    doubled.push(num * 2)
}
```

### 2. Method Chaining

```klang
# ✅ Good: Clear chain
let result = data
    .filter(x => x > 0)
    .map(x => x * 2)
    .reduce((sum, x) => sum + x, 0)

# ❌ Avoid: Separate operations (when chaining is clearer)
let filtered = data.filter(x => x > 0)
let mapped = filtered.map(x => x * 2)
let result = mapped.reduce((sum, x) => sum + x, 0)
```

### 3. Immutability

```klang
# ✅ Good: Don't modify original (when possible)
let original = [1, 2, 3]
let modified = original.map(x => x * 2)

# ⚠️ Caution: Some methods modify original
original.push(4)  # Mutates original
original.reverse() # Mutates original
```

### 4. Check Array Length

```klang
# ✅ Good: Check before accessing
if len(arr) > 0 {
    let first = arr[0]
}

# ❌ Avoid: Accessing without checking
# let first = arr[0]  # Error if array is empty
```

## Common Mistakes

### Mistake 1: Index Out of Bounds

```klang
let arr = [1, 2, 3]

# ❌ Wrong: Index too large
# let x = arr[10]  # Error or undefined

# ✅ Correct: Check bounds
if len(arr) > 10 {
    let x = arr[10]
}
```

### Mistake 2: Modifying While Iterating

```klang
let numbers = [1, 2, 3, 4, 5]

# ⚠️ Dangerous: Modifying array during iteration
# for num of numbers {
#     numbers.push(num * 2)  # Can cause issues
# }

# ✅ Better: Create new array
let doubled = numbers.map(x => x * 2)
```

### Mistake 3: Forgetting Return in reduce

```klang
# ❌ Wrong: No return in accumulator
# let sum = arr.reduce((acc, x) => {
#     acc + x  # Missing return!
# }, 0)

# ✅ Correct: Return accumulator
let sum = arr.reduce((acc, x) => {
    return acc + x
}, 0)

# ✅ Or use implicit return
let sum = arr.reduce((acc, x) => acc + x, 0)
```

## Summary

### Core Methods
- **Add/Remove**: `push()`, `pop()`, `unshift()`, `shift()`
- **Transform**: `map()`, `filter()`, `reduce()`
- **Search**: `find()`, `findIndex()`, `includes()`, `indexOf()`
- **Test**: `some()`, `every()`
- **Manipulate**: `slice()`, `concat()`, `join()`, `reverse()`, `sort()`

### Key Concepts
- Arrays are 0-indexed
- Most methods don't modify original (except push, pop, etc.)
- Method chaining for pipelines
- Use `len()` to get array length

### Best Practices
1. Use built-in methods over manual loops
2. Chain methods for clarity
3. Check array length before accessing
4. Prefer immutable operations
5. Use descriptive variable names

## Next Steps

Master arrays, then explore:

- **[Objects](15-objects.md)** - Work with key-value structures
- **[Functions](13-functions.md)** - Review higher-order functions
- **[Control Flow](12-control-flow.md)** - Array iteration patterns
- **[Advanced Patterns](16-advanced-arrays.md)** - Complex array operations

---

**Harness the power of arrays for data transformation!** 🚀
