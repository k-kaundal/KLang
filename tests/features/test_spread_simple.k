// Simple spread test
println("=== Array Spread Test ===")
const arr1 = [1, 2, 3]
const arr2 = [4, 5, 6]
const combined = [...arr1, ...arr2]
println(combined)

// Mix with literals
const mixed = [0, ...arr1, 99]
println(mixed)
