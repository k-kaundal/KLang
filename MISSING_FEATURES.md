# Missing Advanced Language Features - Implementation Plan

**Status**: Ready for Implementation  
**Priority**: High  
**Based on**: Feature validation results (77% pass rate)

---

## 🔴 Critical Missing Features

### 1. Logical Operators (&&, ||)
**Status**: NOT IMPLEMENTED  
**Priority**: CRITICAL  
**Impact**: Essential for conditional logic

**Current State**:
- No TOKEN_AND_AND or TOKEN_OR_OR in lexer
- No logical AND/OR parsing
- No logical operators in interpreter

**Implementation Required**:
```c
// In lexer.h:
TOKEN_AND_AND,    // &&
TOKEN_OR_OR,      // ||

// In lexer.c:
if (c == '&' && lexer->source[lexer->pos + 1] == '&') {
    lexer->pos += 2;
    return make_token(TOKEN_AND_AND, "&&", line, col);
}
if (c == '|' && lexer->source[lexer->pos + 1] == '|') {
    lexer->pos += 2;
    return make_token(TOKEN_OR_OR, "||", line, col);
}

// In parser.c:
// Add logical_and_expr() and logical_or_expr() parsing functions

// In interpreter.c:
// Implement short-circuit evaluation for && and ||
```

**Test Cases**:
```javascript
// Should work after implementation
let x = 10
let y = 20
if (x > 5 && y > 15) {
    print("Both conditions true")
}

if (x < 0 || y > 15) {
    print("At least one condition true")
}
```

---

### 2. Compound Assignment Operators (+=, -=, *=, /=, %=)
**Status**: NOT IMPLEMENTED  
**Priority**: HIGH  
**Impact**: Common operations, code readability

**Current State**:
- No compound assignment tokens
- Must use verbose: `x = x + 1`
- Affects code style and convenience

**Implementation Required**:
```c
// In lexer.h:
TOKEN_PLUS_ASSIGN,      // +=
TOKEN_MINUS_ASSIGN,     // -=
TOKEN_STAR_ASSIGN,      // *=
TOKEN_SLASH_ASSIGN,     // /=
TOKEN_PERCENT_ASSIGN,   // %=

// In lexer.c:
if (c == '+' && lexer->source[lexer->pos + 1] == '=') {
    lexer->pos += 2;
    return make_token(TOKEN_PLUS_ASSIGN, "+=", line, col);
}
// Similar for -=, *=, /=, %=

// In parser.c:
// Handle compound assignments in assignment_expr()

// In interpreter.c:
// Transform `x += 5` to `x = x + 5` and evaluate
```

**Test Cases**:
```javascript
let x = 10
x += 5      // x becomes 15
x -= 3      // x becomes 12
x *= 2      // x becomes 24
x /= 4      // x becomes 6
x %= 4      // x becomes 2
```

---

### 3. Increment/Decrement Operators (++, --)
**Status**: NOT IMPLEMENTED  
**Priority**: HIGH  
**Impact**: Loop counters, common operations

**Current State**:
- No increment/decrement operators
- Must use `i = i + 1` or `i += 1`
- Affects for loop syntax

**Implementation Required**:
```c
// In lexer.h:
TOKEN_PLUS_PLUS,     // ++
TOKEN_MINUS_MINUS,   // --

// In lexer.c:
if (c == '+' && lexer->source[lexer->pos + 1] == '+') {
    lexer->pos += 2;
    return make_token(TOKEN_PLUS_PLUS, "++", line, col);
}
if (c == '-' && lexer->source[lexer->pos + 1] == '-') {
    lexer->pos += 2;
    return make_token(TOKEN_MINUS_MINUS, "--", line, col);
}

// In parser.c:
// Handle prefix and postfix increment/decrement
// i++, ++i, i--, --i

// In interpreter.c:
// Implement prefix: ++i returns i+1, side effect: i=i+1
// Implement postfix: i++ returns i, side effect: i=i+1
```

**Test Cases**:
```javascript
let i = 0
i++         // i becomes 1
++i         // i becomes 2
let x = i++ // x gets 2, i becomes 3
let y = ++i // y gets 4, i becomes 4
```

---

### 4. Optional Chaining (?.)
**Status**: NOT IMPLEMENTED  
**Priority**: MEDIUM  
**Impact**: Safe property access

**Current State**:
- No optional chaining support
- Must manually check for null/undefined
- Verbose null-checking code

**Implementation Required**:
```c
// In lexer.h:
TOKEN_OPTIONAL_CHAIN,  // ?.

// In lexer.c:
if (c == '?' && lexer->source[lexer->pos + 1] == '.') {
    lexer->pos += 2;
    return make_token(TOKEN_OPTIONAL_CHAIN, "?.", line, col);
}

// In parser.c:
// Parse optional_chain_expr() for property access

// In interpreter.c:
// If left side is null/undefined, return null
// Otherwise, access property normally
```

**Test Cases**:
```javascript
let obj = null
print(obj?.name)  // prints null, doesn't crash

let user = {name: "Alice"}
print(user?.name)  // prints "Alice"
```

---

### 5. Nullish Coalescing (??)
**Status**: NOT IMPLEMENTED  
**Priority**: MEDIUM  
**Impact**: Default value handling

**Current State**:
- No nullish coalescing support
- Must use ternary: `value !== null ? value : default`
- Verbose null/undefined checks

**Implementation Required**:
```c
// In lexer.h:
TOKEN_NULLISH_COALESCE,  // ??

// In lexer.c:
if (c == '?' && lexer->source[lexer->pos + 1] == '?') {
    lexer->pos += 2;
    return make_token(TOKEN_NULLISH_COALESCE, "??", line, col);
}

// In parser.c:
// Parse nullish_coalescing_expr()

// In interpreter.c:
// If left side is null/undefined, return right side
// Otherwise, return left side
```

**Test Cases**:
```javascript
let x = null
let y = x ?? "default"  // y gets "default"

let z = 0
let w = z ?? "default"  // w gets 0 (not "default", because 0 is not null)
```

---

## 🟡 High Priority Parser Fixes

### 6. C-style For Loops
**Status**: PARSING ERROR  
**Priority**: CRITICAL  
**Impact**: Basic loop syntax broken

**Current State**:
- `for (let i = 0; i < 10; i = i + 1)` fails to parse
- Error: "expected IDENT, got LPAREN"
- For-of loops work, but C-style for loops don't

**Fix Required**:
```c
// In parser.c: parse_for_stmt()
// Current code expects: for IDENT in EXPR
// Need to add: for (INIT; COND; UPDATE) BLOCK

// Pseudo-code:
if (peek is LPAREN) {
    // C-style for loop
    consume(TOKEN_LPAREN);
    parse init statement (let i = 0 or i = 0)
    consume(TOKEN_SEMICOLON);
    parse condition (i < 10)
    consume(TOKEN_SEMICOLON);
    parse update (i = i + 1 or i++)
    consume(TOKEN_RPAREN);
    parse body
} else {
    // For-of or for-in loop
    // existing code
}
```

---

### 7. Default Parameters
**Status**: PARSING ERROR  
**Priority**: HIGH  
**Impact**: Function flexibility

**Current State**:
- `fn greet(name = "World")` fails to parse
- Error: "expected RPAREN, got ASSIGN"
- Default parameters not supported

**Fix Required**:
```c
// In parser.c: parse_function()
// When parsing parameters:
while (peek is TOKEN_IDENT) {
    param_name = consume(TOKEN_IDENT);
    
    if (peek is TOKEN_ASSIGN) {
        consume(TOKEN_ASSIGN);
        default_value = parse_expression();
        // Store param with default value
    }
    
    if (peek is TOKEN_COMMA) {
        consume(TOKEN_COMMA);
    }
}
```

---

### 8. Rest Parameters
**Status**: PARSING ERROR  
**Priority**: HIGH  
**Impact**: Variable argument functions

**Current State**:
- `fn sum(...numbers)` fails to parse
- Error: "expected IDENT, got LPAREN"
- Rest parameters not recognized

**Fix Required**:
```c
// In parser.c: parse_function()
// When parsing parameters:
if (peek is TOKEN_SPREAD) {
    consume(TOKEN_SPREAD);
    rest_param = consume(TOKEN_IDENT);
    // Mark as rest parameter
    // Must be last parameter
}
```

---

### 9. Nested Function Declarations
**Status**: PARSING ERROR  
**Priority**: MEDIUM  
**Impact**: Closures, scoping

**Current State**:
- Nested functions fail to parse
- Closures don't work properly
- Can't define function inside function

**Fix Required**:
```c
// In parser.c:
// Allow FN keyword inside function body
// Parse nested function as statement
// Store in local scope, not global
```

---

### 10. Functions as Parameters
**Status**: PARSING ERROR  
**Priority**: MEDIUM  
**Impact**: Higher-order functions

**Current State**:
- Can't pass function as parameter properly
- `fn apply(fn, x)` causes parse errors
- Higher-order functions broken

**Fix Required**:
```c
// In parser.c:
// When parsing function call arguments:
// Allow IDENT that refers to function
// Allow arrow functions as arguments
// Allow function expressions
```

---

## 🟢 Medium Priority Array Methods

### 11. Array Methods (map, filter, reduce, forEach, etc.)
**Status**: NOT IMPLEMENTED  
**Priority**: MEDIUM  
**Impact**: Functional programming style

**Current State**:
- Only basic array operations work
- No array iteration methods
- Must use manual loops

**Implementation Required**:
```c
// In runtime.c or stdlib:
// Implement as built-in methods on Array prototype

array.map(fn)        // Transform each element
array.filter(fn)     // Filter elements
array.reduce(fn, init) // Reduce to single value
array.forEach(fn)    // Iterate with side effects
array.find(fn)       // Find first matching element
array.findIndex(fn)  // Find index of first match
array.some(fn)       // Test if any element matches
array.every(fn)      // Test if all elements match
array.includes(value) // Check if value exists
array.indexOf(value)  // Get index of value
array.slice(start, end) // Get subarray
array.splice(start, count, ...items) // Modify array
array.concat(...arrays) // Merge arrays
array.join(separator) // Join to string
array.reverse()      // Reverse in place
array.sort(fn)       // Sort in place
```

---

## 🔵 Advanced Features (Lower Priority)

### 12. Generators (fn*, yield)
**Status**: TOKEN EXISTS, NOT IMPLEMENTED  
**Priority**: LOW  
**Impact**: Advanced iteration

**Current State**:
- TOKEN_YIELD exists
- No generator parsing
- No generator execution

---

### 13. Async/Await
**Status**: TOKEN EXISTS, NOT IMPLEMENTED  
**Priority**: LOW  
**Impact**: Asynchronous programming

**Current State**:
- TOKEN_ASYNC, TOKEN_AWAIT exist
- No async function parsing
- No promise infrastructure

---

### 14. Modules (import/export)
**Status**: TOKEN EXISTS, NOT IMPLEMENTED  
**Priority**: LOW  
**Impact**: Code organization

**Current State**:
- TOKEN_IMPORT, TOKEN_EXPORT exist
- No module system implemented
- No module resolution

---

## 📊 Implementation Priority

### Phase 1: Critical Operators (Week 1)
1. ✅ Logical operators (&&, ||)
2. ✅ Compound assignment (+=, -=, *=, /=, %=)
3. ✅ Increment/decrement (++, --)

### Phase 2: Parser Fixes (Week 2)
4. ✅ C-style for loops
5. ✅ Default parameters
6. ✅ Rest parameters
7. ✅ Nested functions
8. ✅ Functions as parameters

### Phase 3: Advanced Operators (Week 3)
9. ✅ Optional chaining (?.)
10. ✅ Nullish coalescing (??)

### Phase 4: Array Methods (Week 4)
11. ✅ Core array methods (map, filter, reduce, forEach)
12. ✅ Additional array methods (find, includes, etc.)

### Phase 5: Advanced Features (Future)
13. ⬜ Generators and yield
14. ⬜ Async/await and Promises
15. ⬜ Module system

---

## 🎯 Success Metrics

**Current**: 77% feature pass rate (31/40 tests)

**Target After Phase 1**: 85% (34/40 tests)
- Add 3 critical operators

**Target After Phase 2**: 95% (38/40 tests)
- Fix 7 parser issues

**Target After Phase 3**: 97% (39/40 tests)
- Add 2 advanced operators

**Target After Phase 4**: 100% basic features
- Complete array method suite

---

## 📝 Notes

- All token types for async, generators, modules already exist
- Main work is in parser and interpreter
- Some features need runtime/stdlib support
- Testing infrastructure in place (validate_all_features.sh)

**Last Updated**: March 22, 2026  
**Status**: Ready for Phase 1 Implementation
