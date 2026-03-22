fn* test() {
    yield 42
}

let gen = test()
println("Created generator")
