# JavaScript vs KLang: Complete Feature Comparison

**Version**: KLang v0.3.0  
**JavaScript Reference**: ES5 through ES2024  
**Last Updated**: 2024

---

## Table of Contents

1. [Executive Summary](#executive-summary)
2. [Complete Feature Matrix](#complete-feature-matrix)
3. [Syntax Comparison](#syntax-comparison)
4. [Feature Completeness](#feature-completeness)
5. [What's Missing](#whats-missing)
6. [Implementation Priority](#implementation-priority)
7. [Feature Details](#feature-details)

---

## Executive Summary

KLang is a modern programming language inspired by JavaScript, designed to bring familiar JavaScript syntax with enhanced features and type safety. This document provides a comprehensive comparison between JavaScript (ES5-ES2024) and KLang's current implementation status.

### Quick Stats

| Metric | Status |
|--------|--------|
| **Overall Feature Completeness** | ~40% |
| **Core Features** | 95% |
| **Modern ES6+ Features** | 35% |
| **Advanced Features** | 10% |
| **Test Pass Rate** | 100% (160+ tests) |

### Key Strengths

✅ **Fully Implemented**:
- Variables: `var`, `let`, `const` with proper scoping
- Arrow functions (ES6)
- Template literals with interpolation
- Enhanced object literals (shorthand, computed properties)
- Complete OOP: classes, inheritance, static members, access modifiers, abstract classes
- Comprehensive string manipulation

🚧 **In Progress**:
- Functional programming (spread, rest, destructuring)
- Advanced control flow (ternary, optional chaining, nullish coalescing)
- Error handling (try-catch-finally)

❌ **Not Yet Implemented**:
- Async programming (promises, async/await)
- Modules (import/export)
- Advanced features (generators, proxies, symbols)

---

## Complete Feature Matrix

### 1. Variables and Declarations

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| `var` declaration | ✅ | ✅ | ✅ Full | Function/block scoped in KLang |
| `let` declaration | ✅ | ✅ | ✅ Full | Block-scoped, prevents redeclaration |
| `const` declaration | ✅ | ✅ | ✅ Full | Immutable, requires initialization |
| Type annotations | ❌ (TS only) | ✅ | ✅ Full | `let x: int = 10` |
| Hoisting | ✅ | ❌ | ⚠️ Different | KLang uses strict initialization |
| Temporal Dead Zone | ✅ | ✅ | ✅ Full | For `let` and `const` |

**Compatibility**: 95%

---

### 2. Primitive Types

| Type | JavaScript | KLang | Status | Notes |
|------|-----------|-------|--------|-------|
| Number | ✅ | ✅ | ✅ Full | Separated into `int` and `float` |
| String | ✅ | ✅ | ✅ Full | Full Unicode support |
| Boolean | ✅ | ✅ | ✅ Full | `true` and `false` |
| null | ✅ | ✅ | ✅ Full | Explicit null value |
| undefined | ✅ | ❌ | ❌ Missing | Uses `null` instead |
| Symbol | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| BigInt | ✅ | ❌ | ❌ Missing | Not planned |

**Compatibility**: 60%

---

### 3. Functions

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| Function declarations | ✅ | ✅ | ✅ Full | Different syntax: `fn name() {}` |
| Function expressions | ✅ | ✅ | ✅ Full | `let f = fn() {}` |
| Arrow functions | ✅ | ✅ | ✅ Full | Full ES6 syntax support |
| Arrow function - expression body | ✅ | ✅ | ✅ Full | `x => x * 2` |
| Arrow function - block body | ✅ | ✅ | ✅ Full | `x => { return x * 2; }` |
| Arrow function - no params | ✅ | ✅ | ✅ Full | `() => value` |
| Arrow function - multi params | ✅ | ✅ | ✅ Full | `(a, b) => a + b` |
| Default parameters | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| Rest parameters | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| Spread in calls | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| Closures | ✅ | ✅ | ✅ Full | Full lexical scope support |
| IIFE | ✅ | ✅ | ✅ Full | `(fn() { ... })()` |
| Return statement | ✅ | ✅ | ✅ Full | Required for block bodies |
| Recursion | ✅ | ✅ | ✅ Full | Fully supported |
| Generator functions | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| Async functions | ✅ | ❌ | ❌ Missing | Planned for Phase 4 |
| Named parameters | ❌ | ❌ | ❌ Missing | Not planned |
| Function overloading | ❌ | ❌ | ❌ Missing | Planned for Phase 5 |

**Compatibility**: 55%

---

### 4. Objects

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| Object literals | ✅ | ✅ | ✅ Full | `{ key: value }` |
| Property access (dot) | ✅ | ✅ | ✅ Full | `obj.property` |
| Property access (bracket) | ✅ | ✅ | ✅ Full | `obj["property"]` |
| Property shorthand | ✅ | ✅ | ✅ Full | `{ x, y }` |
| Computed properties | ✅ | ✅ | ✅ Full | `{ [key]: value }` |
| Method shorthand | ✅ | ⚠️ | ⚠️ Partial | Basic support |
| Object destructuring | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| Spread properties | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| Getters/Setters | ✅ | ❌ | ❌ Missing | Planned for Phase 5 |
| Object.keys() | ✅ | ❌ | ❌ Missing | Not planned |
| Object.values() | ✅ | ❌ | ❌ Missing | Not planned |
| Object.entries() | ✅ | ❌ | ❌ Missing | Not planned |
| Object.assign() | ✅ | ❌ | ❌ Missing | Not planned |

**Compatibility**: 50%

---

### 5. Arrays

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| Array literals | ✅ | ✅ | ✅ Full | `[1, 2, 3]` |
| Index access | ✅ | ✅ | ✅ Full | `arr[0]` |
| .length property | ✅ | ✅ | ✅ Full | Read-only in KLang |
| Nested arrays | ✅ | ✅ | ✅ Full | Fully supported |
| Array destructuring | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| Spread operator | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .push() | ✅ | ❌ | ❌ Missing | Not planned |
| .pop() | ✅ | ❌ | ❌ Missing | Not planned |
| .shift() | ✅ | ❌ | ❌ Missing | Not planned |
| .unshift() | ✅ | ❌ | ❌ Missing | Not planned |
| .map() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .filter() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .reduce() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .forEach() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .find() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .findIndex() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .some() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .every() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .concat() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .slice() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .splice() | ✅ | ❌ | ❌ Missing | Not planned |
| .sort() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .reverse() | ✅ | ❌ | ❌ Missing | Not planned |
| .join() | ✅ | ✅ | ✅ Full | String method in KLang |
| .flat() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |
| .flatMap() | ✅ | ❌ | ❌ Missing | Planned for Phase 2 |

**Compatibility**: 15%

---

### 6. Strings

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| String literals (`""`, `''`) | ✅ | ✅ | ✅ Full | Both supported |
| Template literals | ✅ | ✅ | ✅ Full | `` `text ${expr}` `` |
| Multi-line strings | ✅ | ✅ | ✅ Full | Via template literals |
| Escape sequences | ✅ | ✅ | ✅ Full | `\n`, `\t`, etc. |
| .length property | ✅ | ✅ | ✅ Full | Character count |
| .toUpperCase() | ✅ | ✅ | ✅ Full | Case conversion |
| .toLowerCase() | ✅ | ✅ | ✅ Full | Case conversion |
| .trim() | ✅ | ✅ | ✅ Full | Whitespace removal |
| .split() | ✅ | ✅ | ✅ Full | String to array |
| .substring() | ✅ | ✅ | ✅ Full | Extract substring |
| .startsWith() | ✅ | ✅ | ✅ Full | Prefix check |
| .endsWith() | ✅ | ✅ | ✅ Full | Suffix check |
| .includes() | ✅ | ✅ | ✅ Full | Contains check |
| .indexOf() | ✅ | ✅ | ✅ Full | Find position |
| .replace() | ✅ | ✅ | ✅ Full | String replacement |
| .repeat() | ✅ | ✅ | ✅ Full | Repeat string |
| .slice() | ✅ | ❌ | ❌ Missing | Use .substring() |
| .charAt() | ✅ | ❌ | ❌ Missing | Use index access |
| .charCodeAt() | ✅ | ❌ | ❌ Missing | Not planned |
| .concat() | ✅ | ❌ | ❌ Missing | Use `+` operator |
| .match() | ✅ | ❌ | ❌ Missing | Requires regex |
| .search() | ✅ | ❌ | ❌ Missing | Requires regex |
| .padStart() | ✅ | ❌ | ❌ Missing | Not planned |
| .padEnd() | ✅ | ❌ | ❌ Missing | Not planned |

**Compatibility**: 70%

---

### 7. Classes and OOP

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| Class declaration | ✅ | ✅ | ✅ Full | Different syntax |
| Constructor | ✅ | ✅ | ✅ Full | `fn init()` in KLang |
| Instance methods | ✅ | ✅ | ✅ Full | Fully supported |
| Instance fields | ✅ | ✅ | ✅ Full | Declared in class body |
| Static methods | ✅ | ✅ | ✅ Full | `static fn method()` |
| Static fields | ✅ | ✅ | ✅ Full | `static let field` |
| Inheritance (extends) | ✅ | ✅ | ✅ Full | Single inheritance |
| super keyword | ✅ | ✅ | ✅ Full | Access parent methods |
| this keyword | ✅ | ✅ | ✅ Full | Self-reference |
| Method overriding | ✅ | ✅ | ✅ Full | Child overrides parent |
| Private fields (#) | ✅ | ❌ | ⚠️ Different | Use `private` keyword |
| Private methods | ✅ | ✅ | ✅ Full | `private fn method()` |
| Protected members | ❌ | ✅ | ✅ Enhanced | `protected` keyword |
| Public members | ❌ (implicit) | ✅ | ✅ Enhanced | `public` keyword |
| Abstract classes | ❌ | ✅ | ✅ Enhanced | `abstract class` |
| Abstract methods | ❌ | ✅ | ✅ Enhanced | `abstract fn method()` |
| Getters | ✅ | ❌ | ❌ Missing | Planned for Phase 5 |
| Setters | ✅ | ❌ | ❌ Missing | Planned for Phase 5 |
| Class expressions | ✅ | ❌ | ❌ Missing | Not planned |
| Multiple inheritance | ❌ | ❌ | ❌ Missing | Planned for Phase 5 |
| Interfaces | ❌ (TS only) | ❌ | ❌ Missing | Planned for Phase 5 |

**Compatibility**: 75%

---

### 8. Control Flow

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| if statement | ✅ | ✅ | ✅ Full | Same syntax |
| else statement | ✅ | ✅ | ✅ Full | Same syntax |
| else if | ✅ | ✅ | ✅ Full | Same syntax |
| while loop | ✅ | ✅ | ✅ Full | Same syntax |
| do-while loop | ✅ | ❌ | ❌ Missing | Not planned |
| for loop | ✅ | ✅ | ✅ Full | C-style syntax |
| for-in loop | ✅ | ❌ | ❌ Missing | Planned for Phase 3 |
| for-of loop | ✅ | ❌ | ❌ Missing | Planned for Phase 3 |
| break statement | ✅ | ✅ | ✅ Full | Exit loops |
| continue statement | ✅ | ✅ | ✅ Full | Skip iteration |
| return statement | ✅ | ✅ | ✅ Full | Exit function |
| switch statement | ✅ | ❌ | ❌ Missing | Planned for Phase 3 |
| Ternary operator | ✅ | ❌ | ❌ Missing | Planned for Phase 3 |
| Labeled statements | ✅ | ❌ | ❌ Missing | Not planned |

**Compatibility**: 60%

---

### 9. Operators

| Category | Operator | JavaScript | KLang | Status |
|----------|----------|-----------|-------|--------|
| **Arithmetic** | `+` | ✅ | ✅ | ✅ Full |
| | `-` | ✅ | ✅ | ✅ Full |
| | `*` | ✅ | ✅ | ✅ Full |
| | `/` | ✅ | ✅ | ✅ Full |
| | `%` | ✅ | ✅ | ✅ Full |
| | `**` | ✅ | ❌ | ❌ Missing |
| **Assignment** | `=` | ✅ | ✅ | ✅ Full |
| | `+=` | ✅ | ❌ | ❌ Missing |
| | `-=` | ✅ | ❌ | ❌ Missing |
| | `*=` | ✅ | ❌ | ❌ Missing |
| | `/=` | ✅ | ❌ | ❌ Missing |
| | `%=` | ✅ | ❌ | ❌ Missing |
| **Comparison** | `==` | ✅ | ✅ | ✅ Full |
| | `!=` | ✅ | ✅ | ✅ Full |
| | `===` | ✅ | ❌ | ❌ Missing |
| | `!==` | ✅ | ❌ | ❌ Missing |
| | `<` | ✅ | ✅ | ✅ Full |
| | `>` | ✅ | ✅ | ✅ Full |
| | `<=` | ✅ | ✅ | ✅ Full |
| | `>=` | ✅ | ✅ | ✅ Full |
| **Logical** | `&&` | ✅ | ❌ | ❌ Missing |
| | `\|\|` | ✅ | ❌ | ❌ Missing |
| | `!` | ✅ | ✅ | ✅ Full |
| **Unary** | `+` | ✅ | ❌ | ❌ Missing |
| | `-` | ✅ | ✅ | ✅ Full |
| | `++` | ✅ | ❌ | ❌ Missing |
| | `--` | ✅ | ❌ | ❌ Missing |
| | `typeof` | ✅ | ⚠️ | ⚠️ Different |
| | `void` | ✅ | ❌ | ❌ Missing |
| | `delete` | ✅ | ❌ | ❌ Missing |
| **Ternary** | `? :` | ✅ | ❌ | ❌ Missing |
| **Nullish** | `??` | ✅ | ❌ | ❌ Missing |
| **Optional** | `?.` | ✅ | ❌ | ❌ Missing |
| **Spread** | `...` | ✅ | ❌ | ❌ Missing |
| **Bitwise** | `&` | ✅ | ❌ | ❌ Missing |
| | `\|` | ✅ | ❌ | ❌ Missing |
| | `^` | ✅ | ❌ | ❌ Missing |
| | `~` | ✅ | ❌ | ❌ Missing |
| | `<<` | ✅ | ❌ | ❌ Missing |
| | `>>` | ✅ | ❌ | ❌ Missing |
| | `>>>` | ✅ | ❌ | ❌ Missing |
| **Other** | `in` | ✅ | ❌ | ❌ Missing |
| | `instanceof` | ✅ | ❌ | ❌ Missing |
| | `,` (comma) | ✅ | ❌ | ❌ Missing |

**Compatibility**: 30%

---

### 10. Error Handling

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| try-catch | ✅ | ❌ | ❌ Missing | Planned for Phase 3 |
| try-catch-finally | ✅ | ❌ | ❌ Missing | Planned for Phase 3 |
| throw statement | ✅ | ❌ | ❌ Missing | Planned for Phase 3 |
| Error objects | ✅ | ❌ | ❌ Missing | Planned for Phase 3 |
| Error types | ✅ | ❌ | ❌ Missing | Not planned |
| Stack traces | ✅ | ❌ | ❌ Missing | Not planned |
| Custom errors | ✅ | ❌ | ❌ Missing | Not planned |

**Compatibility**: 0%

---

### 11. Async Programming

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| Promises | ✅ | ❌ | ❌ Missing | Planned for Phase 4 |
| .then() | ✅ | ❌ | ❌ Missing | Planned for Phase 4 |
| .catch() | ✅ | ❌ | ❌ Missing | Planned for Phase 4 |
| .finally() | ✅ | ❌ | ❌ Missing | Planned for Phase 4 |
| Promise.all() | ✅ | ❌ | ❌ Missing | Planned for Phase 4 |
| Promise.race() | ✅ | ❌ | ❌ Missing | Planned for Phase 4 |
| Promise.allSettled() | ✅ | ❌ | ❌ Missing | Not planned |
| Promise.any() | ✅ | ❌ | ❌ Missing | Not planned |
| async functions | ✅ | ❌ | ❌ Missing | Planned for Phase 4 |
| await keyword | ✅ | ❌ | ❌ Missing | Planned for Phase 4 |
| Top-level await | ✅ | ❌ | ❌ Missing | Not planned |
| Callbacks | ✅ | ✅ | ✅ Full | Via function passing |
| setTimeout | ✅ | ❌ | ❌ Missing | Not planned |
| setInterval | ✅ | ❌ | ❌ Missing | Not planned |

**Compatibility**: 7%

---

### 12. Modules

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| import statement | ✅ | ❌ | ❌ Missing | Planned for Phase 5 |
| export statement | ✅ | ❌ | ❌ Missing | Planned for Phase 5 |
| default export | ✅ | ❌ | ❌ Missing | Planned for Phase 5 |
| named export | ✅ | ❌ | ❌ Missing | Planned for Phase 5 |
| export * | ✅ | ❌ | ❌ Missing | Not planned |
| import * as | ✅ | ❌ | ❌ Missing | Not planned |
| import { } | ✅ | ❌ | ❌ Missing | Planned for Phase 5 |
| Dynamic import() | ✅ | ❌ | ❌ Missing | Not planned |
| require() | ✅ (Node) | ❌ | ❌ Missing | Not planned |
| module.exports | ✅ (Node) | ❌ | ❌ Missing | Not planned |

**Compatibility**: 0%

---

### 13. Advanced Features

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| Generators | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| yield keyword | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| yield* | ✅ | ❌ | ❌ Missing | Not planned |
| Iterators | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| Async iterators | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| Symbol | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| Symbol.iterator | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| Proxy | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| Reflect | ✅ | ❌ | ❌ Missing | Not planned |
| WeakMap | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| WeakSet | ✅ | ❌ | ❌ Missing | Planned for Phase 6 |
| Map | ✅ | ❌ | ❌ Missing | Not planned |
| Set | ✅ | ❌ | ❌ Missing | Not planned |
| Regular Expressions | ✅ | ❌ | ❌ Missing | Not planned |
| ArrayBuffer | ✅ | ❌ | ❌ Missing | Not planned |
| TypedArrays | ✅ | ❌ | ❌ Missing | Not planned |

**Compatibility**: 0%

---

### 14. Built-in Objects and Functions

| Feature | JavaScript | KLang | Status | Notes |
|---------|-----------|-------|--------|-------|
| console.log() | ✅ | ⚠️ | ⚠️ Different | `print()` / `println()` |
| JSON.parse() | ✅ | ❌ | ❌ Missing | Not planned |
| JSON.stringify() | ✅ | ❌ | ❌ Missing | Not planned |
| Math object | ✅ | ❌ | ❌ Missing | Not planned |
| Date object | ✅ | ❌ | ❌ Missing | Not planned |
| parseInt() | ✅ | ⚠️ | ⚠️ Different | `int()` function |
| parseFloat() | ✅ | ⚠️ | ⚠️ Different | `float()` function |
| isNaN() | ✅ | ❌ | ❌ Missing | Not planned |
| isFinite() | ✅ | ❌ | ❌ Missing | Not planned |
| eval() | ✅ | ❌ | ❌ Missing | Not planned |

**Compatibility**: 20%

---

## Syntax Comparison

### Variables

```javascript
// JavaScript
var x = 10;
let y = 20;
const z = 30;

// Type annotations (TypeScript)
let name: string = "Alice";
```

```klang
// KLang
var x = 10;              // Block-scoped, mutable
let y = 20;              // Block-scoped, mutable, no redeclaration
const z = 30;            // Block-scoped, immutable

// Type annotations (built-in)
let name: string = "Alice";
let age: int = 25;
let price: float = 19.99;
```

---

### Functions

```javascript
// JavaScript

// Regular function
function greet(name) {
    return "Hello, " + name;
}

// Arrow function - expression body
const double = x => x * 2;

// Arrow function - block body
const add = (a, b) => {
    return a + b;
};

// Arrow function - no params
const getRandom = () => Math.random();

// Arrow function - multiple params
const multiply = (x, y) => x * y;
```

```klang
// KLang

// Regular function
fn greet(name: string) -> string {
    return "Hello, " + name;
}

// Arrow function - expression body
let double = x => x * 2;

// Arrow function - block body
let add = (a, b) => {
    return a + b;
};

// Arrow function - no params
let getRandom = () => 42;

// Arrow function - multiple params
let multiply = (x, y) => x * y;
```

---

### Template Literals

```javascript
// JavaScript
const name = "World";
const greeting = `Hello, ${name}!`;
const multiline = `Line 1
Line 2
Line 3`;
```

```klang
// KLang
let name = "World";
let greeting = `Hello, ${name}!`;
let multiline = `Line 1
Line 2
Line 3`;
```

**Compatibility**: 100%

---

### Objects

```javascript
// JavaScript
const x = 10, y = 20;

// Object literal
const obj = {
    name: "Alice",
    age: 25,
    x,              // Shorthand property
    y,
    [key]: value    // Computed property
};

// Property access
obj.name;
obj["age"];

// Destructuring
const { name, age } = obj;
```

```klang
// KLang
let x = 10;
let y = 20;

// Object literal
let obj = {
    name: "Alice",
    age: 25,
    x,              // Shorthand property ✅
    y,
    [key]: value    // Computed property ✅
};

// Property access
obj.name;
obj["age"];

// Destructuring ❌ NOT YET IMPLEMENTED
```

---

### Arrays

```javascript
// JavaScript
const arr = [1, 2, 3, 4, 5];

// Access
arr[0];          // 1
arr.length;      // 5

// Higher-order functions
arr.map(x => x * 2);
arr.filter(x => x > 2);
arr.reduce((sum, x) => sum + x, 0);

// Destructuring
const [first, second, ...rest] = arr;

// Spread
const arr2 = [...arr, 6, 7];
```

```klang
// KLang
let arr = [1, 2, 3, 4, 5];

// Access
arr[0];          // 1 ✅
arr.length;      // 5 ✅

// Higher-order functions ❌ NOT YET IMPLEMENTED
// arr.map(x => x * 2);
// arr.filter(x => x > 2);
// arr.reduce((sum, x) => sum + x, 0);

// Destructuring ❌ NOT YET IMPLEMENTED
// let [first, second, ...rest] = arr;

// Spread ❌ NOT YET IMPLEMENTED
// let arr2 = [...arr, 6, 7];
```

---

### Classes

```javascript
// JavaScript
class Animal {
    constructor(name) {
        this.name = name;
    }
    
    speak() {
        console.log(`${this.name} makes a sound`);
    }
}

class Dog extends Animal {
    constructor(name, breed) {
        super(name);
        this.breed = breed;
    }
    
    speak() {
        console.log(`${this.name} barks`);
    }
}

// Private fields (ES2022)
class Counter {
    #count = 0;
    
    increment() {
        this.#count++;
    }
}

// Static members
class Math {
    static PI = 3.14159;
    
    static square(x) {
        return x * x;
    }
}
```

```klang
// KLang
class Animal {
    public let name: string;
    
    fn init(name: string) {
        this.name = name;
    }
    
    fn speak() {
        println(`${this.name} makes a sound`);
    }
}

class Dog extends Animal {
    public let breed: string;
    
    fn init(name: string, breed: string) {
        super.init(name);
        this.breed = breed;
    }
    
    fn speak() {
        println(`${this.name} barks`);
    }
}

// Private fields
class Counter {
    private let count: int = 0;
    
    fn increment() {
        this.count = this.count + 1;
    }
}

// Static members
class Math {
    static let PI: float = 3.14159;
    
    static fn square(x: float) -> float {
        return x * x;
    }
}

// Abstract classes (KLang enhancement!)
abstract class Shape {
    abstract fn area() -> float;
    abstract fn perimeter() -> float;
}

class Circle extends Shape {
    private let radius: float;
    
    fn init(radius: float) {
        this.radius = radius;
    }
    
    fn area() -> float {
        return 3.14159 * this.radius * this.radius;
    }
    
    fn perimeter() -> float {
        return 2.0 * 3.14159 * this.radius;
    }
}
```

---

### Control Flow

```javascript
// JavaScript

// if-else
if (x > 0) {
    console.log("positive");
} else if (x < 0) {
    console.log("negative");
} else {
    console.log("zero");
}

// Ternary
const result = x > 0 ? "positive" : "negative";

// while
while (i < 10) {
    i++;
}

// for
for (let i = 0; i < 10; i++) {
    console.log(i);
}

// for-of
for (const item of array) {
    console.log(item);
}

// switch
switch (value) {
    case 1:
        console.log("one");
        break;
    case 2:
        console.log("two");
        break;
    default:
        console.log("other");
}
```

```klang
// KLang

// if-else ✅
if (x > 0) {
    println("positive");
} else if (x < 0) {
    println("negative");
} else {
    println("zero");
}

// Ternary ❌ NOT YET IMPLEMENTED
// let result = x > 0 ? "positive" : "negative";

// while ✅
while (i < 10) {
    i = i + 1;
}

// for ✅
for (i: int = 0; i < 10; i = i + 1) {
    println(i);
}

// for-of ❌ NOT YET IMPLEMENTED
// for (item of array) {
//     println(item);
// }

// switch ❌ NOT YET IMPLEMENTED
```

---

### Error Handling

```javascript
// JavaScript
try {
    throw new Error("Something went wrong");
} catch (error) {
    console.log(error.message);
} finally {
    console.log("Cleanup");
}
```

```klang
// KLang

// ❌ NOT YET IMPLEMENTED
// Error handling planned for Phase 3
```

---

### Async Programming

```javascript
// JavaScript

// Promises
fetch('https://api.example.com/data')
    .then(response => response.json())
    .then(data => console.log(data))
    .catch(error => console.error(error));

// Async/Await
async function fetchData() {
    try {
        const response = await fetch('url');
        const data = await response.json();
        return data;
    } catch (error) {
        console.error(error);
    }
}
```

```klang
// KLang

// ❌ NOT YET IMPLEMENTED
// Async programming planned for Phase 4
```

---

### Destructuring and Spread

```javascript
// JavaScript

// Array destructuring
const [a, b, ...rest] = [1, 2, 3, 4, 5];

// Object destructuring
const { name, age, ...others } = person;

// Spread in arrays
const arr = [1, 2, 3];
const arr2 = [...arr, 4, 5];

// Spread in objects
const obj = { a: 1, b: 2 };
const obj2 = { ...obj, c: 3 };

// Rest parameters
function sum(...numbers) {
    return numbers.reduce((a, b) => a + b, 0);
}
```

```klang
// KLang

// ❌ NOT YET IMPLEMENTED
// Destructuring and spread planned for Phase 2
```

---

### Modules

```javascript
// JavaScript

// Export
export const PI = 3.14159;
export function square(x) {
    return x * x;
}
export default class Calculator {}

// Import
import Calculator from './calculator.js';
import { PI, square } from './math.js';
import * as math from './math.js';
```

```klang
// KLang

// ❌ NOT YET IMPLEMENTED
// Modules planned for Phase 5
```

---

## Feature Completeness

### Overall Progress

```
Total JavaScript Features: ~150
Implemented in KLang: ~60
Feature Completeness: 40%
```

### By Category

| Category | Total Features | Implemented | Percentage | Status |
|----------|---------------|-------------|------------|--------|
| **Variables** | 6 | 5 | 83% | 🟢 Excellent |
| **Primitive Types** | 7 | 4 | 57% | 🟡 Good |
| **Functions** | 18 | 10 | 55% | 🟡 Good |
| **Objects** | 12 | 6 | 50% | 🟡 Good |
| **Arrays** | 25 | 4 | 16% | 🔴 Needs Work |
| **Strings** | 24 | 17 | 71% | 🟢 Excellent |
| **Classes/OOP** | 20 | 15 | 75% | 🟢 Excellent |
| **Control Flow** | 14 | 8 | 57% | 🟡 Good |
| **Operators** | 45 | 14 | 31% | 🔴 Needs Work |
| **Error Handling** | 7 | 0 | 0% | 🔴 Not Started |
| **Async** | 14 | 1 | 7% | 🔴 Not Started |
| **Modules** | 10 | 0 | 0% | 🔴 Not Started |
| **Advanced** | 16 | 0 | 0% | 🔴 Not Started |
| **Built-ins** | 10 | 2 | 20% | 🔴 Needs Work |

### Visual Progress

```
Core Features (Variables, Functions, Control Flow):
████████████████████░░░░░░░░ 65%

Modern ES6+ (Arrow Functions, Classes, Templates):
████████████████░░░░░░░░░░░░ 55%

Advanced Features (Async, Modules, Generators):
███░░░░░░░░░░░░░░░░░░░░░░░░ 8%

Overall:
████████████░░░░░░░░░░░░░░░░ 40%
```

---

## What's Missing

### High-Priority Missing Features

#### 1. **Functional Programming** (Phase 2)

❌ **Spread and Rest Operators**
```javascript
// JavaScript
const arr = [1, 2, 3];
const arr2 = [...arr, 4, 5];
function sum(...numbers) { }
```

❌ **Destructuring**
```javascript
// JavaScript
const [a, b] = [1, 2];
const { name, age } = person;
```

❌ **Array Methods**
```javascript
// JavaScript
arr.map(x => x * 2)
arr.filter(x => x > 0)
arr.reduce((sum, x) => sum + x, 0)
```

---

#### 2. **Advanced Control Flow** (Phase 3)

❌ **Ternary Operator**
```javascript
// JavaScript
const result = condition ? trueValue : falseValue;
```

❌ **Logical Operators**
```javascript
// JavaScript
const result = a && b || c;
if (user && user.isActive) { }
```

❌ **Optional Chaining**
```javascript
// JavaScript
const name = user?.profile?.name;
```

❌ **Nullish Coalescing**
```javascript
// JavaScript
const value = input ?? defaultValue;
```

❌ **Switch-Case**
```javascript
// JavaScript
switch (value) {
    case 'a': return 1;
    case 'b': return 2;
    default: return 0;
}
```

❌ **For-Of Loop**
```javascript
// JavaScript
for (const item of array) {
    console.log(item);
}
```

❌ **Try-Catch-Finally**
```javascript
// JavaScript
try {
    riskyOperation();
} catch (error) {
    handleError(error);
} finally {
    cleanup();
}
```

---

#### 3. **Async Programming** (Phase 4)

❌ **Promises**
```javascript
// JavaScript
const promise = new Promise((resolve, reject) => {
    // async operation
});

promise
    .then(result => console.log(result))
    .catch(error => console.error(error));
```

❌ **Async/Await**
```javascript
// JavaScript
async function fetchData() {
    const result = await someAsyncOperation();
    return result;
}
```

---

#### 4. **Modules** (Phase 5)

❌ **Import/Export**
```javascript
// JavaScript
// math.js
export const PI = 3.14159;
export function square(x) { return x * x; }

// main.js
import { PI, square } from './math.js';
```

---

#### 5. **Advanced Features** (Phase 6)

❌ **Generators**
```javascript
// JavaScript
function* fibonacci() {
    let a = 0, b = 1;
    while (true) {
        yield a;
        [a, b] = [b, a + b];
    }
}
```

❌ **Symbols**
```javascript
// JavaScript
const sym = Symbol('description');
const obj = { [sym]: 'value' };
```

❌ **Proxies**
```javascript
// JavaScript
const handler = {
    get(target, prop) {
        return prop in target ? target[prop] : 'default';
    }
};
const proxy = new Proxy(target, handler);
```

---

### Medium-Priority Missing Features

- ❌ Compound assignment operators (`+=`, `-=`, `*=`, `/=`)
- ❌ Increment/decrement (`++`, `--`)
- ❌ Strict equality (`===`, `!==`)
- ❌ Bitwise operators
- ❌ Do-while loops
- ❌ For-in loops
- ❌ Array mutation methods (push, pop, shift, unshift)
- ❌ Object static methods (Object.keys, Object.values, Object.entries)
- ❌ Map and Set data structures
- ❌ Getters and setters
- ❌ Private field syntax (#field)

---

### Low-Priority / Not Planned

- Regular expressions
- JSON parsing/stringify
- Math object
- Date/Time support
- Multiple inheritance
- TypedArrays and ArrayBuffer
- Reflect API
- Dynamic import()
- CommonJS (require/module.exports)
- eval()
- with statement

---

## Implementation Priority

### Phase 1: ✅ **COMPLETED** 
**Target**: Core JavaScript syntax
- ✅ var, let, const with proper scoping
- ✅ Arrow functions (all syntaxes)
- ✅ Template literals
- ✅ Enhanced object literals
- ✅ Complete OOP (classes, inheritance, static, access modifiers, abstract)

**Status**: 100% complete, 160+ tests passing

---

### Phase 2: 🎯 **NEXT UP** - Functional Programming
**Priority**: HIGH  
**Estimated Effort**: 4-6 weeks

**Features**:
1. Spread operator (`...`)
   - Array spread: `[...arr1, ...arr2]`
   - Object spread: `{ ...obj1, ...obj2 }`
   - Function call spread: `func(...args)`

2. Rest parameters
   - Function parameters: `function sum(...numbers) {}`

3. Destructuring
   - Array destructuring: `const [a, b] = array;`
   - Object destructuring: `const { x, y } = obj;`
   - Nested destructuring
   - Default values in destructuring

4. Default parameters
   - `function greet(name = "World") {}`

5. Array higher-order methods
   - `.map(callback)`
   - `.filter(callback)`
   - `.reduce(callback, initial)`
   - `.forEach(callback)`
   - `.find(callback)`
   - `.some(callback)`
   - `.every(callback)`
   - `.findIndex(callback)`
   - `.concat()`
   - `.slice()`
   - `.sort()`
   - `.flat()`
   - `.flatMap()`

**Impact**: 🔥 **CRITICAL** - Enables modern functional programming patterns

---

### Phase 3: 🎯 **HIGH PRIORITY** - Advanced Control Flow
**Priority**: HIGH  
**Estimated Effort**: 3-4 weeks

**Features**:
1. Ternary operator
   - `condition ? trueValue : falseValue`

2. Logical operators
   - AND: `&&`
   - OR: `||`
   - Short-circuit evaluation

3. Optional chaining
   - `obj?.prop`
   - `obj?.method?.()`
   - `arr?.[index]`

4. Nullish coalescing
   - `value ?? default`

5. Switch-case statement
   - `switch (expr) { case val: ...; break; }`
   - Fall-through behavior
   - Default case

6. For-of loop
   - `for (const item of iterable) {}`

7. For-in loop
   - `for (const key in object) {}`

8. Try-catch-finally
   - Error handling
   - throw statement
   - Error objects

**Impact**: 🔥 **CRITICAL** - Essential control flow patterns used everywhere

---

### Phase 4: 📅 **PLANNED** - Async Programming
**Priority**: MEDIUM  
**Estimated Effort**: 6-8 weeks

**Features**:
1. Promises
   - `new Promise(executor)`
   - `.then()`, `.catch()`, `.finally()`
   - `Promise.all()`, `Promise.race()`

2. Async/Await
   - `async function` declaration
   - `await` keyword
   - Error handling with try-catch

3. Event loop (basic)
   - Task queue
   - Microtask queue

**Impact**: 🔶 **IMPORTANT** - Enables async I/O and modern async patterns

---

### Phase 5: 📅 **PLANNED** - Modules and Advanced OOP
**Priority**: MEDIUM  
**Estimated Effort**: 4-6 weeks

**Features**:
1. ES6 Modules
   - `import` statement
   - `export` statement
   - Default exports
   - Named exports

2. Getters and Setters
   - Property getters
   - Property setters
   - Computed properties

3. Enhanced OOP
   - Interfaces/Protocols
   - Multiple inheritance (mixins)
   - Method overloading

**Impact**: 🔶 **IMPORTANT** - Code organization and reusability

---

### Phase 6: 📅 **FUTURE** - Advanced Features
**Priority**: LOW  
**Estimated Effort**: 8-12 weeks

**Features**:
1. Generators
   - `function*` syntax
   - `yield` keyword
   - Generator objects

2. Iterators
   - Iterator protocol
   - Iterable protocol
   - Custom iterators

3. Symbols
   - `Symbol()` constructor
   - Well-known symbols
   - Symbol.iterator

4. Proxies
   - Proxy constructor
   - Handler traps
   - Reflect API

5. WeakMap and WeakSet
   - Weak references
   - Garbage collection considerations

**Impact**: 🔷 **NICE TO HAVE** - Advanced metaprogramming

---

### Quick Wins (Can be done anytime)

**Operators** (1-2 weeks):
- ✅ Compound assignment: `+=`, `-=`, `*=`, `/=`, `%=`
- ✅ Increment/Decrement: `++`, `--`
- ✅ Strict equality: `===`, `!==`
- ✅ Exponentiation: `**`

**Array Methods** (1 week):
- ✅ `.push()`, `.pop()`
- ✅ `.shift()`, `.unshift()`
- ✅ `.includes()`
- ✅ `.indexOf()`
- ✅ `.reverse()`

**Object Methods** (1 week):
- ✅ `Object.keys()`
- ✅ `Object.values()`
- ✅ `Object.entries()`
- ✅ `Object.assign()`

**Impact**: 🔷 **CONVENIENCE** - Developer experience improvements

---

## Feature Details

### Implemented Features Deep Dive

#### Variables and Scoping

KLang implements ES6-style variables with strict scoping rules:

```klang
// Block scoping
{
    let x = 10;
    var y = 20;
    const z = 30;
}
// x, y, z are all out of scope here

// let prevents redeclaration
let a = 1;
let a = 2;  // Error: Cannot redeclare variable 'a'

// var allows redeclaration
var b = 1;
var b = 2;  // OK

// const requires initialization and prevents reassignment
const c = 10;
c = 20;  // Error: Cannot assign to const variable

// Type annotations
let name: string = "Alice";
let age: int = 25;
let price: float = 19.99;
```

**Differences from JavaScript**:
- `var` is block-scoped in KLang (not function-scoped like JS)
- No hoisting in KLang
- Type annotations are built-in (no TypeScript needed)

---

#### Arrow Functions

Full ES6 arrow function support with all syntaxes:

```klang
// Expression body (implicit return)
let double = x => x * 2;
let add = (a, b) => a + b;

// Block body (explicit return)
let factorial = n => {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
};

// No parameters
let getRandom = () => 42;

// Single parameter (no parentheses needed)
let square = x => x * x;

// Multiple parameters (parentheses required)
let multiply = (x, y) => x * y;

// Arrow functions in arrays
let operations = [
    x => x + 1,
    x => x * 2,
    x => x * x
];

// Arrow functions as callbacks (when HOFs are implemented)
// arr.map(x => x * 2)
// arr.filter(x => x > 0)
```

**Fully Compatible with JavaScript ES6**

---

#### Template Literals

Complete template literal support:

```klang
let name = "Alice";
let age = 25;

// Basic interpolation
let greeting = `Hello, ${name}!`;

// Multiple interpolations
let info = `${name} is ${age} years old`;

// Expressions in interpolations
let message = `Next year, ${name} will be ${age + 1}`;

// Multi-line strings
let poem = `Roses are red,
Violets are blue,
KLang is awesome,
And so are you!`;

// Nested expressions
let complex = `Result: ${x > 0 ? `positive (${x})` : `negative (${-x})`}`;

// Function calls in interpolations
let output = `The result is ${calculate(5, 3)}`;
```

**Fully Compatible with JavaScript ES6**

---

#### Enhanced Object Literals

Modern object literal features:

```klang
let x = 10;
let y = 20;
let key = "dynamic";

let obj = {
    // Regular properties
    name: "Alice",
    age: 25,
    
    // Property shorthand
    x,              // Same as x: x
    y,              // Same as y: y
    
    // Computed properties
    [key]: "value",                    // obj["dynamic"] = "value"
    [key + "2"]: "another",            // obj["dynamic2"] = "another"
    [`computed_${key}`]: "computed"    // obj["computed_dynamic"] = "computed"
};

// Property access
println(obj.name);         // "Alice"
println(obj.x);            // 10
println(obj["dynamic"]);   // "value"
```

**Fully Compatible with JavaScript ES6**

---

#### Classes and OOP

Comprehensive OOP support with enhancements:

```klang
// Basic class
class Person {
    public let name: string;
    public let age: int;
    
    fn init(name: string, age: int) {
        this.name = name;
        this.age = age;
    }
    
    fn greet() {
        println(`Hello, I'm ${this.name}`);
    }
}

// Inheritance
class Employee extends Person {
    private let salary: float;
    
    fn init(name: string, age: int, salary: float) {
        super.init(name, age);
        this.salary = salary;
    }
    
    fn greet() {
        super.greet();
        println(`I work here`);
    }
}

// Access modifiers
class BankAccount {
    private let balance: float = 0.0;
    protected let accountNumber: string;
    public let owner: string;
    
    public fn deposit(amount: float) {
        this.balance = this.balance + amount;
    }
    
    private fn calculateInterest() -> float {
        return this.balance * 0.05;
    }
}

// Static members
class MathUtils {
    static let PI: float = 3.14159;
    static let E: float = 2.71828;
    
    static fn square(x: float) -> float {
        return x * x;
    }
    
    static fn circle_area(radius: float) -> float {
        return MathUtils.PI * radius * radius;
    }
}

// Usage
let area = MathUtils.circle_area(5.0);

// Abstract classes (KLang enhancement!)
abstract class Shape {
    protected let color: string;
    
    fn init(color: string) {
        this.color = color;
    }
    
    // Abstract methods - must be implemented by subclasses
    abstract fn area() -> float;
    abstract fn perimeter() -> float;
    
    // Concrete method
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

// Cannot instantiate abstract class
// let shape = Shape("red");  // Error!

// Can instantiate concrete class
let circle = Circle("red", 5.0);
circle.describe();
println(`Area: ${circle.area()}`);
```

**Beyond JavaScript**: Abstract classes are built-in (JavaScript doesn't have them natively)

---

### Notable Differences from JavaScript

#### 1. Function Syntax

```javascript
// JavaScript
function greet(name) {
    return "Hello, " + name;
}
```

```klang
// KLang
fn greet(name: string) -> string {
    return "Hello, " + name;
}
```

**Why Different**: More explicit, supports type annotations

---

#### 2. Constructor Name

```javascript
// JavaScript
class Person {
    constructor(name) {
        this.name = name;
    }
}
```

```klang
// KLang
class Person {
    fn init(name: string) {
        this.name = name;
    }
}
```

**Why Different**: More consistent with other methods

---

#### 3. No `undefined`

```javascript
// JavaScript
let x;           // x is undefined
let obj = {};
obj.missing;     // undefined
```

```klang
// KLang
let x;           // x is null
let obj = {};
obj.missing;     // null
```

**Why Different**: Simplifies mental model, one null value instead of two

---

#### 4. Type Annotations Built-In

```javascript
// JavaScript - needs TypeScript
let name: string = "Alice";
```

```klang
// KLang - native support
let name: string = "Alice";
```

**Why Different**: Type safety without external tools

---

#### 5. No Automatic Semicolon Insertion

```javascript
// JavaScript - ASI can cause issues
return
    {
        value: 42
    }
// Returns undefined, not the object
```

```klang
// KLang - explicit semicolons recommended
return {
    value: 42
};
// Returns the object as expected
```

**Why Different**: More predictable, fewer surprises

---

## Roadmap Summary

### Completed ✅
- Phase 1: Core JavaScript features (100%)

### In Progress 🚧
- Phase 2: Functional programming (0% - Next)

### Planned 📅
- Phase 3: Advanced control flow
- Phase 4: Async programming
- Phase 5: Modules and advanced OOP
- Phase 6: Advanced features

### Timeline Estimate

| Phase | Duration | Completion Date |
|-------|----------|-----------------|
| Phase 1 | 12 weeks | ✅ Complete |
| Phase 2 | 4-6 weeks | ~6 weeks |
| Phase 3 | 3-4 weeks | ~10 weeks |
| Phase 4 | 6-8 weeks | ~18 weeks |
| Phase 5 | 4-6 weeks | ~24 weeks |
| Phase 6 | 8-12 weeks | ~36 weeks |

**Total Estimated Time to 80% JS Compatibility**: ~9 months

---

## Conclusion

KLang has successfully implemented the core foundation of JavaScript with modern ES6+ features. The language provides:

✅ **Strong Foundation**: Variables, functions, classes, and control flow  
✅ **Modern Syntax**: Arrow functions, template literals, enhanced objects  
✅ **Enhanced OOP**: Access modifiers, static members, abstract classes  
✅ **Type Safety**: Native type annotations  
✅ **Excellent Quality**: 100% test pass rate with 160+ tests

### Next Steps

The highest priority is **Phase 2** (functional programming), which will add:
- Spread/rest operators
- Destructuring
- Array methods (map, filter, reduce, etc.)
- Default parameters

These features are essential for modern JavaScript development patterns and will bring KLang to ~55-60% JavaScript compatibility.

### Long-term Vision

KLang aims to achieve 80%+ compatibility with practical JavaScript features while maintaining:
- Better type safety
- Clearer syntax
- Enhanced OOP capabilities
- Predictable behavior

The focus is on **practical, everyday JavaScript** rather than edge cases and legacy features.

---

## Appendix: Quick Reference

### What Works Now ✅

- Variables: `var`, `let`, `const`
- Functions: regular and arrow functions
- Classes: full OOP with inheritance, static, access modifiers, abstract
- Control: `if/else`, `while`, `for`, `break`, `continue`
- Operators: arithmetic, comparison, unary
- Objects: literals, shorthand, computed properties
- Arrays: literals, index access, `.length`
- Strings: literals, templates, 17+ methods
- Types: `int`, `float`, `string`, `bool`, `null`

### What Doesn't Work Yet ❌

- Spread/rest: `...`
- Destructuring: `[a, b] = array`
- Array methods: `.map()`, `.filter()`, etc.
- Ternary: `? :`
- Logical: `&&`, `||`
- Optional: `?.`, `??`
- Switch-case
- For-of/for-in loops
- Try-catch-finally
- Promises, async/await
- Modules: import/export
- Generators, symbols, proxies

---

**Document Version**: 1.0  
**Last Updated**: 2024  
**Maintained By**: KLang Team

For the latest updates, see:
- [README.md](../README.md)
- [JAVASCRIPT_FEATURES_ROADMAP.md](roadmaps/JAVASCRIPT_FEATURES_ROADMAP.md)
- [Implementation Notes](features/IMPLEMENTATION_NOTES.md)
