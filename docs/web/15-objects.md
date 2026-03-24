# Objects

## Complete Guide to Objects in KLang

Objects are collections of key-value pairs that represent structured data. KLang provides intuitive object literal syntax and flexible property access, perfect for representing complex data structures and building modern applications.

## Creating Objects

### Object Literals

The most common way to create objects:

```klang
# Empty object
let empty = {}

# Simple object
let person = {
    name: "Alice",
    age: 25,
    city: "New York"
}

# Multi-line formatting
let user = {
    id: 123,
    username: "alice_wonder",
    email: "alice@example.com",
    active: true,
    role: "admin"
}

# Nested objects
let config = {
    server: {
        host: "localhost",
        port: 8080
    },
    database: {
        name: "mydb",
        user: "admin",
        password: "secret"
    }
}
```

### Objects with Various Value Types

```klang
# Mixed value types
let data = {
    id: 1,                          # Number
    name: "Product",                # String
    inStock: true,                  # Boolean
    tags: ["new", "featured"],      # Array
    metadata: {                     # Nested object
        created: "2024-01-01",
        updated: "2024-01-15"
    },
    calculate: x => x * 2           # Function
}
```

## Accessing Properties

### Dot Notation

```klang
let person = {
    name: "Alice",
    age: 25,
    city: "New York"
}

# Access properties
println(person.name)  # Alice
println(person.age)   # 25
println(person.city)  # New York

# Nested property access
let config = {
    server: {
        host: "localhost",
        port: 8080
    }
}

println(config.server.host)  # localhost
println(config.server.port)  # 8080
```

### Bracket Notation

```klang
let person = {
    name: "Alice",
    age: 25
}

# Access with string key
println(person["name"])  # Alice
println(person["age"])   # 25

# Dynamic property access
let key = "name"
println(person[key])  # Alice

# With variable
let prop = "age"
let value = person[prop]
println(value)  # 25
```

## Modifying Properties

### Adding Properties

```klang
let person = {
    name: "Alice"
}

# Add new properties
person.age = 25
person.city = "New York"

println(person.name)  # Alice
println(person.age)   # 25
println(person.city)  # New York
```

### Updating Properties

```klang
let user = {
    name: "Alice",
    age: 25,
    score: 100
}

# Update existing properties
user.age = 26
user.score = user.score + 50

println(user.age)    # 26
println(user.score)  # 150

# Update nested properties
let config = {
    server: {
        port: 8080
    }
}

config.server.port = 9000
println(config.server.port)  # 9000
```

### Deleting Properties

```klang
# Note: Check if KLang supports delete operator
let obj = {
    name: "Alice",
    age: 25,
    temp: "remove me"
}

# If delete is supported:
# delete obj.temp

# Alternative: Set to null
obj.temp = null
```

## Object Methods

Objects can contain functions as properties:

```klang
# Object with methods
let calculator = {
    add: (a, b) => a + b,
    subtract: (a, b) => a - b,
    multiply: (a, b) => a * b,
    divide: (a, b) => b != 0 ? a / b : null
}

# Call methods
println(calculator.add(5, 3))       # 8
println(calculator.subtract(10, 4)) # 6
println(calculator.multiply(3, 7))  # 21
println(calculator.divide(20, 4))   # 5

# Method accessing object properties (if 'this' is supported)
let counter = {
    count: 0,
    increment: () => {
        # If this is supported:
        # this.count = this.count + 1
        # return this.count
    }
}
```

## Common Patterns

### Pattern 1: Data Records

```klang
# User record
let user = {
    id: 101,
    username: "alice",
    email: "alice@example.com",
    createdAt: "2024-01-15",
    isActive: true
}

# Product record
let product = {
    id: 1001,
    name: "Laptop",
    price: 999.99,
    inStock: true,
    category: "Electronics",
    specs: {
        cpu: "Intel i7",
        ram: "16GB",
        storage: "512GB SSD"
    }
}

println(product.name)             # Laptop
println(product.specs.cpu)        # Intel i7
```

### Pattern 2: Configuration Objects

```klang
# Application configuration
const APP_CONFIG = {
    name: "MyApp",
    version: "1.0.0",
    api: {
        baseUrl: "https://api.example.com",
        timeout: 30000,
        retries: 3
    },
    features: {
        darkMode: true,
        notifications: true,
        analytics: false
    }
}

println(APP_CONFIG.api.baseUrl)         # https://api.example.com
println(APP_CONFIG.features.darkMode)   # true
```

### Pattern 3: Factory Functions

```klang
# Create objects with factory function
fn createUser(name, email) {
    return {
        name: name,
        email: email,
        createdAt: "2024-01-15",
        isActive: true
    }
}

let user1 = createUser("Alice", "alice@example.com")
let user2 = createUser("Bob", "bob@example.com")

println(user1.name)  # Alice
println(user2.name)  # Bob

# With default values
fn createProduct(name, price) {
    return {
        name: name,
        price: price != null ? price : 0,
        inStock: true,
        tags: []
    }
}

let product1 = createProduct("Widget", 19.99)
let product2 = createProduct("Gadget", null)

println(product1.price)  # 19.99
println(product2.price)  # 0
```

### Pattern 4: Object as Dictionary/Map

```klang
# Use object as key-value store
let scores = {
    alice: 95,
    bob: 87,
    charlie: 92
}

println(scores.alice)    # 95
println(scores["bob"])   # 87

# Add new entries
scores.david = 89
println(scores.david)    # 89

# Status codes
let statusMessages = {
    200: "OK",
    404: "Not Found",
    500: "Server Error"
}

let code = 404
println(statusMessages[str(code)])  # Not Found
```

### Pattern 5: Object Methods Pattern

```klang
# Object with data and operations
let bankAccount = {
    balance: 1000,
    deposit: amount => {
        # Note: Without 'this', need different approach
        # This is conceptual
        return "Deposited: " + str(amount)
    },
    withdraw: amount => {
        return "Withdrew: " + str(amount)
    },
    getBalance: () => {
        return 1000  # Would access this.balance
    }
}

println(bankAccount.deposit(500))
println(bankAccount.getBalance())
```

## Arrays of Objects

Common pattern for representing collections:

```klang
# Array of user objects
let users = [
    {id: 1, name: "Alice", age: 25},
    {id: 2, name: "Bob", age: 30},
    {id: 3, name: "Charlie", age: 35}
]

# Access individual users
println(users[0].name)  # Alice
println(users[1].age)   # 30

# Iterate over objects
for user of users {
    println("Name: " + user.name + ", Age: " + str(user.age))
}

# Transform with map
let names = users.map(user => user.name)
println(names)  # ["Alice", "Bob", "Charlie"]

# Filter by condition
let over30 = users.filter(user => user.age > 30)
println("Users over 30: " + str(len(over30)))  # 1

# Find specific user
let alice = users.find(user => user.name == "Alice")
println("Found: " + alice.name)  # Alice
```

## Object Operations

### Checking Properties

```klang
# Check if property exists (conceptual)
let person = {
    name: "Alice",
    age: 25
}

# Check by accessing
let hasName = person.name != null
println("Has name: " + str(hasName))  # true

# Safe access pattern
fn getProperty(obj, key, defaultValue) {
    # Simplified property access with default
    # Real implementation would use object property checking
    return defaultValue
}
```

### Copying Objects

```klang
# Shallow copy pattern
fn copyObject(obj) {
    # Simplified - would need actual object copying
    return {
        name: obj.name,
        age: obj.age
    }
}

let original = {name: "Alice", age: 25}
let copy = copyObject(original)

# Modify copy
copy.age = 26

println(original.age)  # 25 (unchanged)
println(copy.age)      # 26
```

### Merging Objects

```klang
# Merge two objects
fn mergeObjects(obj1, obj2) {
    # Conceptual merge - would copy all properties
    return {
        # Properties from obj1
        # Then properties from obj2 (override)
    }
}

let defaults = {
    theme: "light",
    language: "en",
    notifications: true
}

let userPrefs = {
    theme: "dark",
    notifications: false
}

# Merged would have: theme=dark, language=en, notifications=false
```

## Complex Object Structures

### Nested Objects

```klang
# Deep nesting
let company = {
    name: "TechCorp",
    departments: {
        engineering: {
            manager: "Alice",
            employees: ["Bob", "Charlie", "David"],
            budget: 500000
        },
        sales: {
            manager: "Eve",
            employees: ["Frank", "Grace"],
            budget: 300000
        }
    },
    location: {
        address: "123 Tech St",
        city: "San Francisco",
        country: "USA"
    }
}

# Access deeply nested properties
println(company.name)                              # TechCorp
println(company.departments.engineering.manager)   # Alice
println(company.location.city)                     # San Francisco

# Access nested array
let engEmployees = company.departments.engineering.employees
println(engEmployees[0])  # Bob
```

### Objects in Arrays in Objects

```klang
# Complex structure
let classroom = {
    name: "Math 101",
    teacher: "Dr. Smith",
    students: [
        {name: "Alice", grade: 95},
        {name: "Bob", grade: 87},
        {name: "Charlie", grade: 92}
    ]
}

# Access
println(classroom.name)                # Math 101
println(classroom.students[0].name)    # Alice
println(classroom.students[1].grade)   # 87

# Calculate average
let total = classroom.students.reduce((sum, s) => sum + s.grade, 0)
let average = total / len(classroom.students)
println("Average grade: " + str(average))  # 91.33
```

## Practical Examples

### Example 1: API Response Handling

```klang
# Simulated API response
let apiResponse = {
    status: 200,
    data: {
        user: {
            id: 123,
            name: "Alice",
            email: "alice@example.com"
        },
        posts: [
            {id: 1, title: "First Post", likes: 10},
            {id: 2, title: "Second Post", likes: 25}
        ]
    },
    metadata: {
        timestamp: "2024-01-15",
        duration: 245
    }
}

# Process response
if apiResponse.status == 200 {
    println("Success!")
    println("User: " + apiResponse.data.user.name)
    println("Posts: " + str(len(apiResponse.data.posts)))
    
    # Show post titles
    for post of apiResponse.data.posts {
        println("  - " + post.title)
    }
}
```

### Example 2: Shopping Cart

```klang
# Shopping cart with items
let cart = {
    items: [
        {id: 1, name: "Laptop", price: 999.99, quantity: 1},
        {id: 2, name: "Mouse", price: 25.00, quantity: 2},
        {id: 3, name: "Keyboard", price: 75.00, quantity: 1}
    ],
    discount: 0.1,
    taxRate: 0.08
}

# Calculate subtotal
let subtotal = cart.items.reduce((sum, item) => {
    return sum + (item.price * item.quantity)
}, 0)

# Apply discount
let afterDiscount = subtotal * (1 - cart.discount)

# Add tax
let total = afterDiscount * (1 + cart.taxRate)

println("Subtotal: $" + str(subtotal))
println("After discount: $" + str(afterDiscount))
println("Total with tax: $" + str(total))
```

### Example 3: Configuration with Validation

```klang
# Application settings
let settings = {
    database: {
        host: "localhost",
        port: 5432,
        name: "myapp_db"
    },
    cache: {
        enabled: true,
        ttl: 3600
    },
    logging: {
        level: "info",
        file: "/var/log/app.log"
    }
}

# Validate configuration
fn validateConfig(config) {
    let errors = []
    
    # Check database port
    if config.database.port < 1 || config.database.port > 65535 {
        errors.push("Invalid database port")
    }
    
    # Check cache TTL
    if config.cache.enabled && config.cache.ttl <= 0 {
        errors.push("Invalid cache TTL")
    }
    
    # Check logging level
    let validLevels = ["debug", "info", "warn", "error"]
    if !validLevels.includes(config.logging.level) {
        errors.push("Invalid logging level")
    }
    
    return errors
}

let errors = validateConfig(settings)
if len(errors) == 0 {
    println("Configuration is valid")
} else {
    println("Configuration errors:")
    for error of errors {
        println("  - " + error)
    }
}
```

### Example 4: State Management

```klang
# Application state
let appState = {
    user: {
        id: null,
        name: null,
        isAuthenticated: false
    },
    ui: {
        theme: "light",
        sidebarOpen: true,
        notifications: []
    },
    data: {
        items: [],
        loading: false,
        error: null
    }
}

# State update functions
fn login(state, userId, userName) {
    state.user.id = userId
    state.user.name = userName
    state.user.isAuthenticated = true
    println("User logged in: " + userName)
}

fn logout(state) {
    state.user.id = null
    state.user.name = null
    state.user.isAuthenticated = false
    println("User logged out")
}

fn toggleTheme(state) {
    state.ui.theme = state.ui.theme == "light" ? "dark" : "light"
    println("Theme changed to: " + state.ui.theme)
}

# Use state
login(appState, 123, "Alice")
println("Authenticated: " + str(appState.user.isAuthenticated))

toggleTheme(appState)
println("Current theme: " + appState.ui.theme)

logout(appState)
println("Authenticated: " + str(appState.user.isAuthenticated))
```

## Best Practices

### 1. Use Meaningful Property Names

```klang
# ✅ Good: Descriptive names
let user = {
    firstName: "Alice",
    lastName: "Smith",
    emailAddress: "alice@example.com"
}

# ❌ Avoid: Cryptic names
let user = {
    fn: "Alice",
    ln: "Smith",
    em: "alice@example.com"
}
```

### 2. Group Related Data

```klang
# ✅ Good: Organized structure
let user = {
    personal: {
        name: "Alice",
        age: 25
    },
    contact: {
        email: "alice@example.com",
        phone: "555-0123"
    }
}

# ❌ Avoid: Flat structure for complex data
let user = {
    name: "Alice",
    age: 25,
    email: "alice@example.com",
    phone: "555-0123",
    address: "123 Main St",
    city: "NYC",
    state: "NY",
    zip: "10001"
}
```

### 3. Use Factory Functions

```klang
# ✅ Good: Consistent object creation
fn createUser(name, email) {
    return {
        name: name,
        email: email,
        createdAt: "2024-01-15",
        isActive: true
    }
}

let user1 = createUser("Alice", "alice@example.com")
let user2 = createUser("Bob", "bob@example.com")

# ❌ Avoid: Manual object creation everywhere
let user1 = {
    name: "Alice",
    email: "alice@example.com",
    createdAt: "2024-01-15",
    isActive: true
}
```

### 4. Validate Object Structure

```klang
# ✅ Good: Validate before use
fn isValidUser(user) {
    return user != null &&
           type(user.name) == "string" &&
           type(user.age) == "int" &&
           user.age >= 0
}

if isValidUser(userData) {
    # Use data
} else {
    println("Invalid user data")
}
```

### 5. Use Constants for Config

```klang
# ✅ Good: Immutable configuration
const DATABASE_CONFIG = {
    host: "localhost",
    port: 5432,
    name: "mydb"
}

# Don't modify
# DATABASE_CONFIG.host = "other"  # Should be const
```

## Common Mistakes

### Mistake 1: Typos in Property Names

```klang
let user = {
    name: "Alice",
    email: "alice@example.com"
}

# ❌ Wrong: Typo in property name
# println(user.emial)  # Undefined, should be 'email'

# ✅ Correct: Exact property name
println(user.email)
```

### Mistake 2: Forgetting Null Checks

```klang
let user = null

# ❌ Wrong: Access property without checking
# println(user.name)  # Error: user is null

# ✅ Correct: Check before access
if user != null {
    println(user.name)
}
```

### Mistake 3: Deep Mutation Issues

```klang
# ⚠️ Caution: Modifying nested objects
let original = {
    settings: {
        theme: "light"
    }
}

let copy = original
copy.settings.theme = "dark"

# Both original and copy are affected!
println(original.settings.theme)  # dark
```

## Summary

### Object Basics
- **Creation**: `{key: value}` literal syntax
- **Access**: Dot notation (`obj.prop`) or bracket (`obj["prop"]`)
- **Modification**: Assign to properties
- **Methods**: Functions as property values

### Common Patterns
- Data records
- Configuration objects
- Factory functions
- Dictionary/map usage
- Arrays of objects

### Best Practices
1. Use descriptive property names
2. Group related data
3. Factory functions for consistency
4. Validate structure
5. Use constants for config

## Next Steps

Master objects, then explore:

- **[Classes & OOP](16-classes.md)** - Object-oriented programming
- **[Arrays](14-arrays.md)** - Arrays of objects
- **[Functions](13-functions.md)** - Object methods and factories
- **[Advanced Patterns](17-advanced-objects.md)** - Deep object manipulation

---

**Structure your data with powerful objects!** 🚀
