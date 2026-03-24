# KLang Standard Library

The KLang standard library provides a comprehensive set of modules for common programming tasks.

## Core Modules

### core.kl

Basic utilities and built-in functions.

```klang
import core

# Built-in functions available without import:
println("Hello")
print("World")
len([1, 2, 3])        # 3
str(42)               # "42"
int("42")             # 42
type(value)           # Get type name
```

### fs.kl - File System

File system operations.

```klang
import fs

# File operations
let content = fs.readFile("file.txt")
fs.writeFile("output.txt", "Hello")
fs.appendFile("log.txt", "Log entry\n")
fs.deleteFile("temp.txt")

# Directory operations
fs.createDir("mydir")
fs.removeDir("olddir")
let files = fs.listDir(".")

# Path operations
let path = fs.joinPath("dir", "subdir", "file.txt")
let base = fs.baseName("/path/to/file.txt")  # "file.txt"
let dir = fs.dirName("/path/to/file.txt")    # "/path/to"
let ext = fs.ext("file.txt")                 # ".txt"

# File info
let exists = fs.fileExists("file.txt")
let size = fs.fileSize("file.txt")
let isFile = fs.isFile("file.txt")
let isDir = fs.isDirectory("mydir")

# Working directory
let cwd = fs.cwd()
fs.chdir("/new/path")
```

### datetime.kl - Date and Time

Date and time utilities.

```klang
import datetime

# Current date/time
let now = datetime.now()  # Timestamp
let dt = datetime.DateTime()  # DateTime object

# DateTime object
println(dt.year)
println(dt.month)
println(dt.day)
println(dt.hour)
println(dt.minute)

# Formatting
let formatted = dt.format("%Y-%m-%d %H:%M:%S")
let iso = dt.toISO()  # ISO 8601 format

# Date arithmetic
let tomorrow = dt.addDays(1)
let nextHour = dt.addHours(1)

# Parsing
let parsed = datetime.parse("2024-03-24 10:30:00", "ISO")

# Duration
let dur = datetime.duration(3600)  # 1 hour in seconds
println(dur.hours)    # 1
println(dur.minutes)  # 60

# Timer
let timer = datetime.Timer()
# ... do work ...
println("Elapsed: " + timer.elapsed() + "s")
```

### collections.kl - Data Structures

Advanced data structures.

```klang
import collections

# Stack
let stack = collections.Stack()
stack.push(1)
stack.push(2)
let top = stack.pop()  # 2
let size = stack.size()

# Queue
let queue = collections.Queue()
queue.enqueue(1)
queue.enqueue(2)
let first = queue.dequeue()  # 1

# Priority Queue
let pq = collections.PriorityQueue()
pq.enqueue("high priority", 1)
pq.enqueue("low priority", 10)
let next = pq.dequeue()  # "high priority"

# Linked List
let list = collections.LinkedList()
list.append(1)
list.append(2)
list.prepend(0)
let arr = list.toArray()  # [0, 1, 2]

# Set
let set = collections.Set([1, 2, 3, 2])
set.add(4)
set.has(3)  # true
let values = set.values()

# Set operations
let union = set1.union(set2)
let intersection = set1.intersection(set2)
let difference = set1.difference(set2)

# Ordered Map
let map = collections.OrderedMap()
map.set("key1", "value1")
map.set("key2", "value2")
let val = map.get("key1")
let keys = map.keys()  # Ordered!
```

### test.kl - Testing Framework

Unit testing framework.

```klang
import test

# Test suite
test.describe("Calculator", fn() {
    test.it("adds numbers", fn() {
        test.expect(1 + 1).toBe(2)
    })
    
    test.it("subtracts numbers", fn() {
        test.expect(5 - 3).toBe(2)
    })
    
    test.it("multiplies numbers", fn() {
        let result = 3 * 4
        test.expect(result).toEqual(12)
    })
})

# Setup/teardown
test.describe("Database", fn() {
    test.beforeEach(fn() {
        # Setup before each test
    })
    
    test.afterEach(fn() {
        # Cleanup after each test
    })
    
    test.it("saves data", fn() {
        # Test code
    })
})

# Assertions
test.expect(value).toBe(expected)
test.expect(value).toEqual(expected)
test.expect(value).toBeTruthy()
test.expect(value).toBeFalsy()
test.expect(value).toBeNull()
test.expect(value).toBeGreaterThan(5)
test.expect(value).toBeLessThan(10)
test.expect(array).toContain(item)
test.expect(array).toHaveLength(3)
test.expect(fn).toThrow()

# Negation
test.expect(value).not.toBe(other)
test.expect(value).not.toBeNull()

# Run tests
test.run()  # Returns true if all tests pass
```

### crypto.kl - Cryptography

Cryptographic functions and hashing.

```klang
import crypto

# Hashing
let hash = crypto.hash("data", "sha256")
let md5 = crypto.hash("data", "md5")
let sha1 = crypto.hash("data", "sha1")

# Encoding
let encoded = crypto.base64Encode("Hello World")
let decoded = crypto.base64Decode(encoded)

let hex = crypto.hexEncode("data")
let data = crypto.hexDecode(hex)

# Simple encryption
let encrypted = crypto.xorCipher("message", "key")
let decrypted = crypto.xorCipher(encrypted, "key")

# UUID generation
let id = crypto.uuid()  # UUID v4

# HMAC
let hmac = crypto.hmac("message", "secret", "sha256")

# Password hashing
let hashed = crypto.hashPassword("password123")
# hashed = { hash: "...", salt: "..." }

let valid = crypto.verifyPassword("password123", hashed)
```

## Existing Modules

### string.kl

String manipulation utilities.

```klang
import string

string.upper("hello")      # "HELLO"
string.lower("HELLO")      # "hello"
string.trim("  hello  ")   # "hello"
string.split("a,b,c", ",") # ["a", "b", "c"]
string.join(["a", "b"], ",") # "a,b,c"
```

### math.kl

Mathematical functions.

```klang
import math

math.abs(-5)        # 5
math.sqrt(16)       # 4
math.pow(2, 3)      # 8
math.floor(3.7)     # 3
math.ceil(3.2)      # 4
math.round(3.5)     # 4
math.sin(math.PI)
math.cos(math.PI)
```

### array.kl

Array utilities.

```klang
import array

array.map(arr, fn)
array.filter(arr, fn)
array.reduce(arr, fn, initial)
array.sort(arr)
array.reverse(arr)
array.unique(arr)
```

### json.kl

JSON parsing and stringification.

```klang
import json

let obj = json.parse('{"name": "John", "age": 30}')
let str = json.stringify(obj)
```

### io.kl

Input/output operations.

```klang
import io

let input = io.readline("Enter name: ")
io.println("Hello " + input)
```

### llm.kl, rag.kl, agents.kl, embedding.kl

AI/ML modules (see [AI_NATIVE_GUIDE.md](AI_NATIVE_GUIDE.md)).

## Module Import

```klang
# Import entire module
import fs

# Import specific functions
import { readFile, writeFile } from fs

# Import with alias
import fs as filesystem
```

## Creating Custom Modules

Create your own reusable modules:

```klang
# mymodule.kl
fn greet(name) {
    return "Hello, " + name
}

const VERSION = "1.0.0"

export {
    greet,
    VERSION
}
```

Usage:

```klang
import { greet } from mymodule

println(greet("World"))  # "Hello, World"
```

## Best Practices

1. **Import only what you need**: Use selective imports
2. **Use meaningful names**: Clear module and function names
3. **Document your code**: Add comments for complex logic
4. **Handle errors**: Always check for null/undefined
5. **Test thoroughly**: Use the test module
6. **Keep modules focused**: Single responsibility principle

## Contributing

Want to add a module to the standard library? See [CONTRIBUTING.md](../CONTRIBUTING.md).
