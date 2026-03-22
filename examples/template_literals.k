// Template Literals in KLang
// Demonstrates JavaScript-style template literal syntax

// Basic template literal
const message = `Hello, World!`
print(message)

// String interpolation with variables
const name = "Alice"
const greeting = `Hello, ${name}!`
print(greeting)

// Multi-line string without escape characters
const poem = `Roses are red
Violets are blue
KLang is awesome
And so are you!`
print(poem)

// Multiple interpolations in one template
const x = 10
const y = 20
const result = `${x} + ${y} = ${x + y}`
print(result)

// Nested expressions in interpolations
fn double(n) {
    return n * 2
}

const value = 5
const doubleMsg = `Double of ${value} is ${double(value)}`
print(doubleMsg)

// Complex expressions with operators
const price = 100
const tax = 0.08
const total = `Total: $${price * (1 + tax)}`
print(total)

// Template literals with lists
const items = ["apple", "banana", "cherry"]
const itemCount = `You have ${items.length} items`
print(itemCount)

// Template literals in function returns
fn greet(name, time) {
    return `Good ${time}, ${name}!`
}

print(greet("Bob", "morning"))
print(greet("Charlie", "evening"))

// Comparison results in template literals
const a = 15
const b = 10
print(`Is ${a} > ${b}? ${a > b}`)

// Empty template literal
const empty = ``
print(`Empty string length: ${empty.length}`)

// Template literal with only interpolation
const solo = `${42}`
print(`Solo interpolation: ${solo}`)

// Combining template literals
const first = `Hello`
const second = ` World`
const combined = first + second
print(combined)

// Template literals with boolean values
const isReady = true
const status = `System ready: ${isReady}`
print(status)

// Nested function calls in template literals
fn square(n) { return n * n }
fn cube(n) { return n * n * n }

const num = 3
const powers = `${num}^2 = ${square(num)}, ${num}^3 = ${cube(num)}`
print(powers)

// String concatenation inside interpolation
const firstName = "John"
const lastName = "Doe"
const fullName = `Full name: ${firstName + " " + lastName}`
print(fullName)

// Template literal with float numbers
const pi = 3.14159
const area = pi * 10 * 10
const circleInfo = `Circle with radius 10 has area ${area}`
print(circleInfo)

// Adjacent interpolations (no space between)
const part1 = "Hello"
const part2 = "World"
const noSpace = `${part1}${part2}`
print(noSpace)

print("All template literal examples completed!")
