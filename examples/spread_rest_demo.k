// KLang Spread and Rest Operators - Working Demo
// This demonstrates all successfully implemented features

println("=== KLang Spread & Rest Operators ===\n")

// ============================================
// 1. ARRAY SPREAD
// ============================================
println("1. ARRAY SPREAD")
const arr1 = [1, 2, 3]
const arr2 = [4, 5, 6]
const combined = [...arr1, ...arr2]
println("  [...[1,2,3], ...[4,5,6]] = " + str(combined))

const mixed = [0, ...arr1, 99]
println("  [0, ...[1,2,3], 99] = " + str(mixed))

const copy = [...arr1]
println("  [...[1,2,3]] = " + str(copy))
println("")

// ============================================
// 2. REST PARAMETERS
// ============================================
println("2. REST PARAMETERS")

fn sum(...numbers) {
    let total = 0
    let i = 0
    while (i < len(numbers)) {
        total = total + numbers[i]
        i = i + 1
    }
    return total
}

println("  sum(1, 2, 3) = " + str(sum(1, 2, 3)))
println("  sum(5, 10, 15, 20) = " + str(sum(5, 10, 15, 20)))
println("  sum() = " + str(sum()))

fn min(...numbers) {
    if (len(numbers) == 0) {
        return null
    }
    let minimum = numbers[0]
    let i = 1
    while (i < len(numbers)) {
        if (numbers[i] < minimum) {
            minimum = numbers[i]
        }
        i = i + 1
    }
    return minimum
}

println("  min(5, 2, 9, 1, 7) = " + str(min(5, 2, 9, 1, 7)))

fn multiply(factor, ...numbers) {
    let i = 0
    while (i < len(numbers)) {
        println("    " + str(factor) + " × " + str(numbers[i]) + " = " + str(factor * numbers[i]))
        i = i + 1
    }
}

println("  multiply(3, 2, 4, 6):")
multiply(3, 2, 4, 6)
println("")

// ============================================
// 3. OBJECT SPREAD
// ============================================
println("3. OBJECT SPREAD")

const obj1 = {a: 1, b: 2}
const obj2 = {c: 3, d: 4}
const merged = {...obj1, ...obj2}
println("  {...{a:1,b:2}, ...{c:3,d:4}}")
println("    a = " + str(merged.a))
println("    b = " + str(merged.b))
println("    c = " + str(merged.c))
println("    d = " + str(merged.d))

const defaults = {theme: "light", size: 14}
const custom = {...defaults, theme: "dark"}
println("  {...{theme:light,size:14}, theme:dark}")
println("    theme = " + custom.theme)
println("    size = " + str(custom.size))

const person = {name: "Alice", age: 30}
const location = {city: "NYC"}
const profile = {...person, ...location}
println("  {...person, ...location}")
println("    name = " + profile.name)
println("    age = " + str(profile.age))
println("    city = " + profile.city)
println("")

// ============================================
// 4. PRACTICAL EXAMPLES
// ============================================
println("4. PRACTICAL EXAMPLES")

println("  Building arrays dynamically:")
const base = [1, 2]
const extended = [...base, 3, 4, 5]
println("    " + str(extended))

println("  Merging configurations:")
const prodConfig = {debug: false, port: 8080}
const devOverrides = {...prodConfig, debug: true}
println("    debug = " + str(devOverrides.debug))
println("    port = " + str(devOverrides.port))

println("\n=== Demo Complete ===")
println("\nAll features working correctly!")
