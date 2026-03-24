// Test that we catch invalid parameter ordering
fn invalid(a = 5, b) {
    return a + b
}

print(invalid(1, 2))
