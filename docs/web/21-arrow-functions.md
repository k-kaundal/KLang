# Arrow Functions

## Modern Function Syntax in KLang

Arrow functions provide a concise syntax for writing functions in KLang. They're perfect for callbacks, functional programming, and keeping your code clean and readable. This guide covers everything about arrow functions, from basic syntax to advanced patterns.

## Why Use Arrow Functions?

Arrow functions offer several advantages:
- **Concise Syntax**: Write functions in fewer lines
- **Implicit Returns**: Single-expression functions return automatically
- **Readable Code**: Cleaner callbacks and functional operations
- **Lexical this Binding**: Preserve context in nested functions
- **Functional Programming**: Ideal for map, filter, reduce operations
- **Modern Style**: Write contemporary, expressive code

## Basic Syntax

### Simple Arrow Functions

The basic arrow function syntax uses `=>`:

```klang
# Regular function
fn add(a, b) {
    return a + b
}

# Arrow function
let add = (a, b) => a + b

println(add(5, 3))  # 8

# No parameters
let greet = () => println("Hello!")
greet()  # Hello!

# Single parameter (parentheses optional)
let square = x => x * x
println(square(4))  # 16

# Multiple parameters
let multiply = (a, b) => a * b
println(multiply(3, 7))  # 21
```

### Arrow Functions with Blocks

Use curly braces for multi-line functions:

```klang
# Single expression (implicit return)
let double = x => x * 2

# Block body (explicit return needed)
let processValue = x => {
    let doubled = x * 2
    let squared = doubled * doubled
    return squared
}

println(processValue(3))  # 36

# Multiple statements
let greetUser = name => {
    let greeting = "Hello, " + name
    let timestamp = Date.now()
    println(greeting + " at " + str(timestamp))
}

greetUser("Alice")
```

### Implicit vs Explicit Returns

```klang
# Implicit return (no braces)
let add = (a, b) => a + b

# Explicit return (with braces)
let subtract = (a, b) => {
    return a - b
}

# Implicit return with object (wrap in parentheses)
let createUser = (name, age) => ({
    name: name,
    age: age,
    active: true
})

let user = createUser("Bob", 30)
println(user.name)  # Bob

# Multi-line requires explicit return
let calculate = (x, y) => {
    let sum = x + y
    let product = x * y
    return {sum: sum, product: product}
}
```

## Arrow Functions vs Regular Functions

### Syntax Comparison

```klang
# Regular function declaration
fn regularGreet(name) {
    return "Hello, " + name
}

# Arrow function
let arrowGreet = name => "Hello, " + name

println(regularGreet("Alice"))  # Hello, Alice
println(arrowGreet("Alice"))    # Hello, Alice

# Regular function with multiple lines
fn calculateTotal(price, tax, discount) {
    let subtotal = price + tax
    let total = subtotal - discount
    return total
}

# Arrow function equivalent
let calculateTotalArrow = (price, tax, discount) => {
    let subtotal = price + tax
    let total = subtotal - discount
    return total
}
```

### Context Binding (this)

```klang
class Counter {
    fn constructor() {
        this.count = 0
    }
    
    # Regular method
    fn increment() {
        this.count = this.count + 1
    }
    
    # Using arrow function in callback
    fn startAutoIncrement() {
        # Arrow function preserves 'this' context
        setInterval(() => {
            this.count = this.count + 1
            println("Count: " + str(this.count))
        }, 1000)
    }
    
    # Regular function would lose context
    fn startAutoIncrementWrong() {
        setInterval(fn() {
            # 'this' would be undefined here
            this.count = this.count + 1
        }, 1000)
    }
}

let counter = Counter()
counter.startAutoIncrement()
```

### When to Use Each

```klang
# Use regular functions for:
# 1. Methods that need to be overridden
class Animal {
    fn makeSound() {
        println("Some sound")
    }
}

# 2. Constructors
class Person {
    fn constructor(name) {
        this.name = name
    }
}

# 3. Generators
fn* numberGenerator() {
    yield 1
    yield 2
    yield 3
}

# Use arrow functions for:
# 1. Callbacks
let numbers = [1, 2, 3, 4, 5]
let doubled = numbers.map(x => x * 2)

# 2. Short function expressions
let isEven = n => n % 2 == 0

# 3. Functional programming
let sum = numbers.reduce((acc, n) => acc + n, 0)
```

## Array Methods with Arrow Functions

### map()

Transform array elements:

```klang
let numbers = [1, 2, 3, 4, 5]

# Double each number
let doubled = numbers.map(x => x * 2)
println(doubled)  # [2, 4, 6, 8, 10]

# Convert to strings
let strings = numbers.map(n => "Number: " + str(n))
println(strings)  # ["Number: 1", "Number: 2", ...]

# Complex transformation
let users = [
    {name: "Alice", age: 25},
    {name: "Bob", age: 30},
    {name: "Charlie", age: 35}
]

let names = users.map(user => user.name)
println(names)  # ["Alice", "Bob", "Charlie"]

let summaries = users.map(user => user.name + " is " + str(user.age))
println(summaries)  # ["Alice is 25", "Bob is 30", "Charlie is 35"]
```

### filter()

Select elements that match a condition:

```klang
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

# Filter even numbers
let evens = numbers.filter(x => x % 2 == 0)
println(evens)  # [2, 4, 6, 8, 10]

# Filter numbers greater than 5
let greaterThanFive = numbers.filter(x => x > 5)
println(greaterThanFive)  # [6, 7, 8, 9, 10]

# Filter users by age
let users = [
    {name: "Alice", age: 17},
    {name: "Bob", age: 25},
    {name: "Charlie", age: 15},
    {name: "Dave", age: 30}
]

let adults = users.filter(user => user.age >= 18)
println(adults.length)  # 2

# Complex conditions
let activeAdults = users.filter(user => {
    return user.age >= 18 && user.active == true
})
```

### reduce()

Combine array elements into a single value:

```klang
let numbers = [1, 2, 3, 4, 5]

# Sum all numbers
let sum = numbers.reduce((acc, n) => acc + n, 0)
println(sum)  # 15

# Find maximum
let max = numbers.reduce((acc, n) => n > acc ? n : acc, numbers[0])
println(max)  # 5

# Count occurrences
let fruits = ["apple", "banana", "apple", "orange", "banana", "apple"]
let count = fruits.reduce((acc, fruit) => {
    acc[fruit] = (acc[fruit] || 0) + 1
    return acc
}, {})
println(count)  # {apple: 3, banana: 2, orange: 1}

# Group by property
let people = [
    {name: "Alice", age: 25},
    {name: "Bob", age: 30},
    {name: "Charlie", age: 25}
]

let grouped = people.reduce((acc, person) => {
    let key = person.age
    if !acc[key] {
        acc[key] = []
    }
    acc[key].push(person.name)
    return acc
}, {})
println(grouped)  # {25: ["Alice", "Charlie"], 30: ["Bob"]}
```

### forEach()

Execute a function for each element:

```klang
let numbers = [1, 2, 3, 4, 5]

# Print each number
numbers.forEach(n => println(n))

# With index
numbers.forEach((n, i) => {
    println("Index " + str(i) + ": " + str(n))
})

# Modify external state
let sum = 0
numbers.forEach(n => {
    sum = sum + n
})
println("Total: " + str(sum))  # Total: 15

# Process objects
let users = [{name: "Alice"}, {name: "Bob"}]
users.forEach(user => {
    println("Hello, " + user.name)
})
```

### find() and findIndex()

```klang
let numbers = [10, 20, 30, 40, 50]

# Find first element matching condition
let found = numbers.find(n => n > 25)
println(found)  # 30

# Find index
let index = numbers.findIndex(n => n > 25)
println(index)  # 2

# Find in object array
let users = [
    {id: 1, name: "Alice"},
    {id: 2, name: "Bob"},
    {id: 3, name: "Charlie"}
]

let user = users.find(u => u.id == 2)
println(user.name)  # Bob

# Returns null if not found
let notFound = users.find(u => u.id == 99)
println(notFound)  # null
```

### every() and some()

```klang
let numbers = [2, 4, 6, 8, 10]

# Check if all elements match condition
let allEven = numbers.every(n => n % 2 == 0)
println(allEven)  # true

# Check if any element matches
let hasLarge = numbers.some(n => n > 5)
println(hasLarge)  # true

# Validation
let users = [
    {name: "Alice", age: 25},
    {name: "Bob", age: 30},
    {name: "Charlie", age: 20}
]

let allAdults = users.every(u => u.age >= 18)
println(allAdults)  # true

let hasMinor = users.some(u => u.age < 18)
println(hasMinor)  # false
```

## Chaining Array Methods

### Multiple Operations

```klang
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

# Chain filter and map
let result = numbers
    .filter(n => n % 2 == 0)
    .map(n => n * 2)

println(result)  # [4, 8, 12, 16, 20]

# Complex chain
let users = [
    {name: "Alice", age: 25, active: true},
    {name: "Bob", age: 17, active: true},
    {name: "Charlie", age: 30, active: false},
    {name: "Dave", age: 22, active: true}
]

let activeAdultNames = users
    .filter(u => u.active)
    .filter(u => u.age >= 18)
    .map(u => u.name)

println(activeAdultNames)  # ["Alice", "Dave"]
```

### Practical Examples

```klang
# Process shopping cart
let cart = [
    {name: "Laptop", price: 1000, quantity: 1},
    {name: "Mouse", price: 25, quantity: 2},
    {name: "Keyboard", price: 75, quantity: 1}
]

let total = cart
    .map(item => item.price * item.quantity)
    .reduce((sum, price) => sum + price, 0)

println("Total: $" + str(total))  # Total: $1125

# Get expensive items
let expensive = cart
    .filter(item => item.price * item.quantity > 50)
    .map(item => item.name)

println(expensive)  # ["Laptop", "Keyboard"]

# Process and format data
let products = [
    {id: 1, name: "Apple", price: 1.50},
    {id: 2, name: "Banana", price: 0.75},
    {id: 3, name: "Orange", price: 2.00}
]

let formatted = products
    .filter(p => p.price < 2)
    .map(p => ({
        label: p.name + " - $" + str(p.price),
        value: p.id
    }))

println(formatted)
```

## Callbacks and Event Handlers

### Event Listeners

```klang
# Button click handler
button.onClick(() => {
    println("Button clicked!")
})

# With event object
button.onClick(event => {
    println("Clicked at: " + str(event.x) + ", " + str(event.y))
})

# Form submission
form.onSubmit(event => {
    event.preventDefault()
    let data = form.getData()
    submitData(data)
})

# Input change
input.onChange(value => {
    println("New value: " + value)
    validateInput(value)
})
```

### Timers

```klang
# setTimeout with arrow function
setTimeout(() => {
    println("Executed after 1 second")
}, 1000)

# setInterval
let count = 0
let intervalId = setInterval(() => {
    count = count + 1
    println("Count: " + str(count))
    
    if count >= 5 {
        clearInterval(intervalId)
    }
}, 1000)

# Delayed execution with parameters
let greet = name => {
    setTimeout(() => {
        println("Hello, " + name)
    }, 500)
}

greet("Alice")
```

### Promise Callbacks

```klang
# Promise then/catch with arrows
fetchData()
    .then(data => {
        println("Received: " + data)
        return processData(data)
    })
    .then(result => {
        println("Processed: " + result)
    })
    .catch(error => {
        println("Error: " + error)
    })

# Multiple operations
getData()
    .then(data => data.filter(x => x.active))
    .then(filtered => filtered.map(x => x.name))
    .then(names => {
        println("Active users: " + names.join(", "))
    })
```

## Real-World Use Cases

### Data Processing Pipeline

```klang
let orders = [
    {id: 1, customer: "Alice", items: 3, total: 150, status: "pending"},
    {id: 2, customer: "Bob", items: 1, total: 50, status: "completed"},
    {id: 3, customer: "Charlie", items: 5, total: 300, status: "pending"},
    {id: 4, customer: "Dave", items: 2, total: 100, status: "completed"}
]

# Find high-value pending orders
let highValuePending = orders
    .filter(order => order.status == "pending")
    .filter(order => order.total > 100)
    .map(order => ({
        id: order.id,
        customer: order.customer,
        total: order.total
    }))

println(highValuePending)

# Calculate statistics
let stats = orders.reduce((acc, order) => {
    acc.totalRevenue = acc.totalRevenue + order.total
    acc.totalItems = acc.totalItems + order.items
    acc.orderCount = acc.orderCount + 1
    return acc
}, {totalRevenue: 0, totalItems: 0, orderCount: 0})

println("Total Revenue: $" + str(stats.totalRevenue))
println("Average Order: $" + str(stats.totalRevenue / stats.orderCount))
```

### Form Validation

```klang
let validators = {
    required: value => value != null && value != "",
    email: value => value.contains("@") && value.contains("."),
    minLength: min => value => value.length >= min,
    maxLength: max => value => value.length <= max,
    numeric: value => !isNaN(parseFloat(value))
}

fn validateField(value, rules) {
    let errors = rules
        .map(rule => rule(value))
        .filter(result => result != true)
    
    return errors.length == 0
}

# Usage
let emailValid = validateField("test@example.com", [
    validators.required,
    validators.email
])

let passwordValid = validateField("pass123", [
    validators.required,
    validators.minLength(8)
])

println("Email valid: " + str(emailValid))      # true
println("Password valid: " + str(passwordValid)) # false
```

### API Response Transformation

```klang
# Transform API response
fn processApiResponse(response) {
    return response.data
        .filter(item => item.published)
        .map(item => ({
            id: item.id,
            title: item.title,
            author: item.author.name,
            date: formatDate(item.createdAt),
            excerpt: item.content.substring(0, 100) + "..."
        }))
        .sort((a, b) => b.date - a.date)
}

# Usage
fetchArticles()
    .then(response => processApiResponse(response))
    .then(articles => {
        articles.forEach(article => {
            println(article.title + " by " + article.author)
        })
    })
```

### State Management

```klang
class Store {
    fn constructor() {
        this.state = {}
        this.listeners = []
    }
    
    fn subscribe(listener) {
        this.listeners.push(listener)
        return () => {
            this.listeners = this.listeners.filter(l => l != listener)
        }
    }
    
    fn setState(updates) {
        this.state = {...this.state, ...updates}
        this.listeners.forEach(listener => listener(this.state))
    }
    
    fn getState() {
        return this.state
    }
}

let store = Store()

# Subscribe with arrow function
let unsubscribe = store.subscribe(state => {
    println("State updated: " + JSON.stringify(state))
})

store.setState({user: "Alice", loggedIn: true})
store.setState({theme: "dark"})

# Unsubscribe
unsubscribe()
```

### Functional Utilities

```klang
# Compose functions
let compose = (...fns) => x => {
    return fns.reduceRight((acc, fn) => fn(acc), x)
}

let addOne = x => x + 1
let double = x => x * 2
let square = x => x * x

let transform = compose(square, double, addOne)
println(transform(3))  # ((3 + 1) * 2)^2 = 64

# Pipe functions
let pipe = (...fns) => x => {
    return fns.reduce((acc, fn) => fn(acc), x)
}

let process = pipe(addOne, double, square)
println(process(3))  # ((3 + 1) * 2)^2 = 64

# Curry function
let curry = fn => {
    return (...args) => {
        if args.length >= fn.length {
            return fn(...args)
        }
        return (...more) => curry(fn)(...args, ...more)
    }
}

let add = (a, b, c) => a + b + c
let curriedAdd = curry(add)

println(curriedAdd(1)(2)(3))      # 6
println(curriedAdd(1, 2)(3))      # 6
println(curriedAdd(1, 2, 3))      # 6
```

## Common Patterns

### Mapping with Index

```klang
let items = ["apple", "banana", "cherry"]

# With index
let indexed = items.map((item, i) => ({
    id: i,
    name: item
}))

println(indexed)
# [{id: 0, name: "apple"}, {id: 1, name: "banana"}, {id: 2, name: "cherry"}]

# Format with index
items.forEach((item, i) => {
    println((i + 1) + ". " + item)
})
# Output:
# 1. apple
# 2. banana
# 3. cherry
```

### Conditional Mapping

```klang
let numbers = [1, 2, 3, 4, 5]

# Map with condition
let result = numbers.map(n => n % 2 == 0 ? n * 2 : n)
println(result)  # [1, 4, 3, 8, 5]

# Complex conditional
let users = [
    {name: "Alice", age: 17},
    {name: "Bob", age: 25}
]

let formatted = users.map(user => ({
    name: user.name,
    age: user.age,
    status: user.age >= 18 ? "adult" : "minor"
}))
```

### Filtering with Multiple Conditions

```klang
let products = [
    {name: "Laptop", price: 1000, inStock: true, category: "electronics"},
    {name: "Book", price: 15, inStock: false, category: "books"},
    {name: "Phone", price: 800, inStock: true, category: "electronics"},
    {name: "Pen", price: 2, inStock: true, category: "stationery"}
]

# Multiple filters
let availableElectronics = products.filter(p => {
    return p.inStock && 
           p.category == "electronics" && 
           p.price < 900
})

println(availableElectronics)  # [{name: "Phone", ...}]

# Using every for AND conditions
let filtered = products.filter(p => 
    [p.inStock, p.price > 10, p.category == "electronics"]
        .every(condition => condition)
)
```

### Grouping Data

```klang
let transactions = [
    {id: 1, type: "income", amount: 1000},
    {id: 2, type: "expense", amount: 200},
    {id: 3, type: "income", amount: 500},
    {id: 4, type: "expense", amount: 300}
]

# Group by type
let grouped = transactions.reduce((acc, txn) => {
    if !acc[txn.type] {
        acc[txn.type] = []
    }
    acc[txn.type].push(txn)
    return acc
}, {})

println(grouped)
# {income: [{...}, {...}], expense: [{...}, {...}]}

# Calculate totals by type
let totals = transactions.reduce((acc, txn) => {
    if !acc[txn.type] {
        acc[txn.type] = 0
    }
    acc[txn.type] = acc[txn.type] + txn.amount
    return acc
}, {})

println(totals)  # {income: 1500, expense: 500}
```

### Creating Lookup Tables

```klang
let users = [
    {id: 1, name: "Alice", email: "alice@example.com"},
    {id: 2, name: "Bob", email: "bob@example.com"},
    {id: 3, name: "Charlie", email: "charlie@example.com"}
]

# Create lookup by ID
let usersById = users.reduce((acc, user) => {
    acc[user.id] = user
    return acc
}, {})

println(usersById[2].name)  # Bob

# Create lookup by email
let usersByEmail = users.reduce((acc, user) => {
    acc[user.email] = user
    return acc
}, {})

println(usersByEmail["alice@example.com"].id)  # 1
```

## Best Practices

### 1. Use Implicit Returns When Possible

```klang
# Good: Concise and clear
let double = x => x * 2
let isEven = n => n % 2 == 0

# Avoid: Unnecessary braces for single expressions
let double = x => {
    return x * 2
}
```

### 2. Use Parentheses for Clarity

```klang
# Good: Clear parameter list
let add = (a, b) => a + b

# Acceptable for single parameter
let square = x => x * x

# Good: Wrap object literals
let createUser = name => ({name: name, active: true})
```

### 3. Keep Arrow Functions Short

```klang
# Good: Short and focused
let isValid = user => user.active && user.age >= 18

# Avoid: Too complex for arrow function
let processUser = user => {
    let validated = validateUser(user)
    let transformed = transformUser(validated)
    let saved = saveUser(transformed)
    let notified = notifyUser(saved)
    return notified
}

# Better: Use regular function for complex logic
fn processUser(user) {
    let validated = validateUser(user)
    let transformed = transformUser(validated)
    let saved = saveUser(transformed)
    let notified = notifyUser(saved)
    return notified
}
```

### 4. Name Your Arrow Functions

```klang
# Good: Named for better debugging
let calculateTax = (price, rate) => price * rate

# Avoid: Anonymous everywhere
let result = numbers.map(n => n * 2)

# Better: Named for complex operations
let doubleValue = n => n * 2
let result = numbers.map(doubleValue)
```

### 5. Avoid Side Effects in Array Methods

```klang
# Avoid: Mutating external state
let total = 0
numbers.forEach(n => {
    total = total + n  # Side effect
})

# Better: Use reduce
let total = numbers.reduce((sum, n) => sum + n, 0)

# Avoid: Mutating array elements
users.forEach(user => {
    user.processed = true  # Mutation
})

# Better: Create new objects
let processed = users.map(user => ({
    ...user,
    processed: true
}))
```

## Common Mistakes to Avoid

### 1. Forgetting Return with Braces

```klang
# Wrong: No return
let add = (a, b) => {
    a + b  # This doesn't return anything
}

# Correct
let add = (a, b) => {
    return a + b
}

# Or use implicit return
let add = (a, b) => a + b
```

### 2. Object Literal Ambiguity

```klang
# Wrong: Interpreted as function body
let createUser = name => {name: name}

# Correct: Wrap in parentheses
let createUser = name => ({name: name})
```

### 3. Misusing this Context

```klang
# Problematic
class Timer {
    fn constructor() {
        this.seconds = 0
        
        # Arrow function preserves 'this'
        setInterval(() => {
            this.seconds++
        }, 1000)
    }
}

# Wrong: Regular function loses 'this'
class Timer {
    fn constructor() {
        this.seconds = 0
        
        setInterval(fn() {
            this.seconds++  # 'this' is undefined
        }, 1000)
    }
}
```

## Next Steps

Now that you master arrow functions, explore these related topics:

- **[Closures](22-closures.md)** - Understand scope and closure patterns
- **[Functions](13-functions.md)** - Learn regular function syntax
- **[Async/Await](23-async-await.md)** - Use arrows with async code
- **[Arrays](14-arrays.md)** - Master array manipulation
- **[Functional Programming](28-functional-programming.md)** - Advanced FP patterns

Ready to write more concise code? Start using arrow functions in your array operations and callbacks!
