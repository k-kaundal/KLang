# Spread and Rest Operators

## Mastering Spread and Rest in KLang

The spread (`...`) and rest (`...`) operators are powerful features that make working with arrays and objects easier. Though they look identical, they serve different purposes: spread expands elements, while rest collects them.

## Why Use Spread and Rest?

These operators help you:
- **Combine Arrays/Objects**: Merge data structures efficiently
- **Copy Data**: Create shallow copies without mutation
- **Function Arguments**: Pass multiple arguments dynamically
- **Collect Parameters**: Handle variable number of arguments
- **Array Manipulation**: Clone, concat, and transform arrays
- **Object Merging**: Combine and override properties

## Spread Operator

### Spread with Arrays

Expand array elements:

```klang
let numbers = [1, 2, 3]

# Spread in array literal
let expanded = [...numbers]
println(expanded)  # [1, 2, 3]

# Combine arrays
let moreNumbers = [4, 5, 6]
let combined = [...numbers, ...moreNumbers]
println(combined)  # [1, 2, 3, 4, 5, 6]

# Add elements
let withMore = [0, ...numbers, 4, 5]
println(withMore)  # [0, 1, 2, 3, 4, 5]

# Multiple spreads
let arr1 = [1, 2]
let arr2 = [3, 4]
let arr3 = [5, 6]
let all = [...arr1, ...arr2, ...arr3]
println(all)  # [1, 2, 3, 4, 5, 6]
```

### Copying Arrays

Create shallow copies:

```klang
let original = [1, 2, 3, 4, 5]
let copy = [...original]

# Modify copy
copy[0] = 99

println(original)  # [1, 2, 3, 4, 5] (unchanged)
println(copy)      # [99, 2, 3, 4, 5]

# Compare with reference
let reference = original
reference[0] = 99
println(original)  # [99, 2, 3, 4, 5] (changed!)

# Deep structures need deep copy
let nested = [[1, 2], [3, 4]]
let shallowCopy = [...nested]
shallowCopy[0][0] = 99
println(nested)       # [[99, 2], [3, 4]] (inner array changed!)
println(shallowCopy)  # [[99, 2], [3, 4]]
```

### Spread in Function Calls

```klang
fn sum(a, b, c) {
    return a + b + c
}

let numbers = [1, 2, 3]
println(sum(...numbers))  # 6

# Math functions
let values = [5, 2, 8, 1, 9]
println(Math.max(...values))  # 9
println(Math.min(...values))  # 1

# Push multiple elements
let arr = [1, 2, 3]
arr.push(...[4, 5, 6])
println(arr)  # [1, 2, 3, 4, 5, 6]

# Insert at position
let items = [1, 2, 5, 6]
items.splice(2, 0, ...[3, 4])
println(items)  # [1, 2, 3, 4, 5, 6]
```

### Spread with Objects

Expand object properties:

```klang
let person = {
    name: "Alice",
    age: 25
}

# Spread in object literal
let copied = {...person}
println(copied)  # {name: "Alice", age: 25}

# Combine objects
let contact = {
    email: "alice@example.com",
    phone: "555-1234"
}

let user = {...person, ...contact}
println(user)
# {name: "Alice", age: 25, email: "alice@example.com", phone: "555-1234"}

# Override properties
let updated = {...person, age: 26}
println(updated)  # {name: "Alice", age: 26}

# Order matters
let obj1 = {a: 1, b: 2}
let obj2 = {b: 3, c: 4}

let merged1 = {...obj1, ...obj2}
println(merged1)  # {a: 1, b: 3, c: 4} (obj2.b wins)

let merged2 = {...obj2, ...obj1}
println(merged2)  # {a: 1, b: 2, c: 4} (obj1.b wins)
```

### Adding/Overriding Properties

```klang
let user = {
    id: 1,
    name: "Alice",
    active: true
}

# Add new property
let withEmail = {...user, email: "alice@example.com"}
println(withEmail)
# {id: 1, name: "Alice", active: true, email: "alice@example.com"}

# Override multiple properties
let updated = {
    ...user,
    name: "Alice Smith",
    active: false,
    lastLogin: Date.now()
}

# Conditional spreading
let isAdmin = true
let userWithRole = {
    ...user,
    ...(isAdmin && {role: "admin", permissions: ["read", "write", "delete"]})
}

println(userWithRole)
# {id: 1, name: "Alice", active: true, role: "admin", permissions: [...]}
```

## Rest Operator

### Rest in Arrays

Collect remaining elements:

```klang
# Basic rest
let [first, ...rest] = [1, 2, 3, 4, 5]
println(first)  # 1
println(rest)   # [2, 3, 4, 5]

# Multiple values before rest
let [a, b, ...others] = [1, 2, 3, 4, 5]
println(a)       # 1
println(b)       # 2
println(others)  # [3, 4, 5]

# Rest can be empty
let [x, y, ...remaining] = [1, 2]
println(x)          # 1
println(y)          # 2
println(remaining)  # []

# Works with any iterable
let [head, ...tail] = "Hello"
println(head)  # H
println(tail)  # [e, l, l, o]
```

### Rest in Objects

Collect remaining properties:

```klang
let user = {
    id: 1,
    name: "Alice",
    age: 25,
    email: "alice@example.com",
    city: "NYC"
}

# Basic rest
let {id, name, ...details} = user
println(id)       # 1
println(name)     # Alice
println(details)  # {age: 25, email: "alice@example.com", city: "NYC"}

# Exclude sensitive data
let {password, secretKey, ...publicData} = {
    id: 1,
    name: "Bob",
    password: "secret123",
    secretKey: "xyz789",
    email: "bob@example.com"
}

println(publicData)  # {id: 1, name: "Bob", email: "bob@example.com"}

# Keep specific properties
let {id: userId, ...userData} = user
println(userId)    # 1
println(userData)  # {name: "Alice", age: 25, email: ..., city: "NYC"}
```

### Rest Parameters in Functions

Accept variable number of arguments:

```klang
# Basic rest parameters
fn sum(...numbers) {
    let total = 0
    for let num in numbers {
        total = total + num
    }
    return total
}

println(sum(1, 2, 3))           # 6
println(sum(1, 2, 3, 4, 5))     # 15
println(sum())                   # 0

# Fixed parameters + rest
fn greet(greeting, ...names) {
    for let name in names {
        println(greeting + ", " + name + "!")
    }
}

greet("Hello", "Alice", "Bob", "Charlie")
# Output:
# Hello, Alice!
# Hello, Bob!
# Hello, Charlie!

# Multiple fixed parameters
fn createUser(id, name, ...attributes) {
    return {
        id: id,
        name: name,
        attributes: attributes
    }
}

let user = createUser(1, "Alice", "developer", "admin", "premium")
println(user)
# {id: 1, name: "Alice", attributes: ["developer", "admin", "premium"]}
```

### Rest with Destructuring in Functions

```klang
# Destructure with rest
fn processUser({id, name, ...metadata}) {
    println("User ID: " + str(id))
    println("Name: " + name)
    println("Additional data: " + JSON.stringify(metadata))
}

processUser({
    id: 1,
    name: "Alice",
    age: 25,
    city: "NYC",
    country: "USA"
})

# Array destructuring with rest
fn logFirst([first, ...rest]) {
    println("First: " + first)
    println("Rest: " + rest)
}

logFirst([1, 2, 3, 4, 5])
# Output:
# First: 1
# Rest: [2, 3, 4, 5]
```

## Combining Spread and Rest

### Function Forwarding

```klang
# Wrapper function
fn logAndExecute(fn, ...args) {
    println("Executing with args: " + args)
    return fn(...args)
}

fn add(a, b, c) {
    return a + b + c
}

let result = logAndExecute(add, 1, 2, 3)
# Output: Executing with args: [1, 2, 3]
println(result)  # 6

# Middleware pattern
fn withLogging(fn) {
    return fn(...args) {
        println("Before: " + args)
        let result = fn(...args)
        println("After: " + result)
        return result
    }
}

let loggedAdd = withLogging(add)
loggedAdd(1, 2, 3)
```

### Array Manipulation

```klang
# Insert elements
fn insert(arr, index, ...elements) {
    return [
        ...arr.slice(0, index),
        ...elements,
        ...arr.slice(index)
    ]
}

let numbers = [1, 2, 5, 6]
let result = insert(numbers, 2, 3, 4)
println(result)  # [1, 2, 3, 4, 5, 6]

# Remove elements
fn remove(arr, ...indices) {
    return arr.filter((_, i) => !indices.includes(i))
}

let items = ["a", "b", "c", "d", "e"]
let filtered = remove(items, 1, 3)
println(filtered)  # ["a", "c", "e"]

# Update at index
fn updateAt(arr, index, value) {
    return [
        ...arr.slice(0, index),
        value,
        ...arr.slice(index + 1)
    ]
}

let original = [1, 2, 3, 4, 5]
let updated = updateAt(original, 2, 99)
println(updated)  # [1, 2, 99, 4, 5]
```

### Object Manipulation

```klang
# Merge multiple objects
fn merge(...objects) {
    let result = {}
    for let obj in objects {
        result = {...result, ...obj}
    }
    return result
}

let obj1 = {a: 1, b: 2}
let obj2 = {b: 3, c: 4}
let obj3 = {c: 5, d: 6}

let merged = merge(obj1, obj2, obj3)
println(merged)  # {a: 1, b: 3, c: 5, d: 6}

# Deep merge (one level)
fn deepMerge(target, source) {
    let result = {...target}
    
    for let key in Object.keys(source) {
        if typeof source[key] == "object" && typeof target[key] == "object" {
            result[key] = {...target[key], ...source[key]}
        } else {
            result[key] = source[key]
        }
    }
    
    return result
}

# Pick properties
fn pick(obj, ...keys) {
    let result = {}
    for let key in keys {
        if obj.hasOwnProperty(key) {
            result[key] = obj[key]
        }
    }
    return result
}

let user = {id: 1, name: "Alice", age: 25, email: "alice@example.com"}
let subset = pick(user, "id", "name")
println(subset)  # {id: 1, name: "Alice"}

# Omit properties
fn omit(obj, ...keys) {
    let result = {...obj}
    for let key in keys {
        delete result[key]
    }
    return result
}

let withoutEmail = omit(user, "email", "age")
println(withoutEmail)  # {id: 1, name: "Alice"}
```

## Real-World Use Cases

### React-Style State Updates

```klang
class Component {
    fn constructor(initialState) {
        this.state = initialState
    }
    
    fn setState(updates) {
        this.state = {...this.state, ...updates}
        this.render()
    }
    
    fn render() {
        println("State: " + JSON.stringify(this.state))
    }
}

let comp = Component({count: 0, name: "Counter"})
comp.setState({count: 1})  # Merges with existing state
comp.setState({count: 2, active: true})  # Adds new property
```

### API Request Builder

```klang
class RequestBuilder {
    fn constructor(baseUrl) {
        this.baseUrl = baseUrl
        this.defaultOptions = {
            method: "GET",
            headers: {
                "Content-Type": "application/json"
            },
            timeout: 5000
        }
    }
    
    fn request(endpoint, options = {}) {
        let url = this.baseUrl + endpoint
        
        # Merge options
        let finalOptions = {
            ...this.defaultOptions,
            ...options,
            headers: {
                ...this.defaultOptions.headers,
                ...(options.headers || {})
            }
        }
        
        return fetch(url, finalOptions)
    }
    
    fn get(endpoint, options = {}) {
        return this.request(endpoint, {...options, method: "GET"})
    }
    
    fn post(endpoint, data, options = {}) {
        return this.request(endpoint, {
            ...options,
            method: "POST",
            body: JSON.stringify(data)
        })
    }
}

# Usage
let api = RequestBuilder("https://api.example.com")
api.get("/users", {headers: {"Authorization": "Bearer token"}})
```

### Logger with Levels

```klang
class Logger {
    fn constructor(options = {}) {
        let {
            level = "info",
            prefix = "",
            timestamp = true,
            ...customOptions
        } = options
        
        this.level = level
        this.prefix = prefix
        this.timestamp = timestamp
        this.options = customOptions
    }
    
    fn log(level, ...messages) {
        let parts = []
        
        if this.timestamp {
            parts.push("[" + Date.now() + "]")
        }
        
        if this.prefix {
            parts.push("[" + this.prefix + "]")
        }
        
        parts.push("[" + level + "]")
        parts.push(...messages)
        
        println(...parts)
    }
    
    fn info(...messages) {
        this.log("INFO", ...messages)
    }
    
    fn error(...messages) {
        this.log("ERROR", ...messages)
    }
    
    fn debug(...messages) {
        this.log("DEBUG", ...messages)
    }
}

# Usage
let logger = Logger({prefix: "APP", level: "debug"})
logger.info("Application started")
logger.error("Connection failed", "Retrying...")
logger.debug("User data:", {id: 1, name: "Alice"})
```

### Form Validator

```klang
class FormValidator {
    fn constructor(...rules) {
        this.rules = rules
    }
    
    fn validate(data) {
        let errors = {}
        
        for let rule in this.rules {
            let {field, validators} = rule
            let fieldErrors = []
            
            for let validator in validators {
                let error = validator(data[field], data)
                if error {
                    fieldErrors.push(error)
                }
            }
            
            if fieldErrors.length > 0 {
                errors[field] = fieldErrors
            }
        }
        
        return {
            valid: Object.keys(errors).length == 0,
            errors: errors
        }
    }
    
    fn addRules(...newRules) {
        this.rules = [...this.rules, ...newRules]
    }
}

# Usage
let validator = FormValidator(
    {
        field: "email",
        validators: [
            v => !v && "Email is required",
            v => !v.includes("@") && "Invalid email"
        ]
    },
    {
        field: "password",
        validators: [
            v => !v && "Password is required",
            v => v.length < 8 && "Password too short"
        ]
    }
)

let result = validator.validate({
    email: "alice",
    password: "123"
})

println(result)
# {valid: false, errors: {email: ["Invalid email"], password: ["Password too short"]}}
```

### Immutable Array Operations

```klang
class ImmutableArray {
    fn constructor(...items) {
        this.items = items
    }
    
    fn push(...newItems) {
        return ImmutableArray(...this.items, ...newItems)
    }
    
    fn pop() {
        return ImmutableArray(...this.items.slice(0, -1))
    }
    
    fn shift() {
        return ImmutableArray(...this.items.slice(1))
    }
    
    fn unshift(...newItems) {
        return ImmutableArray(...newItems, ...this.items)
    }
    
    fn map(fn) {
        return ImmutableArray(...this.items.map(fn))
    }
    
    fn filter(fn) {
        return ImmutableArray(...this.items.filter(fn))
    }
    
    fn concat(...others) {
        let allItems = [...this.items]
        for let other in others {
            allItems = [...allItems, ...other.items]
        }
        return ImmutableArray(...allItems)
    }
    
    fn toArray() {
        return [...this.items]
    }
}

# Usage
let arr1 = ImmutableArray(1, 2, 3)
let arr2 = arr1.push(4, 5)
let arr3 = arr2.filter(x => x % 2 == 0)

println(arr1.toArray())  # [1, 2, 3] (unchanged)
println(arr2.toArray())  # [1, 2, 3, 4, 5]
println(arr3.toArray())  # [2, 4]
```

### Configuration Manager

```klang
class Config {
    fn constructor(defaults = {}) {
        this.config = {...defaults}
    }
    
    fn set(...updates) {
        for let update in updates {
            this.config = {...this.config, ...update}
        }
        return this
    }
    
    fn get(...keys) {
        if keys.length == 0 {
            return {...this.config}
        }
        
        if keys.length == 1 {
            return this.config[keys[0]]
        }
        
        let result = {}
        for let key in keys {
            result[key] = this.config[key]
        }
        return result
    }
    
    fn merge(...configs) {
        for let config in configs {
            this.config = {...this.config, ...config}
        }
        return this
    }
    
    fn reset(defaults = {}) {
        this.config = {...defaults}
        return this
    }
}

# Usage
let config = Config({
    apiUrl: "https://api.example.com",
    timeout: 5000,
    retries: 3
})

config.set({timeout: 10000}, {retries: 5})
println(config.get("timeout", "retries"))  # {timeout: 10000, retries: 5}
```

## Common Patterns

### Conditional Spreading

```klang
let isLoggedIn = true
let user = {
    name: "Alice",
    ...(isLoggedIn && {sessionId: "abc123", lastLogin: Date.now()})
}

println(user)
# If logged in: {name: "Alice", sessionId: "abc123", lastLogin: ...}
# If not: {name: "Alice"}

# Multiple conditions
let isPremium = true
let hasAvatar = false

let profile = {
    name: "Bob",
    ...(isPremium && {badge: "premium", features: ["ad-free", "priority"]}),
    ...(hasAvatar && {avatar: "/avatars/bob.jpg"})
}
```

### Array Flattening

```klang
let nested = [[1, 2], [3, 4], [5, 6]]
let flattened = [].concat(...nested)
println(flattened)  # [1, 2, 3, 4, 5, 6]

# One level deep
fn flatten(arr) {
    return [].concat(...arr)
}

let deep = [[1, [2, 3]], [4, [5, 6]]]
let once = flatten(deep)
println(once)  # [1, [2, 3], 4, [5, 6]]

# Recursive flatten
fn flattenDeep(arr) {
    return arr.reduce((acc, val) => {
        return Array.isArray(val)
            ? [...acc, ...flattenDeep(val)]
            : [...acc, val]
    }, [])
}

let deepNested = [1, [2, [3, [4, 5]]]]
let fullyFlat = flattenDeep(deepNested)
println(fullyFlat)  # [1, 2, 3, 4, 5]
```

### Clone with Modifications

```klang
fn cloneAndModify(obj, modifications) {
    return {...obj, ...modifications}
}

let original = {id: 1, name: "Alice", age: 25}
let modified = cloneAndModify(original, {age: 26, city: "NYC"})

println(original)  # {id: 1, name: "Alice", age: 25}
println(modified)  # {id: 1, name: "Alice", age: 26, city: "NYC"}

# Array version
fn cloneArrayAndModify(arr, index, value) {
    return [
        ...arr.slice(0, index),
        value,
        ...arr.slice(index + 1)
    ]
}
```

### Pagination

```klang
fn paginate(items, page, pageSize) {
    let start = (page - 1) * pageSize
    let end = start + pageSize
    
    return {
        items: [...items.slice(start, end)],
        page: page,
        pageSize: pageSize,
        total: items.length,
        hasNext: end < items.length,
        hasPrev: page > 1
    }
}

let allItems = [...Array(100).keys()]  # [0, 1, 2, ..., 99]
let page1 = paginate(allItems, 1, 10)
let page2 = paginate(allItems, 2, 10)

println(page1.items)  # [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
println(page1.hasNext)  # true
```

## Best Practices

### 1. Use Spread for Immutability

```klang
# Good: Immutable update
let user = {name: "Alice", age: 25}
let updated = {...user, age: 26}

# Avoid: Mutating original
let user = {name: "Alice", age: 25}
user.age = 26  # Mutates original
```

### 2. Use Rest for Flexible Functions

```klang
# Good: Flexible parameter count
fn sum(...numbers) {
    return numbers.reduce((a, b) => a + b, 0)
}

# Avoid: Fixed parameters
fn sum(a, b, c, d, e) {
    return a + b + c + d + e  # Limited to 5 numbers
}
```

### 3. Shallow Copy Awareness

```klang
# Be aware of nested objects
let user = {
    name: "Alice",
    address: {city: "NYC"}
}

let copy = {...user}
copy.address.city = "LA"

println(user.address.city)  # LA (changed!)

# Deep copy when needed
let deepCopy = {
    ...user,
    address: {...user.address}
}
```

### 4. Conditional Spreading

```klang
# Good: Clean conditional properties
let obj = {
    name: "Alice",
    ...(condition && {optionalProp: value})
}

# Avoid: Messy conditionals
let obj = {name: "Alice"}
if condition {
    obj.optionalProp = value
}
```

### 5. Order Matters in Merging

```klang
# Properties are overridden left-to-right
let defaults = {a: 1, b: 2}
let custom = {b: 3, c: 4}

let config1 = {...defaults, ...custom}  # {a: 1, b: 3, c: 4}
let config2 = {...custom, ...defaults}  # {a: 1, b: 2, c: 4}
```

## Common Mistakes to Avoid

### 1. Overusing Spread

```klang
# Avoid: Too many spreads
let result = {...obj1, ...obj2, ...obj3, ...obj4, ...obj5}

# Better: Use Object.assign or loop
let result = Object.assign({}, obj1, obj2, obj3, obj4, obj5)
```

### 2. Forgetting Shallow Copy

```klang
# Wrong: Thinking it's a deep copy
let original = {user: {name: "Alice"}}
let copy = {...original}
copy.user.name = "Bob"
println(original.user.name)  # Bob (!)

# Correct: Deep copy
let copy = {
    ...original,
    user: {...original.user}
}
```

### 3. Rest Must Be Last

```klang
# Wrong
fn bad(...rest, last) {  # Syntax error
}

let [...rest, last] = arr  # Syntax error

# Correct
fn good(first, ...rest) {
}

let [first, ...rest] = arr
```

## Next Steps

Now that you understand spread and rest, explore these topics:

- **[Destructuring](25-destructuring.md)** - Extract values efficiently
- **[Arrays](14-arrays.md)** - Array manipulation
- **[Objects](15-objects.md)** - Object operations
- **[Functions](13-functions.md)** - Advanced function patterns
- **[Immutability](29-immutability.md)** - Immutable data patterns

Ready to write more flexible code? Start using spread and rest in your projects!
