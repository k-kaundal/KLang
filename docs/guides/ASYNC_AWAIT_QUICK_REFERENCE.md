# KLang Async/Await Quick Reference

## Syntax

### Async Function Declaration
```javascript
async fn functionName(param1, param2) {
    // Function body
    return value
}
```

### Async Arrow Function
```javascript
const func = async (param) => {
    return value
}

// Single expression
const double = async (x) => x * 2
```

### Await Expression
```javascript
async fn example() {
    const result = await someAsyncFunction()
    return result
}
```

## Key Concepts

### 1. Async Functions Return Promises
```javascript
async fn getValue() {
    return 42
}

const promise = getValue()  // Promise object
promise.then(v => println(v))  // 42
```

### 2. Await Resolves Promises
```javascript
async fn process() {
    const value = await getValue()  // Extracts 42 from Promise
    println(value)  // 42
    return value * 2
}
```

### 3. Await Works on Non-Promises
```javascript
async fn example() {
    const value = await 100  // Returns 100 immediately
    return value
}
```

## Common Patterns

### Sequential Async Operations
```javascript
async fn fetchAllData() {
    const user = await fetchUser()
    const posts = await fetchPosts(user.id)
    const comments = await fetchComments(posts)
    return { user, posts, comments }
}
```

### Promise Chaining
```javascript
async fn step1() { return 10 }
async fn step2(n) { return n * 2 }
async fn step3(n) { return n + 5 }

async fn pipeline() {
    const a = await step1()  // 10
    const b = await step2(a)  // 20
    const c = await step3(b)  // 25
    return c
}

pipeline().then(result => println(result))  // 25
```

### Using with Promise Methods
```javascript
async fn getValue() {
    return 42
}

getValue()
    .then(v => v * 2)
    .then(v => println(v))
    .catch(err => println(`Error: ${err}`))
    .finally(() => println("Done"))
```

### Error Handling (via Promise.catch)
```javascript
async fn mightFail() {
    return Promise.reject("Something went wrong")
}

mightFail().catch(err => println(`Caught: ${err}`))
```

## Examples

### Basic Usage
```javascript
async fn greet(name) {
    return `Hello, ${name}!`
}

greet("Alice").then(msg => println(msg))
```

### With Variables
```javascript
async fn calculate() {
    const a = await Promise.resolve(10)
    const b = await Promise.resolve(20)
    return a + b
}

calculate().then(sum => println(sum))  // 30
```

### In Loops
```javascript
async fn processItems(items) {
    for (let i = 0; i < items.length; i = i + 1) {
        const result = await processItem(items[i])
        println(result)
    }
}
```

### As Class Methods
```javascript
class DataService {
    async fn fetchData(url) {
        return `Data from ${url}`
    }
}

const service = new DataService()
service.fetchData("api.com").then(data => println(data))
```

## Best Practices

### ✅ DO:
- Use async functions when working with Promises
- Use await to extract values from Promises
- Chain Promise methods for transformations
- Return values directly from async functions

### ❌ DON'T:
- Forget that async functions return Promises
- Mix async/await with manual Promise construction unnecessarily
- Await non-async operations without reason

## Debugging Tips

### Check if Function is Async
```javascript
async fn test() { return 42 }
const result = test()
println(result.type)  // Will be Promise
```

### Verify Promise State
```javascript
async fn getValue() { return 100 }
const p = getValue()
// p.state is PROMISE_FULFILLED
// p.result contains the value
```

### Error Messages
```
Warning: awaiting pending promise - returning null
Uncaught (in promise): <error message>
```

## Quick Tips

1. **Always await inside async functions** - Use await to extract Promise values
2. **Async is contagious** - Functions using await must be async
3. **Promises all the way** - Async functions always return Promises
4. **Chain when possible** - Use .then() for simple transformations
5. **Sequential by default** - Use await for each operation in sequence

## Comparison with Promises

### Without Async/Await
```javascript
fn fetchUser(id) {
    return Promise.resolve(`User ${id}`)
}

fetchUser(123).then(user => {
    println(user)
    return fetchPosts(user)
}).then(posts => {
    println(posts)
})
```

### With Async/Await
```javascript
async fn fetchUser(id) {
    return `User ${id}`
}

async fn fetchUserData(id) {
    const user = await fetchUser(id)
    println(user)
    const posts = await fetchPosts(user)
    println(posts)
}
```

## Integration with KLang Features

### With Template Literals
```javascript
async fn greet(name) {
    return `Hello, ${name}!`
}
```

### With Arrow Functions
```javascript
const add = async (a, b) => a + b
```

### With Ternary Operator
```javascript
async fn check(value) {
    const result = await getValue()
    return result > 10 ? "big" : "small"
}
```

### With Array Methods
```javascript
async fn processArray(arr) {
    const results = arr.map(async (item) => {
        return await processItem(item)
    })
    return results
}
```

## Current Limitations

1. **Synchronous Execution** - await blocks until Promise resolves
2. **No Event Loop** - No true asynchronous scheduling yet
3. **Pending Promises** - Return null with warning
4. **Error Handling** - Use .catch() for now (try-catch planned)

## Complete Example

```javascript
// Define async functions
async fn fetchData(id) {
    return `Data ${id}`
}

async fn processData(data) {
    return `Processed: ${data}`
}

async fn saveData(data) {
    return `Saved: ${data}`
}

// Use them together
async fn workflow(id) {
    const data = await fetchData(id)
    const processed = await processData(data)
    const saved = await saveData(processed)
    return saved
}

// Execute
workflow(123).then(result => println(result))
// Output: Saved: Processed: Data 123
```

## Resources

- **Implementation Docs:** `ASYNC_AWAIT_IMPLEMENTATION.md`
- **Test File:** `tests/test_async_await.c`
- **Examples:** `examples/async_await_demo.k`
- **Promise Docs:** `PROMISES_IMPLEMENTATION.md`
