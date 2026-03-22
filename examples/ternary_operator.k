# Ternary Operator Examples in KLang

# Basic usage
let age = 18
let status = age >= 18 ? "adult" : "minor"
println(status)  # Output: adult

# With numbers
let a = 10
let b = 20
let max = a > b ? a : b
println("Max of " + a + " and " + b + " is: " + max)  # Output: Max of 10 and 20 is: 20

# Nested ternary for grade calculation
let score = 85
let grade = score >= 90 ? "A" : score >= 80 ? "B" : score >= 70 ? "C" : score >= 60 ? "D" : "F"
println("Score: " + score + ", Grade: " + grade)  # Output: Score: 85, Grade: B

# In function
fn abs(x) {
    return x >= 0 ? x : -x
}
println("Absolute value of -15: " + abs(-15))  # Output: Absolute value of -15: 15
println("Absolute value of 7: " + abs(7))      # Output: Absolute value of 7: 7

# Multiple ternaries
let x = 5
let y = 3
let r1 = x > 3 ? 10 : 20
let r2 = y > 3 ? 30 : 40
println("r1: " + r1 + ", r2: " + r2 + ", sum: " + (r1 + r2))  # Output: r1: 10, r2: 40, sum: 50

# Complex condition
let num = 15
let category = num > 20 ? "very large" : num > 10 ? "large" : num > 5 ? "medium" : "small"
println("Number " + num + " is " + category)  # Output: Number 15 is large

# In arithmetic expression
let bonus = 100
let multiplier = bonus > 50 ? 2 : 1
let total = bonus * multiplier
println("Bonus: " + bonus + ", with multiplier: " + multiplier + ", total: " + total)  # Output: Bonus: 100, with multiplier: 2, total: 200
