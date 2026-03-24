# Object Literals in KLang

## Overview

KLang **fully supports** object literal syntax, including objects in arrays!

## Syntax

### Simple Object Literal
```klang
let person = {name: "Alice", age: 30, city: "NYC"}
```

### Objects in Arrays
```klang
let users = [
    {id: 1, name: "Alice", role: "Admin"},
    {id: 2, name: "Bob", role: "User"}
]
```

### Nested Objects
```klang
let config = {
    server: {
        host: "localhost",
        port: 8080
    },
    database: {
        name: "mydb",
        type: "postgres"
    }
}
```

### Mixed Type Arrays
```klang
let mixed = [
    42,
    "hello",
    {type: "object", value: 100},
    [1, 2, 3],
    true
]
```

## Use Cases

### 1. AI Message Structures
```klang
let messages = [
    {role: "system", content: "You are helpful"},
    {role: "user", content: "Hello"},
    {role: "assistant", content: "Hi!"}
]
```

### 2. Configuration Objects
```klang
let apiConfig = {
    url: "https://api.example.com",
    method: "POST",
    headers: {
        contentType: "application/json",
        authorization: "Bearer token123"
    },
    timeout: 5000
}
```

### 3. Data Structures
```klang
let graph = {
    nodes: [
        {id: "A", label: "Start"},
        {id: "B", label: "End"}
    ],
    edges: [
        {source: "A", target: "B", weight: 1.0}
    ]
}
```

### 4. Training Data
```klang
let trainingData = [
    {input: [0.1, 0.2], output: [1.0], label: "positive"},
    {input: [0.4, 0.5], output: [0.0], label: "negative"}
]
```

### 5. API Responses
```klang
let response = {
    status: 200,
    success: true,
    data: {
        items: [
            {id: 1, title: "First"},
            {id: 2, title: "Second"}
        ]
    }
}
```

## Important Notes

### ✅ Supported
- Object literals: `{key: value}`
- Objects in arrays: `[{obj1}, {obj2}]`
- Nested objects: unlimited depth
- Mixed type values
- String, number, boolean, array, object values

### ⚠️ Limitations
- **Reserved keywords cannot be property names**
  - Avoid: `from`, `import`, `export`, `class`, `fn`, etc.
  - Use: `source` instead of `from`, `method` instead of `fn`
  
- **Property access**
  - Object notation may not be fully implemented yet
  - May need to convert to dict for property access

### Workaround for Reserved Keywords
```klang
# ❌ Won't work - 'from' is reserved
let edge = {from: "A", to: "B"}

# ✅ Works - use different names
let edge = {source: "A", target: "B"}
```

## Comparison with dict()

### Object Literal (Cleaner)
```klang
let user = {id: 1, name: "Alice", active: true}
```

### dict() Method (Verbose)
```klang
let user = dict()
user.set("id", 1)
user.set("name", "Alice")
user.set("active", true)
```

**Both work**, but object literals are more concise for known structures!

## Examples

See `examples/object_literals_working.kl` for comprehensive examples.

## Status

✅ **Fully Implemented** - Object literals work in KLang!

This feature makes KLang perfect for:
- JSON-like data structures
- AI message formats
- Configuration objects
- API request/response handling
- Data modeling

---

**Use object literals freely in your KLang programs!** 🚀
