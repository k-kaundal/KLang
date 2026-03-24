# Math Functions Reference

## Introduction

KLang provides a comprehensive set of mathematical functions for numerical computations. These functions work with both integers and floating-point numbers.

## Basic Arithmetic

### `abs(number)`

Get absolute value.

```klang
println(abs(-5))      # 5
println(abs(5))       # 5
println(abs(-3.14))   # 3.14
println(abs(0))       # 0
```

### `pow(base, exponent)`

Raise to power.

```klang
println(pow(2, 3))    # 8
println(pow(10, 2))   # 100
println(pow(5, 0))    # 1
println(pow(2, -1))   # 0.5
```

### `sqrt(number)`

Calculate square root.

```klang
println(sqrt(16))     # 4.0
println(sqrt(2))      # 1.414...
println(sqrt(100))    # 10.0

# For perfect squares
let side = sqrt(64)
println("Square side: " + str(side))  # 8.0
```

## Rounding Functions

### `floor(number)`

Round down to nearest integer.

```klang
println(floor(3.7))   # 3
println(floor(3.2))   # 3
println(floor(-3.7))  # -4
println(floor(5))     # 5
```

### `ceil(number)`

Round up to nearest integer.

```klang
println(ceil(3.2))    # 4
println(ceil(3.7))    # 4
println(ceil(-3.7))   # -3
println(ceil(5))      # 5
```

### `round(number)`

Round to nearest integer.

```klang
println(round(3.4))   # 3
println(round(3.5))   # 4
println(round(3.6))   # 4
println(round(-3.5))  # -4
```

## Trigonometric Functions

### `sin(angle)`

Sine of angle (radians).

```klang
println(sin(0))       # 0.0
println(sin(PI / 2))  # 1.0

# Convert degrees to radians
let degrees = 90
let radians = degrees * PI / 180
println(sin(radians))  # 1.0
```

### `cos(angle)`

Cosine of angle (radians).

```klang
println(cos(0))       # 1.0
println(cos(PI))      # -1.0
```

### `tan(angle)`

Tangent of angle (radians).

```klang
println(tan(0))       # 0.0
println(tan(PI / 4))  # 1.0
```

### `asin(value)`, `acos(value)`, `atan(value)`

Inverse trigonometric functions.

```klang
println(asin(1))      # PI / 2
println(acos(0))      # PI / 2
println(atan(1))      # PI / 4
```

### `atan2(y, x)`

Two-argument arctangent.

```klang
let angle = atan2(1, 1)  # Angle of point (1, 1)
println(angle)  # PI / 4
```

## Exponential & Logarithmic

### `exp(number)`

Euler's number raised to power.

```klang
println(exp(0))   # 1.0
println(exp(1))   # 2.718... (e)
println(exp(2))   # 7.389...
```

### `log(number)`

Natural logarithm (base e).

```klang
println(log(1))      # 0.0
println(log(E))      # 1.0
println(log(10))     # 2.302...
```

### `log10(number)`

Base-10 logarithm.

```klang
println(log10(10))    # 1.0
println(log10(100))   # 2.0
println(log10(1000))  # 3.0
```

### `log2(number)`

Base-2 logarithm.

```klang
println(log2(2))   # 1.0
println(log2(8))   # 3.0
println(log2(256)) # 8.0
```

## Min/Max Functions

### `min(a, b, ...)`

Get minimum value.

```klang
println(min(5, 10))        # 5
println(min(3, 1, 7, 2))   # 1
println(min(-5, -10))      # -10

# With array
let numbers = [5, 2, 8, 1, 9]
let minimum = numbers.reduce((a, b) => min(a, b))
println(minimum)  # 1
```

### `max(a, b, ...)`

Get maximum value.

```klang
println(max(5, 10))        # 10
println(max(3, 1, 7, 2))   # 7
println(max(-5, -10))      # -5

# With array
let numbers = [5, 2, 8, 1, 9]
let maximum = numbers.reduce((a, b) => max(a, b))
println(maximum)  # 9
```

## Random Number Generation

### `random()`

Generate random float between 0 and 1.

```klang
let r = random()
println(r)  # 0.742... (random)

# Random integer between 1 and 100
let dice = floor(random() * 100) + 1
println(dice)
```

### `randomInt(min, max)`

Generate random integer in range.

```klang
let dice = randomInt(1, 6)
println("Rolled: " + str(dice))

# Random password digit
let digit = randomInt(0, 9)
```

## Constants

KLang provides mathematical constants:

```klang
const PI = 3.14159265358979323846
const E = 2.71828182845904523536

println("Pi: " + str(PI))
println("e: " + str(E))

# Circle calculations
let radius = 5
let area = PI * pow(radius, 2)
let circumference = 2 * PI * radius
println("Area: " + str(area))
println("Circumference: " + str(circumference))
```

## Sign Functions

### `sign(number)`

Get sign of number.

```klang
println(sign(5))     # 1
println(sign(-5))    # -1
println(sign(0))     # 0
```

## Practical Examples

### Example 1: Distance Calculator

```klang
fn distance(x1, y1, x2, y2) {
    let dx = x2 - x1
    let dy = y2 - y1
    return sqrt(pow(dx, 2) + pow(dy, 2))
}

let dist = distance(0, 0, 3, 4)
println("Distance: " + str(dist))  # 5.0
```

### Example 2: Angle Conversion

```klang
fn degreesToRadians(degrees) {
    return degrees * PI / 180
}

fn radiansToDegrees(radians) {
    return radians * 180 / PI
}

println(degreesToRadians(90))   # 1.570... (PI/2)
println(radiansToDegrees(PI))   # 180.0
```

### Example 3: Statistical Functions

```klang
fn mean(numbers) {
    let sum = numbers.reduce((a, b) => a + b, 0)
    return sum / len(numbers)
}

fn median(numbers) {
    let sorted = numbers.sort((a, b) => a - b)
    let mid = floor(len(sorted) / 2)
    if len(sorted) % 2 == 0 {
        return (sorted[mid - 1] + sorted[mid]) / 2
    }
    return sorted[mid]
}

let data = [5, 2, 8, 1, 9, 3]
println("Mean: " + str(mean(data)))      # 4.666...
println("Median: " + str(median(data)))  # 4.0
```

### Example 4: Percentage Calculator

```klang
fn percentage(value, total) {
    return (value / total) * 100
}

fn percentageChange(old, new) {
    return ((new - old) / old) * 100
}

println(percentage(25, 100))      # 25.0
println(percentageChange(50, 75)) # 50.0 (50% increase)
```

### Example 5: Compound Interest

```klang
fn compoundInterest(principal, rate, years) {
    # A = P(1 + r)^t
    let amount = principal * pow(1 + rate, years)
    return amount
}

let principal = 1000
let rate = 0.05  # 5%
let years = 10

let final = compoundInterest(principal, rate, years)
println("Final amount: $" + str(round(final)))  # $1628
```

### Example 6: Prime Number Check

```klang
fn isPrime(n) {
    if n < 2 {
        return false
    }
    if n == 2 {
        return true
    }
    if n % 2 == 0 {
        return false
    }
    
    let limit = floor(sqrt(n))
    for i in range(3, limit + 1, 2) {
        if n % i == 0 {
            return false
        }
    }
    return true
}

println(isPrime(17))  # true
println(isPrime(18))  # false
println(isPrime(97))  # true
```

### Example 7: Factorial

```klang
fn factorial(n) {
    if n <= 1 {
        return 1
    }
    let result = 1
    for i in range(2, n + 1) {
        result = result * i
    }
    return result
}

println(factorial(5))  # 120
println(factorial(10)) # 3628800
```

### Example 8: GCD (Greatest Common Divisor)

```klang
fn gcd(a, b) {
    a = abs(a)
    b = abs(b)
    
    while b != 0 {
        let temp = b
        b = a % b
        a = temp
    }
    return a
}

println(gcd(48, 18))  # 6
println(gcd(100, 75)) # 25
```

### Example 9: Clamp Function

```klang
fn clamp(value, min_val, max_val) {
    return max(min_val, min(value, max_val))
}

println(clamp(5, 0, 10))   # 5
println(clamp(-5, 0, 10))  # 0
println(clamp(15, 0, 10))  # 10
```

### Example 10: Linear Interpolation

```klang
fn lerp(start, end, t) {
    return start + (end - start) * t
}

println(lerp(0, 100, 0.5))   # 50.0
println(lerp(10, 20, 0.25))  # 12.5
```

## Complete Reference

| Function | Parameters | Returns | Description |
|----------|-----------|---------|-------------|
| `abs` | number | number | Absolute value |
| `pow` | base, exp | number | Power |
| `sqrt` | number | float | Square root |
| `floor` | number | int | Round down |
| `ceil` | number | int | Round up |
| `round` | number | int | Round nearest |
| `sin` | angle | float | Sine (radians) |
| `cos` | angle | float | Cosine (radians) |
| `tan` | angle | float | Tangent (radians) |
| `asin` | value | float | Arcsine |
| `acos` | value | float | Arccosine |
| `atan` | value | float | Arctangent |
| `atan2` | y, x | float | Two-arg arctangent |
| `exp` | number | float | e^x |
| `log` | number | float | Natural log |
| `log10` | number | float | Base-10 log |
| `log2` | number | float | Base-2 log |
| `min` | ...numbers | number | Minimum value |
| `max` | ...numbers | number | Maximum value |
| `random` | - | float | Random [0, 1) |
| `randomInt` | min, max | int | Random integer |
| `sign` | number | int | Sign (-1, 0, 1) |

## Tips & Best Practices

### 1. Precision Handling

```klang
# Floating point precision
let result = 0.1 + 0.2
println(result)  # 0.30000000000000004

# Round for display
let display = round(result * 100) / 100
println(display)  # 0.3
```

### 2. Integer Division

```klang
# Integer division truncates
println(7 / 2)        # 3 (integer)

# Use float conversion for decimals
println(float(7) / 2) # 3.5
```

### 3. Range Checking

```klang
fn safeSqrt(n) {
    if n < 0 {
        println("Error: Cannot take square root of negative number")
        return null
    }
    return sqrt(n)
}
```

### 4. Performance Optimization

```klang
# Cache repeated calculations
let radius = 5
let r_squared = pow(radius, 2)
let area = PI * r_squared
let volume_sphere = (4.0 / 3.0) * PI * r_squared * radius
```

## Next Steps

- [Built-in Functions](50-built-in-functions.md) - Core functions
- [Array Methods](51-array-methods.md) - Array operations
- [Examples](60-examples-basic.md) - Practical examples

---

**Master mathematical operations for computational power!** 🔢
