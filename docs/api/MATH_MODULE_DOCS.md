# KLang Math Module Documentation

## Overview
The Math module provides comprehensive mathematical functions and constants for KLang, similar to JavaScript's Math object and Python's math module.

## Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `Math.PI` | 3.14159... | The ratio of a circle's circumference to its diameter |
| `Math.E` | 2.71828... | Euler's number, the base of natural logarithms |
| `Math.TAU` | 6.28318... | The ratio of a circle's circumference to its radius (2π) |

## Basic Functions

### abs(x)
Returns the absolute value of a number.
```javascript
abs(-5)      // 5
abs(-3.14)   // 3.14
```

### min(...values)
Returns the smallest of the given numbers.
```javascript
min(5, 3, 8, 1)  // 1
min(2.5, 1.5)    // 1.5
```

### max(...values)
Returns the largest of the given numbers.
```javascript
max(5, 3, 8, 1)  // 8
max(2.5, 1.5)    // 2.5
```

## Rounding Functions

### Math.ceil(x)
Rounds a number UP to the nearest integer.
```javascript
Math.ceil(3.2)   // 4
Math.ceil(-3.8)  // -3
```

### Math.floor(x)
Rounds a number DOWN to the nearest integer.
```javascript
Math.floor(3.8)  // 3
Math.floor(-3.2) // -4
```

### Math.round(x)
Rounds a number to the nearest integer.
```javascript
Math.round(3.5)  // 4
Math.round(3.4)  // 3
```

## Power and Root Functions

### Math.pow(base, exponent)
Returns base raised to the exponent power.
```javascript
Math.pow(2, 3)   // 8  (2³)
Math.pow(5, 2)   // 25 (5²)
```

### Math.sqrt(x)
Returns the square root of a number.
```javascript
Math.sqrt(16)    // 4
Math.sqrt(25)    // 5
```

## Exponential and Logarithmic Functions

### Math.exp(x)
Returns e raised to the power of x (eˣ).
```javascript
Math.exp(1)      // 2.71828... (e)
Math.exp(2)      // 7.38906...
```

### Math.log(x)
Returns the natural logarithm (base e) of x.
```javascript
Math.log(Math.E) // 1
Math.log(10)     // 2.30259...
```

### Math.log10(x)
Returns the base 10 logarithm of x.
```javascript
Math.log10(100)  // 2
Math.log10(1000) // 3
```

## Trigonometric Functions
All trigonometric functions work with radians.

### Math.sin(x)
Returns the sine of x (in radians).
```javascript
Math.sin(0)              // 0
Math.sin(Math.PI / 2)    // 1
```

### Math.cos(x)
Returns the cosine of x (in radians).
```javascript
Math.cos(0)      // 1
Math.cos(Math.PI) // -1
```

### Math.tan(x)
Returns the tangent of x (in radians).
```javascript
Math.tan(0)              // 0
Math.tan(Math.PI / 4)    // 1
```

## Inverse Trigonometric Functions

### Math.asin(x)
Returns the arcsine of x (in radians).
```javascript
Math.asin(0)     // 0
Math.asin(1)     // π/2
```

### Math.acos(x)
Returns the arccosine of x (in radians).
```javascript
Math.acos(1)     // 0
Math.acos(0)     // π/2
```

### Math.atan(x)
Returns the arctangent of x (in radians).
```javascript
Math.atan(0)     // 0
Math.atan(1)     // π/4
```

### Math.atan2(y, x)
Returns the arctangent of y/x (in radians), using signs to determine quadrant.
```javascript
Math.atan2(0, 1)  // 0
Math.atan2(1, 0)  // π/2
```

## Hyperbolic Functions

### Math.sinh(x)
Returns the hyperbolic sine of x.
```javascript
Math.sinh(0)     // 0
```

### Math.cosh(x)
Returns the hyperbolic cosine of x.
```javascript
Math.cosh(0)     // 1
```

### Math.tanh(x)
Returns the hyperbolic tangent of x.
```javascript
Math.tanh(0)     // 0
```

## Practical Examples

### Calculate Circle Area
```javascript
let radius = 5.0
let area = Math.PI * Math.pow(radius, 2)
// area = 78.5398
```

### Calculate Distance Between Points
```javascript
let x1 = 0.0, y1 = 0.0
let x2 = 3.0, y2 = 4.0
let distance = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2))
// distance = 5.0
```

### Convert Degrees to Radians
```javascript
let degrees = 90.0
let radians = degrees * Math.PI / 180.0
// radians = 1.5708 (π/2)
```

### Solve Quadratic Equation
```javascript
fn solveQuadratic(a, b, c) {
    let discriminant = Math.pow(b, 2) - 4 * a * c
    if discriminant < 0 {
        return None  // No real solutions
    }
    let sqrt_d = Math.sqrt(discriminant)
    let x1 = (-b + sqrt_d) / (2 * a)
    let x2 = (-b - sqrt_d) / (2 * a)
    return (x1, x2)
}
```

## Compatibility

### JavaScript
KLang's Math module is fully compatible with JavaScript's Math object.

### Python
Most functions are compatible with Python's `math` module:
- `abs()` - Built-in in both
- `Math.ceil()` → `math.ceil()`
- `Math.floor()` → `math.floor()`
- `Math.sqrt()` → `math.sqrt()`
- `Math.sin()` → `math.sin()`
- etc.

### Key Differences
- KLang uses `Math.PI` (object property) vs Python's `math.pi` (module attribute)
- Some functions are global in KLang: `abs()`, `min()`, `max()`
- All other functions require `Math.` prefix

## Performance Notes
- All math functions are implemented using C's standard math library (`math.h`)
- Operations are highly optimized and near-native performance
- No overhead from dynamic typing or object allocation
