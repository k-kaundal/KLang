# Built-in Functions Reference

## Introduction

KLang provides a comprehensive set of built-in functions for common operations. These functions are always available without any imports.

## Output Functions

### `println(value)`

Print value followed by newline.

```klang
println("Hello, World!")
println(42)
println([1, 2, 3])
```

### `print(value)`

Print value without newline.

```klang
print("Loading")
print(".")
print(".")
println("Done!")
# Output: Loading..Done!
```

## Type Functions

### `type(value)`

Get the type of a value.

```klang
println(type(42))        # "int"
println(type(3.14))      # "float"
println(type("hello"))   # "string"
println(type([1, 2]))    # "array"
println(type(true))      # "bool"
println(type(null))      # "null"
```

### `str(value)`

Convert value to string.

```klang
let num = 42
let text = "The answer is " + str(num)
println(text)  # "The answer is 42"

println(str([1, 2, 3]))    # "[1, 2, 3]"
println(str(true))          # "true"
```

### `int(value)`

Convert value to integer.

```klang
println(int("42"))      # 42
println(int(3.14))      # 3
println(int(true))      # 1
println(int(false))     # 0
```

### `float(value)`

Convert value to float.

```klang
println(float("3.14"))  # 3.14
println(float(42))      # 42.0
println(float("2.5e3")) # 2500.0
```

### `bool(value)`

Convert value to boolean.

```klang
println(bool(1))        # true
println(bool(0))        # false
println(bool(""))       # false
println(bool("hello"))  # true
println(bool([]))       # false
println(bool([1]))      # true
```

## Collection Functions

### `len(collection)`

Get length of string, array, or object.

```klang
println(len("hello"))       # 5
println(len([1, 2, 3]))     # 3
println(len({"a": 1}))      # 1
```

### `range(start, end, step?)`

Generate array of numbers.

```klang
println(range(5))           # [0, 1, 2, 3, 4]
println(range(2, 8))        # [2, 3, 4, 5, 6, 7]
println(range(0, 10, 2))    # [0, 2, 4, 6, 8]
```

## String Functions

### `substr(string, start, length?)`

Extract substring.

```klang
let text = "Hello, World!"
println(substr(text, 0, 5))    # "Hello"
println(substr(text, 7))        # "World!"
```

### `split(string, delimiter)`

Split string into array.

```klang
let csv = "apple,banana,orange"
let fruits = split(csv, ",")
println(fruits)  # ["apple", "banana", "orange"]
```

### `join(array, delimiter)`

Join array into string.

```klang
let words = ["Hello", "World"]
println(join(words, " "))  # "Hello World"
```

## File Functions

### `read_file(path)`

Read file contents.

```klang
let content = read_file("data.txt")
println(content)
```

### `write_file(path, content)`

Write content to file.

```klang
write_file("output.txt", "Hello from KLang!")
```

## HTTP Functions

### `fetch(url, options?)`

Make HTTP request.

```klang
# GET request
let data = fetch("https://api.example.com/users")

# POST request
let response = fetch("https://api.example.com/users", {
    method: "POST",
    headers: {"Content-Type": "application/json"},
    body: json_stringify({"name": "Alice"})
})
```

## Environment Functions

### `env(variable_name)`

Get environment variable.

```klang
let api_key = env("API_KEY")
let port = env("PORT") or "8080"
```

## JSON Functions

### `json_parse(string)`

Parse JSON string.

```klang
let obj = json_parse('{"name": "Alice"}')
println(obj["name"])
```

### `json_stringify(value)`

Convert to JSON string.

```klang
let data = {"name": "Bob", "age": 30}
println(json_stringify(data))
```

## Math Functions

### Basic Math

```klang
println(abs(-5))        # 5
println(sqrt(16))       # 4.0
println(pow(2, 3))      # 8
println(floor(3.7))     # 3
println(ceil(3.2))      # 4
println(round(3.5))     # 4
```

## Utility Functions

### `assert(condition, message?)`

Assert condition is true.

```klang
assert(1 + 1 == 2, "Math is broken!")
```

### `typeof(value)`

Get detailed type information.

```klang
println(typeof([1, 2, 3]))  # "array"
println(typeof(null))        # "null"
```

## Complete Reference

| Function | Parameters | Returns | Description |
|----------|-----------|---------|-------------|
| `println` | value | null | Print with newline |
| `print` | value | null | Print without newline |
| `type` | value | string | Get type name |
| `str` | value | string | Convert to string |
| `int` | value | int | Convert to integer |
| `float` | value | float | Convert to float |
| `bool` | value | bool | Convert to boolean |
| `len` | collection | int | Get length |
| `range` | start, end, step? | array | Generate number range |
| `substr` | string, start, len? | string | Extract substring |
| `split` | string, delim | array | Split string |
| `join` | array, delim | string | Join array |
| `read_file` | path | string | Read file |
| `write_file` | path, content | null | Write file |
| `fetch` | url, options? | string | HTTP request |
| `env` | name | string/null | Get env var |
| `json_parse` | string | object | Parse JSON |
| `json_stringify` | value | string | To JSON |
| `abs` | number | number | Absolute value |
| `sqrt` | number | float | Square root |
| `pow` | base, exp | number | Power |
| `floor` | number | int | Round down |
| `ceil` | number | int | Round up |
| `round` | number | int | Round nearest |

## Next Steps

- [Array Methods](51-array-methods.md) - Array-specific methods
- [HTTP Client](40-http-client.md) - Advanced HTTP usage
- [File I/O](41-file-io.md) - Advanced file operations

---

**Master the built-in functions for powerful programming!** 🛠️
