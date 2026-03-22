// Basic generator example - demonstrates yield functionality
fn* counter() {
    yield 1
    yield 2
    yield 3
}

println("=== Basic Generator Demo ===")
println("Creating generator...")
let gen = counter()
