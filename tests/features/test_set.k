# Test set implementation

print("=== Set Tests ===")

# Test 1: Create empty set
print("\nTest 1: Create empty set")
let s = set()
print(s)
print("Size:", len(s))

# Test 2: Add elements
print("\nTest 2: Add elements")
s.add(1)
s.add(2)
s.add(3)
print(s)
print("Size:", len(s))

# Test 3: Add duplicate (should not increase size)
print("\nTest 3: Add duplicate element")
s.add(2)
print(s)
print("Size:", len(s))

# Test 4: Check if element exists
print("\nTest 4: Check if element exists")
print("Has 2:", s.has(2))
print("Has 5:", s.has(5))

# Test 5: Remove element
print("\nTest 5: Remove element")
let removed = s.remove(2)
print("Removed 2:", removed)
print(s)
print("Size:", len(s))

# Test 6: Try to remove non-existent element
print("\nTest 6: Try to remove non-existent element")
let removed2 = s.remove(10)
print("Removed 10:", removed2)

# Test 7: For-of loop over set
print("\nTest 7: For-of loop over set")
let fruits = set()
fruits.add("apple")
fruits.add("banana")
fruits.add("cherry")
fruits.add("apple")  # Duplicate
print("Fruits:")
for (fruit of fruits) {
    print(" -", fruit)
}
print("Total fruits:", fruits.size)

# Test 8: Set with mixed types
print("\nTest 8: Set with mixed types")
let mixed = set()
mixed.add(1)
mixed.add("hello")
mixed.add(3.14)
mixed.add(true)
print(mixed)

# Test 9: Clear set
print("\nTest 9: Clear set")
print("Before clear:", mixed.size)
mixed.clear()
print("After clear:", mixed.size)
print(mixed)

# Test 10: Set operations with numbers
print("\nTest 10: Set operations with numbers")
let numbers = set()
numbers.add(10)
numbers.add(20)
numbers.add(30)
numbers.add(40)

print("Numbers:", numbers)
print("Has 30:", numbers.has(30))
numbers.remove(30)
print("After removing 30:", numbers)
print("Has 30:", numbers.has(30))

# Test 11: Size property
print("\nTest 11: Size property")
print("Numbers size:", numbers.size)

# Test 12: String set
print("\nTest 12: String set")
let words = set()
words.add("hello")
words.add("world")
words.add("klang")
words.add("hello")  # Duplicate

print("Unique words:")
for (word of words) {
    print(word)
}
print("Count:", words.size)

print("\n=== All Set Tests Completed ===")
