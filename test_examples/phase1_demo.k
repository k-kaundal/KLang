# KLang Phase 1 JavaScript Features - Visual Demo

print("╔════════════════════════════════════════════════════╗")
print("║  KLang Phase 1 JavaScript Features Demonstration  ║")
print("╚════════════════════════════════════════════════════╝")
print("")

# ========================================
# 1. VAR/LET/CONST Declarations
# ========================================
print("━━━ 1. Variable Declarations ━━━")
print("")

var mutableVar = 10
print(`var: ${mutableVar}`)
mutableVar = 20
print(`var after update: ${mutableVar}`)

let blockScopedLet = 30
print(`let: ${blockScopedLet}`)
blockScopedLet = 40
print(`let after update: ${blockScopedLet}`)

const IMMUTABLE_CONST = 50
print(`const: ${IMMUTABLE_CONST}`)

print("")

# ========================================
# 2. Arrow Functions
# ========================================
print("━━━ 2. Arrow Functions ━━━")
print("")

# Single parameter without parentheses
const square = x => x * x
print(`square(5) = ${square(5)}`)

# Multiple parameters
const add = (a, b) => a + b
print(`add(3, 7) = ${add(3, 7)}`)

# No parameters
const greet = () => "Hello, KLang!"
print(greet())

# Block body
const complex = x => {
    let doubled = x * 2
    let result = doubled + 1
    return result
}
print(`complex(10) = ${complex(10)}`)

print("")

# ========================================
# 3. Template Literals
# ========================================
print("━━━ 3. Template Literals ━━━")
print("")

let language = "KLang"
let version = 1.0
print(`Welcome to ${language} v${version}!`)

let x = 5
let y = 10
print(`${x} + ${y} = ${x + y}`)
print(`${x} * ${y} = ${x * y}`)

# Multiline
let poem = `KLang is powerful
KLang is modern
KLang has JavaScript features!`
print(poem)

print("")

# ========================================
# 4. Enhanced Object Literals
# ========================================
print("━━━ 4. Enhanced Object Literals ━━━")
print("")

# Property shorthand
let name = "Alice"
let age = 30
let person = {name, age}
print(`Person: ${person.name}, age ${person.age}`)

# Computed property names
let key = "dynamicKey"
let obj = {[key]: "dynamicValue"}
print(`obj.dynamicKey = ${obj.dynamicKey}`)

let propName = "score"
let gameState = {[propName]: 100}
print(`gameState.score = ${gameState.score}`)

print("")

# ========================================
# 5. Combined Features
# ========================================
print("━━━ 5. All Features Combined ━━━")
print("")

# Realistic example: User formatting system
const formatUser = (name, age, score) => `👤 ${name} (${age} years old) - Score: ${score}`

let user1 = "Bob"
let age1 = 25
let score1 = 95

let userData = {
    name: user1,
    age: age1,
    score: score1
}

print(formatUser(userData.name, userData.age, userData.score))

# Mathematical operations
const calculate = (a, b) => {
    let sum = a + b
    let product = a * b
    let message = `${a} + ${b} = ${sum}, ${a} * ${b} = ${product}`
    return message
}

print(calculate(6, 7))

# Temperature converter
const celsiusToFahrenheit = c => (c * 9 / 5) + 32
let celsius = 25
print(`${celsius}°C = ${celsiusToFahrenheit(celsius)}°F`)

print("")

# ========================================
# 6. Scope Demonstration
# ========================================
print("━━━ 6. Scope Demonstration ━━━")
print("")

let outer = "outer"
print(`Before block: ${outer}`)

{
    let inner = "inner"
    print(`Inside block - outer: ${outer}`)
    print(`Inside block - inner: ${inner}`)
}

print(`After block: ${outer}`)
# print(inner)  # This would error: inner not defined

print("")

# ========================================
# 7. Practical Examples
# ========================================
print("━━━ 7. Practical Examples ━━━")
print("")

# Shopping cart
const calculateTotal = (price, quantity, taxRate) => {
    let subtotal = price * quantity
    let tax = subtotal * taxRate
    let total = subtotal + tax
    return total
}

let itemPrice = 10
let itemQty = 3
let tax = 0.08

print(`Shopping Cart:`)
print(`  Item Price: $${itemPrice}`)
print(`  Quantity: ${itemQty}`)
print(`  Tax Rate: ${tax * 100}%`)
print(`  Total: $${calculateTotal(itemPrice, itemQty, tax)}`)

print("")

# String manipulation
const makeGreeting = (name, time) => {
    let greeting = time
    return `${greeting}, ${name}!`
}

let userName = "Developer"
print(makeGreeting(userName, "Good morning"))
print(makeGreeting(userName, "Good evening"))

print("")

# ========================================
# Summary
# ========================================
print("╔════════════════════════════════════════════════════╗")
print("║              ✨ All Features Working! ✨            ║")
print("╚════════════════════════════════════════════════════╝")
print("")
print("✅ var/let/const declarations")
print("✅ Arrow functions (all forms)")
print("✅ Template literals with interpolation")
print("✅ Enhanced object literals")
print("✅ Seamless feature integration")
print("")
print("KLang is now more modern and JavaScript-like!")
