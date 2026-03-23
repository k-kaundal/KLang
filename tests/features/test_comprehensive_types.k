# Comprehensive test for tuple, dict, and set

print("=== Comprehensive Type Tests ===")

# Test 1: Tuples
print("\n--- Tuple Tests ---")
let point = (10, 20, 30)
print("Tuple:", point)
print("First element:", point[0])
print("Length:", len(point))

# Test 2: Dictionaries
print("\n--- Dictionary Tests ---")
let user = dict()
user.set("name", "Bob")
user.set("age", 25)
user.set("city", "SF")
print("User:", user)
print("Name:", user.get("name"))
print("Size:", len(user))

# Test 3: Sets
print("\n--- Set Tests ---")
let tags = set()
tags.add("python")
tags.add("javascript")
tags.add("klang")
print("Tags:", tags)
print("Has 'klang':", tags.has("klang"))
print("Size:", len(tags))

# Test 4: Combined usage
print("\n--- Combined Usage ---")
let data = dict()
data.set("coordinates", (100, 200))
data.set("tags", tags)
data.set("active", true)

print("Data structure:", data)
print("Coordinates:", data.get("coordinates"))

# Test 5: Iteration
print("\n--- Iteration Tests ---")
print("Iterating over dict keys:")
for (key of data) {
    print(" -", key)
}

print("\nIterating over set:")
for (tag of tags) {
    print(" -", tag)
}

print("\nIterating over tuple:")
for (val of point) {
    print(" -", val)
}

# Test 6: Nested structures
print("\n--- Nested Structures ---")
let game = dict()
game.set("players", 4)
game.set("scores", dict())

let scores = game.get("scores")
scores.set("Alice", 100)
scores.set("Bob", 95)
scores.set("Charlie", 87)

print("Game:", game)
print("Scores:", scores)

print("\n=== All Tests Passed! ===")
