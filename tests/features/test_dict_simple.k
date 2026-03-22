# Simple test for dict

print("Creating dict...")
let d = dict()
print("Dict created:", d)

print("\nAdding value...")
d.set("name", "Alice")
print("After set:", d)

print("\nGetting value...")
let name = d.get("name")
print("Name:", name)

print("\nDone!")
