// Test default parameters

// Test 1: Simple default parameter
fn greet(name = "World") {
    return "Hello, " + name
}

let result1 = greet()
let result2 = greet("Alice")
print("Test 1a: " + result1)  // Should be "Hello, World"
print("Test 1b: " + result2)  // Should be "Hello, Alice"

// Test 2: Multiple default parameters
fn add(a = 10, b = 20) {
    return a + b
}

let result3 = add()
let result4 = add(5)
let result5 = add(5, 15)
print("Test 2a: " + result3)  // Should be 30
print("Test 2b: " + result4)  // Should be 25 (5 + 20)
print("Test 2c: " + result5)  // Should be 20

// Test 3: Mix of regular and default parameters
fn format(prefix, suffix = "!") {
    return prefix + suffix
}

let result6 = format("Hello")
let result7 = format("Hello", "?")
print("Test 3a: " + result6)  // Should be "Hello!"
print("Test 3b: " + result7)  // Should be "Hello?"

// Test 4: All parameters have defaults
fn multiply(x = 5, y = 10) {
    return x * y
}

let result8 = multiply()
let result9 = multiply(3)
let result10 = multiply(3, 4)
print("Test 4a: " + result8)   // Should be 50
print("Test 4b: " + result9)   // Should be 30 (3 * 10)
print("Test 4c: " + result10)  // Should be 12
