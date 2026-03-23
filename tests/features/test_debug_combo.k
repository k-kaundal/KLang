# Test 4: Combined usage
let data = dict()
data.set("coordinates", (100, 200))
print("Set coordinates")

let tags = set()
tags.add("test")
print("Created tags")

data.set("tags", tags)
print("Set tags")

data.set("active", true)
print("Set active")

print("Data structure:", data)
