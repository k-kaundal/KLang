// KLang Closures Example
// Demonstrates nested function declarations and closures

// Basic closure - inner function captures outer function's variable
fn outer() {
    let x = 10
    fn inner() {
        return x + 5
    }
    return inner
}

let myFunc = outer()
print(myFunc())  // Output: 15

// Closure with parameters - creating a customized function
fn makeAdder(x) {
    fn adder(y) {
        return x + y
    }
    return adder
}

let add5 = makeAdder(5)
let add10 = makeAdder(10)
print(add5(3))   // Output: 8
print(add10(3))  // Output: 13

// Multiple nested levels
fn level1() {
    let a = 1
    fn level2() {
        let b = 2
        fn level3() {
            return a + b + 3
        }
        return level3
    }
    return level2()
}

let deepFunc = level1()
print(deepFunc())  // Output: 6

// Closure with mutable state (counter pattern)
fn makeCounter() {
    let count = 0
    fn increment() {
        count = count + 1
        return count
    }
    return increment
}

let counter = makeCounter()
print(counter())  // Output: 1
print(counter())  // Output: 2
print(counter())  // Output: 3
