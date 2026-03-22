// =============================================================================
// KLang Promise Examples
// =============================================================================

println("=== JavaScript-Style Promises in KLang ===\n")

// Example 1: Basic Promise.resolve()
println("Example 1: Promise.resolve()")
println("---------------------------------")
const p1 = Promise.resolve(42)
println("Created resolved promise:")
println(p1)
println()

// Example 2: Promise.reject()
println("Example 2: Promise.reject()")
println("---------------------------------")
const p2 = Promise.reject("Something went wrong")
println("Created rejected promise:")
println(p2)
println()

// Example 3: Promise chaining with then()
println("Example 3: Promise chaining")
println("---------------------------------")
Promise.resolve(10)
    .then(value => {
        println(`First then: ${value}`)
        return value * 2
    })
    .then(value => {
        println(`Second then: ${value}`)
        return value + 5
    })
    .then(value => {
        println(`Third then: ${value}`)
    })
println()

// Example 4: Error handling with catch()
println("Example 4: Error handling")
println("---------------------------------")
Promise.reject("Error occurred!")
    .catch(error => {
        println(`Caught error: ${error}`)
        return "Recovered"
    })
    .then(value => {
        println(`After recovery: ${value}`)
    })
println()

// Example 5: finally() for cleanup
println("Example 5: finally() for cleanup")
println("---------------------------------")
Promise.resolve("Success")
    .then(value => println(`Result: ${value}`))
    .finally(() => println("Cleanup complete"))
println()

// Example 6: Promise constructor
println("Example 6: Promise constructor")
println("---------------------------------")
const p6 = new Promise((resolve, reject) => {
    println("Executor running...")
    const success = true
    if (success) {
        resolve("Operation successful!")
    } else {
        reject("Operation failed!")
    }
})

p6.then(value => {
    println(`Promise resolved: ${value}`)
})
println()

// Example 7: Complex chaining
println("Example 7: Complex chaining")
println("---------------------------------")
new Promise((resolve, reject) => {
    resolve(100)
})
.then(value => {
    println(`Step 1: ${value}`)
    return value / 2
})
.then(value => {
    println(`Step 2: ${value}`)
    return value * 3
})
.then(value => {
    println(`Step 3: ${value}`)
})
.catch(error => {
    println(`Error: ${error}`)
})
.finally(() => {
    println("All done!")
})

println("\n=== Demo Complete ===")
