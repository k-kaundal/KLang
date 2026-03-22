// Basic for-of with array
let numbers = [1, 2, 3, 4, 5]
println("Array iteration:")
for const num of numbers {
    println(num)
}

// String iteration
println("\nString iteration:")
for const char of "hello" {
    println(char)
}

// With break
println("\nWith break:")
for const n of [1, 2, 3, 4, 5] {
    if n == 3 { break }
    println(n)
}

// With continue
println("\nWith continue (odds only):")
for const n of [1, 2, 3, 4, 5] {
    if n % 2 == 0 { continue }
    println(n)
}

// Nested loops
println("\nNested loops:")
let matrix = [[1, 2], [3, 4]]
for const row of matrix {
    for const item of row {
        println(item)
    }
}
