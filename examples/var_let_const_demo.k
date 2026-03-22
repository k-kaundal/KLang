// Demonstration of var/let/const in KLang
// Following JavaScript semantics

print("=== CONST: Immutable Variables ===")
const PI = 3.14159
const MAX_SIZE = 100
const APP_NAME = "KLang"

print("PI:")
print(PI)
print("MAX_SIZE:")
print(MAX_SIZE)
print("APP_NAME:")
print(APP_NAME)
print("")

// Trying to reassign const will error
// Uncomment to test:
// PI = 3.14  // Error: cannot assign to const variable 'PI'

print("=== LET: Block-Scoped Mutable Variables ===")
let count = 0
let message = "Hello"

print("Initial count:")
print(count)
print("Initial message:")
print(message)

// let can be reassigned
count = 10
message = "World"
print("After reassignment, count:")
print(count)
print("After reassignment, message:")
print(message)
print("")

// let prevents redeclaration in same scope
// Uncomment to test:
// let count = 20  // Error: identifier 'count' has already been declared

print("=== VAR: Allows Redeclaration ===")
var x = 1
print("Initial x:")
print(x)

// var allows redeclaration
var x = 2
print("x after redeclaration:")
print(x)
print("")

print("=== BLOCK SCOPING ===")
let outer = 100
print("Before block, outer:")
print(outer)

{
    // Inner scope shadows outer
    let outer = 200
    print("Inside block, outer:")
    print(outer)
}

print("After block, outer:")
print(outer)
print("")

print("=== OPTIONAL INITIALIZATION ===")
let uninitialized_let
var uninitialized_var

print("uninitialized_let:")
print(uninitialized_let)
print("uninitialized_var:")
print(uninitialized_var)

// But const MUST be initialized
// Uncomment to test:
// const MUST_INIT  // Error: const declaration must be initialized
print("")

print("=== LOOP SCOPING ===")
for i in 0..5 {
    let loop_var = i * 2
    print("i:")
    print(i)
    print("loop_var:")
    print(loop_var)
}
print("")

print("Demo complete!")

