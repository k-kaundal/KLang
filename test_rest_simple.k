// Test rest parameters
println("=== Rest Parameters Test ===")

fn sum(...numbers) {
    let total = 0
    let i = 0
    while (i < len(numbers)) {
        total = total + numbers[i]
        i = i + 1
    }
    return total
}

println(sum(1, 2, 3))
println(sum(10, 20, 30, 40))
println(sum())

// Rest with regular params
fn greet(greeting, ...names) {
    println(greeting)
    let i = 0
    while (i < len(names)) {
        println(names[i])
        i = i + 1
    }
}

greet("Hello")
greet("Welcome", "Alice", "Bob")
