// Test default parameters

// Test 1: Simple default parameter
fn greet(name = "World") {
    return "Hello, " + name
}

print(greet())
print(greet("Alice"))

// Test 2: Multiple default parameters
fn add(a = 10, b = 20) {
    return a + b
}

print(add())
print(add(5))
print(add(5, 15))

// Test 3: Mix of regular and default parameters
fn format(prefix, suffix = "!") {
    return prefix + suffix
}

print(format("Hello"))
print(format("Hello", "?"))

// Test 4: Default parameter with expression
fn multiply(x = 5, y = x * 2) {
    return x * y
}

print(multiply())
print(multiply(3))
print(multiply(3, 4))

// Test 5: Default parameter with different types
fn describe(name = "unknown", count = 0) {
    return name + " has " + count + " items"
}

print(describe())
print(describe("Box"))
print(describe("Box", 5))
