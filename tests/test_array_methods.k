// Test: JavaScript Array Methods for KLang
// Priority 1 Methods: map, filter, reduce, forEach, find, some, every

println("=== Array Methods Test Suite ===")
println("")

// Test 1: map - Transform each element
println("Test 1: map()")
const numbers = [1, 2, 3, 4, 5]
const doubled = numbers.map(x => x * 2)
println("Original: " + str(numbers))
println("Doubled: " + str(doubled))
println("Expected: [2, 4, 6, 8, 10]")
println("")

// Test 2: map with index
println("Test 2: map() with index")
const withIndex = numbers.map((x, i) => x + i)
println("With index: " + str(withIndex))
println("Expected: [1, 3, 5, 7, 9]")
println("")

// Test 3: filter - Filter elements by condition
println("Test 3: filter()")
const evens = numbers.filter(x => x % 2 == 0)
println("Original: " + str(numbers))
println("Evens: " + str(evens))
println("Expected: [2, 4]")
println("")

// Test 4: filter with complex condition
println("Test 4: filter() with complex condition")
const greaterThan2 = numbers.filter(x => x > 2)
println("Greater than 2: " + str(greaterThan2))
println("Expected: [3, 4, 5]")
println("")

// Test 5: reduce - Reduce to single value
println("Test 5: reduce()")
const sum = numbers.reduce((acc, x) => acc + x, 0)
println("Original: " + str(numbers))
println("Sum: " + str(sum))
println("Expected: 15")
println("")

// Test 6: reduce without initial value
println("Test 6: reduce() without initial value")
const product = numbers.reduce((acc, x) => acc * x)
println("Product: " + str(product))
println("Expected: 120")
println("")

// Test 7: reduce to build string
println("Test 7: reduce() to build string")
const words = ["Hello", "World", "from", "KLang"]
const sentence = words.reduce((acc, word) => acc + " " + word, "")
println("Sentence:" + sentence)
println("Expected:  Hello World from KLang")
println("")

// Test 8: forEach - Iterate over elements
println("Test 8: forEach()")
println("Printing each element:")
numbers.forEach(x => println("  - " + str(x)))
println("")

// Test 9: forEach with index
println("Test 9: forEach() with index")
println("Printing with index:")
numbers.forEach((x, i) => println("  [" + str(i) + "] = " + str(x)))
println("")

// Test 10: find - Find first matching element
println("Test 10: find()")
const found = numbers.find(x => x > 3)
println("Original: " + str(numbers))
println("First > 3: " + str(found))
println("Expected: 4")
println("")

// Test 11: find - No match
println("Test 11: find() with no match")
const notFound = numbers.find(x => x > 10)
println("First > 10: " + str(notFound))
println("Expected: null")
println("")

// Test 12: some - Check if any element matches
println("Test 12: some()")
const hasEven = numbers.some(x => x % 2 == 0)
const hasNegative = numbers.some(x => x < 0)
println("Original: " + str(numbers))
println("Has even: " + str(hasEven))
println("Expected: true")
println("Has negative: " + str(hasNegative))
println("Expected: false")
println("")

// Test 13: every - Check if all elements match
println("Test 13: every()")
const allPositive = numbers.every(x => x > 0)
const allEven = numbers.every(x => x % 2 == 0)
println("Original: " + str(numbers))
println("All positive: " + str(allPositive))
println("Expected: true")
println("All even: " + str(allEven))
println("Expected: false")
println("")

// Test 14: Chain multiple methods
println("Test 14: Chaining methods")
const result = numbers
    .filter(x => x % 2 == 0)
    .map(x => x * 3)
    .reduce((acc, x) => acc + x, 0)
println("Filter evens, map *3, reduce sum:")
println("Result: " + str(result))
println("Expected: 18 (2*3 + 4*3 = 6 + 12 = 18)")
println("")

// Test 15: indexOf
println("Test 15: indexOf()")
const idx = numbers.indexOf(3)
const notIdx = numbers.indexOf(10)
println("Index of 3: " + str(idx))
println("Expected: 2")
println("Index of 10: " + str(notIdx))
println("Expected: -1")
println("")

// Test 16: includes
println("Test 16: includes()")
const has3 = numbers.includes(3)
const has10 = numbers.includes(10)
println("Includes 3: " + str(has3))
println("Expected: true")
println("Includes 10: " + str(has10))
println("Expected: false")
println("")

// Test 17: slice
println("Test 17: slice()")
const sliced = numbers.slice(1, 4)
println("Original: " + str(numbers))
println("Slice(1, 4): " + str(sliced))
println("Expected: [2, 3, 4]")
println("")

// Test 18: slice with negative indices
println("Test 18: slice() with negative indices")
const sliced2 = numbers.slice(-3, -1)
println("Slice(-3, -1): " + str(sliced2))
println("Expected: [3, 4]")
println("")

// Test 19: concat
println("Test 19: concat()")
const arr1 = [1, 2, 3]
const arr2 = [4, 5, 6]
const combined = arr1.concat(arr2)
println("Array 1: " + str(arr1))
println("Array 2: " + str(arr2))
println("Combined: " + str(combined))
println("Expected: [1, 2, 3, 4, 5, 6]")
println("")

// Test 20: join
println("Test 20: join()")
const joined = numbers.join(", ")
println("Original: " + str(numbers))
println("Joined: " + joined)
println("Expected: 1, 2, 3, 4, 5")
println("")

// Test 21: join with custom separator
println("Test 21: join() with custom separator")
const joined2 = numbers.join(" -> ")
println("Joined with '->': " + joined2)
println("Expected: 1 -> 2 -> 3 -> 4 -> 5")
println("")

// Test 22: Working with strings
println("Test 22: map() with strings")
const names = ["alice", "bob", "charlie"]
const uppercased = names.map(name => uppercase(name))
println("Original: " + str(names))
println("Uppercased: " + str(uppercased))
println("Expected: [ALICE, BOB, CHARLIE]")
println("")

// Test 23: Complex data transformation
println("Test 23: Complex transformation")
const data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
const evenSquares = data
    .filter(x => x % 2 == 0)
    .map(x => x * x)
println("Even squares of 1-10: " + str(evenSquares))
println("Expected: [4, 16, 36, 64, 100]")
println("")

// Test 24: Empty array handling
println("Test 24: Empty array handling")
const empty = []
const emptyMap = empty.map(x => x * 2)
const emptyFilter = empty.filter(x => x > 0)
const emptyReduce = empty.reduce((acc, x) => acc + x, 0)
println("Empty map: " + str(emptyMap))
println("Empty filter: " + str(emptyFilter))
println("Empty reduce: " + str(emptyReduce))
println("Expected: [], [], 0")
println("")

println("=== All Array Method Tests Completed ===")
