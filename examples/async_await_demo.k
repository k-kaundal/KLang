// KLang Async/Await Demo
// This demonstrates JavaScript-style async/await syntax

println("=== Async/Await Demonstration ===\n")

// 1. Basic async function
println("1. Basic async function:")
async fn fetchData() {
    return "Hello from async function"
}

// Async functions return Promises
const promise1 = fetchData()
promise1.then(data => println(`   Result: ${data}`))

println("")

// 2. Using await
println("2. Using await:")
async fn processData() {
    const data = await fetchData()
    println(`   Processed: ${data}`)
    return data
}

processData()

println("")

// 3. Async arrow functions
println("3. Async arrow functions:")
const asyncArrow = async () => {
    return "Arrow function result"
}

asyncArrow().then(result => println(`   ${result}`))

println("")

// 4. Sequential async operations
println("4. Sequential async operations:")
async fn getNumber(n) {
    return n
}

async fn calculateSum() {
    const a = await getNumber(10)
    const b = await getNumber(20)
    const c = await getNumber(30)
    const sum = a + b + c
    return sum
}

calculateSum().then(sum => println(`   Sum: ${sum}`))

println("")

// 5. Async with string operations
println("5. Async string operations:")
async fn greet(name) {
    return `Hello, ${name}!`
}

async fn greetMany() {
    const msg1 = await greet("Alice")
    const msg2 = await greet("Bob")
    const msg3 = await greet("Charlie")
    return `${msg1} ${msg2} ${msg3}`
}

greetMany().then(result => println(`   ${result}`))

println("")

// 6. Awaiting non-Promise values
println("6. Awaiting non-Promise values:")
async fn awaitValue() {
    const value = await 42
    println(`   Awaited non-promise: ${value}`)
    return value * 2
}

awaitValue().then(result => println(`   Doubled: ${result}`))

println("")

// 7. Promise chaining with async
println("7. Promise chaining:")
async fn step1() {
    return 5
}

async fn step2(n) {
    return n * 2
}

async fn step3(n) {
    return n + 10
}

async fn pipeline() {
    const a = await step1()
    const b = await step2(a)
    const c = await step3(b)
    return c
}

pipeline().then(result => println(`   Pipeline result: ${result}`))

println("")

// 8. Async with conditionals
println("8. Async with conditionals:")
async fn checkValue(n) {
    const value = await getNumber(n)
    if (value > 50) {
        return "large"
    } else {
        return "small"
    }
}

checkValue(75).then(result => println(`   75 is ${result}`))
checkValue(25).then(result => println(`   25 is ${result}`))

println("")

// 9. Using Promise.resolve with async/await
println("9. Promise.resolve with await:")
async fn usePromiseResolve() {
    const value = await Promise.resolve(123)
    return value + 100
}

usePromiseResolve().then(result => println(`   Result: ${result}`))

println("")

// 10. Nested async calls
println("10. Nested async calls:")
async fn inner(x) {
    return x * 3
}

async fn outer(x) {
    const y = await inner(x)
    const z = await inner(y)
    return z
}

outer(2).then(result => println(`   Nested result: ${result}`))

println("")

println("=== All async/await tests complete ===")
