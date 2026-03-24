# String Methods Reference

## Introduction

KLang provides comprehensive string manipulation methods and functions. Strings in KLang are immutable, so all methods return new strings rather than modifying the original.

## Case Conversion

### `uppercase(string)`

Convert string to uppercase.

```klang
let text = "hello world"
println(uppercase(text))  # "HELLO WORLD"

let greeting = "hi"
println(uppercase(greeting))  # "HI"
```

### `lowercase(string)`

Convert string to lowercase.

```klang
let text = "HELLO WORLD"
println(lowercase(text))  # "hello world"

let shout = "STOP!"
println(lowercase(shout))  # "stop!"
```

## String Searching

### `indexOf(string, search)`

Find first occurrence of substring.

```klang
let text = "Hello, World!"
println(text.indexOf("World"))  # 7
println(text.indexOf("xyz"))    # -1 (not found)

let email = "user@example.com"
let at_pos = email.indexOf("@")
println(at_pos)  # 4
```

### `lastIndexOf(string, search)`

Find last occurrence of substring.

```klang
let text = "hello hello hello"
println(text.lastIndexOf("hello"))  # 12
println(text.lastIndexOf("hi"))     # -1
```

### `includes(string, search)`

Check if string contains substring.

```klang
let text = "Hello, World!"
println(text.includes("World"))  # true
println(text.includes("xyz"))    # false

let url = "https://example.com"
if url.includes("https") {
    println("Secure URL")
}
```

### `startsWith(string, prefix)`

Check if string starts with prefix.

```klang
let url = "https://example.com"
println(url.startsWith("https"))  # true
println(url.startsWith("http"))   # true
println(url.startsWith("ftp"))    # false
```

### `endsWith(string, suffix)`

Check if string ends with suffix.

```klang
let filename = "document.pdf"
println(filename.endsWith(".pdf"))  # true
println(filename.endsWith(".doc"))  # false

let files = ["image.jpg", "data.json", "style.css"]
let json_files = files.filter(f => f.endsWith(".json"))
println(json_files)  # ["data.json"]
```

## String Extraction

### `substr(string, start, length?)`

Extract substring by position and length.

```klang
let text = "Hello, World!"
println(substr(text, 0, 5))    # "Hello"
println(substr(text, 7, 5))    # "World"
println(substr(text, 7))       # "World!"
```

### `substring(string, start, end?)`

Extract substring by start and end positions.

```klang
let text = "Hello, World!"
println(text.substring(0, 5))   # "Hello"
println(text.substring(7, 12))  # "World"
println(text.substring(7))      # "World!"
```

### `slice(string, start, end?)`

Extract portion of string (supports negative indices).

```klang
let text = "Hello, World!"
println(text.slice(0, 5))    # "Hello"
println(text.slice(-6, -1))  # "World"
println(text.slice(-6))      # "World!"
```

### `charAt(string, index)`

Get character at position.

```klang
let text = "Hello"
println(text.charAt(0))  # "H"
println(text.charAt(4))  # "o"
```

## String Splitting and Joining

### `split(string, delimiter)`

Split string into array.

```klang
let csv = "apple,banana,orange"
let fruits = split(csv, ",")
println(fruits)  # ["apple", "banana", "orange"]

let sentence = "Hello World from KLang"
let words = split(sentence, " ")
println(words)  # ["Hello", "World", "from", "KLang"]

# Split on empty string gets characters
let text = "abc"
let chars = split(text, "")
println(chars)  # ["a", "b", "c"]
```

### `join(array, delimiter)`

Join array elements into string.

```klang
let words = ["Hello", "World"]
println(join(words, " "))   # "Hello World"
println(join(words, "-"))   # "Hello-World"

let numbers = [1, 2, 3]
println(join(numbers, ","))  # "1,2,3"
```

## String Trimming

### `trim(string)`

Remove whitespace from both ends.

```klang
let text = "  hello world  "
println(trim(text))  # "hello world"

let input = "\n\t  data  \t\n"
println(trim(input))  # "data"
```

### `trimStart(string)` / `trimLeft(string)`

Remove whitespace from start.

```klang
let text = "  hello"
println(trimStart(text))  # "hello"
```

### `trimEnd(string)` / `trimRight(string)`

Remove whitespace from end.

```klang
let text = "hello  "
println(trimEnd(text))  # "hello"
```

## String Replacement

### `replace(string, search, replacement)`

Replace first occurrence.

```klang
let text = "Hello World, Hello KLang"
println(text.replace("Hello", "Hi"))  # "Hi World, Hello KLang"

let path = "/home/user/file.txt"
println(path.replace("/home", "/root"))  # "/root/user/file.txt"
```

### `replaceAll(string, search, replacement)`

Replace all occurrences.

```klang
let text = "Hello World, Hello KLang"
println(text.replaceAll("Hello", "Hi"))  # "Hi World, Hi KLang"

let code = "var x = 10; var y = 20;"
println(code.replaceAll("var", "let"))  # "let x = 10; let y = 20;"
```

## String Padding

### `padStart(string, length, padString?)`

Pad start of string.

```klang
let num = "5"
println(num.padStart(3, "0"))  # "005"

let id = "42"
println(id.padStart(8, "0"))  # "00000042"
```

### `padEnd(string, length, padString?)`

Pad end of string.

```klang
let text = "hello"
println(text.padEnd(10, "."))  # "hello....."
```

## String Repetition

### `repeat(string, count)`

Repeat string multiple times.

```klang
let dash = "-"
println(dash.repeat(20))  # "--------------------"

let star = "*"
println(star.repeat(5))  # "*****"
```

## String Comparison

### String comparison operators work directly:

```klang
println("apple" == "apple")  # true
println("apple" < "banana")  # true (alphabetical)
println("10" < "2")          # true (string comparison, not numeric!)

# Case-insensitive comparison
let a = lowercase("HELLO")
let b = lowercase("hello")
println(a == b)  # true
```

## String Length

### `len(string)`

Get string length.

```klang
println(len("hello"))       # 5
println(len(""))            # 0
println(len("Hello World")) # 11

let password = "mypass123"
if len(password) < 8 {
    println("Password too short!")
}
```

## Character Code Operations

### `charCodeAt(string, index)`

Get character code at position.

```klang
let text = "ABC"
println(text.charCodeAt(0))  # 65
println(text.charCodeAt(1))  # 66
println(text.charCodeAt(2))  # 67
```

### `fromCharCode(code)`

Create character from code.

```klang
println(fromCharCode(65))  # "A"
println(fromCharCode(97))  # "a"
```

## Practical Examples

### Example 1: Email Validation

```klang
fn isValidEmail(email) {
    if !email.includes("@") {
        return false
    }
    let parts = split(email, "@")
    if len(parts) != 2 {
        return false
    }
    let domain = parts[1]
    return domain.includes(".")
}

println(isValidEmail("user@example.com"))  # true
println(isValidEmail("invalid.email"))     # false
```

### Example 2: Format Name

```klang
fn formatName(firstName, lastName) {
    let first = uppercase(substr(firstName, 0, 1)) + lowercase(substr(firstName, 1))
    let last = uppercase(substr(lastName, 0, 1)) + lowercase(substr(lastName, 1))
    return first + " " + last
}

println(formatName("JOHN", "DOE"))  # "John Doe"
println(formatName("alice", "smith"))  # "Alice Smith"
```

### Example 3: Parse CSV

```klang
fn parseCSV(csv) {
    let lines = split(csv, "\n")
    let rows = []
    
    lines.forEach(line => {
        let cells = split(line, ",")
        rows.push(cells)
    })
    
    return rows
}

let csv = "name,age,city\nAlice,30,NYC\nBob,25,LA"
let data = parseCSV(csv)
println(data)
# [["name", "age", "city"], ["Alice", "30", "NYC"], ["Bob", "25", "LA"]]
```

### Example 4: URL Parser

```klang
fn parseURL(url) {
    let protocol = ""
    let domain = ""
    
    if url.startsWith("https://") {
        protocol = "https"
        domain = substr(url, 8)
    } else if url.startsWith("http://") {
        protocol = "http"
        domain = substr(url, 7)
    }
    
    let path_start = domain.indexOf("/")
    if path_start != -1 {
        domain = substr(domain, 0, path_start)
    }
    
    return {protocol: protocol, domain: domain}
}

let info = parseURL("https://example.com/path")
println(info.protocol)  # "https"
println(info.domain)    # "example.com"
```

### Example 5: Slug Generator

```klang
fn createSlug(title) {
    let slug = lowercase(title)
    slug = slug.replace(" ", "-")
    slug = slug.replaceAll(" ", "-")
    return slug
}

println(createSlug("My Blog Post Title"))  # "my-blog-post-title"
println(createSlug("Hello World 2024"))    # "hello-world-2024"
```

## Method Chaining

String methods can be chained together:

```klang
let input = "  HELLO WORLD  "
let result = trim(lowercase(input))
println(result)  # "hello world"

let text = "  KLang Programming  "
let formatted = lowercase(trim(text)).replace(" ", "-")
println(formatted)  # "klang-programming"
```

## Complete Reference

| Method/Function | Parameters | Returns | Description |
|----------------|-----------|---------|-------------|
| `uppercase` | string | string | Convert to uppercase |
| `lowercase` | string | string | Convert to lowercase |
| `indexOf` | string, search | int | First occurrence index |
| `lastIndexOf` | string, search | int | Last occurrence index |
| `includes` | string, search | bool | Contains substring |
| `startsWith` | string, prefix | bool | Starts with prefix |
| `endsWith` | string, suffix | bool | Ends with suffix |
| `substr` | string, start, len? | string | Extract by length |
| `substring` | string, start, end? | string | Extract by position |
| `slice` | string, start, end? | string | Extract (negative indices) |
| `charAt` | string, index | string | Get character at index |
| `charCodeAt` | string, index | int | Get char code |
| `fromCharCode` | code | string | Create from code |
| `split` | string, delim | array | Split to array |
| `join` | array, delim | string | Join from array |
| `trim` | string | string | Remove whitespace |
| `trimStart` | string | string | Remove start whitespace |
| `trimEnd` | string | string | Remove end whitespace |
| `replace` | string, old, new | string | Replace first |
| `replaceAll` | string, old, new | string | Replace all |
| `padStart` | string, len, pad? | string | Pad start |
| `padEnd` | string, len, pad? | string | Pad end |
| `repeat` | string, count | string | Repeat string |
| `len` | string | int | Get length |

## Tips & Best Practices

### 1. Immutability

Strings are immutable - methods return new strings:

```klang
let text = "hello"
uppercase(text)  # Returns new string
println(text)    # Still "hello"

let upper = uppercase(text)  # Store result
println(upper)  # "HELLO"
```

### 2. Check Before Accessing

```klang
let text = "hello"
if len(text) > 5 {
    println(text.charAt(5))
}
```

### 3. Combine with Array Methods

```klang
let words = ["APPLE", "BANANA", "CHERRY"]
let lower = words.map(w => lowercase(w))
println(lower)  # ["apple", "banana", "cherry"]
```

### 4. String Building

For building strings, use concatenation or arrays:

```klang
# Concatenation
let name = "Alice"
let age = 30
let info = "Name: " + name + ", Age: " + str(age)

# Array joining (more efficient for many parts)
let parts = ["Name:", name, "Age:", str(age)]
let info = join(parts, " ")
```

## Common Patterns

### Pattern 1: Input Sanitization

```klang
fn sanitizeInput(input) {
    let cleaned = trim(input)
    cleaned = lowercase(cleaned)
    return cleaned
}

let user_input = "  HELLO  "
println(sanitizeInput(user_input))  # "hello"
```

### Pattern 2: Title Case

```klang
fn toTitleCase(text) {
    let words = split(text, " ")
    let titleWords = words.map(word => {
        if len(word) == 0 {
            return word
        }
        let first = uppercase(substr(word, 0, 1))
        let rest = lowercase(substr(word, 1))
        return first + rest
    })
    return join(titleWords, " ")
}

println(toTitleCase("hello world"))  # "Hello World"
println(toTitleCase("the quick BROWN fox"))  # "The Quick Brown Fox"
```

### Pattern 3: Word Count

```klang
fn countWords(text) {
    let trimmed = trim(text)
    if len(trimmed) == 0 {
        return 0
    }
    let words = split(trimmed, " ")
    return len(words)
}

println(countWords("Hello World"))      # 2
println(countWords("  one two three "))  # 3
```

### Pattern 4: String Templating

```klang
fn formatTemplate(template, data) {
    let result = template
    for key of data {
        let placeholder = "{" + key + "}"
        result = result.replaceAll(placeholder, data[key])
    }
    return result
}

let template = "Hello {name}, you are {age} years old!"
let data = {name: "Alice", age: "30"}
println(formatTemplate(template, data))
# "Hello Alice, you are 30 years old!"
```

### Pattern 5: Truncate with Ellipsis

```klang
fn truncate(text, maxLength) {
    if len(text) <= maxLength {
        return text
    }
    return substr(text, 0, maxLength - 3) + "..."
}

let long_text = "This is a very long text that needs truncation"
println(truncate(long_text, 20))  # "This is a very lo..."
println(truncate("Short", 20))     # "Short"
```

## Unicode and Special Characters

```klang
# KLang supports UTF-8 strings
let emoji = "Hello 👋 World 🌍"
println(emoji)
println(len(emoji))  # Length in characters

let unicode = "Café"
println(unicode)
println(lowercase(unicode))  # "café"
```

## Escape Sequences

```klang
println("Line 1\nLine 2")      # Newline
println("Tab\there")           # Tab
println("Quote: \"Hello\"")    # Escaped quotes
println("Backslash: \\")       # Escaped backslash
println("Path: C:\\Users")     # Windows path
```

## String Interpolation (Alternative)

While KLang doesn't have template literals yet, you can create a helper:

```klang
fn interpolate(str, values) {
    let result = str
    let i = 0
    while result.includes("{}") {
        if i < len(values) {
            result = result.replace("{}", str(values[i]))
            i = i + 1
        } else {
            break
        }
    }
    return result
}

let message = interpolate("Hello {}, you have {} messages", ["Alice", 5])
println(message)  # "Hello Alice, you have 5 messages"
```

## Next Steps

- [Array Methods](51-array-methods.md) - Array manipulation methods
- [Math Functions](53-math-functions.md) - Mathematical operations
- [Built-in Functions](50-built-in-functions.md) - Core functions

---

**Master string manipulation for powerful text processing!** 📝
