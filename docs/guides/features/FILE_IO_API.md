# KLang File I/O API Documentation

## Overview

KLang provides comprehensive file I/O operations that allow programs to read, write, and manipulate files on the filesystem. The API includes both low-level file handle operations and high-level convenience functions.

## File Operations

### High-Level Convenience Functions

These functions handle file opening and closing automatically, making them ideal for simple file operations.

#### `readFile(path: string) -> string`

Reads the entire contents of a file and returns it as a string.

**Parameters:**
- `path` - Path to the file to read

**Returns:** File contents as a string, or empty string on error

**Example:**
```klang
let content = readFile("/tmp/data.txt")
println(content)
```

---

#### `writeFile(path: string, content: string) -> bool`

Writes content to a file, creating it if it doesn't exist, or overwriting if it does.

**Parameters:**
- `path` - Path to the file to write
- `content` - String content to write

**Returns:** `true` on success, `false` on error

**Example:**
```klang
let success = writeFile("/tmp/output.txt", "Hello World!")
if success {
    println("File written successfully")
}
```

---

#### `appendFile(path: string, content: string) -> bool`

Appends content to the end of a file, creating it if it doesn't exist.

**Parameters:**
- `path` - Path to the file
- `content` - String content to append

**Returns:** `true` on success, `false` on error

**Example:**
```klang
appendFile("/tmp/log.txt", "Log entry: " + timestamp + "\n")
```

---

### Low-Level File Handle Operations

These functions provide fine-grained control over file operations using file handles.

#### `fopen(path: string, mode: string) -> file`

Opens a file and returns a file handle for subsequent operations.

**Parameters:**
- `path` - Path to the file
- `mode` - File open mode:
  - `"r"` - Read mode (file must exist)
  - `"w"` - Write mode (creates or truncates file)
  - `"a"` - Append mode (creates or appends to file)
  - `"r+"` - Read/write mode (file must exist)
  - `"w+"` - Read/write mode (creates or truncates file)
  - `"a+"` - Read/append mode (creates or appends to file)

**Returns:** File handle object, or `null` on error

**Example:**
```klang
let file = fopen("/tmp/data.txt", "r")
if type(file) == "file" {
    println("File opened successfully")
}
```

---

#### `fclose(file: file) -> bool`

Closes an open file handle.

**Parameters:**
- `file` - File handle to close

**Returns:** `true` on success, `false` on error

**Example:**
```klang
let file = fopen("/tmp/data.txt", "r")
# ... operations ...
fclose(file)
```

---

#### `fread(file: file) -> string`

Reads the entire remaining content from an open file.

**Parameters:**
- `file` - Open file handle

**Returns:** File contents as string, or empty string on error

**Example:**
```klang
let file = fopen("/tmp/data.txt", "r")
let content = fread(file)
fclose(file)
println(content)
```

---

#### `freadline(file: file) -> string`

Reads a single line from an open file (up to newline or EOF).

**Parameters:**
- `file` - Open file handle

**Returns:** Line content (without newline), or empty string at EOF

**Example:**
```klang
let file = fopen("/tmp/data.txt", "r")
let line1 = freadline(file)
let line2 = freadline(file)
fclose(file)
```

---

#### `fwrite(file: file, content: string) -> bool`

Writes content to an open file.

**Parameters:**
- `file` - Open file handle (must be writable)
- `content` - String to write

**Returns:** `true` on success, `false` on error

**Example:**
```klang
let file = fopen("/tmp/output.txt", "w")
fwrite(file, "Line 1\n")
fwrite(file, "Line 2\n")
fclose(file)
```

---

### File Management Functions

#### `fexists(path: string) -> bool`

Checks if a file exists at the given path.

**Parameters:**
- `path` - Path to check

**Returns:** `true` if file exists, `false` otherwise

**Example:**
```klang
if fexists("/tmp/config.txt") {
    println("Config file found")
} else {
    println("Config file not found")
}
```

---

#### `fdelete(path: string) -> bool`

Deletes a file from the filesystem.

**Parameters:**
- `path` - Path to file to delete

**Returns:** `true` on success, `false` on error

**Example:**
```klang
if fdelete("/tmp/temp.txt") {
    println("File deleted")
}
```

---

#### `frename(oldpath: string, newpath: string) -> bool`

Renames or moves a file.

**Parameters:**
- `oldpath` - Current file path
- `newpath` - New file path

**Returns:** `true` on success, `false` on error

**Example:**
```klang
frename("/tmp/old.txt", "/tmp/new.txt")
```

---

#### `fsize(path: string) -> int`

Gets the size of a file in bytes.

**Parameters:**
- `path` - Path to file

**Returns:** File size in bytes, or `-1` on error

**Example:**
```klang
let size = fsize("/tmp/data.txt")
println("File size: " + str(size) + " bytes")
```

---

## Complete Examples

### Example 1: Simple Read and Write

```klang
# Write to a file
writeFile("/tmp/greeting.txt", "Hello, KLang!")

# Read from the file
let content = readFile("/tmp/greeting.txt")
println(content)  # Output: Hello, KLang!
```

### Example 2: Line-by-Line Processing

```klang
# Write multiple lines
writeFile("/tmp/data.txt", "Line 1\nLine 2\nLine 3\n")

# Read line by line
let file = fopen("/tmp/data.txt", "r")
let line1 = freadline(file)
let line2 = freadline(file)
let line3 = freadline(file)
fclose(file)

println(line1)  # Output: Line 1
println(line2)  # Output: Line 2
println(line3)  # Output: Line 3
```

### Example 3: Append to Log File

```klang
# Create initial log
writeFile("/tmp/app.log", "[START] Application started\n")

# Append log entries
appendFile("/tmp/app.log", "[INFO] Processing data\n")
appendFile("/tmp/app.log", "[INFO] Task completed\n")
appendFile("/tmp/app.log", "[END] Application stopped\n")

# Read entire log
let log = readFile("/tmp/app.log")
println(log)
```

### Example 4: File Management

```klang
# Check if file exists
if fexists("/tmp/old_data.txt") {
    # Get file size
    let size = fsize("/tmp/old_data.txt")
    println("File size: " + str(size))
    
    # Rename file
    if frename("/tmp/old_data.txt", "/tmp/new_data.txt") {
        println("File renamed successfully")
    }
}

# Clean up
if fexists("/tmp/new_data.txt") {
    fdelete("/tmp/new_data.txt")
    println("File deleted")
}
```

### Example 5: CSV Processing

```klang
# Write CSV data
let csv = "Name,Age,City\nAlice,30,NYC\nBob,25,LA\nCarol,35,Chicago\n"
writeFile("/tmp/data.csv", csv)

# Read and process line by line
let file = fopen("/tmp/data.csv", "r")
let header = freadline(file)
println("Header: " + header)

# Process each data row
let row1 = freadline(file)
let row2 = freadline(file)
let row3 = freadline(file)
fclose(file)

println("Row 1: " + row1)
println("Row 2: " + row2)
println("Row 3: " + row3)
```

### Example 6: Configuration File

```klang
# Write configuration
let config = "app_name=MyApp\nversion=1.0.0\ndebug=true\n"
writeFile("/tmp/config.ini", config)

# Read configuration
let file = fopen("/tmp/config.ini", "r")
let line = freadline(file)
while len(line) > 0 {
    println("Config: " + line)
    line = freadline(file)
}
fclose(file)
```

---

## Best Practices

### 1. Always Close Files

When using low-level operations, always close files to free system resources:

```klang
let file = fopen(path, mode)
# ... operations ...
fclose(file)  # Important!
```

### 2. Check Return Values

Check boolean return values to handle errors:

```klang
if writeFile(path, content) {
    println("Success")
} else {
    println("Error writing file")
}
```

### 3. Check File Existence

Check if files exist before operating on them:

```klang
if fexists(path) {
    let content = readFile(path)
    # ... process content ...
}
```

### 4. Use Convenience Functions When Possible

For simple operations, use high-level functions:

```klang
# Good - simple and clean
let content = readFile(path)

# Overkill for simple read
let file = fopen(path, "r")
let content = fread(file)
fclose(file)
```

### 5. Handle Large Files Carefully

For large files, use line-by-line reading:

```klang
let file = fopen("/tmp/large.txt", "r")
let line = freadline(file)
while len(line) > 0 {
    # Process line
    println(line)
    line = freadline(file)
}
fclose(file)
```

---

## Error Handling

Most file operations return boolean values or special values on error:

- **Boolean functions** (`fclose`, `fwrite`, `fdelete`, `frename`, `writeFile`, `appendFile`): Return `false` on error
- **`fopen`**: Returns `null` on error
- **`fread`, `freadline`, `readFile`**: Return empty string on error
- **`fsize`**: Returns `-1` on error

Example error handling:

```klang
let file = fopen(path, "r")
if type(file) != "file" {
    println("Error: Could not open file")
} else {
    let content = fread(file)
    fclose(file)
}
```

---

## Performance Notes

1. **Buffering**: File operations are buffered for performance
2. **Large Files**: Use `freadline` for memory-efficient processing of large files
3. **Multiple Operations**: Keep files open for multiple operations rather than opening/closing repeatedly
4. **Convenience Functions**: High-level functions (readFile, writeFile, appendFile) automatically handle opening and closing

---

## Platform Support

File I/O operations are supported on all platforms where KLang runs:
- ✅ Linux
- ✅ macOS  
- ✅ Windows (via POSIX compatibility layer)

---

## See Also

- [stdlib/io.kl](../stdlib/io.kl) - I/O helper functions
- [examples/file_io_test.kl](../examples/file_io_test.kl) - Comprehensive test examples
- [KLang Language Reference](LANGUAGE_REFERENCE.md) - Full language documentation
