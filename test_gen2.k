// Basic generator example - demonstrates yield functionality
fn* counter() {
    yield 1
    yield 2
    yield 3
}

println("=== Basic Generator Demo ===")
println("Creating generator...")
let gen = counter()

println("Calling next() three times:")
let result1 = gen.next()
println("First call:")
println(result1.value)
println(result1.done)

let result2 = gen.next()
println("Second call:")
println(result2.value)
