# Generators

## Mastering Generator Functions in KLang

Generator functions are special functions that can pause and resume execution, allowing you to create iterators, handle streams of data, and implement complex control flow. They're powerful tools for creating lazy sequences and managing asynchronous operations.

## Why Use Generators?

Generators help you:
- **Create Iterators**: Build custom iteration protocols
- **Lazy Evaluation**: Generate values on-demand, not all at once
- **Memory Efficiency**: Process large datasets without loading everything
- **Infinite Sequences**: Create endless sequences like Fibonacci
- **Coroutines**: Implement cooperative multitasking
- **State Machines**: Manage complex state transitions

## Generator Basics

### Creating Generators

Use `function*` syntax and `yield` keyword:

```klang
# Basic generator
fn* simpleGenerator() {
    yield 1
    yield 2
    yield 3
}

let gen = simpleGenerator()
println(gen.next())  # {value: 1, done: false}
println(gen.next())  # {value: 2, done: false}
println(gen.next())  # {value: 3, done: false}
println(gen.next())  # {value: undefined, done: true}
```

### Using Generators in Loops

```klang
fn* countToFive() {
    yield 1
    yield 2
    yield 3
    yield 4
    yield 5
}

# Use for...of loop
for let num in countToFive() {
    println(num)
}
# Output: 1, 2, 3, 4, 5

# Spread operator
let numbers = [...countToFive()]
println(numbers)  # [1, 2, 3, 4, 5]

# Array.from()
let array = Array.from(countToFive())
println(array)  # [1, 2, 3, 4, 5]
```

### Yielding Values

```klang
fn* greetings() {
    yield "Hello"
    yield "Bonjour"
    yield "Hola"
    yield "Ciao"
}

let greet = greetings()
println(greet.next().value)  # Hello
println(greet.next().value)  # Bonjour
println(greet.next().value)  # Hola
println(greet.next().value)  # Ciao

# Generator with expressions
fn* squares() {
    let i = 1
    while i <= 5 {
        yield i * i
        i = i + 1
    }
}

for let square in squares() {
    println(square)  # 1, 4, 9, 16, 25
}
```

## Generator Patterns

### Range Generator

```klang
fn* range(start, end, step) {
    step = step || 1
    let current = start
    
    while current < end {
        yield current
        current = current + step
    }
}

# Usage
for let i in range(0, 10) {
    println(i)  # 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
}

for let i in range(0, 20, 5) {
    println(i)  # 0, 5, 10, 15
}

let numbers = [...range(1, 6)]
println(numbers)  # [1, 2, 3, 4, 5]
```

### Infinite Sequences

```klang
fn* infiniteSequence() {
    let i = 0
    while true {
        yield i
        i = i + 1
    }
}

# Use with caution - need to break/limit
let gen = infiniteSequence()
println(gen.next().value)  # 0
println(gen.next().value)  # 1
println(gen.next().value)  # 2

# Take first N values
fn take(generator, n) {
    let results = []
    for let i = 0; i < n; i++ {
        let {value, done} = generator.next()
        if done {
            break
        }
        results.push(value)
    }
    return results
}

let numbers = take(infiniteSequence(), 10)
println(numbers)  # [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

### Fibonacci Sequence

```klang
fn* fibonacci() {
    let a = 0
    let b = 1
    
    while true {
        yield a
        let temp = a
        a = b
        b = temp + b
    }
}

# Get first 10 Fibonacci numbers
let fibs = []
let gen = fibonacci()

for let i = 0; i < 10; i++ {
    fibs.push(gen.next().value)
}

println(fibs)  # [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]

# Or use take function
let firstTen = take(fibonacci(), 10)
println(firstTen)
```

### ID Generator

```klang
fn* idGenerator(prefix) {
    let id = 1
    
    while true {
        yield prefix + "-" + str(id)
        id = id + 1
    }
}

let userIds = idGenerator("USER")
println(userIds.next().value)  # USER-1
println(userIds.next().value)  # USER-2
println(userIds.next().value)  # USER-3

let orderIds = idGenerator("ORDER")
println(orderIds.next().value)  # ORDER-1
println(orderIds.next().value)  # ORDER-2
```

## Passing Values to Generators

### Using next() with Arguments

```klang
fn* twoWayGenerator() {
    let received = yield "First yield"
    println("Received: " + received)
    
    received = yield "Second yield"
    println("Received: " + received)
    
    return "Done"
}

let gen = twoWayGenerator()

println(gen.next())           # {value: "First yield", done: false}
println(gen.next("Hello"))    # {value: "Second yield", done: false}
                              # Prints: Received: Hello
println(gen.next("World"))    # {value: "Done", done: true}
                              # Prints: Received: World
```

### Interactive Generator

```klang
fn* questionAnswer() {
    let name = yield "What is your name?"
    let age = yield "What is your age?"
    let city = yield "What city do you live in?"
    
    return {
        name: name,
        age: age,
        city: city
    }
}

let qa = questionAnswer()

println(qa.next().value)              # What is your name?
println(qa.next("Alice").value)       # What is your age?
println(qa.next(25).value)            # What city do you live in?
let result = qa.next("NYC")
println(result.value)                 # {name: "Alice", age: 25, city: "NYC"}
```

### Accumulator Generator

```klang
fn* accumulator(initial) {
    let sum = initial || 0
    
    while true {
        let value = yield sum
        if value != undefined {
            sum = sum + value
        }
    }
}

let acc = accumulator(0)
println(acc.next().value)      # 0
println(acc.next(5).value)     # 5
println(acc.next(10).value)    # 15
println(acc.next(3).value)     # 18
```

## Delegating Generators

### yield*

Delegate to another generator:

```klang
fn* generator1() {
    yield 1
    yield 2
}

fn* generator2() {
    yield 3
    yield 4
}

fn* combinedGenerator() {
    yield* generator1()
    yield* generator2()
    yield 5
}

let combined = [...combinedGenerator()]
println(combined)  # [1, 2, 3, 4, 5]
```

### Nested Generators

```klang
fn* innerGenerator() {
    yield "a"
    yield "b"
    yield "c"
}

fn* outerGenerator() {
    yield 1
    yield* innerGenerator()
    yield 2
}

for let value in outerGenerator() {
    println(value)
}
# Output: 1, a, b, c, 2

# Recursive generator
fn* tree(node) {
    yield node.value
    
    if node.children {
        for let child in node.children {
            yield* tree(child)
        }
    }
}

let treeData = {
    value: 1,
    children: [
        {value: 2, children: [{value: 4}, {value: 5}]},
        {value: 3, children: [{value: 6}]}
    ]
}

let values = [...tree(treeData)]
println(values)  # [1, 2, 4, 5, 3, 6]
```

## Generators with Arrays

### Filter Generator

```klang
fn* filter(iterable, predicate) {
    for let item in iterable {
        if predicate(item) {
            yield item
        }
    }
}

let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
let evens = [...filter(numbers, n => n % 2 == 0)]
println(evens)  # [2, 4, 6, 8, 10]

# Chain multiple filters
let largeEvens = [
    ...filter(
        filter(numbers, n => n % 2 == 0),
        n => n > 5
    )
]
println(largeEvens)  # [6, 8, 10]
```

### Map Generator

```klang
fn* map(iterable, fn) {
    for let item in iterable {
        yield fn(item)
    }
}

let numbers = [1, 2, 3, 4, 5]
let squared = [...map(numbers, n => n * n)]
println(squared)  # [1, 4, 9, 16, 25]

let doubled = [...map(numbers, n => n * 2)]
println(doubled)  # [2, 4, 6, 8, 10]

# Chain map and filter
let result = [
    ...map(
        filter(numbers, n => n % 2 == 0),
        n => n * n
    )
]
println(result)  # [4, 16] (squares of even numbers)
```

### Take and Drop

```klang
fn* take(iterable, n) {
    let count = 0
    for let item in iterable {
        if count >= n {
            break
        }
        yield item
        count = count + 1
    }
}

fn* drop(iterable, n) {
    let count = 0
    for let item in iterable {
        if count >= n {
            yield item
        }
        count = count + 1
    }
}

let numbers = range(1, 100)
let first10 = [...take(numbers, 10)]
println(first10)  # [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

let after10 = [...take(drop(range(1, 100), 10), 5)]
println(after10)  # [11, 12, 13, 14, 15]
```

## Real-World Use Cases

### Pagination Iterator

```klang
fn* paginatedResults(fetchPage, pageSize) {
    let page = 1
    let hasMore = true
    
    while hasMore {
        let results = fetchPage(page, pageSize)
        
        if results.length == 0 {
            hasMore = false
        } else {
            for let result in results {
                yield result
            }
            page = page + 1
        }
    }
}

# Usage
fn fetchUsersPage(page, pageSize) {
    # Simulated API call
    let allUsers = generateMockUsers(100)
    let start = (page - 1) * pageSize
    let end = start + pageSize
    return allUsers.slice(start, end)
}

let users = paginatedResults(fetchUsersPage, 10)

for let user in users {
    println(user.name)
}
# Automatically fetches all pages
```

### Stream Processing

```klang
class DataStream {
    fn constructor(source) {
        this.source = source
    }
    
    fn* stream() {
        for let item in this.source {
            yield item
        }
    }
    
    fn* map(fn) {
        for let item in this.stream() {
            yield fn(item)
        }
    }
    
    fn* filter(predicate) {
        for let item in this.stream() {
            if predicate(item) {
                yield item
            }
        }
    }
    
    fn* take(n) {
        let count = 0
        for let item in this.stream() {
            if count >= n {
                break
            }
            yield item
            count = count + 1
        }
    }
    
    fn collect() {
        return [...this.stream()]
    }
}

# Usage
let data = DataStream([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

let result = data
    .filter(n => n % 2 == 0)
    .map(n => n * n)
    .take(3)
    .collect()

println(result)  # [4, 16, 36]
```

### CSV Parser

```klang
fn* parseCSV(text) {
    let lines = text.split("\n")
    let headers = lines[0].split(",")
    
    for let i = 1; i < lines.length; i++ {
        if lines[i].trim() == "" {
            continue
        }
        
        let values = lines[i].split(",")
        let row = {}
        
        for let j = 0; j < headers.length; j++ {
            row[headers[j].trim()] = values[j].trim()
        }
        
        yield row
    }
}

let csvData = `
name,age,city
Alice,25,NYC
Bob,30,LA
Charlie,35,Chicago
`

for let row in parseCSV(csvData) {
    println(row.name + " is " + row.age + " years old")
}
# Output:
# Alice is 25 years old
# Bob is 30 years old
# Charlie is 35 years old
```

### Async Generator (Simulated)

```klang
fn* asyncDataFetcher(urls) {
    for let url in urls {
        # In real implementation, this would be async
        let data = fetchSync(url)  # Simulated
        yield data
    }
}

fn processDataStream(urls) {
    for let data in asyncDataFetcher(urls) {
        processData(data)
        println("Processed data from: " + data.url)
    }
}

# Usage
let urls = [
    "https://api.example.com/data1",
    "https://api.example.com/data2",
    "https://api.example.com/data3"
]

processDataStream(urls)
```

### State Machine with Generator

```klang
fn* trafficLight() {
    while true {
        yield "red"
        yield "yellow"
        yield "green"
        yield "yellow"
    }
}

let light = trafficLight()

for let i = 0; i < 10; i++ {
    let state = light.next().value
    println("Light is: " + state)
    # Wait for appropriate time
}
# Output: red, yellow, green, yellow, red, yellow, green, yellow, red, yellow
```

### Backtracking Algorithm

```klang
fn* permutations(array) {
    if array.length <= 1 {
        yield array
        return
    }
    
    for let i = 0; i < array.length; i++ {
        let rest = [...array.slice(0, i), ...array.slice(i + 1)]
        
        for let perm in permutations(rest) {
            yield [array[i], ...perm]
        }
    }
}

# Generate all permutations
let perms = [...permutations([1, 2, 3])]
for let perm in perms {
    println(perm)
}
# Output:
# [1, 2, 3]
# [1, 3, 2]
# [2, 1, 3]
# [2, 3, 1]
# [3, 1, 2]
# [3, 2, 1]
```

### Batch Processor

```klang
fn* batch(iterable, size) {
    let batch = []
    
    for let item in iterable {
        batch.push(item)
        
        if batch.length >= size {
            yield batch
            batch = []
        }
    }
    
    # Yield remaining items
    if batch.length > 0 {
        yield batch
    }
}

# Usage
let numbers = range(1, 26)  # 1-25

for let batchGroup in batch(numbers, 5) {
    println("Processing batch: " + batchGroup)
    # Process 5 items at a time
}
# Output:
# Processing batch: [1, 2, 3, 4, 5]
# Processing batch: [6, 7, 8, 9, 10]
# Processing batch: [11, 12, 13, 14, 15]
# Processing batch: [16, 17, 18, 19, 20]
# Processing batch: [21, 22, 23, 24, 25]
```

## Advanced Patterns

### Generator Composition

```klang
fn* compose(...generators) {
    for let generator in generators {
        yield* generator
    }
}

fn* numbers() {
    yield 1
    yield 2
    yield 3
}

fn* letters() {
    yield "a"
    yield "b"
    yield "c"
}

fn* symbols() {
    yield "!"
    yield "@"
    yield "#"
}

let combined = [...compose(numbers(), letters(), symbols())]
println(combined)  # [1, 2, 3, "a", "b", "c", "!", "@", "#"]
```

### Lazy Chain

```klang
class LazySequence {
    fn constructor(generator) {
        this.generator = generator
    }
    
    fn* [Symbol.iterator]() {
        yield* this.generator
    }
    
    fn map(fn) {
        let self = this
        return LazySequence(fn*() {
            for let item in self {
                yield fn(item)
            }
        })
    }
    
    fn filter(predicate) {
        let self = this
        return LazySequence(fn*() {
            for let item in self {
                if predicate(item) {
                    yield item
                }
            }
        })
    }
    
    fn take(n) {
        let self = this
        return LazySequence(fn*() {
            let count = 0
            for let item in self {
                if count >= n {
                    break
                }
                yield item
                count = count + 1
            }
        })
    }
    
    fn toArray() {
        return [...this]
    }
}

# Usage
let result = LazySequence(range(1, 1000))
    .filter(n => n % 2 == 0)
    .map(n => n * n)
    .take(5)
    .toArray()

println(result)  # [4, 16, 36, 64, 100]
```

### Generator Pipeline

```klang
fn pipe(...operations) {
    return fn*(input) {
        let current = input
        
        for let operation in operations {
            current = operation(current)
        }
        
        yield* current
    }
}

let pipeline = pipe(
    iterable => filter(iterable, n => n > 0),
    iterable => map(iterable, n => n * 2),
    iterable => take(iterable, 5)
)

let numbers = [-5, -3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
let result = [...pipeline(numbers)]
println(result)  # [2, 4, 6, 8, 10]
```

### Memoized Generator

```klang
fn memoizeGenerator(generatorFn) {
    let cache = []
    let generator = null
    
    return fn*() {
        # Yield cached values
        for let value in cache {
            yield value
        }
        
        # Initialize generator if needed
        if !generator {
            generator = generatorFn()
        }
        
        # Yield and cache new values
        let result = generator.next()
        while !result.done {
            cache.push(result.value)
            yield result.value
            result = generator.next()
        }
    }
}

let memoFib = memoizeGenerator(fibonacci)

# First call
let first5 = take(memoFib(), 5)
println(first5)  # [0, 1, 1, 2, 3]

# Second call uses cache
let first10 = take(memoFib(), 10)
println(first10)  # [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]
```

## Common Patterns

### Cycle

```klang
fn* cycle(iterable) {
    let saved = []
    
    for let item in iterable {
        yield item
        saved.push(item)
    }
    
    while saved.length > 0 {
        for let item in saved {
            yield item
        }
    }
}

let colors = cycle(["red", "green", "blue"])
let first10 = take(colors, 10)
println(first10)  # [red, green, blue, red, green, blue, red, green, blue, red]
```

### Zip

```klang
fn* zip(...iterables) {
    let iterators = iterables.map(it => it[Symbol.iterator]())
    
    while true {
        let results = iterators.map(it => it.next())
        
        if results.some(r => r.done) {
            break
        }
        
        yield results.map(r => r.value)
    }
}

let names = ["Alice", "Bob", "Charlie"]
let ages = [25, 30, 35]
let cities = ["NYC", "LA", "Chicago"]

for let [name, age, city] in zip(names, ages, cities) {
    println(name + " is " + str(age) + " from " + city)
}
# Output:
# Alice is 25 from NYC
# Bob is 30 from LA
# Charlie is 35 from Chicago
```

### Enumerate

```klang
fn* enumerate(iterable, start) {
    start = start || 0
    let index = start
    
    for let item in iterable {
        yield [index, item]
        index = index + 1
    }
}

let fruits = ["apple", "banana", "cherry"]

for let [i, fruit] in enumerate(fruits) {
    println(str(i) + ": " + fruit)
}
# Output:
# 0: apple
# 1: banana
# 2: cherry

# With custom start
for let [i, fruit] in enumerate(fruits, 1) {
    println(str(i) + ": " + fruit)
}
# Output:
# 1: apple
# 2: banana
# 3: cherry
```

## Best Practices

### 1. Use Generators for Large Datasets

```klang
# Good: Memory efficient
fn* processLargeFile(filePath) {
    let lines = readFileLines(filePath)
    for let line in lines {
        yield processLine(line)
    }
}

# Avoid: Loads everything into memory
fn processLargeFile(filePath) {
    let lines = readFile(filePath).split("\n")
    return lines.map(processLine)
}
```

### 2. Document When Generators Are Infinite

```klang
# Good: Clear documentation
/**
 * Generates an infinite sequence of natural numbers
 * @returns {Generator<number>} Infinite sequence starting from 0
 */
fn* naturalNumbers() {
    let n = 0
    while true {
        yield n
        n = n + 1
    }
}
```

### 3. Combine with Array Methods Carefully

```klang
# Good: Process in chunks
fn* processInChunks(generator, chunkSize) {
    let chunk = []
    
    for let item in generator {
        chunk.push(item)
        
        if chunk.length >= chunkSize {
            yield chunk
            chunk = []
        }
    }
}

# Avoid: Converting entire generator to array
let all = [...infiniteGenerator()]  # Never completes!
```

### 4. Handle Cleanup with try/finally

```klang
fn* resourceGenerator() {
    let resource = acquireResource()
    
    try {
        while true {
            yield resource.getData()
        }
    } finally {
        resource.release()
        println("Resource released")
    }
}
```

### 5. Use for Control Flow

```klang
# Good: Generator for state management
fn* requestHandler() {
    yield "parsing"
    yield "validating"
    yield "processing"
    yield "saving"
    yield "complete"
}

# Track progress
let handler = requestHandler()
for let state in handler {
    println("Current state: " + state)
    updateProgress(state)
}
```

## Common Mistakes to Avoid

### 1. Forgetting the Asterisk

```klang
# Wrong: Not a generator
fn myGenerator() {
    yield 1  # Syntax error
}

# Correct
fn* myGenerator() {
    yield 1
}
```

### 2. Not Consuming Infinite Generators Safely

```klang
# Dangerous: Never completes
let all = [...infiniteSequence()]

# Safe: Limit consumption
let first10 = take(infiniteSequence(), 10)
```

### 3. Modifying Iterated Collection

```klang
# Problematic
fn* badGenerator(array) {
    for let i = 0; i < array.length; i++ {
        array.push(i)  # Modifying while iterating
        yield array[i]
    }
}

# Better: Don't modify
fn* goodGenerator(array) {
    for let item in array {
        yield item
    }
}
```

## Next Steps

Now that you understand generators, explore these topics:

- **[Iterators](27-iterators.md)** - Custom iteration protocols
- **[Async/Await](23-async-await.md)** - Async generators
- **[Functional Programming](28-functional-programming.md)** - FP patterns
- **[Arrays](14-arrays.md)** - Array methods
- **[Performance](45-performance.md)** - Optimize generators

Ready to create efficient, lazy sequences? Start using generators in your projects!
