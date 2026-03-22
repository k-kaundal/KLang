# Test basic tuple creation and printing
print("Test 1: Basic tuple creation")
let coords = (10, 20, 30)
print(coords)

# Test empty tuple
print("\nTest 2: Empty tuple")
let empty = ()
print(empty)

# Test single element tuple
print("\nTest 3: Single element tuple")
let single = (42,)
print(single)

# Test tuple indexing
print("\nTest 4: Tuple indexing")
let point = (100, 200, 300)
print(point[0])
print(point[1])
print(point[2])

# Test tuple unpacking
print("\nTest 5: Tuple unpacking")
let (x, y, z) = (5, 10, 15)
print(x)
print(y)
print(z)

# Test for-of with tuples
print("\nTest 6: For-of with tuples")
let numbers = (1, 2, 3, 4, 5)
for (num of numbers) {
    print(num)
}

# Test function returning multiple values
print("\nTest 7: Function returning tuple")
fn getPosition() {
    return (100, 200)
}
let pos = getPosition()
print(pos)
let (px, py) = getPosition()
print(px)
print(py)

# Test tuple with different types
print("\nTest 8: Mixed type tuple")
let mixed = (42, "hello", true, 3.14)
print(mixed)

print("\nAll tuple tests completed!")
