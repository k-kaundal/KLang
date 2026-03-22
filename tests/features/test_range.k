# Test range() function - Python-style

print("=== Testing range() Function ===\n")

# Test 1: range(n) - 0 to n-1
print("Test 1: range(5)")
for (i of range(5)) {
    print(i)
}
print("")

# Test 2: range(start, stop)
print("Test 2: range(2, 7)")
for (i of range(2, 7)) {
    print(i)
}
print("")

# Test 3: range(start, stop, step)
print("Test 3: range(0, 10, 2) - even numbers")
for (i of range(0, 10, 2)) {
    print(i)
}
print("")

# Test 4: range with negative step (countdown)
print("Test 4: range(10, 0, -1) - countdown")
for (i of range(10, 0, -1)) {
    print(i)
}
print("")

# Test 5: range(start, stop, step) with negative step
print("Test 5: range(10, 0, -2)")
for (i of range(10, 0, -2)) {
    print(i)
}
print("")

# Test 6: Empty ranges
print("Test 6: Empty ranges")
print("range(5, 2) length: " + str(len(range(5, 2))))
print("range(0, 0) length: " + str(len(range(0, 0))))
print("")

# Test 7: Using range in list comprehensions (when implemented)
print("Test 7: Convert range to list")
let numbers = range(1, 6)
print("Numbers 1-5:")
for (n of numbers) {
    print(n)
}
print("")

# Test 8: Practical example - sum of range
print("Test 8: Sum numbers 1 to 10")
let sum = 0
for (i of range(1, 11)) {
    sum = sum + i
}
print("Sum = " + str(sum))
print("")

# Test 9: Generate multiplication table
print("Test 9: Multiplication table for 5")
for (i of range(1, 11)) {
    print("5 x " + str(i) + " = " + str(5 * i))
}

print("\n=== All range() tests completed! ===")
