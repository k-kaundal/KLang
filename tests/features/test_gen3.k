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
println(result2.done)

let result3 = gen.next()
println("Third call:")
println(result3.value)
println(result3.done)

let result4 = gen.next()
println("Fourth call (exhausted):")
println(result4.value)
println(result4.done)

println("")
println("=== Generator with Parameters ===")

fn* greet(name) {
    yield name
}

let greeter = greet("Alice")
