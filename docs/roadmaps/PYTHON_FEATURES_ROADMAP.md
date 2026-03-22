# Python Features Roadmap for KLang

**Goal**: Add Python's most valuable features to KLang while maintaining static typing and performance.

**Current State**: KLang has 68% JavaScript compatibility with OOP, async/await, generators, and modules.

---

## 🎯 Quick Reference: Top 10 Python Features to Add

| # | Feature | Value | Effort | Phase |
|---|---------|-------|--------|-------|
| 1 | **Tuple type** | Multi-return values | 2w | 1 |
| 2 | **Dictionary type (true hash maps)** | Essential data structure | 2w | 1 |
| 3 | **List comprehensions** | Most Pythonic syntax | 2w | 2 |
| 4 | **range() function** | Memory-efficient loops | 1w | 2 |
| 5 | **File I/O module** | Basic requirement | 2w | 4 |
| 6 | **JSON module** | Config/API support | 3w | 4 |
| 7 | **enumerate() function** | Common pattern | 1w | 2 |
| 8 | **Slice notation** | Pythonic syntax | 2w | 3 |
| 9 | **Set type** | Algorithms/uniqueness | 1.5w | 1 |
| 10 | **None/nil type** | Null safety | 1w | 1 |

---

## 📋 Phase-by-Phase Implementation Plan

### Phase 1: Essential Data Structures (4-6 weeks)

#### 1.1 Tuple Type ⭐⭐⭐ HIGH PRIORITY
**Effort**: 2 weeks

**Syntax**:
```python
# Declaration
let coords = (10, 20, 30)
let point: (int, int) = (5, 10)

# Indexing
let x = coords[0]  # 10

# Unpacking (already have destructuring)
let (x, y, z) = coords

# Multiple return values
fn getPosition() -> (int, int) {
    return (100, 200)
}
let (x, y) = getPosition()
```

**Implementation Tasks**:
- [ ] Add `TOKEN_LPAREN` tuple literal handling in parser
- [ ] Create `TupleType` and `TupleValue` in AST
- [ ] Add tuple type inference: `(int, float, string)`
- [ ] Implement tuple indexing (immutable)
- [ ] Add tuple unpacking (leverage existing destructuring)
- [ ] Runtime support for tuple creation/access
- [ ] Tests: creation, indexing, unpacking, multi-return
- [ ] Examples: coordinates, multi-return functions

**Files to Modify**:
- `src/lexer.c` - No changes (parens exist)
- `src/parser.c` - Distinguish tuple from grouping
- `src/ast.h` - Add `ASTNodeType_Tuple`, `TupleType`
- `src/interpreter.c` - Tuple evaluation, indexing
- `src/runtime.c` - Tuple creation helpers

---

#### 1.2 Dictionary Type (True Hash Maps) ⭐⭐⭐ HIGH PRIORITY
**Effort**: 2 weeks

**Syntax**:
```python
# Creation
let scores = {"Alice": 95, "Bob": 87, "Charlie": 92}
let ages: {string: int} = {"Alice": 30, "Bob": 25}

# Access
let aliceScore = scores["Alice"]  # 95
scores["David"] = 88

# Methods
scores.set("Eve", 91)
let hasAlice = scores.has("Alice")  # true
let keys = scores.keys()  # ["Alice", "Bob", "Charlie", "David"]
let values = scores.values()  # [95, 87, 92, 88]

# Iteration
for (key, value of scores.entries()) {
    print(`${key}: ${value}`)
}

# Get with default
let score = scores.get("Unknown", 0)  # Returns 0 if not found
```

**Implementation Tasks**:
- [ ] Add `TOKEN_LBRACE` dict literal handling (distinguish from objects)
- [ ] Create `DictType` and `DictValue` in AST
- [ ] Implement hash map data structure (C)
- [ ] Add dict type inference: `{KeyType: ValueType}`
- [ ] Implement dict subscript access `dict[key]`
- [ ] Built-in methods: `get()`, `set()`, `has()`, `delete()`, `size()`
- [ ] Built-in methods: `keys()`, `values()`, `entries()`
- [ ] Support for-of iteration with entries
- [ ] Hash functions for int, string, bool
- [ ] Tests: CRUD operations, iteration, type inference
- [ ] Examples: word count, caching, config

**Files to Modify**:
- `src/parser.c` - Dict literal vs object literal
- `src/ast.h` - Add `ASTNodeType_Dict`, `DictType`
- `src/interpreter.c` - Dict operations
- `src/runtime.c` - Hash map implementation, dict methods
- `include/runtime.h` - Dict API

**Design Decision**: 
- Use `{}` syntax with type inference: `{"a": 1}` is dict, `{a: 1}` could be object or dict
- Or use `new Dict([["a", 1], ["b", 2]])` constructor
- **Recommended**: Allow both, prefer literal syntax

---

#### 1.3 Set Type ⭐⭐⭐ MEDIUM PRIORITY
**Effort**: 1.5 weeks

**Syntax**:
```python
# Creation
let numbers = new Set([1, 2, 3, 4])
let unique: Set<int> = new Set()

# Methods
numbers.add(5)
let hasThree = numbers.has(3)  # true
numbers.delete(2)
let size = numbers.size()  # 4

# Set operations
let a = new Set([1, 2, 3])
let b = new Set([3, 4, 5])
let union = a.union(b)  # {1, 2, 3, 4, 5}
let intersection = a.intersection(b)  # {3}
let difference = a.difference(b)  # {1, 2}

# Iteration
for (item of numbers) {
    print(item)
}

# From array (remove duplicates)
let items = [1, 2, 2, 3, 3, 4]
let uniqueItems = new Set(items)  # {1, 2, 3, 4}
```

**Implementation Tasks**:
- [ ] Create `SetType` and `SetValue` in AST
- [ ] Implement set data structure (hash set in C)
- [ ] Constructor: `new Set()`, `new Set(array)`
- [ ] Methods: `add()`, `has()`, `delete()`, `size()`, `clear()`
- [ ] Set operations: `union()`, `intersection()`, `difference()`, `isSubset()`
- [ ] Support for-of iteration
- [ ] Tests: basic ops, set operations, iteration
- [ ] Examples: unique values, set algebra

**Files to Modify**:
- `src/ast.h` - Add `ASTNodeType_Set`, `SetType`
- `src/interpreter.c` - Set operations
- `src/runtime.c` - Hash set implementation
- `include/runtime.h` - Set API

---

#### 1.4 None/Nil Type ⭐⭐ HIGH PRIORITY
**Effort**: 1 week

**Syntax**:
```python
# Explicit null
let result: int? = None
let value: string? = nil

# Optional types
fn findUser(id: int) -> User? {
    if (id < 0) {
        return None
    }
    return new User(id)
}

# Null checking
let user = findUser(5)
if (user != None) {
    print(user.name)
}

# Null coalescing (future)
let name = user?.name ?? "Unknown"
```

**Implementation Tasks**:
- [ ] Add `TOKEN_NONE` or `TOKEN_NIL` keyword
- [ ] Create `NoneType` in type system
- [ ] Add optional type syntax: `Type?` or `Option<Type>`
- [ ] Null safety checks in type checker
- [ ] Runtime representation of None
- [ ] Tests: none assignment, optional returns, null checks
- [ ] Examples: optional values, error handling

**Files to Modify**:
- `src/lexer.c` - Add `None`/`nil` keyword
- `src/parser.c` - Parse optional types `Type?`
- `src/ast.h` - Add `TYPE_NONE`, optional type flag
- `src/interpreter.c` - None value handling
- `src/runtime.c` - None singleton

**Design Decision**: 
- Use `None` (Python-style) or `nil` (Go-style)?
- **Recommended**: Support both, prefer `None`

---

### Phase 2: Pythonic Syntax Sugar (3-4 weeks)

#### 2.1 List Comprehensions ⭐⭐⭐ HIGH PRIORITY
**Effort**: 2 weeks

**Syntax**:
```python
# Basic comprehension
let squares = [x * x for x in range(10)]

# With filter
let evens = [x for x in range(20) if x % 2 == 0]

# Multiple conditions
let filtered = [x for x in range(100) if x % 2 == 0 if x % 3 == 0]

# Nested comprehensions
let matrix = [[i * j for j in range(5)] for i in range(5)]

# Transform
let names = [user.name for user in users]
```

**Compiles To**:
```javascript
// [x * x for x in range(10)]
range(10).map(x => x * x)

// [x for x in range(20) if x % 2 == 0]
range(20).filter(x => x % 2 == 0)

// Combined
range(100).filter(x => x % 2 == 0).filter(x => x % 3 == 0)
```

**Implementation Tasks**:
- [ ] Extend parser to recognize `[expr for var in iterable]`
- [ ] Add optional `if` clause: `[expr for var in iterable if condition]`
- [ ] Support nested comprehensions
- [ ] Compile to chain of `.map()` and `.filter()` calls
- [ ] Type inference from source iterable
- [ ] Tests: basic, filtered, nested, type inference
- [ ] Examples: data transformation, filtering

**Files to Modify**:
- `src/parser.c` - Parse list comprehension syntax
- `src/ast.h` - Add `ASTNodeType_ListComprehension`
- `src/interpreter.c` - Desugar to map/filter or custom eval
- Optional: Direct implementation without desugaring

---

#### 2.2 Dictionary Comprehensions ⭐⭐ MEDIUM PRIORITY
**Effort**: 1 week (after list comprehensions)

**Syntax**:
```python
# Basic dict comprehension
let squares = {x: x*x for x in range(5)}
# Result: {0: 0, 1: 1, 2: 4, 3: 9, 4: 16}

# From pairs
let users = [("Alice", 30), ("Bob", 25)]
let ages = {name: age for (name, age) in users}

# With filter
let filtered = {x: x*2 for x in range(10) if x % 2 == 0}
```

**Implementation Tasks**:
- [ ] Extend parser: `{key_expr: val_expr for var in iterable}`
- [ ] Compile to dict builder loop
- [ ] Support optional `if` clause
- [ ] Type inference: `{KeyType: ValueType}`
- [ ] Tests: basic, filtered, from pairs
- [ ] Examples: data transformation

**Files to Modify**:
- `src/parser.c` - Parse dict comprehension
- `src/ast.h` - Add `ASTNodeType_DictComprehension`
- `src/interpreter.c` - Evaluate comprehension

---

#### 2.3 range() Function ⭐⭐⭐ HIGH PRIORITY
**Effort**: 1 week

**Syntax**:
```python
# Single argument (0 to n-1)
for (i of range(10)) {  # 0, 1, 2, ..., 9
    print(i)
}

# Start and end
for (i of range(5, 10)) {  # 5, 6, 7, 8, 9
    print(i)
}

# With step
for (i of range(0, 10, 2)) {  # 0, 2, 4, 6, 8
    print(i)
}

# Negative step (reverse)
for (i of range(10, 0, -1)) {  # 10, 9, 8, ..., 1
    print(i)
}

# Convert to array
let numbers = Array.from(range(5))  # [0, 1, 2, 3, 4]
```

**Implementation**:
- Return a generator/iterator (lazy evaluation)
- Memory efficient (doesn't create full array)

**Implementation Tasks**:
- [ ] Add `range()` built-in function in runtime
- [ ] Return generator object (leverage existing generator support)
- [ ] Support 1, 2, or 3 arguments: `range(end)`, `range(start, end)`, `range(start, end, step)`
- [ ] Implement `.next()` for iteration
- [ ] Tests: various argument combinations, for-of loops
- [ ] Examples: loops, list generation

**Files to Modify**:
- `src/runtime.c` - Implement `range()` built-in
- Uses existing generator infrastructure
- `include/runtime.h` - Function declaration

---

#### 2.4 enumerate() Function ⭐⭐⭐ HIGH PRIORITY
**Effort**: 1 week

**Syntax**:
```python
let items = ["apple", "banana", "cherry"]

# Default (start at 0)
for (index, item of enumerate(items)) {
    print(`${index}: ${item}`)
}
# Output:
# 0: apple
# 1: banana
# 2: cherry

# Custom start index
for (i, item of enumerate(items, 1)) {
    print(`${i}. ${item}`)
}
# Output:
# 1. apple
# 2. banana
# 3. cherry
```

**Implementation Tasks**:
- [ ] Add `enumerate()` built-in function
- [ ] Return iterator of `(index, value)` tuples
- [ ] Support optional `start` parameter
- [ ] Works with any iterable (arrays, generators)
- [ ] Tests: basic enumeration, custom start, with arrays/generators
- [ ] Examples: numbered lists, index access in loops

**Files to Modify**:
- `src/runtime.c` - Implement `enumerate()`
- Returns generator of tuples
- `include/runtime.h` - Function declaration

---

#### 2.5 zip() Function ⭐⭐ MEDIUM PRIORITY
**Effort**: 1 week

**Syntax**:
```python
let names = ["Alice", "Bob", "Charlie"]
let ages = [30, 25, 35]

# Zip two lists
for (name, age of zip(names, ages)) {
    print(`${name} is ${age} years old`)
}

# Zip multiple lists
let first = [1, 2, 3]
let second = [4, 5, 6]
let third = [7, 8, 9]
for (a, b, c of zip(first, second, third)) {
    print(`${a}, ${b}, ${c}`)
}

# Convert to array of tuples
let pairs = Array.from(zip(names, ages))
# [(Alice, 30), (Bob, 25), (Charlie, 35)]
```

**Implementation Tasks**:
- [ ] Add `zip()` built-in function (variadic)
- [ ] Return iterator of tuples
- [ ] Stop at shortest iterable length
- [ ] Support 2+ iterables
- [ ] Tests: two lists, multiple lists, different lengths
- [ ] Examples: parallel iteration

**Files to Modify**:
- `src/runtime.c` - Implement `zip(...iterables)`
- `include/runtime.h` - Function declaration

---

### Phase 3: Advanced Python Features (4-5 weeks)

#### 3.1 Slice Notation ⭐⭐⭐ HIGH PRIORITY
**Effort**: 2 weeks

**Syntax**:
```python
let list = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

# Basic slicing
let sub = list[2:5]  # [2, 3, 4] (indices 2-4)
let start = list[:3]  # [0, 1, 2] (first 3)
let end = list[7:]  # [7, 8, 9] (from index 7)
let copy = list[:]  # Full copy

# With step
let evens = list[::2]  # [0, 2, 4, 6, 8] (every other)
let odds = list[1::2]  # [1, 3, 5, 7, 9]

# Negative indices
let last = list[-1]  # 9 (last element)
let lastThree = list[-3:]  # [7, 8, 9]

# Reverse
let reversed = list[::-1]  # [9, 8, 7, ..., 0]

# Strings too
let text = "Hello, World!"
let hello = text[:5]  # "Hello"
let world = text[7:]  # "World!"
```

**Implementation Tasks**:
- [ ] Extend lexer to handle `[start:end:step]` syntax
- [ ] Distinguish from normal indexing `[index]`
- [ ] Parse optional start, end, step (can be omitted)
- [ ] Support negative indices (-1 = last)
- [ ] Implement slice operations in runtime
- [ ] Support for arrays and strings
- [ ] Tests: all slice variations, negative indices, strings
- [ ] Examples: array manipulation, string parsing

**Files to Modify**:
- `src/lexer.c` - Recognize colon in brackets
- `src/parser.c` - Parse slice expressions
- `src/ast.h` - Add `ASTNodeType_Slice`
- `src/interpreter.c` - Evaluate slices
- `src/runtime.c` - Slice helper functions

---

#### 3.2 With Statement (Context Managers) ⭐⭐ MEDIUM PRIORITY
**Effort**: 2 weeks

**Syntax**:
```python
# File I/O (auto-close)
with (open("data.txt", "r") as file) {
    let content = file.read()
    print(content)
}  # file.close() called automatically

# Multiple resources
with (open("in.txt") as inFile, open("out.txt", "w") as outFile) {
    let data = inFile.read()
    outFile.write(data.uppercase())
}

# Custom context managers (future)
class Timer {
    fn __enter__() {
        this.start = Date.now()
        return this
    }
    
    fn __exit__() {
        let elapsed = Date.now() - this.start
        print(`Elapsed: ${elapsed}ms`)
    }
}

with (new Timer() as timer) {
    # Timed code here
}
```

**Prerequisites**:
- ✅ Try-catch-finally must be complete (for error handling)

**Implementation Tasks**:
- [ ] Add `TOKEN_WITH` keyword
- [ ] Parse `with (expr as var) { body }`
- [ ] Desugar to try-finally block:
  ```javascript
  let var = expr.__enter__()
  try {
      body
  } finally {
      var.__exit__()
  }
  ```
- [ ] Add `__enter__` and `__exit__` protocol
- [ ] Implement for file handles
- [ ] Support multiple resources
- [ ] Tests: file I/O, multiple resources, error handling
- [ ] Examples: file operations, resource management

**Files to Modify**:
- `src/lexer.c` - Add `with` keyword
- `src/parser.c` - Parse with statement
- `src/ast.h` - Add `ASTNodeType_With`
- `src/interpreter.c` - Desugar or direct implementation
- `src/runtime.c` - Context manager protocol

**Note**: Requires try-catch-finally to be fully implemented first.

---

#### 3.3 Property Decorators ⭐⭐ MEDIUM PRIORITY
**Effort**: 2 weeks

**Syntax**:
```python
class Circle {
    private let radius: float
    
    fn init(r: float) {
        this.radius = r
    }
    
    @property
    fn area() -> float {
        return 3.14159 * this.radius * this.radius
    }
    
    @property
    fn diameter() -> float {
        return this.radius * 2
    }
    
    @diameter.setter
    fn diameter(value: float) {
        this.radius = value / 2
    }
}

# Usage
let circle = new Circle(5.0)
print(circle.area)  # Looks like field access, calls method
circle.diameter = 20  # Calls setter
print(circle.radius)  # 10
```

**Implementation Tasks**:
- [ ] Add decorator syntax: `@decorator` before function
- [ ] Implement `@property` decorator
- [ ] Implement `@<property>.setter` decorator
- [ ] Transform getter methods to property access
- [ ] Transform setter methods to property assignment
- [ ] Tests: read-only properties, getters/setters
- [ ] Examples: computed properties, encapsulation

**Files to Modify**:
- `src/lexer.c` - Add `@` token
- `src/parser.c` - Parse decorators
- `src/ast.h` - Add decorator metadata to functions
- `src/interpreter.c` - Handle property access/assignment
- `src/runtime.c` - Property descriptor system

---

#### 3.4 Match Statement (Pattern Matching) ⭐⭐ MEDIUM PRIORITY
**Effort**: 2 weeks

**Syntax**:
```python
# Basic matching
match status {
    case 200 => print("OK")
    case 404 => print("Not Found")
    case 500 => print("Server Error")
    case _ => print("Unknown")
}

# Match with destructuring
match point {
    case (0, 0) => print("Origin")
    case (x, 0) => print(`On x-axis at ${x}`)
    case (0, y) => print(`On y-axis at ${y}`)
    case (x, y) => print(`At (${x}, ${y})`)
}

# Match with guards
match age {
    case x if x < 0 => print("Invalid")
    case x if x < 18 => print("Minor")
    case x if x < 65 => print("Adult")
    case _ => print("Senior")
}

# Match types
match value {
    case int => print("Integer")
    case string => print("String")
    case _ => print("Other")
}
```

**Implementation Tasks**:
- [ ] Add `TOKEN_MATCH`, `TOKEN_CASE` keywords
- [ ] Parse match expression and case clauses
- [ ] Support literal patterns (numbers, strings)
- [ ] Support destructuring patterns (tuples, arrays, objects)
- [ ] Support guard clauses (if conditions)
- [ ] Support type patterns
- [ ] Wildcard pattern `_`
- [ ] Compile to if-else chain or jump table
- [ ] Tests: all pattern types
- [ ] Examples: state machines, parsers

**Files to Modify**:
- `src/lexer.c` - Add `match`, `case` keywords
- `src/parser.c` - Parse match statement
- `src/ast.h` - Add `ASTNodeType_Match`, pattern nodes
- `src/interpreter.c` - Pattern matching logic
- `src/runtime.c` - Pattern matching helpers

---

### Phase 4: Standard Library (6-8 weeks core, ongoing)

#### 4.1 File I/O Module ⭐⭐⭐ CRITICAL
**Effort**: 2 weeks

**API**:
```python
import { open, readFile, writeFile } from "fs"

# Method 1: open() with methods
let file = open("data.txt", "r")  # modes: "r", "w", "a", "r+", "w+", "a+"
let content = file.read()  # Read all
file.close()

# Method 2: readFile helper (auto-closes)
let text = readFile("data.txt")

# Method 3: with statement (auto-closes)
with (open("data.txt", "r") as f) {
    let line = f.readline()  # Read one line
    let lines = f.readlines()  # Read all lines as array
}

# Writing
with (open("output.txt", "w") as f) {
    f.write("Hello, World!\n")
    f.writelines(["Line 1\n", "Line 2\n"])
}

# Write helper
writeFile("output.txt", "Content here")

# Binary mode
let data = readFile("image.png", "rb")
writeFile("copy.png", data, "wb")

# Check if file exists
import { exists, isFile, isDirectory } from "fs"
if (exists("data.txt")) {
    print("File exists")
}
```

**Implementation Tasks**:
- [ ] Create `fs` module (file system)
- [ ] `open(path, mode)` returns file handle object
- [ ] File handle methods: `read()`, `readline()`, `readlines()`, `write()`, `writelines()`, `close()`
- [ ] Helper functions: `readFile()`, `writeFile()`
- [ ] Utility functions: `exists()`, `isFile()`, `isDirectory()`, `remove()`, `rename()`
- [ ] Text and binary modes
- [ ] Error handling (file not found, permission denied)
- [ ] Integration with `with` statement
- [ ] Tests: read/write text, read/write binary, error cases
- [ ] Examples: read config, write logs, file copying

**Files to Create**:
- `stdlib/fs.kl` - File system module (if modules in KLang)
- Or `src/runtime_fs.c` - C implementation with runtime_init_fs()
- `include/runtime_fs.h` - File I/O API

**C Implementation**:
- Use standard C `fopen()`, `fread()`, `fwrite()`, `fclose()`
- Cross-platform path handling

---

#### 4.2 JSON Module ⭐⭐⭐ HIGH PRIORITY
**Effort**: 3 weeks

**API**:
```python
import json from "json"

# Parse JSON string to object
let text = '{"name": "Alice", "age": 30, "active": true}'
let obj = json.parse(text)
print(obj.name)  # "Alice"

# Stringify object to JSON
let data = {"users": ["Alice", "Bob"], "count": 2}
let jsonText = json.stringify(data)
print(jsonText)  # {"users":["Alice","Bob"],"count":2}

# Pretty print (with indentation)
let pretty = json.stringify(data, 2)  # 2-space indent

# Handle errors
try {
    let bad = json.parse("{invalid json}")
} catch (err) {
    print(`JSON error: ${err.message}`)
}
```

**Implementation Tasks**:
- [ ] Create `json` module
- [ ] `json.parse(text)` function:
  - Parse JSON string to KLang object/array
  - Support: objects, arrays, strings, numbers, booleans, null
  - Error handling for invalid JSON
- [ ] `json.stringify(value, indent?)` function:
  - Convert KLang value to JSON string
  - Optional indentation for pretty printing
  - Handle nested objects/arrays
- [ ] Type mapping:
  - JSON object → KLang object (or dict)
  - JSON array → KLang array
  - JSON null → KLang None/nil
- [ ] Tests: parse objects, arrays, primitives, stringify, errors
- [ ] Examples: config files, API responses

**Files to Create**:
- `stdlib/json.kl` or `src/runtime_json.c`
- JSON parser implementation (recursive descent)
- JSON stringifier (recursive traversal)

**Implementation Options**:
1. Write custom JSON parser in C
2. Integrate existing C library (e.g., cJSON, json-c)
3. **Recommended**: Custom implementation for learning + control

---

#### 4.3 Regular Expression Module ⭐⭐⭐ HIGH PRIORITY
**Effort**: 4 weeks

**API**:
```python
import regex from "regex"

# Match entire string
let match = regex.match(/\d+/, "Age: 25")
if (match) {
    print(match.group(0))  # "25"
}

# Search for pattern
let result = regex.search(/[A-Z][a-z]+/, "Hello World")
print(result.start())  # 0
print(result.end())  # 5
print(result.group())  # "Hello"

# Find all matches
let matches = regex.findall(/\d+/, "Numbers: 1, 22, 333")
# Returns: ["1", "22", "333"]

# Replace
let text = "Hello, World!"
let newText = regex.replace(/World/, "KLang", text)
# "Hello, KLang!"

# Replace with function
let doubled = regex.replace(/\d+/, match => String(int(match) * 2), "Value: 42")
# "Value: 84"

# Compile for reuse
let pattern = regex.compile(/\w+@\w+\.\w+/)
let isEmail = pattern.test("user@example.com")  # true

# Groups
let match = regex.match(/(\d{3})-(\d{4})/, "Phone: 555-1234")
print(match.group(1))  # "555"
print(match.group(2))  # "1234"

# Flags
let pattern = regex.compile(/hello/, "i")  # Case insensitive
let match = pattern.match("HELLO")  # Matches
```

**Implementation Tasks**:
- [ ] Create `regex` module
- [ ] Integrate PCRE2 C library (or write custom regex engine)
- [ ] `regex.match(pattern, text)` - Match from start
- [ ] `regex.search(pattern, text)` - Find anywhere
- [ ] `regex.findall(pattern, text)` - Find all occurrences
- [ ] `regex.replace(pattern, replacement, text)` - Replace matches
- [ ] `regex.compile(pattern, flags?)` - Compile regex for reuse
- [ ] Match objects with `group()`, `start()`, `end()` methods
- [ ] Support groups/captures
- [ ] Flags: `i` (case insensitive), `m` (multiline), `s` (dotall), `g` (global)
- [ ] Tests: basic matching, groups, replace, flags
- [ ] Examples: validation, parsing, text processing

**Files to Create**:
- `stdlib/regex.kl` or `src/runtime_regex.c`
- `include/runtime_regex.h`

**Implementation Recommended**:
- Use PCRE2 library (industry standard)
- Alternative: Write simplified regex engine (educational but time-consuming)

---

#### 4.4 Math Module ⭐⭐ MEDIUM PRIORITY
**Effort**: 1 week

**API**:
```python
import math from "math"

# Constants
print(math.PI)  # 3.141592653589793
print(math.E)  # 2.718281828459045

# Trigonometry
let sine = math.sin(math.PI / 2)  # 1.0
let cosine = math.cos(0)  # 1.0
let tangent = math.tan(math.PI / 4)  # 1.0

# Inverse trig
let angle = math.asin(1.0)  # PI/2
let angle2 = math.acos(0.5)  # PI/3

# Exponential/logarithm
let exp = math.exp(1)  # e^1 = 2.718...
let log = math.log(math.E)  # 1.0
let log10 = math.log10(100)  # 2.0
let log2 = math.log2(8)  # 3.0

# Power and roots
let squared = math.pow(5, 2)  # 25
let cubed = math.pow(2, 3)  # 8
let root = math.sqrt(16)  # 4.0
let cbrt = math.cbrt(27)  # 3.0

# Rounding
let up = math.ceil(3.2)  # 4
let down = math.floor(3.8)  # 3
let rounded = math.round(3.5)  # 4

# Other
let absolute = math.abs(-5)  # 5
let max = math.max(5, 10, 3)  # 10
let min = math.min(5, 10, 3)  # 3
```

**Implementation Tasks**:
- [ ] Create `math` module
- [ ] Bind C `math.h` functions
- [ ] Constants: PI, E
- [ ] Trig: sin, cos, tan, asin, acos, atan, atan2
- [ ] Exponential: exp, log, log10, log2
- [ ] Power: pow, sqrt, cbrt
- [ ] Rounding: ceil, floor, round, trunc
- [ ] Utility: abs, max, min, sign
- [ ] Tests: all functions
- [ ] Examples: physics calculations, geometry

**Files to Create**:
- `stdlib/math.kl` or `src/runtime_math.c`
- Simple wrappers around C math.h

---

#### 4.5 DateTime Module ⭐⭐ MEDIUM PRIORITY
**Effort**: 2 weeks

**API**:
```python
import datetime from "datetime"

# Current date/time
let now = datetime.now()
print(now.year)  # 2025
print(now.month)  # 1
print(now.day)  # 15
print(now.hour)  # 14
print(now.minute)  # 30
print(now.second)  # 45

# Create specific date
let date = datetime.date(2025, 1, 15)
let time = datetime.time(14, 30, 45)
let dt = datetime.datetime(2025, 1, 15, 14, 30, 45)

# Formatting
let formatted = now.format("%Y-%m-%d %H:%M:%S")
# "2025-01-15 14:30:45"

# Parsing
let parsed = datetime.parse("2025-01-15", "%Y-%m-%d")

# Arithmetic
let tomorrow = now.add(days: 1)
let nextWeek = now.add(weeks: 1)
let yesterday = now.subtract(days: 1)

# Difference
let diff = date2.subtract(date1)
print(diff.days)  # Number of days between dates

# Timestamp
let timestamp = now.timestamp()  # Unix timestamp
let fromTs = datetime.fromTimestamp(timestamp)
```

**Implementation Tasks**:
- [ ] Create `datetime` module
- [ ] `Date`, `Time`, `DateTime` classes
- [ ] `now()` function (current date/time)
- [ ] Constructors for specific dates
- [ ] Format/parse with format strings
- [ ] Date arithmetic (add/subtract days, hours, etc.)
- [ ] Difference between dates
- [ ] Unix timestamp conversion
- [ ] Tests: creation, formatting, arithmetic
- [ ] Examples: scheduling, time tracking

**Files to Create**:
- `stdlib/datetime.kl` or `src/runtime_datetime.c`
- Use C `time.h` for system time

---

#### 4.6 Random Module ⭐⭐ MEDIUM PRIORITY
**Effort**: 1 week

**API**:
```python
import random from "random"

# Random float [0.0, 1.0)
let r = random.random()

# Random integer [a, b]
let n = random.randint(1, 10)  # 1-10 inclusive

# Random float in range [a, b]
let f = random.uniform(0.0, 10.0)

# Random choice from array
let items = ["apple", "banana", "cherry"]
let item = random.choice(items)

# Random sample (multiple unique items)
let sample = random.sample(items, 2)  # 2 random items

# Shuffle array in place
random.shuffle(items)

# Seed for reproducibility
random.seed(42)
let r1 = random.random()
random.seed(42)
let r2 = random.random()  # Same as r1
```

**Implementation Tasks**:
- [ ] Create `random` module
- [ ] `random()` - [0.0, 1.0) float
- [ ] `randint(a, b)` - Random integer
- [ ] `uniform(a, b)` - Random float in range
- [ ] `choice(array)` - Random element
- [ ] `sample(array, k)` - k random unique elements
- [ ] `shuffle(array)` - Shuffle in place
- [ ] `seed(value)` - Set random seed
- [ ] Tests: all functions, seeding
- [ ] Examples: games, simulations

**Files to Create**:
- `stdlib/random.kl` or `src/runtime_random.c`
- Use C `stdlib.h` rand/srand

---

### Phase 5: Advanced/Power Features (4-6 weeks)

#### 5.1 Type Introspection ⭐⭐ MEDIUM PRIORITY
**Effort**: 2 weeks

**API**:
```python
# Runtime type checking
let x = 42
if (isinstance(x, int)) {
    print("x is an integer")
}

# Check class instances
let user = new User()
if (isinstance(user, User)) {
    print("user is a User")
}

# Check multiple types
if (isinstance(value, [int, float])) {
    print("value is a number")
}

# Attribute checking
if (hasattr(obj, "name")) {
    print(obj.name)
}

# Get attribute dynamically
let value = getattr(obj, "field", "default")

# Type name
print(type(x).__name__)  # "int"
```

**Implementation Tasks**:
- [ ] `isinstance(obj, Type)` built-in function
- [ ] `hasattr(obj, "field")` built-in function
- [ ] `getattr(obj, "field", default?)` built-in function
- [ ] Runtime Type Information (RTTI) system
- [ ] Type.__name__ property
- [ ] Tests: all introspection functions
- [ ] Examples: serialization, debugging

---

#### 5.2 Operator Overloading ⭐⭐ MEDIUM PRIORITY
**Effort**: 3 weeks

**Syntax**:
```python
class Vector {
    let x: float
    let y: float
    
    fn init(x: float, y: float) {
        this.x = x
        this.y = y
    }
    
    fn operator+(other: Vector) -> Vector {
        return new Vector(this.x + other.x, this.y + other.y)
    }
    
    fn operator-(other: Vector) -> Vector {
        return new Vector(this.x - other.x, this.y - other.y)
    }
    
    fn operator*(scalar: float) -> Vector {
        return new Vector(this.x * scalar, this.y * scalar)
    }
    
    fn operator==(other: Vector) -> bool {
        return this.x == other.x && this.y == other.y
    }
    
    fn toString() -> string {
        return `(${this.x}, ${this.y})`
    }
}

# Usage
let v1 = new Vector(1, 2)
let v2 = new Vector(3, 4)
let v3 = v1 + v2  # Calls operator+
let v4 = v1 * 2.5  # Calls operator*
if (v1 == v2) { }  # Calls operator==
```

**Implementation Tasks**:
- [ ] Add `operator` keyword
- [ ] Support operators: `+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `[]`
- [ ] Parse operator methods in classes
- [ ] Compile operator expressions to method calls
- [ ] Type checking for operator overloads
- [ ] Tests: arithmetic, comparison, custom types
- [ ] Examples: Vector, Complex, Matrix

---

## 📊 Timeline Summary

| Phase | Features | Duration | Priority |
|-------|----------|----------|----------|
| **Phase 1** | Tuples, Dicts, Sets, None | 4-6 weeks | ⭐⭐⭐ CRITICAL |
| **Phase 2** | Comprehensions, range, enumerate, zip | 3-4 weeks | ⭐⭐⭐ HIGH |
| **Phase 3** | Slice notation, with, properties, match | 4-5 weeks | ⭐⭐ MEDIUM |
| **Phase 4** | File I/O, JSON, Regex, Math, DateTime, Random | 6-8 weeks | ⭐⭐⭐ HIGH |
| **Phase 5** | Type introspection, operator overload | 4-6 weeks | ⭐⭐ MEDIUM |

**Total Estimated Time**: 21-29 weeks (5-7 months)

**Quick Wins (First 6-8 weeks)**: Phases 1-2
- Tuples, dicts, sets
- List comprehensions
- range(), enumerate(), zip()

**After Quick Wins**: KLang will feel significantly more Python-like for basic programs.

---

## 🎯 Success Criteria

### After Phase 1-2 (10 weeks)
- ✅ Can write Python-style data structures (tuples, dicts, sets)
- ✅ Can use list comprehensions
- ✅ Can iterate with range() and enumerate()
- ✅ 50+ working Python-style examples

### After Phase 4 (18 weeks)
- ✅ Can read/write files
- ✅ Can parse/generate JSON
- ✅ Can use regex for text processing
- ✅ Real-world applications possible

### Full Completion (25 weeks)
- ✅ 80%+ Python feature compatibility (where applicable)
- ✅ 70%+ Python code translates with minimal changes
- ✅ Comprehensive standard library
- ✅ 100+ working examples

---

## 🚀 Getting Started

### Immediate Next Steps

1. **Complete JavaScript features** (try-catch-finally, switch-case) - ~1-2 weeks
2. **Begin Phase 1.1: Tuple type** - 2 weeks
3. **Continue Phase 1.2-1.4**: Dict, Set, None - 4 weeks
4. **Phase 2: List comprehensions** - 2 weeks

### Recommended Order

```
Week 1-2:   Finish try-catch-finally (prerequisite for 'with')
Week 3-4:   Tuple type
Week 5-6:   Dictionary type
Week 7-8:   Set type + None type
Week 9-10:  List comprehensions
Week 11:    range() function
Week 12:    enumerate() + zip()
```

---

## 📝 Notes

### Design Philosophy

- **Maintain static typing**: Python features must work with KLang's type system
- **No dynamic typing**: KLang is statically typed by design
- **Performance**: Features should compile efficiently
- **Safety**: Prefer compile-time errors over runtime errors
- **Compatibility**: Syntax should feel Python-like where possible
- **Pragmatism**: Skip features that don't make sense (metaclasses, duck typing, etc.)

### Syntax Decisions

Where Python and KLang differ:
- **Blocks**: Python uses indentation, KLang uses braces `{}`
- **Comments**: Both support `#`, KLang also has `//` and `/* */`
- **This vs Self**: KLang uses `this`, Python uses `self`
- **Function keyword**: KLang uses `fn`, Python uses `def`
- **Type hints**: KLang enforces types, Python's are optional

### Integration Strategy

- **Leverage existing features**: Use generators for range(), existing destructuring for tuples
- **Compile to existing**: List comprehensions → map/filter chains
- **C runtime**: Standard library implemented in C for performance
- **Module system**: Use ES6-style imports, create Python-style stdlib modules

---

## 📚 References

- [PYTHON_FEATURES_ANALYSIS.md](./PYTHON_FEATURES_ANALYSIS.md) - Detailed feature analysis
- [JAVASCRIPT_FEATURES_ROADMAP.md](./JAVASCRIPT_FEATURES_ROADMAP.md) - JavaScript features already added
- [COMPLETE_ROADMAP_TO_100.md](./COMPLETE_ROADMAP_TO_100.md) - Overall project roadmap
- [README.md](./README.md) - Project overview

---

**Created**: 2025  
**Status**: Planning Phase  
**Owner**: KLang Development Team
