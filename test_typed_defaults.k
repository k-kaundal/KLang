// Test with type annotations
fn typed(count: int = 42) {
    return count
}

print("Typed with default: " + typed())
print("Typed with arg: " + typed(100))

// Test with multiple params and types
fn calculate(x: int = 10, y: int = 20) {
    return x + y
}

print("Calculate: " + calculate())
print("Calculate: " + calculate(5))
print("Calculate: " + calculate(5, 10))
