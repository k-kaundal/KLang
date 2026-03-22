println("Starting test")

fn* test() {
    println("Inside generator")
    yield 42
    println("After yield")
}

println("About to create generator")
let gen = test()
println("Created generator")
println(gen)
