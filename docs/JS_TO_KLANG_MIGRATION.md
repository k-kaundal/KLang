# JavaScript to KLang Migration Guide

**For JavaScript developers learning KLang**

---

## Quick Start: What You Already Know

If you know JavaScript, you already know ~40% of KLang! Here's what works exactly the same:

### ✅ Identical Syntax

```javascript
// These work the same in both languages:

let x = 10;
const y = 20;

if (x > 5) {
    println("x is greater than 5");
}

while (x > 0) {
    x = x - 1;
}

for (i: int = 0; i < 10; i = i + 1) {
    println(i);
}

let arr = [1, 2, 3, 4, 5];
let obj = { name: "Alice", age: 25 };

// Arrow functions!
let double = x => x * 2;
let add = (a, b) => a + b;

// Template literals!
let greeting = `Hello, ${name}!`;
```

---

## Key Differences to Know

### 1. Function Declaration Syntax

```javascript
// JavaScript
function greet(name) {
    return "Hello, " + name;
}
```

```klang
// KLang - use 'fn' keyword
fn greet(name: string) -> string {
    return "Hello, " + name;
}
```

**Why**: More explicit, supports type annotations

---

### 2. Constructor Name

```javascript
// JavaScript
class Person {
    constructor(name) {
        this.name = name;
    }
}
```

```klang
// KLang - use 'init' instead of 'constructor'
class Person {
    fn init(name: string) {
        this.name = name;
    }
}
```

**Why**: More consistent naming across all methods

---

### 3. No `undefined` - Only `null`

```javascript
// JavaScript
let x;              // undefined
let obj = {};
obj.missing;        // undefined
```

```klang
// KLang - everything is null by default
let x;              // null
let obj = {};
obj.missing;        // null
```

**Why**: Simpler mental model, one null type instead of two

---

### 4. Type Annotations (Built-in!)

```javascript
// JavaScript - need TypeScript
let name: string = "Alice";
let age: number = 25;
```

```klang
// KLang - native support
let name: string = "Alice";
let age: int = 25;           // Note: separate int and float
let price: float = 19.99;
```

**Types**: `int`, `float`, `string`, `bool`, `null`

---

### 5. Access Modifiers

```javascript
// JavaScript ES2022+
class Person {
    #privateField = 0;  // Private with #
    
    #privateMethod() {
        // ...
    }
}
```

```klang
// KLang - explicit keywords
class Person {
    private let privateField: int = 0;
    public let publicField: int = 0;
    protected let protectedField: int = 0;
    
    private fn privateMethod() {
        // ...
    }
}
```

**Why**: More explicit and clearer intent

---

### 6. Print/Console

```javascript
// JavaScript
console.log("Hello");
console.log("Hello", "World");
```

```klang
// KLang
print("Hello");      // No newline
println("Hello");    // With newline

// Multiple arguments - use template literals instead
println(`Hello ${world}`);
```

---

## What's Not Yet Implemented (But Coming Soon!)

### Phase 2: Functional Programming

```javascript
// JavaScript - NOT YET in KLang

// Destructuring
const [a, b] = [1, 2];
const { name, age } = person;

// Spread
const arr2 = [...arr1, 4, 5];
const obj2 = { ...obj1, newKey: "value" };

// Rest parameters
function sum(...numbers) { }

// Array methods
arr.map(x => x * 2);
arr.filter(x => x > 0);
arr.reduce((sum, x) => sum + x, 0);

// Default parameters
function greet(name = "World") { }
```

**Status**: Planned for Phase 2 (4-6 weeks)

---

### Phase 3: Advanced Control Flow

```javascript
// JavaScript - NOT YET in KLang

// Ternary
const result = x > 0 ? "positive" : "negative";

// Logical operators
if (user && user.isActive) { }
const value = a || b || c;

// Optional chaining
const name = user?.profile?.name;

// Nullish coalescing
const value = input ?? defaultValue;

// Switch-case
switch (value) {
    case 1: return "one";
    case 2: return "two";
    default: return "other";
}

// For-of
for (const item of array) { }

// Try-catch
try {
    riskyOperation();
} catch (error) {
    handleError(error);
}
```

**Status**: Planned for Phase 3 (3-4 weeks)

---

### Phase 4: Async Programming

```javascript
// JavaScript - NOT YET in KLang

// Promises
fetch(url)
    .then(response => response.json())
    .then(data => console.log(data))
    .catch(error => console.error(error));

// Async/Await
async function getData() {
    const response = await fetch(url);
    const data = await response.json();
    return data;
}
```

**Status**: Planned for Phase 4 (6-8 weeks)

---

### Phase 5: Modules

```javascript
// JavaScript - NOT YET in KLang

// Export
export const PI = 3.14159;
export function square(x) { return x * x; }
export default class Calculator { }

// Import
import Calculator from './calculator.js';
import { PI, square } from './math.js';
```

**Status**: Planned for Phase 5 (4-6 weeks)

---

## Migration Patterns

### Pattern 1: Console.log → println

```javascript
// JavaScript
console.log("Hello, World!");
console.log("Value:", x);
console.log(`Result: ${result}`);
```

```klang
// KLang
println("Hello, World!");
println(`Value: ${x}`);           // Use template literals
println(`Result: ${result}`);
```

---

### Pattern 2: Array Methods → Loops (Temporary)

```javascript
// JavaScript
const doubled = numbers.map(x => x * 2);
const evens = numbers.filter(x => x % 2 === 0);
const sum = numbers.reduce((acc, x) => acc + x, 0);
```

```klang
// KLang - until Phase 2 implements array methods
let doubled = [];
for (i: int = 0; i < numbers.length; i = i + 1) {
    # doubled.push() not yet implemented - build array differently
}

// Alternative: wait for Phase 2 or use different approach
```

---

### Pattern 3: Destructuring → Multiple Assignments (Temporary)

```javascript
// JavaScript
const [x, y] = point;
const { name, age } = person;
```

```klang
// KLang - until Phase 2
let x = point[0];
let y = point[1];

let name = person.name;
let age = person.age;
```

---

### Pattern 4: Ternary → If-Else (Temporary)

```javascript
// JavaScript
const result = condition ? trueValue : falseValue;
```

```klang
// KLang - until Phase 3
let result = null;
if (condition) {
    result = trueValue;
} else {
    result = falseValue;
}
```

---

### Pattern 5: Logical AND/OR → Nested If (Temporary)

```javascript
// JavaScript
if (user && user.isActive) {
    // ...
}

const value = a || b || c;
```

```klang
// KLang - until Phase 3
if (user != null) {
    if (user.isActive) {
        // ...
    }
}

let value = a;
if (value == null) {
    value = b;
}
if (value == null) {
    value = c;
}
```

---

### Pattern 6: Try-Catch → Return Codes (Temporary)

```javascript
// JavaScript
try {
    const result = riskyOperation();
    return result;
} catch (error) {
    console.error(error);
    return null;
}
```

```klang
// KLang - until Phase 3
// Design functions to return null on error
fn riskyOperation() -> string {
    if (/* error condition */) {
        return null;
    }
    return result;
}

let result = riskyOperation();
if (result == null) {
    println("Error occurred");
}
```

---

## Common Gotchas

### 1. Variable Scope - `var` is Block-Scoped

```javascript
// JavaScript - var is function-scoped
if (true) {
    var x = 10;
}
console.log(x);  // 10 - var leaks out of block
```

```klang
// KLang - var is block-scoped
if (true) {
    var x = 10;
}
println(x);  // Error! x is out of scope
```

**Solution**: This is actually better! Less confusing than JavaScript

---

### 2. No Hoisting

```javascript
// JavaScript - hoisting works
console.log(x);  // undefined
var x = 10;
```

```klang
// KLang - no hoisting
println(x);  // Error! x not defined yet
let x = 10;
```

**Solution**: Always declare variables before use (better practice anyway)

---

### 3. Semicolons Matter More

```javascript
// JavaScript - ASI (Automatic Semicolon Insertion)
return
    {
        value: 42
    }
// Returns undefined, not the object!
```

```klang
// KLang - be explicit
return {
    value: 42
};  // Works correctly
```

**Solution**: Always use explicit semicolons, keep opening braces on same line

---

### 4. Type Conversions

```javascript
// JavaScript
parseInt("42");
parseFloat("3.14");
String(42);
```

```klang
// KLang
int("42");
float("3.14");
str(42);
```

---

### 5. String Concatenation

```javascript
// JavaScript - both work
const msg1 = "Hello" + " " + "World";
const msg2 = `Hello ${"World"}`;
```

```klang
// KLang - both work
let msg1 = "Hello" + " " + "World";
let msg2 = `Hello ${"World"}`;

// Prefer template literals for readability
```

---

## Complete Migration Example

### JavaScript Version

```javascript
// calculator.js

class Calculator {
    #history = [];
    
    add(a, b) {
        const result = a + b;
        this.#history.push({ op: 'add', a, b, result });
        return result;
    }
    
    getHistory() {
        return [...this.#history];
    }
    
    getStats() {
        const total = this.#history.length;
        const results = this.#history.map(h => h.result);
        const sum = results.reduce((acc, r) => acc + r, 0);
        return { total, sum };
    }
}

const calc = new Calculator();
console.log(calc.add(5, 3));
console.log(calc.add(10, 20));

const { total, sum } = calc.getStats();
console.log(`Total operations: ${total}, Sum: ${sum}`);
```

---

### KLang Version (Current)

```klang
// calculator.kl

class Calculator {
    private let history: list;
    
    fn init() {
        this.history = [];
    }
    
    fn add(a: int, b: int) -> int {
        let result = a + b;
        # Note: array push not yet implemented
        # this.history.push({ op: "add", a: a, b: b, result: result });
        return result;
    }
    
    fn getHistoryLength() -> int {
        return this.history.length;
    }
}

let calc = Calculator();
println(calc.add(5, 3));
println(calc.add(10, 20));

let total = calc.getHistoryLength();
println(`Total operations: ${total}`);
```

---

### KLang Version (After Phase 2-3)

```klang
// calculator.kl - Future version with more features

class Calculator {
    private let history: list;
    
    fn init() {
        this.history = [];
    }
    
    fn add(a: int, b: int) -> int {
        let result = a + b;
        this.history.push({ op: "add", a, b, result });  // Phase 2
        return result;
    }
    
    fn getHistory() -> list {
        return [...this.history];  // Phase 2: spread
    }
    
    fn getStats() -> object {
        let total = this.history.length;
        let results = this.history.map(h => h.result);    // Phase 2: map
        let sum = results.reduce((acc, r) => acc + r, 0); // Phase 2: reduce
        return { total, sum };
    }
}

let calc = Calculator();
println(calc.add(5, 3));
println(calc.add(10, 20));

let { total, sum } = calc.getStats();  // Phase 2: destructuring
println(`Total operations: ${total}, Sum: ${sum}`);
```

---

## What Works Great Right Now

### 1. Object-Oriented Programming

KLang's OOP is actually **better** than JavaScript in many ways:

```klang
abstract class Shape {
    protected let color: string;
    
    fn init(color: string) {
        this.color = color;
    }
    
    abstract fn area() -> float;
    abstract fn perimeter() -> float;
    
    fn describe() {
        println(`A ${this.color} shape`);
    }
}

class Circle extends Shape {
    private let radius: float;
    
    fn init(color: string, radius: float) {
        super.init(color);
        this.radius = radius;
    }
    
    fn area() -> float {
        return 3.14159 * this.radius * this.radius;
    }
    
    fn perimeter() -> float {
        return 2.0 * 3.14159 * this.radius;
    }
}

let circle = Circle("red", 5.0);
circle.describe();
println(`Area: ${circle.area()}`);
```

**Features JavaScript doesn't have natively**:
- Abstract classes
- Abstract methods
- Explicit access modifiers (public, private, protected)
- Type annotations built-in

---

### 2. String Manipulation

Full template literal support plus comprehensive methods:

```klang
let name = "Alice";
let age = 25;

// Template literals (same as JS)
let greeting = `Hello, ${name}!`;
let info = `${name} is ${age} years old`;

// Rich string methods
let text = "  Hello World  ";
println(text.trim());                    // "Hello World"
println(text.toUpperCase());             // "  HELLO WORLD  "
println(text.contains("World"));         // true
println(text.startsWith("  Hello"));     // true
println(text.replace("World", "KLang")); // "  Hello KLang  "
println(text.repeat(3));                 // repeat 3 times

let parts = "a,b,c".split(",");          // ["a", "b", "c"]
```

---

### 3. Arrow Functions

Complete ES6 arrow function support:

```klang
// All these work!
let double = x => x * 2;
let add = (a, b) => a + b;
let getRandom = () => 42;

let factorial = n => {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
};

// Use with callback-style functions
let operations = [
    x => x + 1,
    x => x * 2,
    x => x * x
];

for (i: int = 0; i < operations.length; i = i + 1) {
    let op = operations[i];
    println(op(5));  // Call each operation
}
```

---

### 4. Enhanced Object Literals

```klang
let x = 10;
let y = 20;
let key = "dynamic";

let obj = {
    // Regular properties
    name: "Alice",
    age: 25,
    
    // Property shorthand (ES6)
    x,    // Same as x: x
    y,    // Same as y: y
    
    // Computed properties (ES6)
    [key]: "value",
    [`computed_${key}`]: "computed"
};

println(obj.name);         // "Alice"
println(obj.x);            // 10
println(obj["dynamic"]);   // "value"
```

---

## Tips for JavaScript Developers

### 1. Embrace Type Annotations

```klang
// Make your code more explicit and safer
fn calculateTotal(items: list, taxRate: float) -> float {
    let subtotal: float = 0.0;
    for (i: int = 0; i < items.length; i = i + 1) {
        subtotal = subtotal + items[i].price;
    }
    return subtotal * (1.0 + taxRate);
}
```

### 2. Use Template Literals Everywhere

```klang
// Much more readable than concatenation
println(`User ${name} (age ${age}) has ${points} points`);

// Instead of:
println("User " + name + " (age " + str(age) + ") has " + str(points) + " points");
```

### 3. Be Explicit with Access Modifiers

```klang
class BankAccount {
    // Good: clear intent
    private let balance: float;
    public let accountNumber: string;
    protected let internalId: int;
    
    public fn deposit(amount: float) {
        this.balance = this.balance + amount;
    }
    
    private fn validateAmount(amount: float) -> bool {
        return amount > 0.0;
    }
}
```

### 4. Design for Future Features

```klang
// Write code that will work well when new features arrive

// Structure for easy refactoring to array methods
fn filterEvens(numbers: list) -> list {
    let result = [];
    for (i: int = 0; i < numbers.length; i = i + 1) {
        if (numbers[i] % 2 == 0) {
            # result.push(numbers[i]);  // Will work in Phase 2
        }
    }
    return result;
}

// Later becomes:
// fn filterEvens(numbers: list) -> list {
//     return numbers.filter(x => x % 2 == 0);
// }
```

---

## Learning Path

### Week 1: Fundamentals
- Variables: `let`, `const`, `var`
- Functions: regular and arrow
- Control flow: `if`, `while`, `for`
- Basic types and operators

**Practice**: Write simple algorithms (fibonacci, factorial, etc.)

---

### Week 2: Objects and Classes
- Object literals
- Classes and constructors
- Inheritance and `super`
- `this` keyword

**Practice**: Build a class hierarchy (shapes, animals, etc.)

---

### Week 3: Advanced OOP
- Access modifiers
- Static members
- Abstract classes
- Method overriding

**Practice**: Design a complete system (game, app, etc.)

---

### Week 4: Strings and Data Structures
- Template literals
- String methods
- Arrays and objects
- Nested structures

**Practice**: Text processing, data transformation

---

### Week 5+: Wait for Phase 2-3
- Keep track of roadmap
- Prepare to refactor code
- Learn upcoming features

---

## Migration Checklist

When porting JavaScript code to KLang:

- [ ] Change `function` to `fn`
- [ ] Change `constructor` to `init`
- [ ] Add type annotations
- [ ] Replace `console.log` with `println`
- [ ] Check for `undefined` → use `null`
- [ ] Convert array methods to loops (temporary)
- [ ] Convert destructuring to property access (temporary)
- [ ] Convert ternary to if-else (temporary)
- [ ] Convert logical operators to nested ifs (temporary)
- [ ] Add explicit access modifiers to classes
- [ ] Remove async/await (not yet supported)
- [ ] Remove imports/exports (not yet supported)
- [ ] Test thoroughly in KLang REPL

---

## Resources

- **Full Comparison**: [JS_KLANG_FEATURE_COMPARISON.md](JS_KLANG_FEATURE_COMPARISON.md)
- **Quick Summary**: [FEATURE_COMPARISON_SUMMARY.md](FEATURE_COMPARISON_SUMMARY.md)
- **Language Reference**: [LANGUAGE_REFERENCE.md](LANGUAGE_REFERENCE.md)
- **OOP Guide**: [OOP_LANGUAGE_SPEC.md](OOP_LANGUAGE_SPEC.md)
- **Roadmap**: [JAVASCRIPT_FEATURES_ROADMAP.md](../JAVASCRIPT_FEATURES_ROADMAP.md)

---

## FAQ

### Q: When will KLang have feature parity with modern JavaScript?

**A**: Aiming for 80% compatibility within 9 months:
- Phase 2 (functional): 4-6 weeks
- Phase 3 (control flow): 3-4 weeks  
- Phase 4 (async): 6-8 weeks
- Phase 5 (modules): 4-6 weeks
- Phase 6 (advanced): 8-12 weeks

### Q: Should I wait for more features before learning KLang?

**A**: No! The current feature set is solid for:
- Learning programming
- Building OOP systems
- Educational projects
- Understanding language design

### Q: Will my KLang code break when new features are added?

**A**: No. New features are additive. Your code will continue to work and you can refactor to use new features when ready.

### Q: Is KLang production-ready?

**A**: Not yet. Great for:
- ✅ Learning and education
- ✅ Experimentation
- ✅ Prototyping
- ❌ Production systems (wait for Phase 4+)

### Q: Can I contribute to missing features?

**A**: Yes! See [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines.

---

**Last Updated**: 2024  
**KLang Version**: v0.3.0  
**Target Audience**: JavaScript developers learning KLang
