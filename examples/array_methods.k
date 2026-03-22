// KLang Array Methods Examples
// Demonstrates JavaScript-like array methods

println("=== KLang Array Methods Examples ===")
println("")

// Example 1: Data transformation with map
println("--- Example 1: Data Transformation ---")
const prices = [10, 20, 30, 40, 50]
const withTax = prices.map(price => price * 1.1)
println("Original prices: " + str(prices))
println("With 10% tax: " + str(withTax))
println("")

// Example 2: Filtering data
println("--- Example 2: Filtering Data ---")
const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
const evens = numbers.filter(n => n % 2 == 0)
const odds = numbers.filter(n => n % 2 != 0)
const bigNumbers = numbers.filter(n => n > 5)
println("Numbers: " + str(numbers))
println("Evens: " + str(evens))
println("Odds: " + str(odds))
println("Greater than 5: " + str(bigNumbers))
println("")

// Example 3: Calculating totals with reduce
println("--- Example 3: Calculating Totals ---")
const cartItems = [100, 250, 75, 150, 300]
const total = cartItems.reduce((sum, item) => sum + item, 0)
const average = total / len(cartItems)
println("Cart items: " + str(cartItems))
println("Total: $" + str(total))
println("Average: $" + str(average))
println("")

// Example 4: Processing with forEach
println("--- Example 4: Processing Items ---")
const tasks = ["Write code", "Test features", "Document API"]
println("TODO List:")
tasks.forEach((task, index) => println("  " + str(index + 1) + ". " + task))
println("")

// Example 5: Finding specific elements
println("--- Example 5: Finding Elements ---")
const users = [
    {name: "Alice", age: 25},
    {name: "Bob", age: 30},
    {name: "Charlie", age: 35}
]

// Simulating object with arrays (since KLang has objects, but showing arrays)
const ages = [25, 30, 35, 40, 45]
const firstOver30 = ages.find(age => age > 30)
const hasTeenager = ages.some(age => age < 20)
const allAdults = ages.every(age => age >= 18)
println("Ages: " + str(ages))
println("First age over 30: " + str(firstOver30))
println("Has teenager: " + str(hasTeenager))
println("All adults: " + str(allAdults))
println("")

// Example 6: Complex data pipeline
println("--- Example 6: Data Pipeline ---")
const transactions = [10, -5, 20, -3, 15, -8, 25, -2, 30, -1]
println("Transactions: " + str(transactions))

// Calculate total positive transactions
const positiveTotal = transactions
    .filter(t => t > 0)
    .reduce((sum, t) => sum + t, 0)
println("Total income: $" + str(positiveTotal))

// Calculate total negative transactions
const negativeTotal = transactions
    .filter(t => t < 0)
    .reduce((sum, t) => sum + t, 0)
println("Total expenses: $" + str(negativeTotal))

// Net balance
const balance = transactions.reduce((sum, t) => sum + t, 0)
println("Net balance: $" + str(balance))
println("")

// Example 7: Array manipulation
println("--- Example 7: Array Manipulation ---")
let myArray = [1, 2, 3]
println("Original: " + str(myArray))

const sliced = myArray.slice(0, 2)
println("Sliced (0, 2): " + str(sliced))

const moreNumbers = [4, 5, 6]
const combined = myArray.concat(moreNumbers)
println("Concatenated: " + str(combined))

const joined = combined.join(" -> ")
println("Joined: " + joined)
println("")

// Example 8: String processing
println("--- Example 8: String Processing ---")
const words = ["hello", "world", "klang", "rocks"]
const capitalized = words.map(w => uppercase(w))
const longWords = words.filter(w => len(w) > 4)
const sentence = words.join(" ")
println("Words: " + str(words))
println("Capitalized: " + str(capitalized))
println("Long words (>4 chars): " + str(longWords))
println("Sentence: " + sentence)
println("")

// Example 9: Search and validate
println("--- Example 9: Search and Validate ---")
const inventory = [5, 10, 15, 20, 25]
const hasItem = inventory.includes(15)
const itemIndex = inventory.indexOf(20)
const lowStock = inventory.some(qty => qty < 10)
const allInStock = inventory.every(qty => qty > 0)
println("Inventory: " + str(inventory))
println("Has 15 units: " + str(hasItem))
println("Index of 20: " + str(itemIndex))
println("Has low stock (<10): " + str(lowStock))
println("All in stock: " + str(allInStock))
println("")

// Example 10: Mathematical operations
println("--- Example 10: Mathematical Operations ---")
const values = [1, 2, 3, 4, 5]
const squared = values.map(x => x * x)
const cubed = values.map(x => x * x * x)
const sumOfSquares = squared.reduce((sum, x) => sum + x, 0)
println("Values: " + str(values))
println("Squared: " + str(squared))
println("Cubed: " + str(cubed))
println("Sum of squares: " + str(sumOfSquares))
println("")

// Example 11: Chaining multiple operations
println("--- Example 11: Method Chaining ---")
const data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
const result = data
    .filter(x => x % 2 == 0)        // Get evens: [2, 4, 6, 8, 10]
    .map(x => x * 2)                // Double them: [4, 8, 12, 16, 20]
    .filter(x => x > 10)            // Keep > 10: [12, 16, 20]
    .reduce((sum, x) => sum + x, 0) // Sum: 48

println("Original: " + str(data))
println("After: filter evens -> map *2 -> filter >10 -> reduce sum")
println("Result: " + str(result))
println("Expected: 48 (12 + 16 + 20)")
println("")

// Example 12: Building formatted output
println("--- Example 12: Formatted Output ---")
const scores = [85, 92, 78, 95, 88]

// Simpler approach - use ternary or simple mapping
const grades = scores.map(score => score >= 90 ? "A" : (score >= 80 ? "B" : (score >= 70 ? "C" : "F")))
println("Scores: " + str(scores))
println("Grades: " + str(grades))
println("")

scores.forEach((score, i) => println("  Student " + str(i + 1) + ": " + str(score) + " (" + str(grades[i]) + ")"))
println("")

println("=== All Examples Complete ===")
