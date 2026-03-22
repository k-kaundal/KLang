// Comprehensive test for default parameters

// Test 1: Basic default parameter
fn test1(x = 5) {
    return x
}
print("Test 1: " + (test1() == 5 ? "PASS" : "FAIL"))
print("Test 1b: " + (test1(10) == 10 ? "PASS" : "FAIL"))

// Test 2: Multiple defaults
fn test2(a = 1, b = 2, c = 3) {
    return a + b + c
}
print("Test 2a: " + (test2() == 6 ? "PASS" : "FAIL"))
print("Test 2b: " + (test2(10) == 15 ? "PASS" : "FAIL"))
print("Test 2c: " + (test2(10, 20) == 33 ? "PASS" : "FAIL"))
print("Test 2d: " + (test2(10, 20, 30) == 60 ? "PASS" : "FAIL"))

// Test 3: Mix of required and default params
fn test3(required, optional = "default") {
    return required + " " + optional
}
print("Test 3a: " + (test3("hello") == "hello default" ? "PASS" : "FAIL"))
print("Test 3b: " + (test3("hello", "world") == "hello world" ? "PASS" : "FAIL"))

// Test 4: Default with different types
fn test4(str = "text", num = 42, bool = true) {
    if (bool) {
        return str + num
    }
    return "false"
}
print("Test 4a: " + (test4() == "text42" ? "PASS" : "FAIL"))
print("Test 4b: " + (test4("custom") == "custom42" ? "PASS" : "FAIL"))
print("Test 4c: " + (test4("custom", 100) == "custom100" ? "PASS" : "FAIL"))

// Test 5: Nested function calls with defaults
fn outer(x = 10) {
    fn inner(y = 5) {
        return x + y
    }
    return inner()
}
print("Test 5: " + (outer() == 15 ? "PASS" : "FAIL"))
print("Test 5b: " + (outer(20) == 25 ? "PASS" : "FAIL"))

// Test 6: Default parameters with expressions
fn test6(a = 2, b = a * 3) {
    return a + b
}
print("Test 6a: " + (test6() == 8 ? "PASS" : "FAIL"))
print("Test 6b: " + (test6(5) == 20 ? "PASS" : "FAIL"))

print("All tests completed!")
