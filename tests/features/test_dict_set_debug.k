# Test dict set return

let d = dict()
print("Dict type:", type(d))

let result = d.set("a", 1)
print("Set result:", result)
print("Set result type:", type(result))

let size = len(d)
print("Size:", size)
print("Size type:", type(size))
