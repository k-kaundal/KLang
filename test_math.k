# Math Module Comprehensive Test

print("=== KLang Math Module Test ===\n")

# Test Math constants
print("Math Constants:")
print("Math.PI = " + str(Math.PI))
print("Math.E = " + str(Math.E))
print("Math.TAU = " + str(Math.TAU))
print("")

# Test basic arithmetic
print("Basic Arithmetic:")
print("abs(-5) = " + str(abs(-5)))
print("abs(-3.14) = " + str(abs(-3.14)))
print("min(5, 3, 8, 1) = " + str(min(5, 3, 8, 1)))
print("max(5, 3, 8, 1) = " + str(max(5, 3, 8, 1)))
print("")

# Test rounding functions
print("Rounding Functions:")
print("Math.ceil(3.2) = " + str(Math.ceil(3.2)))
print("Math.floor(3.8) = " + str(Math.floor(3.8)))
print("Math.round(3.5) = " + str(Math.round(3.5)))
print("Math.round(3.4) = " + str(Math.round(3.4)))
print("")

# Test power and roots
print("Power and Roots:")
print("Math.pow(2, 3) = " + str(Math.pow(2, 3)))
print("Math.pow(5, 2) = " + str(Math.pow(5, 2)))
print("Math.sqrt(16) = " + str(Math.sqrt(16)))
print("Math.sqrt(25) = " + str(Math.sqrt(25)))
print("")

# Test exponential and logarithmic
print("Exponential and Logarithmic:")
print("Math.exp(1) ≈ " + str(Math.exp(1)))
print("Math.log(Math.E) ≈ " + str(Math.log(Math.E)))
print("Math.log10(100) = " + str(Math.log10(100)))
print("Math.log10(1000) = " + str(Math.log10(1000)))
print("")

# Test trigonometric functions (in radians)
print("Trigonometric Functions:")
print("Math.sin(0) = " + str(Math.sin(0)))
print("Math.cos(0) = " + str(Math.cos(0)))
print("Math.tan(0) = " + str(Math.tan(0)))
print("Math.sin(Math.PI / 2) ≈ " + str(Math.sin(Math.PI / 2)))
print("Math.cos(Math.PI) ≈ " + str(Math.cos(Math.PI)))
print("")

# Test inverse trigonometric functions
print("Inverse Trigonometric Functions:")
print("Math.asin(0) = " + str(Math.asin(0)))
print("Math.acos(1) = " + str(Math.acos(1)))
print("Math.atan(0) = " + str(Math.atan(0)))
print("Math.atan2(0, 1) = " + str(Math.atan2(0, 1)))
print("")

# Test hyperbolic functions
print("Hyperbolic Functions:")
print("Math.sinh(0) = " + str(Math.sinh(0)))
print("Math.cosh(0) = " + str(Math.cosh(0)))
print("Math.tanh(0) = " + str(Math.tanh(0)))
print("")

# Practical examples
print("Practical Examples:")

# Calculate circle area
let radius = 5.0
let area = Math.PI * Math.pow(radius, 2)
print("Circle area (r=5): " + str(area))

# Calculate distance between two points
let x1 = 0.0
let y1 = 0.0
let x2 = 3.0
let y2 = 4.0
let distance = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2))
print("Distance between (0,0) and (3,4): " + str(distance))

# Convert degrees to radians
let degrees = 90.0
let radians = degrees * Math.PI / 180.0
print("90 degrees in radians: " + str(radians))
print("sin(90°) = " + str(Math.sin(radians)))

print("\n=== All Math Tests Completed! ===")
