// Comprehensive demo of default parameters in KLang

print("=== Default Parameters Demo ===")
print("")

// Example 1: Simple greeting function
fn greet(name = "World") {
    return "Hello, " + name
}
print("1. Simple default:")
print("   greet() = " + greet())
print("   greet('Alice') = " + greet("Alice"))
print("")

// Example 2: Math functions
fn power(base = 2, exponent = 3) {
    let result = 1
    for (let i = 0; i < exponent; i = i + 1) {
        result = result * base
    }
    return result
}
print("2. Multiple defaults:")
print("   power() = " + power())            // 2^3 = 8
print("   power(5) = " + power(5))          // 5^3 = 125
print("   power(2, 10) = " + power(2, 10))  // 2^10 = 1024
print("")

// Example 3: String formatting
fn formatMessage(prefix, suffix = "!", repeat = 1) {
    let result = prefix + suffix
    if (repeat > 1) {
        for (let i = 1; i < repeat; i = i + 1) {
            result = result + " " + prefix + suffix
        }
    }
    return result
}
print("3. Mix of required and default:")
print("   formatMessage('Hi') = " + formatMessage("Hi"))
print("   formatMessage('Hi', '?') = " + formatMessage("Hi", "?"))
print("   formatMessage('Go', '!', 3) = " + formatMessage("Go", "!", 3))
print("")

// Example 4: With type annotations
fn calculate(x: int = 10, y: int = 5, operation = "+") {
    if (operation == "+") {
        return x + y
    }
    if (operation == "-") {
        return x - y
    }
    if (operation == "*") {
        return x * y
    }
    return 0
}
print("4. Type annotations with defaults:")
print("   calculate() = " + calculate())                        // 10 + 5
print("   calculate(20) = " + calculate(20))                    // 20 + 5
print("   calculate(20, 3) = " + calculate(20, 3))              // 20 + 3
print("   calculate(20, 3, '*') = " + calculate(20, 3, "*"))    // 20 * 3
print("")

// Example 5: Dependent defaults
fn makeRange(start = 0, end = start + 10, step = 1) {
    let values = []
    for (let i = start; i < end; i = i + step) {
        values.push(i)
    }
    return values
}
print("5. Dependent defaults:")
print("   makeRange() = " + makeRange())           // [0..10]
print("   makeRange(5) = " + makeRange(5))         // [5..15]
print("   makeRange(0, 5, 2) = " + makeRange(0, 5, 2))  // [0,2,4]
print("")

print("=== All demos completed! ===")
