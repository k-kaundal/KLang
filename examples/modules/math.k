// math.k - Example module with various exports

export const PI = 3.14159
export const E = 2.71828

export fn add(a, b) {
    return a + b
}

export fn subtract(a, b) {
    return a - b
}

export fn multiply(a, b) {
    return a * b
}

export fn divide(a, b) {
    if (b == 0) {
        return null
    }
    return a / b
}

// Default export
export default fn square(x) {
    return x * x
}
