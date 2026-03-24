# Basic Examples

## Introduction

This page provides practical examples covering KLang's fundamental features. Each example is self-contained and demonstrates real-world usage patterns.

## Variables & Data Types

### Example 1: Shopping Cart

```klang
# Shopping cart with different data types
let cart_id = 12345
let customer_name = "Alice"
let is_premium = true
let discount = 0.15
let items = ["laptop", "mouse", "keyboard"]

println("Cart ID: " + str(cart_id))
println("Customer: " + customer_name)
println("Premium: " + str(is_premium))
println("Items: " + str(items))

if is_premium {
    println("Discount applied: " + str(discount * 100) + "%")
}
```

### Example 2: Type Checking

```klang
fn describe(value) {
    let t = type(value)
    println("Value: " + str(value))
    println("Type: " + t)
    println("---")
}

describe(42)
describe("hello")
describe([1, 2, 3])
describe(true)
describe(3.14)
```

## Functions

### Example 3: Temperature Converter

```klang
fn celsiusToFahrenheit(celsius) {
    return (celsius * 9 / 5) + 32
}

fn fahrenheitToCelsius(fahrenheit) {
    return (fahrenheit - 32) * 5 / 9
}

let temp_c = 25
let temp_f = celsiusToFahrenheit(temp_c)
println(str(temp_c) + "°C = " + str(temp_f) + "°F")

let room_f = 72
let room_c = fahrenheitToCelsius(room_f)
println(str(room_f) + "°F = " + str(room_c) + "°C")
```

### Example 4: Password Validator

```klang
fn isStrongPassword(password) {
    # Check length
    if len(password) < 8 {
        return false
    }
    
    # Check for number
    let has_number = false
    for i in range(len(password)) {
        let char = password.charAt(i)
        if char >= "0" and char <= "9" {
            has_number = true
            break
        }
    }
    
    return has_number
}

println(isStrongPassword("weak"))        # false
println(isStrongPassword("strong123"))   # true
```

### Example 5: Fibonacci Sequence

```klang
fn fibonacci(n) {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

# Generate first 10 Fibonacci numbers
println("Fibonacci sequence:")
for i in range(10) {
    println("F(" + str(i) + ") = " + str(fibonacci(i)))
}
```

## Arrays

### Example 6: Grade Calculator

```klang
let grades = [85, 92, 78, 95, 88, 76, 90]

# Calculate average
let sum = grades.reduce((acc, g) => acc + g, 0)
let average = sum / len(grades)

# Find min and max
let min_grade = grades.reduce((a, b) => min(a, b))
let max_grade = grades.reduce((a, b) => max(a, b))

# Count passing grades (>= 80)
let passing = grades.filter(g => g >= 80)

println("Average: " + str(average))
println("Min: " + str(min_grade))
println("Max: " + str(max_grade))
println("Passing: " + str(len(passing)) + "/" + str(len(grades)))
```

### Example 7: Shopping List

```klang
let shopping_list = []

# Add items
shopping_list.push("milk")
shopping_list.push("bread")
shopping_list.push("eggs")
shopping_list.push("butter")

println("Shopping List:")
shopping_list.forEach((item, index) => {
    println(str(index + 1) + ". " + item)
})

# Remove first item (bought it)
let bought = shopping_list.shift()
println("\nBought: " + bought)
println("Remaining: " + str(len(shopping_list)))
```

### Example 8: Array Transformation Pipeline

```klang
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

# Get squares of even numbers greater than 4
let result = numbers
    .filter(n => n % 2 == 0)      # [2, 4, 6, 8, 10]
    .filter(n => n > 4)            # [6, 8, 10]
    .map(n => n * n)               # [36, 64, 100]

println("Result: " + str(result))  # [36, 64, 100]

# Calculate sum
let total = result.reduce((sum, n) => sum + n, 0)
println("Sum: " + str(total))  # 200
```

## Objects

### Example 9: User Profile

```klang
let user = {
    id: 101,
    username: "alice",
    email: "alice@example.com",
    age: 28,
    is_active: true,
    roles: ["user", "admin"]
}

println("User Profile:")
println("Username: " + user.username)
println("Email: " + user.email)
println("Age: " + str(user.age))
println("Active: " + str(user.is_active))
println("Roles: " + str(user.roles))
```

### Example 10: Product Inventory

```klang
let products = [
    {id: 1, name: "Laptop", price: 999, stock: 5},
    {id: 2, name: "Mouse", price: 25, stock: 50},
    {id: 3, name: "Keyboard", price: 75, stock: 30},
    {id: 4, name: "Monitor", price: 350, stock: 10}
]

# Find expensive products (> $100)
let expensive = products.filter(p => p.price > 100)
println("Expensive products:")
expensive.forEach(p => {
    println("- " + p.name + ": $" + str(p.price))
})

# Calculate total inventory value
let total_value = products.reduce((sum, p) => {
    return sum + (p.price * p.stock)
}, 0)
println("\nTotal inventory value: $" + str(total_value))
```

## Control Flow

### Example 11: Number Classifier

```klang
fn classifyNumber(n) {
    if n < 0 {
        return "negative"
    } else if n == 0 {
        return "zero"
    } else if n > 0 and n < 10 {
        return "single digit"
    } else if n >= 10 and n < 100 {
        return "double digit"
    } else {
        return "large number"
    }
}

let numbers = [-5, 0, 7, 42, 150]
numbers.forEach(n => {
    println(str(n) + " is " + classifyNumber(n))
})
```

### Example 12: FizzBuzz

```klang
# Classic FizzBuzz problem
for i in range(1, 31) {
    if i % 15 == 0 {
        println("FizzBuzz")
    } else if i % 3 == 0 {
        println("Fizz")
    } else if i % 5 == 0 {
        println("Buzz")
    } else {
        println(str(i))
    }
}
```

### Example 13: Nested Loops - Multiplication Table

```klang
println("Multiplication Table (1-5):")
println("")

for i in range(1, 6) {
    let row = ""
    for j in range(1, 6) {
        let product = i * j
        let padded = str(product).padStart(4, " ")
        row = row + padded
    }
    println(row)
}
```

## String Processing

### Example 14: Word Counter

```klang
fn countWords(text) {
    let trimmed = trim(text)
    if len(trimmed) == 0 {
        return 0
    }
    let words = split(trimmed, " ")
    return len(words)
}

fn wordFrequency(text) {
    let words = split(lowercase(trim(text)), " ")
    let freq = {}
    
    words.forEach(word => {
        if word in freq {
            freq[word] = freq[word] + 1
        } else {
            freq[word] = 1
        }
    })
    
    return freq
}

let text = "hello world hello klang world hello"
println("Word count: " + str(countWords(text)))

let freq = wordFrequency(text)
println("Frequency: " + str(freq))
```

### Example 15: Text Formatter

```klang
fn formatText(text, options) {
    let result = text
    
    if options.uppercase {
        result = uppercase(result)
    }
    
    if options.trim {
        result = trim(result)
    }
    
    if options.reverse {
        let chars = split(result, "")
        chars.reverse()
        result = join(chars, "")
    }
    
    return result
}

let text = "  hello world  "
println(formatText(text, {trim: true, uppercase: true}))
# "HELLO WORLD"
```

## File I/O

### Example 16: Log Writer

```klang
fn writeLog(message) {
    let timestamp = "2024-01-15 10:30:00"  # Simplified
    let log_entry = "[" + timestamp + "] " + message + "\n"
    
    # In real usage, append to file
    write_file("app.log", log_entry)
    println("Log written: " + message)
}

writeLog("Application started")
writeLog("User logged in")
writeLog("Data processed")
```

### Example 17: Config Reader

```klang
fn loadConfig(filename) {
    let content = read_file(filename)
    let lines = split(content, "\n")
    let config = {}
    
    lines.forEach(line => {
        if len(trim(line)) > 0 and !line.startsWith("#") {
            let parts = split(line, "=")
            if len(parts) == 2 {
                config[trim(parts[0])] = trim(parts[1])
            }
        }
    })
    
    return config
}

# Example config file content:
# port=8080
# host=localhost
# debug=true

# let config = loadConfig("app.config")
# println("Port: " + config.port)
```

## Data Processing

### Example 18: CSV Parser

```klang
fn parseCSV(csv_text) {
    let lines = split(csv_text, "\n")
    let data = []
    
    lines.forEach(line => {
        if len(trim(line)) > 0 {
            let fields = split(line, ",")
            data.push(fields)
        }
    })
    
    return data
}

let csv = "name,age,city\nAlice,30,NYC\nBob,25,LA\nCharlie,35,Chicago"
let data = parseCSV(csv)

println("Parsed data:")
data.forEach(row => {
    println(str(row))
})
```

### Example 19: Simple Statistics

```klang
fn statistics(numbers) {
    let sum = numbers.reduce((a, b) => a + b, 0)
    let count = len(numbers)
    let mean = sum / count
    
    let sorted = numbers.sort((a, b) => a - b)
    let median = sorted[floor(count / 2)]
    
    let min_val = sorted[0]
    let max_val = sorted[count - 1]
    
    return {
        count: count,
        sum: sum,
        mean: mean,
        median: median,
        min: min_val,
        max: max_val
    }
}

let scores = [85, 92, 78, 95, 88, 76, 90, 84]
let stats = statistics(scores)

println("Statistics:")
println("Count: " + str(stats.count))
println("Sum: " + str(stats.sum))
println("Mean: " + str(stats.mean))
println("Median: " + str(stats.median))
println("Min: " + str(stats.min))
println("Max: " + str(stats.max))
```

### Example 20: Data Filter and Transform

```klang
let employees = [
    {name: "Alice", dept: "Engineering", salary: 90000},
    {name: "Bob", dept: "Sales", salary: 70000},
    {name: "Charlie", dept: "Engineering", salary: 85000},
    {name: "Diana", dept: "Marketing", salary: 75000},
    {name: "Eve", dept: "Engineering", salary: 95000}
]

# Get engineering department employees
let engineers = employees.filter(e => e.dept == "Engineering")
println("Engineers: " + str(len(engineers)))

# Calculate average engineering salary
let avg_salary = engineers
    .map(e => e.salary)
    .reduce((sum, s) => sum + s, 0) / len(engineers)

println("Average Engineering Salary: $" + str(avg_salary))

# List high earners (>= 85000)
println("\nHigh Earners:")
employees
    .filter(e => e.salary >= 85000)
    .forEach(e => {
        println("- " + e.name + " ($" + str(e.salary) + ")")
    })
```

## Algorithm Examples

### Example 21: Binary Search

```klang
fn binarySearch(arr, target) {
    let left = 0
    let right = len(arr) - 1
    
    while left <= right {
        let mid = floor((left + right) / 2)
        let mid_val = arr[mid]
        
        if mid_val == target {
            return mid
        } else if mid_val < target {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return -1  # Not found
}

let numbers = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
println("Found 7 at index: " + str(binarySearch(numbers, 7)))
println("Found 15 at index: " + str(binarySearch(numbers, 15)))
println("Found 20 at index: " + str(binarySearch(numbers, 20)))
```

### Example 22: Bubble Sort

```klang
fn bubbleSort(arr) {
    let n = len(arr)
    let sorted = arr  # Copy array
    
    for i in range(n) {
        for j in range(n - i - 1) {
            if sorted[j] > sorted[j + 1] {
                # Swap
                let temp = sorted[j]
                sorted[j] = sorted[j + 1]
                sorted[j + 1] = temp
            }
        }
    }
    
    return sorted
}

let numbers = [64, 34, 25, 12, 22, 11, 90]
println("Original: " + str(numbers))

let sorted = bubbleSort(numbers)
println("Sorted: " + str(sorted))
```

### Example 23: Palindrome Checker

```klang
fn isPalindrome(text) {
    # Clean and normalize
    let cleaned = lowercase(trim(text))
    cleaned = cleaned.replaceAll(" ", "")
    
    # Compare with reverse
    let chars = split(cleaned, "")
    let reversed = chars.reverse()
    let reversed_text = join(reversed, "")
    
    return cleaned == reversed_text
}

println(isPalindrome("racecar"))           # true
println(isPalindrome("A man a plan a canal Panama"))  # true
println(isPalindrome("hello"))             # false
```

## String Manipulation

### Example 24: URL Slug Generator

```klang
fn createSlug(title) {
    let slug = lowercase(trim(title))
    slug = slug.replaceAll(" ", "-")
    slug = slug.replaceAll("_", "-")
    
    # Remove special characters (simplified)
    let allowed = "abcdefghijklmnopqrstuvwxyz0123456789-"
    let result = ""
    
    for i in range(len(slug)) {
        let char = slug.charAt(i)
        if allowed.includes(char) {
            result = result + char
        }
    }
    
    return result
}

println(createSlug("My First Blog Post!"))  # "my-first-blog-post"
println(createSlug("Hello_World 2024"))     # "hello-world-2024"
```

### Example 25: Text Analyzer

```klang
fn analyzeText(text) {
    let words = split(trim(text), " ")
    let chars = split(text, "")
    
    let char_count = len(text)
    let word_count = len(words)
    let line_count = len(split(text, "\n"))
    
    # Count vowels
    let vowels = "aeiouAEIOU"
    let vowel_count = 0
    chars.forEach(c => {
        if vowels.includes(c) {
            vowel_count = vowel_count + 1
        }
    })
    
    return {
        characters: char_count,
        words: word_count,
        lines: line_count,
        vowels: vowel_count
    }
}

let text = "Hello World\nWelcome to KLang\nHappy coding!"
let analysis = analyzeText(text)

println("Text Analysis:")
println("Characters: " + str(analysis.characters))
println("Words: " + str(analysis.words))
println("Lines: " + str(analysis.lines))
println("Vowels: " + str(analysis.vowels))
```

## Number Processing

### Example 26: Prime Number Generator

```klang
fn isPrime(n) {
    if n < 2 {
        return false
    }
    if n == 2 {
        return true
    }
    if n % 2 == 0 {
        return false
    }
    
    let limit = floor(sqrt(n))
    for i in range(3, limit + 1, 2) {
        if n % i == 0 {
            return false
        }
    }
    return true
}

fn generatePrimes(max) {
    let primes = []
    for i in range(2, max + 1) {
        if isPrime(i) {
            primes.push(i)
        }
    }
    return primes
}

let primes = generatePrimes(50)
println("Primes up to 50:")
println(str(primes))
```

### Example 27: Number Formatter

```klang
fn formatNumber(num) {
    let str_num = str(num)
    let result = ""
    let count = 0
    
    # Add commas every 3 digits from right
    for i in range(len(str_num) - 1, -1, -1) {
        if count == 3 {
            result = "," + result
            count = 0
        }
        result = str_num.charAt(i) + result
        count = count + 1
    }
    
    return result
}

println(formatNumber(1000))       # "1,000"
println(formatNumber(1234567))    # "1,234,567"
println(formatNumber(999))        # "999"
```

## Practical Applications

### Example 28: Todo List Manager

```klang
let todos = []
let next_id = 1

fn addTodo(title) {
    let todo = {
        id: next_id,
        title: title,
        done: false
    }
    todos.push(todo)
    next_id = next_id + 1
    println("Added: " + title)
}

fn listTodos() {
    println("\nTodo List:")
    if len(todos) == 0 {
        println("No tasks yet!")
        return
    }
    
    todos.forEach(todo => {
        let status = todo.done ? "✓" : " "
        println("[" + status + "] " + str(todo.id) + ". " + todo.title)
    })
}

fn completeTodo(id) {
    let todo = todos.find(t => t.id == id)
    if todo != null {
        todo.done = true
        println("Completed: " + todo.title)
    }
}

# Usage
addTodo("Learn KLang basics")
addTodo("Build a project")
addTodo("Write tests")
listTodos()
completeTodo(1)
listTodos()
```

### Example 29: Simple Calculator

```klang
fn calculate(a, operator, b) {
    if operator == "+" {
        return a + b
    } else if operator == "-" {
        return a - b
    } else if operator == "*" {
        return a * b
    } else if operator == "/" {
        if b == 0 {
            return "Error: Division by zero"
        }
        return a / b
    } else if operator == "%" {
        return a % b
    } else if operator == "**" {
        return pow(a, b)
    } else {
        return "Error: Unknown operator"
    }
}

println("10 + 5 = " + str(calculate(10, "+", 5)))
println("10 - 5 = " + str(calculate(10, "-", 5)))
println("10 * 5 = " + str(calculate(10, "*", 5)))
println("10 / 5 = " + str(calculate(10, "/", 5)))
println("10 % 3 = " + str(calculate(10, "%", 3)))
println("2 ** 8 = " + str(calculate(2, "**", 8)))
```

### Example 30: Contact Book

```klang
let contacts = []

fn addContact(name, phone, email) {
    contacts.push({
        name: name,
        phone: phone,
        email: email
    })
    println("Added contact: " + name)
}

fn findContact(name) {
    return contacts.find(c => lowercase(c.name) == lowercase(name))
}

fn listContacts() {
    println("\nContacts:")
    contacts.forEach(c => {
        println("Name: " + c.name)
        println("Phone: " + c.phone)
        println("Email: " + c.email)
        println("---")
    })
}

# Usage
addContact("Alice", "555-1234", "alice@example.com")
addContact("Bob", "555-5678", "bob@example.com")
listContacts()

let contact = findContact("alice")
if contact != null {
    println("\nFound: " + contact.name + " - " + contact.phone)
}
```

## Next Steps

Now that you've mastered the basics, explore more advanced topics:

- [OOP Examples](61-examples-oop.md) - Object-oriented programming examples
- [AI Examples](62-examples-ai.md) - AI feature examples
- [Project Examples](63-examples-projects.md) - Complete project examples
- [Functions](13-functions.md) - Advanced function techniques
- [Arrays](14-arrays.md) - Deep dive into arrays

---

**Build amazing things with these fundamental patterns!** 🎯
