# Test range() function - simplified

print("=== Testing range() Function ===")
print("")

# Test 1: range(n) - 0 to n-1
print("Test 1: range(5)")
let r1 = range(5)
print(r1)
print("Length: " + str(len(r1)))
print("")

# Test 2: range(start, stop)
print("Test 2: range(2, 7)")
let r2 = range(2, 7)
print(r2)
print("")

# Test 3: range(start, stop, step)
print("Test 3: range(0, 10, 2) - even numbers")
let r3 = range(0, 10, 2)
print(r3)
print("")

# Test 4: range with negative step (countdown)
print("Test 4: range(10, 0, -1) - countdown")
let r4 = range(10, 0, -1)
print(r4)
print("")

# Test 5: range(start, stop, step) with negative step
print("Test 5: range(10, 0, -2)")
let r5 = range(10, 0, -2)
print(r5)
print("")

# Test 6: Empty ranges
print("Test 6: Empty ranges")
print("range(5, 2): " + str(range(5, 2)))
print("range(0, 0): " + str(range(0, 0)))
print("")

# Test 7: Large range
print("Test 7: range(1, 101) for sum")
let numbers = range(1, 101)
print("Generated " + str(len(numbers)) + " numbers")
print("")

# Test 8: Using range for indexing
print("Test 8: Access elements by index")
let seq = range(10, 20)
print("First element: " + str(seq[0]))
print("Fifth element: " + str(seq[4]))
print("Last element: " + str(seq[9]))

print("")
print("=== All range() tests completed! ===")
