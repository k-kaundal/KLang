// Comprehensive test of var/let/const semantics
// This file tests all features of the var/let/const implementation

print("=== Test 1: Basic declarations ===")
let a = 1
var b = 2
const C = 3
print(a)
print(b)
print(C)
print("PASS")
print("")

print("=== Test 2: Reassignment ===")
a = 10
b = 20
// C = 30  // Would error: cannot assign to const
print(a)
print(b)
print(C)
print("PASS")
print("")

print("=== Test 3: var redeclaration ===")
var x = 100
var x = 200
print(x)
print("PASS")
print("")

print("=== Test 4: let no redeclaration ===")
// This would error:
// let y = 1
// let y = 2
let y = 1
y = 2
print(y)
print("PASS")
print("")

print("=== Test 5: Block scoping ===")
let outer = 1
{
    let outer = 2
    print(outer)  // Should be 2
}
print(outer)  // Should be 1
print("PASS")
print("")

print("=== Test 6: Nested blocks ===")
let level1 = 10
{
    let level2 = 20
    {
        let level3 = 30
        print(level1)
        print(level2)
        print(level3)
    }
    print(level1)
    print(level2)
}
print(level1)
print("PASS")
print("")

print("=== Test 7: Loop scoping ===")
for i in 0..3 {
    let loop_local = i * 10
    print(loop_local)
}
print("PASS")
print("")

print("=== Test 8: Function scoping ===")
fn testFunc() {
    let func_local = 42
    const FUNC_CONST = 100
    var func_var = 200
    return func_local
}
print(testFunc())
print("PASS")
print("")

print("=== Test 9: Optional initialization ===")
let uninit1
var uninit2
print(uninit1)  // null
print(uninit2)  // null
uninit1 = 5
uninit2 = 10
print(uninit1)
print(uninit2)
print("PASS")
print("")

print("=== Test 10: Mixed declarations ===")
let mix_let = 1
var mix_var = 2
const MIX_CONST = 3
{
    let mix_let = 10
    var mix_var = 20
    const MIX_CONST = 30
    print(mix_let)
    print(mix_var)
    print(MIX_CONST)
}
print(mix_let)
print(mix_var)
print(MIX_CONST)
print("PASS")
print("")

print("=== All tests completed successfully! ===")

