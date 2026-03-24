# Destructuring

## Mastering Destructuring in KLang

Destructuring is a powerful syntax that lets you extract values from arrays and objects into distinct variables. It makes your code cleaner, more readable, and helps you work with complex data structures efficiently.

## Why Use Destructuring?

Destructuring helps you:
- **Extract Values Easily**: Pull data from arrays and objects in one line
- **Swap Variables**: Exchange values without temporary variables
- **Function Parameters**: Accept complex data with clean syntax
- **Default Values**: Provide fallbacks during extraction
- **Ignore Values**: Skip unwanted elements
- **Cleaner Code**: Write more expressive, readable code

## Array Destructuring

### Basic Array Destructuring

Extract array elements into variables:

```klang
# Basic destructuring
let numbers = [1, 2, 3]
let [a, b, c] = numbers

println(a)  # 1
println(b)  # 2
println(c)  # 3

# Works with any iterable
let [first, second] = "Hello"
println(first)   # H
println(second)  # e

# Destructure function return values
fn getCoordinates() {
    return [10, 20]
}

let [x, y] = getCoordinates()
println("x: " + str(x) + ", y: " + str(y))  # x: 10, y: 20
```

### Skipping Elements

```klang
let numbers = [1, 2, 3, 4, 5]

# Skip elements with commas
let [first, , third] = numbers
println(first)  # 1
println(third)  # 3

# Skip multiple elements
let [a, , , d] = numbers
println(a)  # 1
println(d)  # 4

# Get first and last
let [head, , , , tail] = numbers
println(head)  # 1
println(tail)  # 5
```

### Rest Pattern

Collect remaining elements:

```klang
let numbers = [1, 2, 3, 4, 5]

# Rest of array
let [first, ...rest] = numbers
println(first)  # 1
println(rest)   # [2, 3, 4, 5]

# Multiple values then rest
let [a, b, ...others] = numbers
println(a)       # 1
println(b)       # 2
println(others)  # [3, 4, 5]

# Works anywhere except last position
let [head, ...middle, last] = numbers  # Error!
let [...initial, last] = numbers        # Error in some implementations
```

### Default Values

Provide fallback values:

```klang
# Basic defaults
let [a = 10, b = 20] = []
println(a)  # 10
println(b)  # 20

# Partial defaults
let [x = 1, y = 2, z = 3] = [100]
println(x)  # 100
println(y)  # 2
println(z)  # 3

# Default with undefined
let [m = 5, n = 10] = [undefined, 20]
println(m)  # 5
println(n)  # 20

# Default with expressions
let count = 0
let [value = count++] = []
println(value)  # 0
println(count)  # 1
```

### Swapping Variables

```klang
let a = 1
let b = 2

# Swap without temporary variable
[a, b] = [b, a]

println(a)  # 2
println(b)  # 1

# Rotate three variables
let x = 1, y = 2, z = 3
[x, y, z] = [z, x, y]
println([x, y, z])  # [3, 1, 2]

# Swap in algorithm
fn bubbleSort(arr) {
    for let i = 0; i < arr.length; i++ {
        for let j = 0; j < arr.length - 1; j++ {
            if arr[j] > arr[j + 1] {
                [arr[j], arr[j + 1]] = [arr[j + 1], arr[j]]
            }
        }
    }
    return arr
}
```

## Object Destructuring

### Basic Object Destructuring

Extract object properties:

```klang
let user = {
    name: "Alice",
    age: 25,
    email: "alice@example.com"
}

# Extract properties
let {name, age, email} = user
println(name)   # Alice
println(age)    # 25
println(email)  # alice@example.com

# Order doesn't matter
let {email: userEmail, name: userName} = user
println(userName)   # Alice
println(userEmail)  # alice@example.com

# Extract subset
let {name} = user
println(name)  # Alice
```

### Renaming Variables

```klang
let user = {
    name: "Bob",
    age: 30
}

# Rename during destructuring
let {name: userName, age: userAge} = user
println(userName)  # Bob
println(userAge)   # 30

# Useful for avoiding conflicts
let name = "Alice"
let {name: personName} = user
println(name)        # Alice (unchanged)
println(personName)  # Bob

# Multiple renames
let person = {firstName: "John", lastName: "Doe"}
let {firstName: first, lastName: last} = person
println(first + " " + last)  # John Doe
```

### Default Values

```klang
# Basic defaults
let {name = "Unknown", age = 0} = {}
println(name)  # Unknown
println(age)   # 0

# Partial defaults
let user = {name: "Alice"}
let {name, age = 25, city = "NYC"} = user
println(name)  # Alice
println(age)   # 25
println(city)  # NYC

# Rename with defaults
let {name: userName = "Guest", age: userAge = 18} = {}
println(userName)  # Guest
println(userAge)   # 18

# Complex defaults
let config = {timeout: 5000}
let {
    timeout = 3000,
    retries = 3,
    baseUrl = "https://api.example.com"
} = config

println(timeout)  # 5000
println(retries)  # 3
```

### Rest Pattern with Objects

```klang
let user = {
    id: 1,
    name: "Alice",
    age: 25,
    email: "alice@example.com",
    city: "NYC"
}

# Extract specific props, rest goes to another object
let {id, name, ...details} = user
println(id)       # 1
println(name)     # Alice
println(details)  # {age: 25, email: "alice@example.com", city: "NYC"}

# Useful for filtering
let {password, ...publicUser} = {
    id: 1,
    name: "Bob",
    password: "secret123",
    email: "bob@example.com"
}
println(publicUser)  # {id: 1, name: "Bob", email: "bob@example.com"}
```

## Nested Destructuring

### Nested Arrays

```klang
let matrix = [[1, 2], [3, 4], [5, 6]]

# Destructure nested arrays
let [[a, b], [c, d]] = matrix
println(a)  # 1
println(b)  # 2
println(c)  # 3
println(d)  # 4

# Mixed levels
let data = [1, [2, 3], 4]
let [x, [y, z], w] = data
println(x)  # 1
println(y)  # 2
println(z)  # 3
println(w)  # 4

# Deep nesting
let nested = [1, [2, [3, [4, 5]]]]
let [a, [b, [c, [d, e]]]] = nested
println(e)  # 5
```

### Nested Objects

```klang
let user = {
    id: 1,
    name: "Alice",
    address: {
        street: "123 Main St",
        city: "NYC",
        zipCode: "10001"
    },
    contact: {
        email: "alice@example.com",
        phone: "555-1234"
    }
}

# Extract nested properties
let {
    name,
    address: {city, zipCode},
    contact: {email}
} = user

println(name)     # Alice
println(city)     # NYC
println(zipCode)  # 10001
println(email)    # alice@example.com

# Rename nested properties
let {
    address: {street: userStreet, city: userCity}
} = user

println(userStreet)  # 123 Main St
println(userCity)    # NYC
```

### Mixed Nested Structures

```klang
let data = {
    id: 1,
    name: "Product",
    variants: [
        {id: 101, size: "S", colors: ["red", "blue"]},
        {id: 102, size: "M", colors: ["green", "yellow"]}
    ]
}

# Complex destructuring
let {
    name: productName,
    variants: [
        {size: firstSize, colors: [firstColor]},
        {size: secondSize}
    ]
} = data

println(productName)   # Product
println(firstSize)     # S
println(firstColor)    # red
println(secondSize)    # M

# Real-world API response
let response = {
    status: 200,
    data: {
        users: [
            {id: 1, name: "Alice", posts: [{title: "Hello"}]},
            {id: 2, name: "Bob", posts: [{title: "World"}]}
        ]
    }
}

let {
    status,
    data: {
        users: [
            {name: firstName, posts: [{title: firstPost}]},
            {name: secondName}
        ]
    }
} = response

println(status)      # 200
println(firstName)   # Alice
println(firstPost)   # Hello
println(secondName)  # Bob
```

## Function Parameters

### Array Parameter Destructuring

```klang
# Destructure array parameters
fn printCoordinates([x, y]) {
    println("x: " + str(x) + ", y: " + str(y))
}

printCoordinates([10, 20])  # x: 10, y: 20

# With defaults
fn drawPoint([x = 0, y = 0]) {
    println("Drawing point at (" + str(x) + ", " + str(y) + ")")
}

drawPoint([5, 10])  # Drawing point at (5, 10)
drawPoint([5])      # Drawing point at (5, 0)
drawPoint([])       # Drawing point at (0, 0)

# With rest
fn sum([first, ...rest]) {
    let total = first
    for let num in rest {
        total = total + num
    }
    return total
}

println(sum([1, 2, 3, 4, 5]))  # 15
```

### Object Parameter Destructuring

```klang
# Clean function signatures
fn createUser({name, age, email}) {
    println("Creating user: " + name)
    return {
        id: generateId(),
        name: name,
        age: age,
        email: email,
        createdAt: Date.now()
    }
}

createUser({
    name: "Alice",
    age: 25,
    email: "alice@example.com"
})

# With defaults
fn fetchData({url, method = "GET", timeout = 5000, headers = {}}) {
    println("Fetching " + url + " with " + method)
    # ... fetch logic
}

fetchData({url: "/api/users"})  # Uses defaults
fetchData({url: "/api/posts", method: "POST", timeout: 10000})

# With renaming
fn processUser({name: userName, age: userAge}) {
    println(userName + " is " + str(userAge) + " years old")
}

processUser({name: "Bob", age: 30})  # Bob is 30 years old
```

### Nested Parameter Destructuring

```klang
fn displayUser({name, address: {city, country}, contact: {email}}) {
    println(name + " from " + city + ", " + country)
    println("Email: " + email)
}

displayUser({
    name: "Alice",
    address: {city: "NYC", country: "USA"},
    contact: {email: "alice@example.com"}
})

# Complex configuration
fn initializeApp({
    server: {host = "localhost", port = 3000},
    database: {url, poolSize = 10},
    logging: {level = "info", ...logOptions}
}) {
    println("Server: " + host + ":" + str(port))
    println("Database: " + url)
    println("Pool size: " + str(poolSize))
    println("Log level: " + level)
}
```

## Real-World Use Cases

### API Response Handling

```klang
async fn fetchUserProfile(userId) {
    let response = await fetch("/api/users/" + str(userId))
    
    # Destructure response
    let {
        data: {
            id,
            profile: {name, bio, avatar},
            stats: {followers, following, posts}
        },
        meta: {cached, timestamp}
    } = response
    
    return {
        id,
        name,
        bio,
        avatar,
        followers,
        following,
        posts,
        cached,
        timestamp
    }
}

# Usage
let profile = await fetchUserProfile(123)
let {name, followers, posts} = profile
println(name + " has " + str(followers) + " followers and " + str(posts) + " posts")
```

### React-Style Props

```klang
# Component with destructured props
fn UserCard({user: {name, age, avatar}, onEdit, onDelete}) {
    return {
        render: fn() {
            return `
                <div class="user-card">
                    <img src="${avatar}" alt="${name}" />
                    <h3>${name}</h3>
                    <p>Age: ${age}</p>
                    <button onclick="${onEdit}">Edit</button>
                    <button onclick="${onDelete}">Delete</button>
                </div>
            `
        }
    }
}

# Usage
let card = UserCard({
    user: {name: "Alice", age: 25, avatar: "/avatars/alice.jpg"},
    onEdit: () => println("Edit clicked"),
    onDelete: () => println("Delete clicked")
})
```

### Configuration Objects

```klang
class ApiClient {
    fn constructor({
        baseUrl,
        timeout = 30000,
        headers = {},
        retries = 3,
        auth: {token, type = "Bearer"} = {}
    }) {
        this.baseUrl = baseUrl
        this.timeout = timeout
        this.headers = headers
        this.retries = retries
        this.token = token
        this.authType = type
    }
    
    async fn request({
        endpoint,
        method = "GET",
        body = null,
        headers: customHeaders = {}
    }) {
        let url = this.baseUrl + endpoint
        let headers = {...this.headers, ...customHeaders}
        
        if this.token {
            headers["Authorization"] = this.authType + " " + this.token
        }
        
        return await fetch(url, {method, headers, body})
    }
}

# Usage
let api = ApiClient({
    baseUrl: "https://api.example.com",
    timeout: 10000,
    auth: {token: "abc123"}
})
```

### Array Transformations

```klang
# Swap pairs in array
fn swapPairs(arr) {
    let result = []
    
    for let i = 0; i < arr.length; i += 2 {
        if i + 1 < arr.length {
            let [a, b] = [arr[i], arr[i + 1]]
            result.push(b, a)
        } else {
            result.push(arr[i])
        }
    }
    
    return result
}

println(swapPairs([1, 2, 3, 4, 5]))  # [2, 1, 4, 3, 5]

# Partition array
fn partition(arr, predicate) {
    let passed = []
    let failed = []
    
    for let item in arr {
        if predicate(item) {
            passed.push(item)
        } else {
            failed.push(item)
        }
    }
    
    return [passed, failed]
}

let [evens, odds] = partition([1, 2, 3, 4, 5], n => n % 2 == 0)
println(evens)  # [2, 4]
println(odds)   # [1, 3, 5]
```

### Loop Destructuring

```klang
let users = [
    {id: 1, name: "Alice", age: 25},
    {id: 2, name: "Bob", age: 30},
    {id: 3, name: "Charlie", age: 35}
]

# Destructure in for loop
for let {name, age} in users {
    println(name + " is " + str(age) + " years old")
}

# With index
for let [i, {name}] in users.entries() {
    println(str(i + 1) + ". " + name)
}

# Object entries
let config = {host: "localhost", port: 3000, ssl: true}
for let [key, value] in Object.entries(config) {
    println(key + " = " + str(value))
}
```

### Error Handling

```klang
async fn fetchWithErrorHandling(url) {
    try {
        let response = await fetch(url)
        let {data, error, status} = response
        
        if error {
            let {message, code} = error
            throw "Error " + str(code) + ": " + message
        }
        
        return data
    } catch {message, stack} {
        println("Caught error: " + message)
        logError(stack)
        return null
    }
}

# Multi-level error handling
fn processResult(result) {
    let {
        success,
        data = null,
        error: {
            type = "unknown",
            message = "An error occurred",
            details = {}
        } = {}
    } = result
    
    if !success {
        println("Error type: " + type)
        println("Message: " + message)
        return null
    }
    
    return data
}
```

### State Management

```klang
class Store {
    fn constructor(initialState = {}) {
        this.state = initialState
        this.listeners = []
    }
    
    fn setState(updates) {
        # Keep old state, merge updates
        this.state = {...this.state, ...updates}
        this.notify()
    }
    
    fn getState() {
        return {...this.state}
    }
    
    fn select(...keys) {
        let result = {}
        for let key in keys {
            result[key] = this.state[key]
        }
        return result
    }
    
    fn notify() {
        for let listener in this.listeners {
            listener(this.state)
        }
    }
}

# Usage
let store = Store({
    user: {name: "Alice", age: 25},
    theme: "dark",
    language: "en"
})

# Subscribe with destructuring
store.listeners.push(({user: {name}, theme}) => {
    println("User: " + name + ", Theme: " + theme)
})

store.setState({theme: "light"})

# Select specific state
let {theme, language} = store.select("theme", "language")
```

## Common Patterns

### Optional Chaining with Destructuring

```klang
fn getUser(id) {
    # May return null
    return database.users.find(u => u.id == id)
}

let user = getUser(123)

# Safe destructuring with defaults
let {
    name = "Unknown",
    email = "No email",
    address: {city = "Unknown"} = {}
} = user || {}

println(name)   # Safe access
println(city)   # Safe access
```

### Merging Objects

```klang
let defaults = {
    timeout: 5000,
    retries: 3,
    method: "GET"
}

let userOptions = {
    timeout: 10000,
    headers: {"Authorization": "Bearer token"}
}

# Merge with destructuring
let options = {...defaults, ...userOptions}
println(options)
# {timeout: 10000, retries: 3, method: "GET", headers: {...}}

# Conditional merging
let finalOptions = {
    ...defaults,
    ...(userOptions.timeout && {timeout: userOptions.timeout}),
    ...(userOptions.headers && {headers: userOptions.headers})
}
```

### Pick and Omit

```klang
fn pick(obj, ...keys) {
    let result = {}
    for let key in keys {
        if obj[key] != undefined {
            result[key] = obj[key]
        }
    }
    return result
}

fn omit(obj, ...keys) {
    let {[keys[0]]: _, ...rest} = obj
    if keys.length > 1 {
        return omit(rest, ...keys.slice(1))
    }
    return rest
}

let user = {id: 1, name: "Alice", age: 25, email: "alice@example.com"}

let nameAndAge = pick(user, "name", "age")
println(nameAndAge)  # {name: "Alice", age: 25}

let withoutEmail = omit(user, "email")
println(withoutEmail)  # {id: 1, name: "Alice", age: 25}
```

### Array to Object

```klang
fn arrayToObject(arr, keyFn) {
    let result = {}
    
    for let item in arr {
        let key = keyFn(item)
        result[key] = item
    }
    
    return result
}

let users = [
    {id: 1, name: "Alice"},
    {id: 2, name: "Bob"},
    {id: 3, name: "Charlie"}
]

let usersById = arrayToObject(users, ({id}) => id)
println(usersById[2])  # {id: 2, name: "Bob"}

# Using destructuring in reducer
let userMap = users.reduce((acc, {id, ...user}) => {
    acc[id] = user
    return acc
}, {})
```

## Best Practices

### 1. Use Destructuring for Clarity

```klang
# Good: Clear and concise
let {name, age, email} = user

# Avoid: Repetitive
let name = user.name
let age = user.age
let email = user.email
```

### 2. Provide Meaningful Defaults

```klang
# Good: Sensible defaults
fn createServer({host = "localhost", port = 3000, ssl = false}) {
    # ...
}

# Avoid: No defaults
fn createServer({host, port, ssl}) {
    # Might break if options are missing
}
```

### 3. Don't Over-Nest

```klang
# Avoid: Too complex
let {a: {b: {c: {d: {e}}}}} = obj

# Better: Step by step
let {a} = obj
let {b} = a
let {c} = b
# ...
```

### 4. Use Rest for Unknown Properties

```klang
# Good: Handle extra properties
fn processUser({id, name, ...metadata}) {
    saveUser(id, name)
    saveMetadata(id, metadata)
}

# All extra properties go to metadata
processUser({
    id: 1,
    name: "Alice",
    age: 25,
    city: "NYC",
    country: "USA"
})
```

### 5. Rename for Clarity

```klang
# Good: Clear naming
let {
    name: userName,
    id: userId,
    created_at: createdDate
} = response.data

# Avoid: Confusing names
let {a: x, b: y, c: z} = obj
```

## Common Mistakes to Avoid

### 1. Destructuring undefined/null

```klang
# Problematic
let {name} = null  # Error!

# Safe
let {name} = user || {}
let {name} = user ?? {}
```

### 2. Missing Default Object

```klang
# Wrong
fn config({host, port}) {
    # ...
}
config()  # Error: Cannot destructure undefined

# Correct
fn config({host, port} = {}) {
    # ...
}
config()  # OK
```

### 3. Forgetting Parentheses

```klang
# Wrong: Syntax error
let {name} = user;
{age} = user

# Correct
let {name, age} = user

# Or if separate statements
let {name} = user
;({age} = user)  # Need semicolon and parentheses
```

## Next Steps

Now that you master destructuring, explore these topics:

- **[Spread/Rest Operators](26-spread-rest.md)** - Learn spread and rest
- **[Arrow Functions](21-arrow-functions.md)** - Combine with destructuring
- **[Objects](15-objects.md)** - Master object manipulation
- **[Arrays](14-arrays.md)** - Array operations
- **[Functions](13-functions.md)** - Advanced function patterns

Ready to write cleaner code? Start using destructuring in your projects!
