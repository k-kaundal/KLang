// Test array destructuring
println("=== Array Destructuring ===")
const [a, b, c] = [1, 2, 3]
println(a)
println(b)
println(c)

// Array with defaults
const [x = 10, y = 20] = [5]
println(x)
println(y)

// Skip elements
const [first, , third] = [1, 2, 3]
println(first)
println(third)

// Rest in arrays
const [head, ...tail] = [1, 2, 3, 4, 5]
println(head)
println(tail)

// Test object destructuring
println("=== Object Destructuring ===")
const person = {name: "Alice", age: 30}
const {name, age} = person
println(name)
println(age)

// Object with defaults
const {p = 10, q = 20} = {p: 5}
println(p)
println(q)

// Object with rename
const {name: userName, age: userAge} = {name: "Bob", age: 25}
println(userName)
println(userAge)

println("All tests passed!")
