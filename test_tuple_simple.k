# Test basic tuple creation and printing
print("Test 1: Basic tuple creation")
let coords = (10, 20, 30)
print(coords)

# Test empty tuple
print("Test 2: Empty tuple")
let empty = ()
print(empty)

# Test single element tuple
print("Test 3: Single element tuple")
let single = (42,)
print(single)

# Test tuple indexing
print("Test 4: Tuple indexing")
let point = (100, 200, 300)
print(point[0])
print(point[1])
print(point[2])

# Test tuple unpacking
print("Test 5: Tuple unpacking")
let (x, y, z) = (5, 10, 15)
print(x)
print(y)
print(z)

# Test function returning multiple values
print("Test 6: Function returning tuple")
fn getPosition() {
    return (100, 200)
}
let pos = getPosition()
print(pos)
let (px, py) = getPosition()
print(px)
print(py)

# Test tuple with different types
print("Test 7: Mixed type tuple")
let mixed = (42, "hello", true, 3.14)
print(mixed)

print("All tuple tests completed!")
