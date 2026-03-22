// Test default parameters with arrow functions
let arrow = (x = 5) => {
    return x * 2
}

print("Arrow function: " + arrow())
print("Arrow function: " + arrow(10))

// Test with type annotations
fn typed(count: int = 42) {
    return count
}

print("Typed: " + typed())
print("Typed: " + typed(100))
