# Closures

## Mastering Closures and Lexical Scope in KLang

Closures are one of the most powerful features in KLang, allowing functions to "remember" and access variables from their outer scope even after that scope has finished executing. Understanding closures is essential for writing advanced, maintainable code.

## Why Use Closures?

Closures enable you to:
- **Encapsulate Data**: Create private variables that can't be accessed directly
- **Create Factories**: Build functions that generate customized functions
- **Maintain State**: Preserve values between function calls without global variables
- **Implement Callbacks**: Access outer scope variables in asynchronous code
- **Build Modules**: Create self-contained, reusable code modules
- **Functional Programming**: Support currying, partial application, and composition

## Understanding Scope

### Lexical Scope Basics

Lexical scope means a function can access variables from its parent scope:

```klang
let globalVar = "I'm global"

fn outerFunction() {
    let outerVar = "I'm outer"
    
    fn innerFunction() {
        let innerVar = "I'm inner"
        
        # Can access all three scopes
        println(globalVar)   # I'm global
        println(outerVar)    # I'm outer
        println(innerVar)    # I'm inner
    }
    
    innerFunction()
}

outerFunction()
```

### Scope Chain

Variables are looked up through the scope chain:

```klang
let level1 = "Level 1"

fn first() {
    let level2 = "Level 2"
    
    fn second() {
        let level3 = "Level 3"
        
        fn third() {
            let level4 = "Level 4"
            
            # Access all levels
            println(level1)  # Level 1
            println(level2)  # Level 2
            println(level3)  # Level 3
            println(level4)  # Level 4
        }
        
        third()
    }
    
    second()
}

first()
```

### Block Scope

Variables declared with `let` are block-scoped:

```klang
fn demonstrateBlockScope() {
    let x = 1
    
    if true {
        let x = 2  # Different variable
        println(x)  # 2
    }
    
    println(x)  # 1
    
    for let i = 0; i < 3; i++ {
        let x = i  # Different variable for each iteration
        println(x)
    }
    
    println(x)  # 1 (unchanged)
}

demonstrateBlockScope()
```

## Basic Closures

### Simple Closure

A function that remembers its environment:

```klang
fn makeGreeter(greeting) {
    # 'greeting' is captured by the closure
    return fn(name) {
        return greeting + ", " + name + "!"
    }
}

let sayHello = makeGreeter("Hello")
let sayHi = makeGreeter("Hi")

println(sayHello("Alice"))  # Hello, Alice!
println(sayHi("Bob"))       # Hi, Bob!
```

### Closure with State

```klang
fn createCounter() {
    let count = 0  # Private variable
    
    return fn() {
        count = count + 1
        return count
    }
}

let counter1 = createCounter()
let counter2 = createCounter()

println(counter1())  # 1
println(counter1())  # 2
println(counter1())  # 3

println(counter2())  # 1 (separate state)
println(counter2())  # 2
```

### Multiple Functions Sharing State

```klang
fn createAccount(initialBalance) {
    let balance = initialBalance  # Private
    
    return {
        deposit: fn(amount) {
            balance = balance + amount
            println("Deposited: $" + str(amount))
        },
        
        withdraw: fn(amount) {
            if amount <= balance {
                balance = balance - amount
                println("Withdrawn: $" + str(amount))
                return true
            } else {
                println("Insufficient funds")
                return false
            }
        },
        
        getBalance: fn() {
            return balance
        }
    }
}

let myAccount = createAccount(1000)
myAccount.deposit(500)                      # Deposited: $500
myAccount.withdraw(200)                     # Withdrawn: $200
println("Balance: $" + str(myAccount.getBalance()))  # Balance: $1300

# Can't access balance directly
println(myAccount.balance)  # undefined
```

## Advanced Closure Patterns

### Function Factory

Create customized functions:

```klang
fn makeMultiplier(factor) {
    return fn(number) {
        return number * factor
    }
}

let double = makeMultiplier(2)
let triple = makeMultiplier(3)
let quadruple = makeMultiplier(4)

println(double(5))      # 10
println(triple(5))      # 15
println(quadruple(5))   # 20

# More complex factory
fn createValidator(rules) {
    return fn(value) {
        for let rule in rules {
            if !rule(value) {
                return false
            }
        }
        return true
    }
}

let isValidEmail = createValidator([
    fn(v) { return v.length > 0 },
    fn(v) { return v.contains("@") },
    fn(v) { return v.contains(".") }
])

println(isValidEmail("test@example.com"))  # true
println(isValidEmail("invalid"))            # false
```

### Partial Application

Pre-fill some arguments:

```klang
fn partial(fn, ...fixedArgs) {
    return fn(...dynamicArgs) {
        return fn(...fixedArgs, ...dynamicArgs)
    }
}

fn add(a, b, c) {
    return a + b + c
}

let addFive = partial(add, 5)
println(addFive(3, 2))  # 10 (5 + 3 + 2)

let addFiveAndThree = partial(add, 5, 3)
println(addFiveAndThree(2))  # 10 (5 + 3 + 2)

# Practical example
fn greetUser(greeting, punctuation, name) {
    return greeting + ", " + name + punctuation
}

let sayHello = partial(greetUser, "Hello", "!")
println(sayHello("Alice"))  # Hello, Alice!

let askQuestion = partial(greetUser, "Hello", "?")
println(askQuestion("Bob"))  # Hello, Bob?
```

### Currying

Transform multi-argument function into sequence of single-argument functions:

```klang
fn curry(fn) {
    return fn curried(...args) {
        if args.length >= fn.length {
            return fn(...args)
        }
        return fn(...nextArgs) {
            return curried(...args, ...nextArgs)
        }
    }
}

fn add(a, b, c) {
    return a + b + c
}

let curriedAdd = curry(add)

println(curriedAdd(1)(2)(3))      # 6
println(curriedAdd(1, 2)(3))      # 6
println(curriedAdd(1)(2, 3))      # 6

# Practical example
fn formatString(prefix, text, suffix) {
    return prefix + text + suffix
}

let format = curry(formatString)
let wrapInBrackets = format("[")("]")
let wrapInParens = format("(")(")") 

println(wrapInBrackets("hello"))  # [hello]
println(wrapInParens("world"))    # (world)
```

### Memoization

Cache function results:

```klang
fn memoize(fn) {
    let cache = {}
    
    return fn(...args) {
        let key = JSON.stringify(args)
        
        if cache[key] != undefined {
            println("Returning cached result")
            return cache[key]
        }
        
        println("Computing result")
        let result = fn(...args)
        cache[key] = result
        return result
    }
}

fn fibonacci(n) {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

let memoFib = memoize(fibonacci)

println(memoFib(10))  # Computing result
println(memoFib(10))  # Returning cached result

# More complex memoization
fn expensiveCalculation(a, b, c) {
    # Simulate expensive operation
    let result = 0
    for let i = 0; i < 1000000; i++ {
        result = result + (a * b * c)
    }
    return result
}

let memoCalc = memoize(expensiveCalculation)
memoCalc(2, 3, 4)  # Takes time
memoCalc(2, 3, 4)  # Instant (cached)
```

## Private Variables and Encapsulation

### Module Pattern

Create private state and public interface:

```klang
let userModule = (fn() {
    # Private variables
    let users = []
    let currentId = 0
    
    # Private function
    let generateId = fn() {
        currentId = currentId + 1
        return currentId
    }
    
    # Public interface
    return {
        addUser: fn(name, email) {
            let user = {
                id: generateId(),
                name: name,
                email: email
            }
            users.push(user)
            return user
        },
        
        getUser: fn(id) {
            for let user in users {
                if user.id == id {
                    return user
                }
            }
            return null
        },
        
        getAllUsers: fn() {
            return users.slice()  # Return copy
        },
        
        deleteUser: fn(id) {
            let newUsers = []
            for let user in users {
                if user.id != id {
                    newUsers.push(user)
                }
            }
            users = newUsers
        }
    }
})()

# Usage
let user1 = userModule.addUser("Alice", "alice@example.com")
let user2 = userModule.addUser("Bob", "bob@example.com")

println(userModule.getUser(1).name)  # Alice
println(userModule.getAllUsers().length)  # 2

# Can't access private variables
println(userModule.users)  # undefined
```

### Singleton Pattern with Closure

```klang
let Database = (fn() {
    let instance = null
    
    fn createInstance() {
        let connection = null
        
        return {
            connect: fn() {
                connection = "Connected to DB"
                println("Database connected")
            },
            
            query: fn(sql) {
                if !connection {
                    println("Not connected")
                    return null
                }
                println("Executing: " + sql)
                return []
            },
            
            disconnect: fn() {
                connection = null
                println("Database disconnected")
            }
        }
    }
    
    return {
        getInstance: fn() {
            if !instance {
                instance = createInstance()
            }
            return instance
        }
    }
})()

let db1 = Database.getInstance()
let db2 = Database.getInstance()

db1.connect()
db2.query("SELECT * FROM users")  # Uses same connection
println(db1 == db2)  # true (same instance)
```

### Private Methods

```klang
fn createSecureStore() {
    let data = {}
    let accessLog = []
    
    # Private method
    let log = fn(action, key) {
        accessLog.push({
            action: action,
            key: key,
            timestamp: Date.now()
        })
    }
    
    # Private validation
    let isValidKey = fn(key) {
        return typeof key == "string" && key.length > 0
    }
    
    # Public methods
    return {
        set: fn(key, value) {
            if !isValidKey(key) {
                throw "Invalid key"
            }
            data[key] = value
            log("set", key)
        },
        
        get: fn(key) {
            log("get", key)
            return data[key]
        },
        
        has: fn(key) {
            return data[key] != undefined
        },
        
        delete: fn(key) {
            delete data[key]
            log("delete", key)
        },
        
        getAccessLog: fn() {
            return accessLog.slice()  # Return copy
        }
    }
}

let store = createSecureStore()
store.set("user", "Alice")
store.get("user")
println(store.has("user"))  # true
println(store.getAccessLog().length)  # 2 (set and get)
```

## Event Handlers and Callbacks

### Event Emitter

```klang
fn createEventEmitter() {
    let listeners = {}
    
    return {
        on: fn(event, callback) {
            if !listeners[event] {
                listeners[event] = []
            }
            listeners[event].push(callback)
        },
        
        off: fn(event, callback) {
            if listeners[event] {
                let newListeners = []
                for let listener in listeners[event] {
                    if listener != callback {
                        newListeners.push(listener)
                    }
                }
                listeners[event] = newListeners
            }
        },
        
        emit: fn(event, data) {
            if listeners[event] {
                for let listener in listeners[event] {
                    listener(data)
                }
            }
        },
        
        once: fn(event, callback) {
            let wrappedCallback = fn(data) {
                callback(data)
                this.off(event, wrappedCallback)
            }
            this.on(event, wrappedCallback)
        }
    }
}

let emitter = createEventEmitter()

# Add listeners
emitter.on("userLogin", fn(user) {
    println(user.name + " logged in")
})

emitter.on("userLogin", fn(user) {
    println("Welcome back, " + user.name)
})

# Trigger event
emitter.emit("userLogin", {name: "Alice"})
# Output:
# Alice logged in
# Welcome back, Alice
```

### Debounce Function

```klang
fn debounce(fn, delay) {
    let timeoutId = null
    
    return fn(...args) {
        if timeoutId {
            clearTimeout(timeoutId)
        }
        
        timeoutId = setTimeout(fn() {
            fn(...args)
        }, delay)
    }
}

# Usage
let expensiveOperation = fn(value) {
    println("Processing: " + value)
}

let debouncedOperation = debounce(expensiveOperation, 500)

# Called multiple times quickly
debouncedOperation("a")
debouncedOperation("ab")
debouncedOperation("abc")
debouncedOperation("abcd")
# Only "Processing: abcd" is printed after 500ms
```

### Throttle Function

```klang
fn throttle(fn, interval) {
    let lastCall = 0
    
    return fn(...args) {
        let now = Date.now()
        
        if now - lastCall >= interval {
            lastCall = now
            fn(...args)
        }
    }
}

# Usage
let logScroll = fn(position) {
    println("Scrolled to: " + str(position))
}

let throttledScroll = throttle(logScroll, 1000)

# Called many times
for let i = 0; i < 10; i++ {
    throttledScroll(i * 100)
}
# Only logs every 1000ms
```

## Real-World Use Cases

### Shopping Cart

```klang
fn createShoppingCart() {
    let items = []
    let discountRate = 0
    
    return {
        addItem: fn(product, quantity) {
            let existingItem = null
            
            for let item in items {
                if item.product.id == product.id {
                    existingItem = item
                    break
                }
            }
            
            if existingItem {
                existingItem.quantity = existingItem.quantity + quantity
            } else {
                items.push({product: product, quantity: quantity})
            }
            
            println("Added " + str(quantity) + "x " + product.name)
        },
        
        removeItem: fn(productId) {
            let newItems = []
            for let item in items {
                if item.product.id != productId {
                    newItems.push(item)
                }
            }
            items = newItems
        },
        
        setDiscount: fn(rate) {
            discountRate = rate
            println("Discount set to " + str(rate * 100) + "%")
        },
        
        getTotal: fn() {
            let subtotal = 0
            for let item in items {
                subtotal = subtotal + (item.product.price * item.quantity)
            }
            return subtotal * (1 - discountRate)
        },
        
        getItems: fn() {
            return items.slice()
        },
        
        clear: fn() {
            items = []
            discountRate = 0
        }
    }
}

let cart = createShoppingCart()
cart.addItem({id: 1, name: "Laptop", price: 1000}, 1)
cart.addItem({id: 2, name: "Mouse", price: 25}, 2)
cart.setDiscount(0.1)
println("Total: $" + str(cart.getTotal()))  # Total: $945
```

### API Client with Rate Limiting

```klang
fn createApiClient(baseUrl, rateLimit) {
    let requestQueue = []
    let lastRequestTime = 0
    let minInterval = 1000 / rateLimit  # milliseconds between requests
    
    let processQueue = fn() {
        if requestQueue.length == 0 {
            return
        }
        
        let now = Date.now()
        if now - lastRequestTime >= minInterval {
            let request = requestQueue.shift()
            lastRequestTime = now
            
            # Make actual request
            fetch(baseUrl + request.endpoint, request.options)
                .then(request.resolve)
                .catch(request.reject)
            
            # Process next in queue
            setTimeout(processQueue, minInterval)
        } else {
            let waitTime = minInterval - (now - lastRequestTime)
            setTimeout(processQueue, waitTime)
        }
    }
    
    return {
        get: fn(endpoint) {
            return Promise(fn(resolve, reject) {
                requestQueue.push({
                    endpoint: endpoint,
                    options: {method: "GET"},
                    resolve: resolve,
                    reject: reject
                })
                processQueue()
            })
        },
        
        post: fn(endpoint, data) {
            return Promise(fn(resolve, reject) {
                requestQueue.push({
                    endpoint: endpoint,
                    options: {method: "POST", body: data},
                    resolve: resolve,
                    reject: reject
                })
                processQueue()
            })
        }
    }
}

let api = createApiClient("https://api.example.com", 10)  # 10 requests/second
api.get("/users")
api.get("/posts")
api.get("/comments")
# Automatically rate limited
```

### State Machine

```klang
fn createStateMachine(initialState, transitions) {
    let currentState = initialState
    let history = [initialState]
    
    return {
        getState: fn() {
            return currentState
        },
        
        transition: fn(action) {
            let nextState = transitions[currentState]?.[action]
            
            if nextState {
                history.push(nextState)
                currentState = nextState
                println("Transitioned to: " + nextState)
                return true
            } else {
                println("Invalid transition: " + action + " from " + currentState)
                return false
            }
        },
        
        canTransition: fn(action) {
            return transitions[currentState]?.[action] != undefined
        },
        
        getHistory: fn() {
            return history.slice()
        }
    }
}

let doorStateMachine = createStateMachine("closed", {
    closed: {
        open: "opened",
        lock: "locked"
    },
    opened: {
        close: "closed"
    },
    locked: {
        unlock: "closed"
    }
})

doorStateMachine.transition("open")    # Transitioned to: opened
doorStateMachine.transition("close")   # Transitioned to: closed
doorStateMachine.transition("lock")    # Transitioned to: locked
doorStateMachine.transition("open")    # Invalid transition
println(doorStateMachine.getHistory()) # [closed, opened, closed, locked]
```

### Middleware Pattern

```klang
fn createMiddlewareStack() {
    let middlewares = []
    
    return {
        use: fn(middleware) {
            middlewares.push(middleware)
        },
        
        execute: fn(context) {
            let index = 0
            
            let next = fn() {
                if index < middlewares.length {
                    let middleware = middlewares[index]
                    index = index + 1
                    middleware(context, next)
                }
            }
            
            next()
        }
    }
}

let app = createMiddlewareStack()

# Add middlewares
app.use(fn(ctx, next) {
    println("1. Logging request: " + ctx.url)
    next()
})

app.use(fn(ctx, next) {
    println("2. Authentication")
    if ctx.authenticated {
        next()
    } else {
        println("   Authentication failed")
    }
})

app.use(fn(ctx, next) {
    println("3. Processing request")
    ctx.result = "Success"
    next()
})

# Execute
app.execute({url: "/api/users", authenticated: true})
# Output:
# 1. Logging request: /api/users
# 2. Authentication
# 3. Processing request
```

## Common Patterns

### Counter with Multiple Operations

```klang
fn createFlexibleCounter(initial) {
    let count = initial || 0
    
    return {
        increment: fn(amount) {
            count = count + (amount || 1)
            return count
        },
        
        decrement: fn(amount) {
            count = count - (amount || 1)
            return count
        },
        
        reset: fn() {
            count = initial || 0
            return count
        },
        
        getValue: fn() {
            return count
        },
        
        setValue: fn(value) {
            count = value
            return count
        }
    }
}

let counter = createFlexibleCounter(10)
counter.increment()      # 11
counter.increment(5)     # 16
counter.decrement(3)     # 13
counter.reset()          # 10
```

### Configuration Manager

```klang
fn createConfig(defaults) {
    let config = {...defaults}
    let locked = false
    
    return {
        get: fn(key) {
            return config[key]
        },
        
        set: fn(key, value) {
            if locked {
                throw "Configuration is locked"
            }
            config[key] = value
        },
        
        lock: fn() {
            locked = true
            println("Configuration locked")
        },
        
        unlock: fn() {
            locked = false
            println("Configuration unlocked")
        },
        
        getAll: fn() {
            return {...config}  # Return copy
        }
    }
}

let config = createConfig({
    apiUrl: "https://api.example.com",
    timeout: 5000,
    retries: 3
})

println(config.get("apiUrl"))
config.set("timeout", 10000)
config.lock()
# config.set("retries", 5)  # Would throw error
```

### Lazy Evaluation

```klang
fn lazy(fn) {
    let cached = false
    let value = null
    
    return fn() {
        if !cached {
            value = fn()
            cached = true
        }
        return value
    }
}

let expensiveComputation = lazy(fn() {
    println("Computing...")
    let result = 0
    for let i = 0; i < 1000000; i++ {
        result = result + i
    }
    return result
})

println("Before first call")
println(expensiveComputation())  # Computing... then result
println("Before second call")
println(expensiveComputation())  # Returns cached result immediately
```

## Best Practices

### 1. Keep Closures Simple

```klang
# Good: Clear and focused
fn createLogger(prefix) {
    return fn(message) {
        println(prefix + ": " + message)
    }
}

# Avoid: Too complex
fn createComplexThing(a, b, c, d) {
    let x = a + b
    let y = c * d
    # ... many lines ...
    return fn(arg) {
        # ... complex logic ...
    }
}
```

### 2. Don't Overuse Closures

```klang
# Good: Simple case, no closure needed
fn add(a, b) {
    return a + b
}

# Avoid: Unnecessary closure
fn createAdder() {
    return fn(a, b) {
        return a + b
    }
}
let add = createAdder()
```

### 3. Be Careful with Loops

```klang
# Problematic: All callbacks reference same 'i'
let callbacks = []
for let i = 0; i < 3; i++ {
    callbacks.push(fn() {
        println(i)
    })
}
callbacks.forEach(cb => cb())  # Might print 3, 3, 3

# Better: Create new scope with IIFE
let callbacks = []
for let i = 0; i < 3; i++ {
    callbacks.push((fn(index) {
        return fn() {
            println(index)
        }
    })(i))
}
callbacks.forEach(cb => cb())  # Prints 0, 1, 2

# Best: Use modern syntax
let callbacks = []
for let i = 0; i < 3; i++ {
    let index = i  # Block-scoped
    callbacks.push(fn() {
        println(index)
    })
}
```

### 4. Document Captured Variables

```klang
fn createValidator(rules) {
    # Captures: rules (array of validation functions)
    
    return fn(value) {
        # Validates value against all captured rules
        for let rule in rules {
            if !rule(value) {
                return false
            }
        }
        return true
    }
}
```

### 5. Avoid Memory Leaks

```klang
# Problematic: Holds reference to large object
fn createProcessor(largeData) {
    return fn() {
        # Uses largeData
    }
}

# Better: Extract only what you need
fn createProcessor(largeData) {
    let needed = largeData.someSmallProperty
    return fn() {
        # Uses only needed data
    }
}
```

## Common Mistakes to Avoid

### 1. Modifying Captured Variables Unexpectedly

```klang
# Unexpected behavior
fn createFunctions() {
    let funcs = []
    let i = 0
    
    while i < 3 {
        funcs.push(fn() {
            println(i)
        })
        i = i + 1
    }
    
    return funcs
}

let funcs = createFunctions()
funcs[0]()  # Prints 3 (not 0!)
```

### 2. Creating Unnecessary Closures in Loops

```klang
# Inefficient
let processed = []
for let item in items {
    processed.push((fn(x) {
        return fn() {
            return process(x)
        }
    })(item))
}

# Better
let processed = items.map(item => () => process(item))
```

### 3. Not Understanding Scope

```klang
# Wrong: Variable not captured
fn broken() {
    if true {
        let temp = "value"
    }
    
    return fn() {
        println(temp)  # Error: temp not in scope
    }
}

# Correct
fn working() {
    let temp = "value"
    
    return fn() {
        println(temp)  # OK
    }
}
```

## Next Steps

Now that you understand closures, explore these related topics:

- **[Functions](13-functions.md)** - Master function fundamentals
- **[Arrow Functions](21-arrow-functions.md)** - Learn concise function syntax
- **[Classes](20-classes-oop.md)** - Use classes for encapsulation
- **[Async/Await](23-async-await.md)** - Closures in async code
- **[Functional Programming](28-functional-programming.md)** - Advanced patterns

Ready to write more powerful code? Start using closures for data privacy and function factories!
