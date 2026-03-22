fn* greet(name) {
    yield name
}

let greeter = greet("Alice")
println("Done")
