# Error Handling Guide

This guide explains how errors work in KLang and how to handle them effectively.

## Table of Contents
- [Error Types](#error-types)
- [Common Error Messages](#common-error-messages)
- [Try-Catch-Finally](#try-catch-finally)
- [Error Best Practices](#error-best-practices)
- [Debugging Tips](#debugging-tips)

## Error Types

KLang has several types of errors:

### 1. Syntax Errors
Errors in your code syntax, caught at parse time:

```klang
// Missing closing brace
fn myFunction() {
    println("Hello"
// Error: Expected '}' at end of function

// Invalid syntax
let x = 
// Error: Unexpected end of expression
```

### 2. Runtime Errors
Errors that occur during program execution:

```klang
// Undefined variable
println(undefinedVar)
// Error: undefined variable 'undefinedVar'

// Type errors
let x = "hello"
let y = x + 5
// Error: Cannot add string and number

// Null reference
let obj = null
println(obj.property)
// Error: Cannot access property of null
```

### 3. Type Errors
Errors from incompatible types:

```klang
// Incorrect function arguments
fn add(a, b) { return a + b }
add(1, 2, 3)
// Error: Function expects 2 arguments, got 3

// Method doesn't exist
let arr = [1, 2, 3]
arr.doesNotExist()
// Error: array has no method 'doesNotExist'
```

## Common Error Messages

### "undefined variable 'X'"

**Problem:** Trying to use a variable that hasn't been declared.

```klang
println(name)  // Error!
```

**Solution:**
```klang
let name = "John"
println(name)  // OK
```

### "Cannot access property of null/undefined"

**Problem:** Trying to access a property of a null or undefined value.

```klang
let obj = null
println(obj.name)  // Error!
```

**Solution:**
```klang
let obj = {name: "John"}
println(obj.name)  // OK

// Or check first
if obj != null {
    println(obj.name)
}

// Or use optional chaining (if available)
println(obj?.name)
```

### "array has no method 'X'"

**Problem:** Calling a method that doesn't exist on arrays.

```klang
let arr = [1, 2, 3]
arr.doesNotExist()  // Error!
```

**Solution:**
```klang
// Check available array methods
arr.push(4)      // OK
arr.pop()        // OK
arr.map(x => x * 2)  // OK
arr.filter(x => x > 1)  // OK
```

**Common array methods:**
- `push(item)` - Add to end
- `pop()` - Remove from end
- `shift()` - Remove from start
- `unshift(item)` - Add to start
- `map(fn)` - Transform each element
- `filter(fn)` - Keep matching elements
- `reduce(fn, init)` - Reduce to single value
- `find(fn)` - Find first match
- `forEach(fn)` - Iterate over elements
- `some(fn)` - Check if any match
- `every(fn)` - Check if all match
- `includes(item)` - Check if contains item
- `indexOf(item)` - Find index of item
- `slice(start, end)` - Extract portion
- `concat(arr)` - Combine arrays
- `reverse()` - Reverse order
- `sort(fn)` - Sort elements

### "promise has no method 'X'"

**Problem:** Calling a method that doesn't exist on promises.

```klang
let promise = someAsyncFunction()
promise.doesNotExist()  // Error!
```

**Solution:**
```klang
// Use correct promise methods
promise.then(value => println(value))
promise.catch(error => println("Error:", error))
promise.finally(() => println("Done"))

// Or use async/await
async fn main() {
    try {
        let value = await promise
        println(value)
    } catch (error) {
        println("Error:", error)
    }
}
```

### "Function expects X arguments, got Y"

**Problem:** Calling a function with wrong number of arguments.

```klang
fn greet(name, age) {
    return `Hello ${name}, age ${age}`
}

greet("John")  // Error: expects 2, got 1
```

**Solution:**
```klang
greet("John", 30)  // OK

// Or use default parameters
fn greet(name, age = 0) {
    return `Hello ${name}, age ${age}`
}
greet("John")  // OK, age defaults to 0
```

### "Cannot read file 'X'"

**Problem:** Trying to read a file that doesn't exist or can't be accessed.

```klang
let content = readFile("missing.txt")  // Error!
```

**Solution:**
```klang
// Check if file exists first
if fexists("data.txt") {
    let content = readFile("data.txt")
    println(content)
} else {
    println("File not found")
}

// Or use try-catch
try {
    let content = readFile("data.txt")
    println(content)
} catch (error) {
    println("Error reading file:", error)
}
```

## Try-Catch-Finally

KLang supports standard try-catch-finally error handling:

### Basic Try-Catch

```klang
try {
    // Code that might fail
    let result = riskyOperation()
    println(result)
} catch (error) {
    // Handle the error
    println("An error occurred:", error)
}
```

### Try-Catch-Finally

```klang
let file = null
try {
    file = fopen("data.txt", "r")
    let content = fread(file)
    println(content)
} catch (error) {
    println("Error:", error)
} finally {
    // This always runs, even if there's an error
    if file != null {
        fclose(file)
    }
}
```

### Catching Specific Errors

```klang
try {
    let data = JSON.parse(invalidJson)
} catch (error) {
    if contains(str(error), "JSON") {
        println("JSON parsing error")
    } else {
        println("Other error:", error)
    }
}
```

### Re-throwing Errors

```klang
fn processData(data) {
    try {
        // Process the data
        return transform(data)
    } catch (error) {
        // Log the error
        println("Error in processData:", error)
        // Re-throw to caller
        throw error
    }
}
```

## Error Best Practices

### 1. Always Handle Errors

**Bad:**
```klang
let data = readFile("config.json")  // Might fail!
let config = JSON.parse(data)       // Might fail!
```

**Good:**
```klang
try {
    let data = readFile("config.json")
    let config = JSON.parse(data)
    return config
} catch (error) {
    println("Error loading config:", error)
    return defaultConfig()
}
```

### 2. Use Meaningful Error Messages

**Bad:**
```klang
if !isValid(data) {
    throw "Error"  // Not helpful!
}
```

**Good:**
```klang
if !isValid(data) {
    throw `Invalid data: expected object with 'name' and 'age' fields, got ${type(data)}`
}
```

### 3. Clean Up Resources

**Bad:**
```klang
let file = fopen("data.txt", "w")
fwrite(file, content)  // What if this fails?
fclose(file)
```

**Good:**
```klang
let file = fopen("data.txt", "w")
try {
    fwrite(file, content)
} finally {
    fclose(file)  // Always closes
}
```

### 4. Provide Defaults or Fallbacks

**Bad:**
```klang
fn getConfig() {
    return JSON.parse(readFile("config.json"))
    // Crashes if file doesn't exist
}
```

**Good:**
```klang
fn getConfig() {
    try {
        return JSON.parse(readFile("config.json"))
    } catch (error) {
        println("Using default config")
        return {
            port: 3000,
            host: "localhost"
        }
    }
}
```

### 5. Validate Input Early

**Bad:**
```klang
fn divide(a, b) {
    return a / b  // Fails if b is 0
}
```

**Good:**
```klang
fn divide(a, b) {
    if b == 0 {
        throw "Division by zero"
    }
    return a / b
}
```

### 6. Use Async Error Handling

**Bad:**
```klang
async fn fetchUser(id) {
    let response = await http.get(`/api/users/${id}`)
    // What if request fails?
    return JSON.parse(response)
}
```

**Good:**
```klang
async fn fetchUser(id) {
    try {
        let response = await http.get(`/api/users/${id}`)
        return JSON.parse(response)
    } catch (error) {
        println(`Error fetching user ${id}:`, error)
        return null
    }
}
```

## Debugging Tips

### 1. Use Print Debugging

```klang
fn complexFunction(data) {
    println("Input:", data)
    
    let processed = processStep1(data)
    println("After step 1:", processed)
    
    let filtered = processStep2(processed)
    println("After step 2:", filtered)
    
    return filtered
}
```

### 2. Check Variable Types

```klang
fn handleValue(value) {
    println("Type of value:", type(value))
    
    if type(value) == "string" {
        // Handle string
    } else if type(value) == "number" {
        // Handle number
    } else {
        println("Unexpected type:", type(value))
    }
}
```

### 3. Use the REPL for Testing

```bash
$ klang repl
>>> let x = [1, 2, 3]
>>> x.map(n => n * 2)
[2, 4, 6]
>>> type(x)
"array"
```

### 4. Enable Verbose Error Reporting

```bash
# Run with debug mode
klang run --debug myfile.kl

# Or with verbose flag
klang run --verbose myfile.kl
```

### 5. Add Assertions

```klang
fn processArray(arr) {
    // Validate input
    if type(arr) != "array" {
        throw `Expected array, got ${type(arr)}`
    }
    
    if len(arr) == 0 {
        throw "Array cannot be empty"
    }
    
    // Process safely
    return arr.map(x => x * 2)
}
```

### 6. Use Boundary Testing

```klang
fn testFunction() {
    // Test edge cases
    println("Test empty:", processData([]))
    println("Test single:", processData([1]))
    println("Test multiple:", processData([1, 2, 3]))
    println("Test null:", processData(null))
}
```

## Error Recovery Patterns

### Retry Pattern

```klang
fn fetchWithRetry(url, maxRetries = 3) {
    let retries = 0
    
    while retries < maxRetries {
        try {
            return http.get(url)
        } catch (error) {
            retries++
            if retries >= maxRetries {
                throw `Failed after ${maxRetries} retries: ${error}`
            }
            println(`Retry ${retries}/${maxRetries}`)
            sleep(1000 * retries)  // Exponential backoff
        }
    }
}
```

### Graceful Degradation

```klang
fn getDataWithFallback() {
    // Try primary source
    try {
        return fetchFromAPI()
    } catch (error) {
        println("API failed, trying cache")
    }
    
    // Try cache
    try {
        return loadFromCache()
    } catch (error) {
        println("Cache failed, using defaults")
    }
    
    // Use defaults
    return getDefaultData()
}
```

### Circuit Breaker

```klang
class CircuitBreaker {
    constructor(threshold = 5) {
        this.failures = 0
        this.threshold = threshold
        this.isOpen = false
    }
    
    call(fn) {
        if this.isOpen {
            throw "Circuit breaker is open"
        }
        
        try {
            let result = fn()
            this.failures = 0  // Reset on success
            return result
        } catch (error) {
            this.failures++
            if this.failures >= this.threshold {
                this.isOpen = true
                println("Circuit breaker opened!")
            }
            throw error
        }
    }
}
```

## Getting Help

If you encounter an error you can't solve:

1. **Check the error message** - It usually tells you what's wrong
2. **Read the documentation** - [docs/USER_GUIDE.md](USER_GUIDE.md)
3. **Check examples** - [examples/](../examples/)
4. **Search issues** - [GitHub Issues](https://github.com/k-kaundal/KLang/issues)
5. **Ask the community** - [GitHub Discussions](https://github.com/k-kaundal/KLang/discussions)

## Common Error Solutions Quick Reference

| Error | Quick Fix |
|-------|-----------|
| Undefined variable | Declare with `let` or `const` |
| Null/undefined access | Check with `if` before accessing |
| Method not found | Check method name and available methods |
| Wrong argument count | Match function signature or use defaults |
| File not found | Check path and use `fexists()` |
| JSON parse error | Validate JSON format |
| Type mismatch | Check types with `type()` function |
| Division by zero | Check divisor before dividing |
| Array index out of bounds | Check length with `len()` |
| Promise error | Use `.catch()` or `try-catch` with await |

Remember: Good error handling makes your code more robust and easier to debug! 🛡️
