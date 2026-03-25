# KLang Basic Features Quick Reference

## Print Statements

### Basic Usage
```klang
print("text")        # Print without newline
println("text")      # Print with newline
println()            # Print blank line
```

### Multiple Arguments
```klang
println("x =", 5, ", y =", 3)
```

### Data Types
```klang
println(42)          # Integer
println(3.14)        # Float
println(true)        # Boolean
println(null)        # Null
println("text")      # String
```

## Arithmetic in Print

### Basic Operations
```klang
println(2 + 3)       # Addition: 5
println(10 - 4)      # Subtraction: 6
println(5 * 6)       # Multiplication: 30
println(20 / 4)      # Division: 5
println(17 % 5)      # Modulo: 2
```

### Power Operations
```klang
# Use Math.pow() - NOT **
println(Math.pow(2, 4))    # 16
println(Math.pow(3, 3))    # 27
```

### Complex Expressions
```klang
println((2 + 3) * 4)              # 20
println(5 + 3 * Math.pow(2, 2))   # 17
```

## Comparisons and Logic

### Comparison Operators
```klang
println(5 > 3)       # true
println(2 < 10)      # true
println(5 == 5)      # true
println(3 != 4)      # true
println(7 >= 7)      # true
println(4 <= 9)      # true
```

### Logical Operators
```klang
println(true && true)    # true
println(false || true)   # true
println(!true)           # false
```

## String Operations

### Concatenation
```klang
println("Hello " + "World")
let name = "KLang"
println("Hello, " + name)
```

### Number to String
```klang
println("Result: " + str(42))
let x = 100
println("Value: " + str(x))
```

## Comments

### Single-Line Comments
```klang
# Python-style comment
// C-style comment
let x = 5  # Inline comment
```

### Multi-Line Comments
```klang
/*
This is a multi-line comment
It can span multiple lines
*/
```

### Documentation Comments
```klang
/// This is a documentation comment
/// Used to document code
fn myFunction() {
    // Implementation
}
```

## Variables in Calculations

```klang
let a = 5
let b = 3

println("a + b =", a + b)          # 8
println("a * b =", a * b)          # 15
println("Math.pow(a, b) =", Math.pow(a, b))  # 125
```

## Common Patterns

### Print formatted output
```klang
let name = "Alice"
let age = 30
println(name, "is", age, "years old")
```

### Print calculations
```klang
let total = 100
let discount = 20
println("Final price:", total - discount)
```

### Print with variables and text
```klang
let result = Math.pow(2, 8)
println("2 to the power of 8 is:", result)
```

## Important Notes

⚠️ **The `**` operator is NOT supported**
- Use `Math.pow(base, exponent)` instead
- ❌ `println(2 ** 4)` - Does not work
- ✅ `println(Math.pow(2, 4))` - Use this

✅ **All three comment styles work**
- `#` for quick comments
- `//` for C-style comments
- `/* */` for multi-line blocks

✅ **Print accepts multiple types**
- Numbers, strings, booleans, null
- Multiple arguments separated by commas
- Automatic type conversion for display
