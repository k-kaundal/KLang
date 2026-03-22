// Test object spread
println("=== Object Spread Test ===")

const obj1 = {a: 1, b: 2}
const obj2 = {c: 3, d: 4}
const merged = {...obj1, ...obj2}
println(merged)

// Override properties
const base = {x: 10, y: 20}
const override = {...base, y: 99, z: 30}
println(override)
