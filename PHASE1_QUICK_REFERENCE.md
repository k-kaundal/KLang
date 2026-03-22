# Phase 1 JavaScript Features - Quick Reference

Quick reference for KLang's new JavaScript-inspired features.

---

## 1. var/let/const Declarations

### var - Function Scoped, Allows Redeclaration
```javascript
var x = 10          // Declare and initialize
var x = 20          // ✅ Redeclaration allowed
x = 30              // ✅ Reassignment allowed
var y               // ✅ Can declare without init (becomes null)
```

### let - Block Scoped, No Redeclaration
```javascript
let x = 10          // Declare and initialize
let x = 20          // ❌ Error: cannot redeclare
x = 30              // ✅ Reassignment allowed
let y               // ✅ Can declare without init (becomes null)

{
    let x = 100     // ✅ Different scope, OK
}
```

### const - Block Scoped, Immutable
```javascript
const PI = 3.14     // ✅ Must initialize
PI = 3.15           // ❌ Error: cannot reassign to const
const X             // ❌ Error: const must be initialized
```

### Comparison Table

| Feature | var | let | const |
|---------|-----|-----|-------|
| Scope | Function | Block | Block |
| Redeclaration | ✅ Yes | ❌ No | ❌ No |
| Reassignment | ✅ Yes | ✅ Yes | ❌ No |
| No-init allowed | ✅ Yes | ✅ Yes | ❌ No |

---

## 2. Arrow Functions

### Basic Syntax
```javascript
// Single parameter (no parentheses needed)
const double = x => x * 2

// Multiple parameters (parentheses required)
const add = (a, b) => a + b

// No parameters (empty parentheses required)
const greet = () => "Hello!"

// Expression body (implicit return)
const square = x => x * x

// Block body (explicit return)
const complex = x => {
    let result = x * 2
    return result + 1
}
```

### Quick Patterns
```javascript
// Mathematical operations
const multiply = (a, b) => a * b
const divide = (a, b) => a / b
const mod = (a, b) => a % b

// String operations
const uppercase = s => s.toUpper()
const concat = (a, b) => a + b

// Comparisons
const max = (a, b) => a > b ? a : b
const min = (a, b) => a < b ? a : b

// Array operations (if supported)
const getFirst = arr => arr[0]
const getLength = arr => arr.length
```

---

## 3. Template Literals

### Basic Usage
```javascript
// Simple text
let msg = `Hello, World!`

// Variable interpolation
let name = "Alice"
let greeting = `Hello, ${name}!`

// Expression evaluation
let x = 10, y = 20
let result = `${x} + ${y} = ${x + y}`

// Multiple interpolations
let user = "Bob"
let age = 25
let info = `${user} is ${age} years old`
```

### Multiline Strings
```javascript
let poem = `Roses are red
Violets are blue
KLang is awesome
And so are you!`
```

### Common Patterns
```javascript
// Formatting output
const formatPrice = price => `$${price.toFixed(2)}`
const formatDate = (y, m, d) => `${y}-${m}-${d}`

// Building messages
const errorMsg = code => `Error ${code}: Operation failed`
const successMsg = user => `Welcome back, ${user}!`

// Debug output
const debug = (name, value) => `[DEBUG] ${name} = ${value}`

// Templates in functions
const greet = (name, time) => `${time}, ${name}!`
greet("Alice", "Good morning")  // "Good morning, Alice!"
```

---

## 4. Enhanced Object Literals

### Property Shorthand
```javascript
let x = 10
let y = 20

// Traditional syntax
let point1 = {x: x, y: y}

// Shorthand syntax ✅
let point2 = {x, y}  // Same as above
```

### Computed Property Names
```javascript
// Dynamic property names
let key = "username"
let user = {[key]: "alice"}
user.username  // "alice"

// Expression as key
let prefix = "user"
let obj = {[prefix + "Name"]: "Bob"}
obj.userName  // "Bob"

// Variable keys
let prop = "score"
let game = {[prop]: 100}
game.score  // 100
```

### Method Shorthand (⚠️ Known Limitation)
```javascript
// This has issues currently:
let obj1 = {
    greet() {          // ⚠️ Not working yet
        return "Hi"
    }
}

// Use this instead: ✅
let obj2 = {
    greet: fn() {      // ✅ Works correctly
        return "Hi"
    }
}
```

### Combined Patterns
```javascript
let name = "Alice"
let age = 30
let score = 95

let player = {
    name,              // Property shorthand
    age,               // Property shorthand
    score,             // Property shorthand
    ["level"]: 5,      // Computed property
    greet: fn() {      // Method (use fn syntax)
        return "Hello!"
    }
}
```

---

## 5. Combining Features

### Real-World Examples

#### User Profile Formatter
```javascript
const formatProfile = (name, age, email) => {
    let profile = {name, age, email}
    return `📧 ${profile.email} - ${profile.name} (${profile.age})`
}

let user = "Alice"
let years = 30
let mail = "alice@example.com"

print(formatProfile(user, years, mail))
// Output: 📧 alice@example.com - Alice (30)
```

#### Temperature Converter
```javascript
const c2f = c => (c * 9 / 5) + 32
const f2c = f => (f - 32) * 5 / 9

let celsius = 25
let fahrenheit = c2f(celsius)

print(`${celsius}°C = ${fahrenheit}°F`)
// Output: 25°C = 77°F
```

#### Shopping Cart Calculator
```javascript
const calcTotal = (price, qty, tax) => {
    let subtotal = price * qty
    let taxAmount = subtotal * tax
    let total = subtotal + taxAmount
    
    return {
        subtotal,      // Shorthand
        taxAmount,     // Shorthand
        total          // Shorthand
    }
}

let result = calcTotal(10, 3, 0.08)
print(`Subtotal: $${result.subtotal}`)
print(`Tax: $${result.taxAmount}`)
print(`Total: $${result.total}`)
```

#### Data Processing Pipeline
```javascript
const process = data => {
    let cleaned = data.trim()
    let upper = cleaned.toUpper()
    return `Processed: ${upper}`
}

const formatList = items => {
    let count = items.length
    return `Found ${count} items`
}

// Chain operations
const pipeline = input => {
    let processed = process(input)
    return `✓ ${processed}`
}
```

---

## 6. Migration Guide

### From Traditional to Modern Syntax

#### Variables
```javascript
// Old style
let x = 10

// New options
var x = 10    // Function scoped
let x = 10    // Block scoped
const x = 10  // Immutable
```

#### Functions
```javascript
// Old style
fn double(x) {
    return x * 2
}

// New arrow function
const double = x => x * 2

// Both work! Use what fits your style
```

#### Strings
```javascript
// Old style
let name = "Alice"
let msg = "Hello, " + name + "!"

// New template literal
let msg = `Hello, ${name}!`
```

#### Objects
```javascript
// Old style
let x = 10
let y = 20
let point = {x: x, y: y}

// New shorthand
let point = {x, y}
```

---

## 7. Best Practices

### When to Use var/let/const

✅ **Use const by default** - Immutability helps prevent bugs
```javascript
const MAX_SIZE = 100
const API_URL = "https://api.example.com"
const config = {timeout: 5000}
```

✅ **Use let when you need to reassign**
```javascript
let counter = 0
counter++

let status = "pending"
status = "complete"
```

⚠️ **Use var sparingly** - Only when you need function scope
```javascript
var globalConfig = {}  // Function-scoped
```

### Arrow Function Tips

✅ **Use arrows for simple operations**
```javascript
const double = x => x * 2
const isPositive = n => n > 0
```

✅ **Use block body for complex logic**
```javascript
const validate = input => {
    let trimmed = input.trim()
    let valid = trimmed.length > 0
    return valid
}
```

❌ **Don't use arrows unnecessarily**
```javascript
// Simple enough for arrow
const add = (a, b) => a + b

// Too complex, use regular function
fn complexProcess(data) {
    // Many lines of logic
    // Multiple operations
    // Complex flow control
}
```

### Template Literal Tips

✅ **Use for string interpolation**
```javascript
let name = "Alice"
let msg = `Hello, ${name}!`  // Better than: "Hello, " + name + "!"
```

✅ **Use for multiline strings**
```javascript
let html = `
    <div>
        <h1>Title</h1>
        <p>Content</p>
    </div>
`
```

✅ **Use for formatted output**
```javascript
const log = (level, msg) => `[${level}] ${msg}`
```

---

## 8. Common Patterns

### Configuration Objects
```javascript
let host = "localhost"
let port = 8080
let ssl = true

let config = {
    host,
    port,
    ssl,
    url: `${ssl ? 'https' : 'http'}://${host}:${port}`
}
```

### Factory Functions
```javascript
const createUser = (name, age) => {
    return {
        name,
        age,
        greet: fn() {
            return `Hi, I'm ${name}`
        }
    }
}

let user = createUser("Alice", 30)
```

### Formatters
```javascript
const formatters = {
    currency: amount => `$${amount.toFixed(2)}`,
    percentage: value => `${value * 100}%`,
    date: (y, m, d) => `${y}-${m}-${d}`
}

print(formatters.currency(42.5))  // "$42.50"
```

---

## 9. Quick Debugging

### Debug Templates
```javascript
// Variable inspection
const inspect = (name, value) => `${name} = ${value}`

let x = 42
print(inspect("x", x))  // "x = 42"

// Object inspection
let obj = {x: 10, y: 20}
print(`obj.x = ${obj.x}, obj.y = ${obj.y}`)
```

---

## 10. Cheat Sheet

```
VAR/LET/CONST
├─ var x = 10     → function scope, redeclarable
├─ let x = 10     → block scope, no redeclaration
└─ const X = 10   → block scope, immutable, must init

ARROW FUNCTIONS
├─ x => x * 2                → single param, implicit return
├─ (a, b) => a + b          → multiple params
├─ () => "value"            → no params
└─ x => { return x * 2 }    → block body, explicit return

TEMPLATE LITERALS
├─ `text`                   → basic string
├─ `Hello ${name}`          → interpolation
├─ `${x} + ${y} = ${x+y}`  → expressions
└─ `line1\nline2`          → multiline

ENHANCED OBJECTS
├─ {x, y}                   → property shorthand
├─ {[key]: value}           → computed properties
└─ {fn: fn() {}}           → methods (use fn syntax)
```

---

**Reference Version:** Phase 1  
**Last Updated:** March 2024  
**For more details, see:** PHASE1_TEST_REPORT.md
