# KLang Complete Feature Guide: 0 to 100%

**A step-by-step guide to all KLang features from beginner to advanced**

Version: 1.0 | Last Updated: March 2026 | Completeness: 40% of JavaScript features

---

## Table of Contents

- [Level 0: Getting Started (Prerequisites)](#level-0-getting-started)
- [Level 1: Basics (0-20%)](#level-1-basics-0-20)
- [Level 2: Variables & Types (20-30%)](#level-2-variables--types-20-30)
- [Level 3: Functions (30-40%)](#level-3-functions-30-40)
- [Level 4: Control Flow (40-50%)](#level-4-control-flow-40-50)
- [Level 5: Data Structures (50-60%)](#level-5-data-structures-50-60)
- [Level 6: Object-Oriented Programming (60-75%)](#level-6-object-oriented-programming-60-75)
- [Level 7: Advanced Functions (75-85%)](#level-7-advanced-functions-75-85)
- [Level 8: Asynchronous Programming (85-90%)](#level-8-asynchronous-programming-85-90)
- [Level 9: Modules & Organization (90-95%)](#level-9-modules--organization-90-95)
- [Level 10: Advanced Features (95-100%)](#level-10-advanced-features-95-100)

---

## Level 0: Getting Started (Prerequisites)

### Installation

```bash
# Clone the repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Build KLang
make

# Test installation
./klang repl
```

### Your First Program

Create `hello.k`:
```javascript
print("Hello, KLang!")
```

Run it:
```bash
./klang run hello.k
```

**✅ Checkpoint**: You can write and run basic programs

---

## Level 1: Basics (0-20%)

### 1.1 Comments

```javascript
// Single-line comment

/* Multi-line
   comment */

/// Documentation comment
```

**Status**: ✅ Fully Implemented

### 1.2 Output

```javascript
print("Hello")        // No newline
println("World!")     // With newline
```

**Status**: ✅ Fully Implemented

### 1.3 Basic Arithmetic

```javascript
let a = 10
let b = 20

println(a + b)   // 30
println(a - b)   // -10
println(a * b)   // 200
println(a / b)   // 0.5
println(a % b)   // 10
```

**Status**: ✅ Fully Implemented

### 1.4 Variables (Legacy)

```javascript
let x = 10
let name = "Alice"
let active = true
```

**Status**: ✅ Fully Implemented

**Practice Exercise**:
```javascript
// Calculate the area of a rectangle
let width = 5
let height = 10
let area = width * height
println(`Area: ${area}`)
```

**✅ Checkpoint**: You can perform calculations and display results

---

## Level 2: Variables & Types (20-30%)

### 2.1 Modern Variable Declarations ✨ NEW

```javascript
// const - immutable
const PI = 3.14159
// PI = 3.14  // ❌ Error: cannot assign to const

// let - mutable, block-scoped
let count = 0
count = count + 1  // ✅ OK

// var - mutable, block-scoped (legacy support)
var temp = 42
var temp = 43  // ✅ OK (allows redeclaration)
```

**Status**: ✅ Fully Implemented (Phase 1)

### 2.2 Type System

```javascript
// Primitive Types
let num = 42              // int
let decimal = 3.14        // float
let text = "hello"        // string
let flag = true           // bool
let nothing = null        // null

// Check types
println(type(num))        // "int"
println(type(text))       // "string"
```

**Status**: ✅ Fully Implemented

### 2.3 Type Conversion

```javascript
let s = "123"
let n = int(s)            // String to int
let f = float(s)          // String to float
let str = str(42)         // Int to string
```

**Status**: ✅ Fully Implemented

### 2.4 String Basics

```javascript
// String creation
let single = 'hello'
let double = "world"
let template = `Hello ${name}`  // ✨ NEW

// String operations
let upper = text.uppercase()
let lower = text.lowercase()
let len = len(text)
```

**Status**: ✅ Single/double quotes, ✅ Template literals (Phase 1)

**Practice Exercise**:
```javascript
const firstName = "John"
const lastName = "Doe"
const age = 30
const greeting = `Hello, my name is ${firstName} ${lastName} and I am ${age} years old.`
println(greeting)
```

**✅ Checkpoint**: You understand all variable types and can use modern declarations

---

## Level 3: Functions (30-40%)

### 3.1 Basic Functions

```javascript
fn add(a, b) {
    return a + b
}

let result = add(5, 3)
println(result)  // 8
```

**Status**: ✅ Fully Implemented

### 3.2 Arrow Functions ✨ NEW

```javascript
// Expression body (implicit return)
const square = x => x * x

// Multiple parameters
const add = (a, b) => a + b

// No parameters
const greet = () => "Hello!"

// Block body (explicit return)
const complex = x => {
    let temp = x * 2
    return temp + 1
}

println(square(5))    // 25
println(add(3, 7))    // 10
```

**Status**: ✅ Fully Implemented (Phase 1)

### 3.3 Function Parameters

```javascript
// Optional parameters with defaults
fn greet(name, greeting = "Hello") {
    return `${greeting}, ${name}!`
}

println(greet("Alice"))              // "Hello, Alice!"
println(greet("Bob", "Hi"))          // "Hi, Bob!"
```

**Status**: ✅ Default parameters supported

### 3.4 Higher-Order Functions

```javascript
// Functions as arguments
fn apply(x, func) {
    return func(x)
}

let doubled = apply(5, x => x * 2)
println(doubled)  // 10

// Functions as return values
fn multiplier(factor) {
    return x => x * factor
}

const double = multiplier(2)
const triple = multiplier(3)
println(double(5))  // 10
println(triple(5))  // 15
```

**Status**: ✅ Fully Implemented

**Practice Exercise**:
```javascript
// Create a calculator using arrow functions
const calc = {
    add: (a, b) => a + b,
    subtract: (a, b) => a - b,
    multiply: (a, b) => a * b,
    divide: (a, b) => b != 0 ? a / b : null
}

println(calc.add(10, 5))       // 15
println(calc.multiply(4, 3))   // 12
```

**✅ Checkpoint**: You can write functions using both traditional and arrow syntax

---

## Level 4: Control Flow (40-50%)

### 4.1 Conditionals

```javascript
// if-else
if (x > 0) {
    println("Positive")
} else if (x < 0) {
    println("Negative")
} else {
    println("Zero")
}
```

**Status**: ✅ Fully Implemented

### 4.2 Comparison Operators

```javascript
let a = 10
let b = 20

println(a == b)   // false (equal)
println(a != b)   // true (not equal)
println(a < b)    // true (less than)
println(a > b)    // false (greater than)
println(a <= b)   // true (less or equal)
println(a >= b)   // false (greater or equal)
```

**Status**: ✅ Fully Implemented

### 4.3 Logical Operators

```javascript
let x = true
let y = false

println(x && y)   // false (AND)
println(x || y)   // true (OR)
println(!x)       // false (NOT)
```

**Status**: ✅ Fully Implemented

### 4.4 While Loops

```javascript
let i = 0
while (i < 5) {
    println(i)
    i = i + 1
}
```

**Status**: ✅ Fully Implemented

### 4.5 For Loops (Range)

```javascript
// For loop with range
for i in 0 .. 5 {
    println(i)  // 0, 1, 2, 3, 4
}
```

**Status**: ✅ Fully Implemented

### 4.6 Break and Continue

```javascript
for i in 0 .. 10 {
    if (i == 3) continue  // Skip 3
    if (i == 7) break     // Stop at 7
    println(i)
}
```

**Status**: ✅ Fully Implemented

**Practice Exercise**:
```javascript
// FizzBuzz
for i in 1 .. 101 {
    if (i % 15 == 0) {
        println("FizzBuzz")
    } else if (i % 3 == 0) {
        println("Fizz")
    } else if (i % 5 == 0) {
        println("Buzz")
    } else {
        println(i)
    }
}
```

**✅ Checkpoint**: You can control program flow with conditions and loops

---

## Level 5: Data Structures (50-60%)

### 5.1 Lists/Arrays

```javascript
// Create a list
let numbers = [1, 2, 3, 4, 5]
let mixed = [1, "two", 3.0, true]

// Access elements
println(numbers[0])    // 1
println(numbers[4])    // 5

// Modify elements
numbers[0] = 10
println(numbers[0])    // 10

// List length
println(len(numbers))  // 5
```

**Status**: ✅ Basic arrays implemented

### 5.2 Array Methods (Coming in Phase 2)

```javascript
// ⏳ NOT YET IMPLEMENTED
// Future support:
let numbers = [1, 2, 3, 4, 5]

// numbers.map(x => x * 2)        // [2, 4, 6, 8, 10]
// numbers.filter(x => x > 2)     // [3, 4, 5]
// numbers.reduce((a, b) => a + b, 0)  // 15
```

**Status**: ❌ Phase 2 - Not yet implemented

### 5.3 Objects ✨ Enhanced

```javascript
// Basic object
let person = {
    name: "Alice",
    age: 30,
    city: "NYC"
}

// Property shorthand ✨ NEW
let name = "Bob"
let age = 25
let user = {name, age}  // Same as {name: name, age: age}

// Computed property names ✨ NEW
let key = "score"
let data = {
    [key]: 100,
    ["item" + "1"]: "value"
}
```

**Status**: ✅ Basic objects, ✅ Shorthand (Phase 1), ✅ Computed properties (Phase 1)

### 5.4 Strings (Advanced)

```javascript
let text = "Hello, World!"

// String methods
text.uppercase()           // "HELLO, WORLD!"
text.lowercase()           // "hello, world!"
text.trim()                // Remove whitespace
text.split(",")            // ["Hello", " World!"]
text.substring(0, 5)       // "Hello"
text.contains("World")     // true
text.indexOf("o")          // 4
text.replace("World", "KLang")  // "Hello, KLang!"
```

**Status**: ✅ Fully Implemented

**Practice Exercise**:
```javascript
// Create a contacts list
const contacts = [
    {name: "Alice", phone: "555-1234"},
    {name: "Bob", phone: "555-5678"},
    {name: "Charlie", phone: "555-9012"}
]

// Print all contacts
for i in 0 .. len(contacts) {
    const c = contacts[i]
    println(`${c.name}: ${c.phone}`)
}
```

**✅ Checkpoint**: You can work with arrays, objects, and strings

---

## Level 6: Object-Oriented Programming (60-75%)

### 6.1 Classes

```javascript
class Person {
    init(name, age) {
        this.name = name
        this.age = age
    }
    
    greet() {
        return `Hello, I'm ${this.name}`
    }
}

let alice = new Person("Alice", 30)
println(alice.greet())  // "Hello, I'm Alice"
```

**Status**: ✅ Fully Implemented

### 6.2 Inheritance

```javascript
class Animal {
    init(name) {
        this.name = name
    }
    
    speak() {
        return `${this.name} makes a sound`
    }
}

class Dog extends Animal {
    speak() {
        return `${this.name} barks`
    }
}

let dog = new Dog("Buddy")
println(dog.speak())  // "Buddy barks"
```

**Status**: ✅ Fully Implemented

### 6.3 Access Modifiers

```javascript
class BankAccount {
    public let accountNumber
    private let balance
    protected let accountType
    
    init(number, initial) {
        this.accountNumber = number
        this.balance = initial
        this.accountType = "checking"
    }
    
    public fn getBalance() {
        return this.balance
    }
    
    private fn validateAmount(amount) {
        return amount > 0
    }
}
```

**Status**: ✅ Fully Implemented

### 6.4 Static Members

```javascript
class MathHelper {
    static let PI = 3.14159
    
    static fn square(x) {
        return x * x
    }
}

println(MathHelper.PI)          // 3.14159
println(MathHelper.square(5))   // 25
```

**Status**: ✅ Fully Implemented

### 6.5 Abstract Classes

```javascript
abstract class Shape {
    abstract fn area()
    
    fn describe() {
        return `Area: ${this.area()}`
    }
}

class Circle extends Shape {
    init(radius) {
        this.radius = radius
    }
    
    fn area() {
        return 3.14 * this.radius * this.radius
    }
}

let circle = new Circle(5)
println(circle.describe())  // "Area: 78.5"
```

**Status**: ✅ Fully Implemented

**Practice Exercise**:
```javascript
// Create a vehicle hierarchy
abstract class Vehicle {
    init(brand, model) {
        this.brand = brand
        this.model = model
    }
    
    abstract fn start()
    
    fn info() {
        return `${this.brand} ${this.model}`
    }
}

class Car extends Vehicle {
    fn start() {
        return `${this.info()} engine started`
    }
}

class ElectricCar extends Car {
    fn start() {
        return `${this.info()} electric motor started`
    }
}

let tesla = new ElectricCar("Tesla", "Model 3")
println(tesla.start())
```

**✅ Checkpoint**: You can design and implement object-oriented programs

---

## Level 7: Advanced Functions (75-85%)

### 7.1 Closures

```javascript
fn makeCounter() {
    let count = 0
    return () => {
        count = count + 1
        return count
    }
}

const counter1 = makeCounter()
println(counter1())  // 1
println(counter1())  // 2
println(counter1())  // 3
```

**Status**: ✅ Fully Implemented

### 7.2 Nested Functions

```javascript
fn outer(x) {
    fn inner(y) {
        return x + y
    }
    return inner(10)
}

println(outer(5))  // 15
```

**Status**: ✅ Fully Implemented

### 7.3 Callback Pattern

```javascript
fn process(value, callback) {
    let result = value * 2
    callback(result)
}

process(5, x => println(`Result: ${x}`))  // "Result: 10"
```

**Status**: ✅ Fully Implemented

**Practice Exercise**:
```javascript
// Implement a simple event system
fn createEmitter() {
    let listeners = []
    
    return {
        on: callback => {
            // Add listener (array methods needed)
        },
        emit: data => {
            // Call all listeners (array methods needed)
        }
    }
}
```

**✅ Checkpoint**: You understand advanced function concepts

---

## Level 8: Asynchronous Programming (85-90%)

### 8.1 Callbacks

```javascript
// Current: Basic callbacks work
fn fetchData(callback) {
    let data = "some data"
    callback(data)
}

fetchData(data => {
    println(`Got: ${data}`)
})
```

**Status**: ✅ Basic callbacks work

### 8.2 Promises (Coming in Phase 4)

```javascript
// ⏳ NOT YET IMPLEMENTED
// Future support:
/*
const promise = new Promise((resolve, reject) => {
    if (success) {
        resolve("Success!")
    } else {
        reject("Error!")
    }
})

promise
    .then(value => println(value))
    .catch(error => println(error))
*/
```

**Status**: ❌ Phase 4 - Not yet implemented

### 8.3 Async/Await (Coming in Phase 4)

```javascript
// ⏳ NOT YET IMPLEMENTED
// Future support:
/*
async fn fetchUser(id) {
    const response = await fetch(`/api/users/${id}`)
    const data = await response.json()
    return data
}
*/
```

**Status**: ❌ Phase 4 - Not yet implemented

**✅ Checkpoint**: You understand current async capabilities and future roadmap

---

## Level 9: Modules & Organization (90-95%)

### 9.1 Modules (Coming in Phase 5)

```javascript
// ⏳ NOT YET IMPLEMENTED
// Future support:
/*
// math.k
export fn add(a, b) {
    return a + b
}

export const PI = 3.14159

// main.k
import {add, PI} from "./math.k"
println(add(2, 3))
*/
```

**Status**: ❌ Phase 5 - Not yet implemented

### 9.2 Current Organization

```javascript
// Current best practice: Use classes for organization
class MathUtils {
    static fn add(a, b) {
        return a + b
    }
    
    static fn multiply(a, b) {
        return a * b
    }
}

// Use like a module
println(MathUtils.add(2, 3))
```

**Status**: ✅ Workaround using static classes

**✅ Checkpoint**: You know how to organize code now and what's coming

---

## Level 10: Advanced Features (95-100%)

### 10.1 Destructuring (Coming in Phase 2)

```javascript
// ⏳ NOT YET IMPLEMENTED
// Future support:
/*
const {name, age} = person
const [first, second] = array
*/
```

**Status**: ❌ Phase 2 - Not yet implemented

### 10.2 Spread/Rest (Coming in Phase 2)

```javascript
// ⏳ NOT YET IMPLEMENTED
// Future support:
/*
const arr1 = [1, 2, 3]
const arr2 = [...arr1, 4, 5]  // [1, 2, 3, 4, 5]

fn sum(...numbers) {
    // numbers is an array
}
*/
```

**Status**: ❌ Phase 2 - Not yet implemented

### 10.3 Generators (Coming in Phase 6)

```javascript
// ⏳ NOT YET IMPLEMENTED
// Future support:
/*
fn* fibonacci() {
    let a = 0, b = 1
    while (true) {
        yield a
        [a, b] = [b, a + b]
    }
}
*/
```

**Status**: ❌ Phase 6 - Not yet implemented

### 10.4 Try-Catch (Coming in Phase 3)

```javascript
// ⏳ NOT YET IMPLEMENTED
// Future support:
/*
try {
    riskyOperation()
} catch (error) {
    println(`Error: ${error}`)
} finally {
    cleanup()
}
*/
```

**Status**: ❌ Phase 3 - Not yet implemented

**✅ Checkpoint**: You know what advanced features are coming

---

## Feature Implementation Status

### ✅ Fully Implemented (40%)

1. **Variables**: `let`, `var`, `const` ✨
2. **Functions**: Traditional, arrow ✨, closures
3. **Control Flow**: if/else, while, for-in, break/continue
4. **Data Types**: int, float, string, bool, null, lists
5. **Strings**: All methods (uppercase, lowercase, split, etc.)
6. **Template Literals**: Interpolation, multi-line ✨
7. **Objects**: Basic, shorthand ✨, computed properties ✨
8. **OOP**: Classes, inheritance, abstract classes, access modifiers, static members
9. **Operators**: Arithmetic, comparison, logical

### ⏳ Coming Soon

**Phase 2 (Weeks 4-6)**: Spread/rest, destructuring, array methods  
**Phase 3 (Weeks 7-9)**: for-of, try-catch, ternary operator  
**Phase 4 (Weeks 10-13)**: Promises, async/await, event loop  
**Phase 5 (Weeks 14-16)**: ES6 modules, private fields  
**Phase 6 (Weeks 17-20+)**: Generators, symbols, proxies  

---

## Learning Path Recommendations

### For JavaScript Developers
1. Start at Level 2 (review variables)
2. Focus on Level 3.2 (arrow functions)
3. Review Level 6 (OOP might be familiar but different)
4. Check Level 8-10 (see what's not yet available)

### For Beginners
1. Start at Level 0 and work through sequentially
2. Complete all practice exercises
3. Don't skip levels
4. Focus on levels 1-7 first

### For Advanced Users
1. Review Phase 1 features (Levels 2-3)
2. Understand OOP enhancements (Level 6)
3. Check roadmap for missing features (Levels 8-10)
4. Contribute to Phase 2+ implementation

---

## Complete Feature Checklist

Use this to track your learning progress:

### Basics
- [ ] Write hello world
- [ ] Use variables and constants
- [ ] Perform arithmetic operations
- [ ] Use template literals
- [ ] Create and call functions
- [ ] Write arrow functions

### Intermediate
- [ ] Use if/else conditionals
- [ ] Write loops (while, for)
- [ ] Work with arrays
- [ ] Create objects
- [ ] Use object shorthand
- [ ] Create classes
- [ ] Implement inheritance

### Advanced
- [ ] Use closures
- [ ] Implement callbacks
- [ ] Use abstract classes
- [ ] Apply access modifiers
- [ ] Create static methods
- [ ] Design OOP systems

### Expert (Future)
- [ ] Use destructuring (Phase 2)
- [ ] Apply spread operator (Phase 2)
- [ ] Use array methods (Phase 2)
- [ ] Handle errors with try-catch (Phase 3)
- [ ] Write async functions (Phase 4)
- [ ] Use promises (Phase 4)
- [ ] Import/export modules (Phase 5)

---

## Next Steps

1. **Complete Beginner**: Work through Levels 1-4, completing all exercises
2. **Intermediate**: Master Levels 5-7, build a small project
3. **Advanced**: Explore Level 6 OOP in depth, contribute to KLang
4. **Expert**: Help implement Phase 2+ features

---

## Additional Resources

- **Main Documentation**: `/docs/` directory
- **Examples**: `/examples/` directory
- **Roadmap**: `JAVASCRIPT_FEATURES_ROADMAP.md`
- **Comparison**: `docs/JS_KLANG_FEATURE_COMPARISON.md`
- **Migration Guide**: `docs/JS_TO_KLANG_MIGRATION.md`

---

## Summary

**Current Completeness**: 40% of JavaScript features  
**Phase 1**: ✅ Complete (var/let/const, arrows, templates, objects)  
**Next Phase**: Phase 2 - Functional Programming (6 weeks)  
**Full Completion**: ~9 months for 80% compatibility  

You now have a complete guide to all KLang features from absolute beginner (0%) to advanced (100% of currently implemented features). Each level builds on the previous, with clear checkpoints and practice exercises.

Happy coding! 🚀
