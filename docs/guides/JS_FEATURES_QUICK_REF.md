# JavaScript Features Quick Reference

This document provides a **quick lookup table** for all JavaScript features planned for KLang, organized by priority and complexity.

---

## Priority Legend
- 🔴 **HIGH** - Essential for JavaScript compatibility
- 🟡 **MEDIUM** - Important but can be deferred
- 🟢 **LOW** - Nice-to-have, advanced features

---

## Feature Matrix

| Feature | Priority | Complexity | Phase | Agent | Est. Days | Dependencies |
|---------|----------|------------|-------|-------|-----------|--------------|
| **var/let/const** | 🔴 HIGH | Low | 1 | compiler | 2-3 | None |
| **Arrow Functions** | 🔴 HIGH | Low-Med | 1 | compiler | 3-4 | Variables |
| **Template Literals** | 🟡 MEDIUM | Medium | 1 | compiler | 3-4 | Expressions |
| **Object Literals** | 🔴 HIGH | Medium | 1 | compiler | 4-5 | Parser |
| **Spread/Rest** | 🔴 HIGH | Medium | 2 | compiler | 4-5 | Arrays |
| **Destructuring** | 🟡 MEDIUM | High | 2 | compiler | 5-6 | Objects, Arrays |
| **Default Parameters** | 🟡 MEDIUM | Low-Med | 2 | compiler | 2-3 | Functions |
| **Array Methods** | 🔴 HIGH | Low | 2 | stdlib | 3-4 | Callbacks |
| **For-of Loops** | 🟡 MEDIUM | Medium | 3 | compiler | 3-4 | Arrays |
| **Switch Enhanced** | 🟢 LOW | Low | 3 | compiler | 2-3 | Expressions |
| **Try-Catch-Finally** | 🟡 MEDIUM | High | 3 | compiler | 5-6 | Error objects |
| **Ternary/Nullish** | 🟢 LOW | Low | 3 | compiler | 3-4 | Expressions |
| **Callbacks** | 🔴 HIGH | Medium | 4 | runtime | 4-5 | Functions |
| **Promises** | 🔴 HIGH | High | 4 | runtime | 7-8 | Callbacks |
| **Async/Await** | 🔴 HIGH | Very High | 4 | compiler+runtime | 8-10 | Promises |
| **ES6 Modules** | 🟡 MEDIUM | Very High | 5 | compiler | 8-10 | Multi-file |
| **Enhanced Classes** | 🟡 MEDIUM | Medium | 5 | compiler | 5-6 | OOP |
| **Generators** | 🟢 LOW | Very High | 6 | compiler | 8-10 | Iterators |
| **Symbols** | 🟢 LOW | Medium | 6 | compiler | 3-4 | Objects |
| **Proxy/Reflect** | 🟢 LOW | Very High | 6 | compiler | 8-10 | Objects |
| **WeakMap/WeakSet** | 🟢 LOW | High | 6 | gc | 6-8 | GC |
| **Regular Expressions** | 🟢 LOW | Very High | 6 | compiler | 10-12 | External lib |

---

## Implementation Complexity Scale

### Low (2-4 days)
- Simple syntax additions
- Straightforward evaluation
- Minimal architectural changes
- Examples: var/let/const, default parameters, ternary operator

### Medium (4-6 days)
- Moderate parser changes
- New value types
- Some architectural adjustments
- Examples: arrow functions, object literals, spread/rest

### High (5-8 days)
- Complex parser logic
- Multiple interacting systems
- Significant new infrastructure
- Examples: destructuring, try-catch, promises

### Very High (8-12+ days)
- Major architectural changes
- Cross-cutting concerns
- Multiple phases of work
- Examples: async/await, modules, generators, regex

---

## Features by Use Case

### Daily JavaScript (Phase 1-2)
These cover **80% of typical JavaScript usage**:
- ✅ var/let/const
- ✅ Arrow functions
- ✅ Template literals
- ✅ Object literals
- ✅ Spread/rest operators
- ✅ Destructuring
- ✅ Array methods (map, filter, reduce)

### Modern JavaScript (Phase 3-4)
Essential for modern codebases:
- ✅ For-of loops
- ✅ Try-catch-finally
- ✅ Promises
- ✅ Async/await
- ✅ Nullish coalescing (??)
- ✅ Optional chaining (?.)

### Professional JavaScript (Phase 5)
For production applications:
- ✅ ES6 modules
- ✅ Private class fields
- ✅ Getters/setters
- ✅ Enhanced error handling

### Advanced JavaScript (Phase 6)
Cutting-edge features:
- ✅ Generators
- ✅ Symbols
- ✅ Proxy/Reflect
- ✅ WeakMap/WeakSet
- ✅ Regular expressions

---

## Quick Win Features (Implement First)

### Week 1 Targets
1. **let/const** - 2 days
2. **Arrow functions (basic)** - 2 days
3. **Template literals (basic)** - 2 days

**Impact**: Immediate developer productivity boost

### Week 2 Targets
1. **Object literals** - 3 days
2. **Spread in arrays** - 2 days

**Impact**: Modern data structure handling

### Week 3 Targets
1. **Array.map()** - 1 day
2. **Array.filter()** - 1 day
3. **Array.reduce()** - 1 day

**Impact**: Functional programming patterns

---

## Syntax Examples by Feature

### Variables
```javascript
var x = 10;           // Function-scoped
let y = 20;           // Block-scoped
const Z = 30;         // Immutable binding
```

### Arrow Functions
```javascript
let double = x => x * 2;
let add = (a, b) => a + b;
let greet = () => "Hello!";
```

### Template Literals
```javascript
let name = "World";
let greeting = `Hello, ${name}!`;
let multi = `Line 1
Line 2`;
```

### Object Literals
```javascript
let point = { x: 10, y: 20 };
let user = { name, age };  // Shorthand
let obj = {
    method() { return this.value; }
};
```

### Spread/Rest
```javascript
let arr = [1, 2, 3];
let newArr = [...arr, 4, 5];
function sum(...nums) { }
```

### Destructuring
```javascript
let [a, b] = [1, 2];
let {x, y} = point;
let {name: userName} = user;
```

### For-of
```javascript
for (let item of items) {
    println(item);
}
```

### Try-Catch
```javascript
try {
    throw new Error("Failed");
} catch (e) {
    println(e.message);
} finally {
    cleanup();
}
```

### Promises
```javascript
let promise = new Promise((resolve, reject) => {
    resolve("Done!");
});

promise.then(result => println(result));
```

### Async/Await
```javascript
async function fetchData() {
    let result = await fetch("api/data");
    return result;
}
```

### Modules
```javascript
// math.kl
export function add(a, b) { return a + b; }

// main.kl
import { add } from './math.kl';
```

### Generators
```javascript
function* counter() {
    let i = 0;
    while (true) yield i++;
}
```

---

## Files to Modify by Feature

### Lexer Changes (lexer.c/h)
- var/let/const keywords
- Arrow operator (=>)
- Template literal parsing (backticks)
- Spread operator (...)
- async/await keywords
- import/export keywords
- function* syntax

### Parser Changes (parser.c)
- Variable declaration parsing
- Arrow function expressions
- Object literal expressions
- Spread in arrays/calls
- Destructuring patterns
- For-of statements
- Try-catch-finally blocks
- Async function declarations
- Module import/export statements
- Generator function declarations

### AST Changes (ast.h)
- NODE_ARROW_FUNC
- NODE_TEMPLATE_LITERAL
- NODE_OBJECT_LITERAL
- NODE_SPREAD
- NODE_DESTRUCTURE
- NODE_FOR_OF
- NODE_TRY_CATCH
- NODE_ASYNC_FUNC
- NODE_AWAIT
- NODE_MODULE_IMPORT
- NODE_MODULE_EXPORT
- NODE_GENERATOR

### Interpreter Changes (interpreter.c)
- All evaluation cases for new node types
- Async scheduler/event loop
- Module registry
- Generator state machine
- Error propagation

### Value System Changes (interpreter.h)
- VAL_OBJECT_LITERAL
- VAL_PROMISE
- VAL_SYMBOL
- VAL_GENERATOR
- VAL_MODULE

### Runtime Changes (runtime.c)
- setTimeout/setInterval
- Promise constructor
- Array methods (map, filter, reduce, etc.)
- Object methods (keys, values, entries)
- JSON.parse/stringify
- Symbol functions

---

## Testing Strategy

### Unit Tests (Per Feature)
```javascript
// Test arrow functions
test("arrow_basic", () => {
    let double = x => x * 2;
    assert(double(5) == 10);
});

test("arrow_lexical_this", () => {
    class C {
        init() { this.x = 5; }
        method() {
            let f = () => this.x;
            return f();
        }
    }
    let c = new C();
    assert(c.method() == 5);
});
```

### Integration Tests
```javascript
// Test multiple features together
test("destructuring_with_spread", () => {
    let arr = [1, 2, 3, 4, 5];
    let [first, ...rest] = arr;
    assert(first == 1);
    assert(rest.length == 4);
});
```

### Regression Tests
```javascript
// Ensure old features still work
test("existing_functions", () => {
    function add(a, b) { return a + b; }
    assert(add(2, 3) == 5);
});
```

---

## Performance Benchmarks

### Target Metrics
- **Parsing**: < 100ms for 1000 LOC
- **Execution**: Within 2x of interpreted Python
- **Memory**: < 50MB for typical programs
- **GC pause**: < 10ms per collection

### Benchmark Suite
```javascript
// Array methods performance
benchmark("map_1000_elements", () => {
    let arr = range(1, 1000);
    let result = arr.map(x => x * 2);
});

// Promise chaining
benchmark("promise_chain_100", () => {
    let p = Promise.resolve(0);
    for (let i = 0; i < 100; i++) {
        p = p.then(x => x + 1);
    }
    return p;
});
```

---

## Common Patterns & Idioms

### Pattern: Functional Array Processing
```javascript
let numbers = [1, 2, 3, 4, 5];
let result = numbers
    .filter(x => x % 2 == 0)
    .map(x => x * 2)
    .reduce((acc, x) => acc + x, 0);
```

### Pattern: Async Error Handling
```javascript
async function safeOperation() {
    try {
        let data = await fetchData();
        return processData(data);
    } catch (error) {
        console.error("Operation failed:", error);
        return null;
    }
}
```

### Pattern: Module Organization
```javascript
// utils.kl
export const VERSION = "1.0.0";
export function helper() { ... }

// main.kl
import { VERSION, helper } from './utils.kl';
```

### Pattern: Class with Private State
```javascript
class Counter {
    #count = 0;
    
    increment() { this.#count++; }
    get value() { return this.#count; }
}
```

---

## Migration Guide

### From Current KLang to JavaScript-style

#### Before (Current KLang)
```javascript
function double(x) {
    return x * 2;
}

let numbers = [1, 2, 3];
let doubled = [];
for (let i = 0; i < numbers.length; i++) {
    doubled.push(double(numbers[i]));
}
```

#### After (JavaScript-style)
```javascript
const double = x => x * 2;
const numbers = [1, 2, 3];
const doubled = numbers.map(double);
```

---

## FAQ

### Q: Which features should I implement first?
**A**: Start with Phase 1 (var/let/const, arrow functions, template literals, object literals). These provide the biggest bang for the buck.

### Q: Can I skip phases?
**A**: Not recommended. Each phase builds on previous ones. However, within a phase, you can reorder features.

### Q: How do I handle breaking changes?
**A**: Use feature flags during development. Provide migration scripts. Maintain backward compatibility where possible.

### Q: What if a feature is too complex?
**A**: Break it down further. Implement a minimal version first, then enhance. For example, start with basic destructuring before nested patterns.

### Q: How do I test async features?
**A**: Use a test harness with async support. Implement `waitFor()` utility. Run tests in an event loop.

---

## Resources

### JavaScript Specifications
- ES6 (ECMAScript 2015) - Major modern features
- ES7-ES13 (2016-2022) - Incremental improvements
- MDN Web Docs - Best practical reference

### Implementation References
- V8 (Chrome's JavaScript engine) - Complex but reference quality
- QuickJS - Lightweight, readable implementation
- Duktape - Embeddable, well-documented

### Testing Resources
- Test262 - Official ECMAScript test suite
- JavaScript test patterns
- Async testing guides

---

## Next Steps

1. **Review this document** with the team
2. **Select starting features** (recommend Phase 1)
3. **Set up project tracking** (GitHub issues/milestones)
4. **Assign agents** to features
5. **Begin implementation** with `compiler-agent`
6. **Write tests first** (TDD approach)
7. **Iterate and adapt** based on learnings

---

**Last Updated**: 2024  
**Version**: 1.0  
**Companion to**: JAVASCRIPT_FEATURES_ROADMAP.md
