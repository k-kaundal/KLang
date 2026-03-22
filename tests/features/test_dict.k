# Test dictionary implementation

print("=== Dictionary Tests ===")

# Test 1: Create empty dictionary
print("\nTest 1: Create empty dictionary")
let d = dict()
print(d)
print("Size:", len(d))

# Test 2: Add key-value pairs
print("\nTest 2: Add key-value pairs")
d.set("name", "Alice")
d.set("age", 30)
d.set("city", "NYC")
print(d)
print("Size:", len(d))

# Test 3: Get values
print("\nTest 3: Get values")
print("name:", d.get("name"))
print("age:", d.get("age"))
print("city:", d.get("city"))

# Test 4: Check if key exists
print("\nTest 4: Check if key exists")
print("Has 'name':", d.has("name"))
print("Has 'email':", d.has("email"))

# Test 5: Get with default value
print("\nTest 5: Get with default value")
print("Get existing:", d.get("name", "Unknown"))
print("Get missing:", d.get("email", "no-email"))

# Test 6: Update existing key
print("\nTest 6: Update existing key")
d.set("age", 31)
print("Updated age:", d.get("age"))
print(d)

# Test 7: Get keys
print("\nTest 7: Get keys")
let keys = d.keys()
print("Keys:", keys)

# Test 8: Get values
print("\nTest 8: Get values")
let values = d.values()
print("Values:", values)

# Test 9: Delete a key
print("\nTest 9: Delete a key")
let deleted = d.delete("city")
print("Deleted 'city':", deleted)
print(d)
print("Size:", len(d))

# Test 10: Try to delete non-existent key
print("\nTest 10: Try to delete non-existent key")
let deleted2 = d.delete("email")
print("Deleted 'email':", deleted2)

# Test 11: For-of loop over dictionary keys
print("\nTest 11: For-of loop over dictionary keys")
let contacts = dict()
contacts.set("Alice", "alice@example.com")
contacts.set("Bob", "bob@example.com")
contacts.set("Charlie", "charlie@example.com")

print("Contacts:")
for (name of contacts) {
    let email = contacts.get(name)
    print(name, "->", email)
}

# Test 12: Integer keys
print("\nTest 12: Integer keys")
let scores = dict()
scores.set(1, 100)
scores.set(2, 95)
scores.set(3, 87)
print(scores)
print("Score for player 2:", scores.get(2))

# Test 13: Size property
print("\nTest 13: Size property")
print("Contacts size:", contacts.size)
print("Scores size:", scores.size)

print("\n=== All Dictionary Tests Completed ===")
