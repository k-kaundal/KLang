# Classes and Objects in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What is a Class?](#what-is-a-class)
3. [What is an Object?](#what-is-an-object)
4. [Basic Syntax](#basic-syntax)
5. [Creating Your First Class](#creating-your-first-class)
6. [Instantiating Objects](#instantiating-objects)
7. [Best Practices](#best-practices)
8. [Common Mistakes](#common-mistakes)
9. [Examples](#examples)

---

## Introduction

Object-Oriented Programming (OOP) is a programming paradigm that organizes code into **classes** and **objects**. KLang fully supports OOP, allowing you to write clean, modular, and reusable code.

### Why Use Classes and Objects?

- **Organization**: Group related data and functionality together
- **Reusability**: Create multiple instances from a single class definition
- **Maintainability**: Easier to update and debug organized code
- **Real-world modeling**: Classes represent real-world entities naturally

---

## What is a Class?

A **class** is a blueprint or template for creating objects. It defines:
- **Properties** (data/attributes)
- **Methods** (functions/behavior)

Think of a class as a cookie cutter, and objects as the cookies made from it.

### Analogy
- **Class**: Recipe for making a cake
- **Object**: The actual cake you bake

---

## What is an Object?

An **object** is a concrete instance of a class. Each object has:
- Its own set of property values
- Access to the class's methods

### Example
```klang
# Class: Blueprint for a Car
class Car { ... }

# Objects: Actual cars
let myCar = new Car()      # Object 1
let yourCar = new Car()    # Object 2
```

---

## Basic Syntax

### Class Declaration

```klang
class ClassName {
    # Properties
    let property1: type = defaultValue
    let property2: type = defaultValue
    
    # Methods
    fn methodName() {
        # Method body
    }
}
```

### Object Creation

```klang
let objectName = new ClassName()
```

### Accessing Members

```klang
# Access property
objectName.propertyName

# Call method
objectName.methodName()
```

---

## Creating Your First Class

### Step 1: Define the Class

```klang
class Person {
    let name: string = "Unknown"
    let age: int = 0
    
    fn greet() {
        println("Hello, my name is " + this.name)
    }
}
```

**Explanation:**
- `class Person` - Declares a class named Person
- `let name: string = "Unknown"` - Property with type and default value
- `fn greet()` - Method that can be called on Person objects
- `this.name` - Refers to the current object's name property

### Step 2: Create Objects

```klang
let person1 = new Person()
let person2 = new Person()
```

### Step 3: Use the Objects

```klang
# Modify properties
person1.name = "Alice"
person1.age = 30

person2.name = "Bob"
person2.age = 25

# Call methods
person1.greet()  # Output: Hello, my name is Alice
person2.greet()  # Output: Hello, my name is Bob
```

---

## Quick Reference

### Class Definition
```klang
class MyClass {
    let property: type = value
    fn method() { }
}
```

### Create Object
```klang
let obj = new MyClass()
```

### Access Property
```klang
obj.property = newValue
let val = obj.property
```

### Call Method
```klang
obj.method()
let result = obj.method()
```

---

## Instantiating Objects

Creating objects (also called **instantiation**) is the process of making a concrete instance from a class blueprint.

### Basic Object Creation

```klang
# Syntax
let variableName = new ClassName()
```

### Example: Creating Multiple Objects

```klang
class Dog {
    let name: string = "Unknown"
    let breed: string = "Mixed"
    let age: int = 0
    
    fn bark() {
        println(this.name + " says: Woof!")
    }
}

# Create three different dog objects
let dog1 = new Dog()
dog1.name = "Max"
dog1.breed = "Golden Retriever"
dog1.age = 3

let dog2 = new Dog()
dog2.name = "Bella"
dog2.breed = "Labrador"
dog2.age = 2

let dog3 = new Dog()
dog3.name = "Charlie"
dog3.breed = "Beagle"
dog3.age = 5

# Each object is independent
dog1.bark()  # Max says: Woof!
dog2.bark()  # Bella says: Woof!
dog3.bark()  # Charlie says: Woof!
```

### Object Independence

Each object has its own copy of properties:

```klang
class Counter {
    let count: int = 0
    
    fn increment() {
        this.count += 1
    }
}

let counter1 = new Counter()
let counter2 = new Counter()

counter1.increment()
counter1.increment()
counter1.increment()

println(counter1.count)  # Output: 3
println(counter2.count)  # Output: 0 (independent!)
```

---

## Best Practices

### 1. Meaningful Class Names
Use descriptive, noun-based names that represent the entity:

```klang
# Good
class Customer { }
class ShoppingCart { }
class OrderProcessor { }

# Avoid
class Data { }
class Manager { }
class Handler { }
```

### 2. Single Responsibility Principle
Each class should have one clear purpose:

```klang
# Good - Each class has a focused responsibility
class User {
    let username: string
    let email: string
    
    fn validate() {
        # Validation logic
    }
}

class UserRepository {
    fn save(user: User) {
        # Save to database
    }
    
    fn find(id: int) {
        # Retrieve from database
    }
}

# Avoid - God class doing too much
class UserManager {
    fn validate() { }
    fn save() { }
    fn sendEmail() { }
    fn generateReport() { }
    fn processPayment() { }  # Too many responsibilities!
}
```

### 3. Proper Encapsulation
Keep data private when possible and provide controlled access:

```klang
class BankAccount {
    private let balance: float = 0.0
    
    fn deposit(amount: float) {
        if amount > 0 {
            this.balance += amount
        }
    }
    
    fn getBalance() {
        return this.balance
    }
}
```

### 4. Initialize Properties Properly
Always provide sensible default values or use constructors:

```klang
class Rectangle {
    let width: float = 1.0    # Sensible default
    let height: float = 1.0
    
    fn area() {
        return this.width * this.height
    }
}
```

### 5. Use Descriptive Method Names
Methods should be verbs that describe actions:

```klang
class Calculator {
    fn add(a: int, b: int) { }        # Good
    fn subtract(a: int, b: int) { }   # Good
    fn multiply(a: int, b: int) { }   # Good
    
    # Avoid vague names
    fn process(x, y) { }  # What does this do?
}
```

---

## Common Mistakes

### 1. Forgetting `this` Keyword

**Problem:**
```klang
class Counter {
    let count: int = 0
    
    fn increment() {
        count += 1  # ERROR: 'count' not defined
    }
}
```

**Solution:**
```klang
class Counter {
    let count: int = 0
    
    fn increment() {
        this.count += 1  # Correct: Use 'this'
    }
}
```

### 2. Modifying Objects Incorrectly

**Problem:**
```klang
let person = new Person()
person.getName() = "Alice"  # ERROR: Can't assign to method call
```

**Solution:**
```klang
let person = new Person()
person.name = "Alice"  # Correct: Assign to property
```

### 3. Creating Classes When Functions Suffice

**Problem:**
```klang
# Overkill for simple operations
class MathUtils {
    fn add(a: int, b: int) {
        return a + b
    }
}

let utils = new MathUtils()
let result = utils.add(5, 3)
```

**Solution:**
```klang
# Simple function is better
fn add(a: int, b: int) {
    return a + b
}

let result = add(5, 3)
```

### 4. Not Reusing Class Instances

**Problem:**
```klang
# Creating new objects unnecessarily
fn processUsers(users: array) {
    for user in users {
        let validator = new Validator()  # Created in loop!
        validator.validate(user)
    }
}
```

**Solution:**
```klang
# Reuse the same instance
fn processUsers(users: array) {
    let validator = new Validator()  # Created once
    for user in users {
        validator.validate(user)
    }
}
```

### 5. Tight Coupling Between Classes

**Problem:**
```klang
class Order {
    fn save() {
        let db = new MySQLDatabase()  # Tightly coupled
        db.save(this)
    }
}
```

**Solution:**
```klang
class Order {
    fn save(database) {
        database.save(this)  # Dependency injection
    }
}
```

---

## Examples

### Example 1: Book Management System

```klang
class Book {
    let title: string = ""
    let author: string = ""
    let pages: int = 0
    let isbn: string = ""
    private let isAvailable: bool = true
    
    fn getInfo() {
        return this.title + " by " + this.author + 
               " (" + str(this.pages) + " pages)"
    }
    
    fn borrow() {
        if this.isAvailable {
            this.isAvailable = false
            println("Book borrowed: " + this.title)
            return true
        } else {
            println("Book not available")
            return false
        }
    }
    
    fn returnBook() {
        this.isAvailable = true
        println("Book returned: " + this.title)
    }
    
    fn checkAvailability() {
        return this.isAvailable
    }
}

# Usage
let book1 = new Book()
book1.title = "The KLang Guide"
book1.author = "Jane Developer"
book1.pages = 350
book1.isbn = "978-1234567890"

println(book1.getInfo())
book1.borrow()
book1.borrow()  # Try to borrow again
book1.returnBook()
```

**Output:**
```
The KLang Guide by Jane Developer (350 pages)
Book borrowed: The KLang Guide
Book not available
Book returned: The KLang Guide
```

### Example 2: Student Grade Tracker

```klang
class Student {
    let name: string = ""
    let studentId: int = 0
    let grades: array = []
    
    fn addGrade(grade: float) {
        if grade >= 0 && grade <= 100 {
            this.grades.push(grade)
            println("Grade added: " + str(grade))
        } else {
            println("Invalid grade! Must be between 0 and 100")
        }
    }
    
    fn getAverage() {
        if len(this.grades) == 0 {
            return 0.0
        }
        
        let sum: float = 0.0
        for grade in this.grades {
            sum += grade
        }
        return sum / len(this.grades)
    }
    
    fn getLetterGrade() {
        let avg = this.getAverage()
        if avg >= 90 { return "A" }
        else if avg >= 80 { return "B" }
        else if avg >= 70 { return "C" }
        else if avg >= 60 { return "D" }
        else { return "F" }
    }
    
    fn displayReport() {
        println("\n=== Student Report ===")
        println("Name: " + this.name)
        println("ID: " + str(this.studentId))
        println("Grades: " + str(this.grades))
        println("Average: " + str(this.getAverage()))
        println("Letter Grade: " + this.getLetterGrade())
        println("====================\n")
    }
}

# Usage
let student = new Student()
student.name = "Alice Johnson"
student.studentId = 12345

student.addGrade(95.5)
student.addGrade(87.0)
student.addGrade(92.5)
student.addGrade(88.0)

student.displayReport()
```

### Example 3: Temperature Converter

```klang
class Temperature {
    private let celsius: float = 0.0
    
    fn setCelsius(temp: float) {
        this.celsius = temp
    }
    
    fn setFahrenheit(temp: float) {
        this.celsius = (temp - 32) * 5 / 9
    }
    
    fn setKelvin(temp: float) {
        this.celsius = temp - 273.15
    }
    
    fn getCelsius() {
        return this.celsius
    }
    
    fn getFahrenheit() {
        return (this.celsius * 9 / 5) + 32
    }
    
    fn getKelvin() {
        return this.celsius + 273.15
    }
    
    fn display() {
        println("Temperature:")
        println("  Celsius: " + str(this.getCelsius()) + "°C")
        println("  Fahrenheit: " + str(this.getFahrenheit()) + "°F")
        println("  Kelvin: " + str(this.getKelvin()) + "K")
    }
}

# Usage
let temp = new Temperature()
temp.setCelsius(25)
temp.display()

println("\nAfter setting to 100°F:")
temp.setFahrenheit(100)
temp.display()
```

### Example 4: Simple Shopping Cart

```klang
class Product {
    let name: string = ""
    let price: float = 0.0
    let quantity: int = 1
    
    fn getTotal() {
        return this.price * this.quantity
    }
}

class ShoppingCart {
    let items: array = []
    
    fn addItem(product: Product) {
        this.items.push(product)
        println("Added: " + product.name)
    }
    
    fn removeItem(productName: string) {
        let index = -1
        for i in range(len(this.items)) {
            if this.items[i].name == productName {
                index = i
                break
            }
        }
        
        if index >= 0 {
            this.items.splice(index, 1)
            println("Removed: " + productName)
        } else {
            println("Product not found: " + productName)
        }
    }
    
    fn getTotal() {
        let total: float = 0.0
        for item in this.items {
            total += item.getTotal()
        }
        return total
    }
    
    fn displayCart() {
        println("\n=== Shopping Cart ===")
        if len(this.items) == 0 {
            println("Cart is empty")
        } else {
            for item in this.items {
                println(item.name + " x" + str(item.quantity) + 
                       " = $" + str(item.getTotal()))
            }
            println("--------------------")
            println("Total: $" + str(this.getTotal()))
        }
        println("====================\n")
    }
}

# Usage
let cart = new ShoppingCart()

let product1 = new Product()
product1.name = "Laptop"
product1.price = 999.99
product1.quantity = 1

let product2 = new Product()
product2.name = "Mouse"
product2.price = 25.50
product2.quantity = 2

cart.addItem(product1)
cart.addItem(product2)
cart.displayCart()

cart.removeItem("Mouse")
cart.displayCart()
```

### Example 5: Timer/Stopwatch

```klang
class Stopwatch {
    private let startTime: float = 0.0
    private let endTime: float = 0.0
    private let isRunning: bool = false
    
    fn start() {
        if this.isRunning {
            println("Stopwatch is already running!")
            return
        }
        
        this.startTime = time()
        this.isRunning = true
        println("Stopwatch started")
    }
    
    fn stop() {
        if !this.isRunning {
            println("Stopwatch is not running!")
            return
        }
        
        this.endTime = time()
        this.isRunning = false
        println("Stopwatch stopped")
    }
    
    fn reset() {
        this.startTime = 0.0
        this.endTime = 0.0
        this.isRunning = false
        println("Stopwatch reset")
    }
    
    fn getElapsedTime() {
        if this.isRunning {
            return time() - this.startTime
        } else {
            return this.endTime - this.startTime
        }
    }
    
    fn display() {
        let elapsed = this.getElapsedTime()
        println("Elapsed time: " + str(elapsed) + " seconds")
    }
}

# Usage
let stopwatch = new Stopwatch()
stopwatch.start()
# ... do some work ...
stopwatch.stop()
stopwatch.display()
```

---

## Interview Problems

### Problem 1: Design a Circle Class
**Question:** Create a `Circle` class with radius property and methods to calculate area and circumference.

**Solution:**
```klang
class Circle {
    let radius: float = 0.0
    const PI: float = 3.14159
    
    fn setRadius(r: float) {
        if r > 0 {
            this.radius = r
        } else {
            println("Radius must be positive!")
        }
    }
    
    fn getArea() {
        return this.PI * this.radius * this.radius
    }
    
    fn getCircumference() {
        return 2 * this.PI * this.radius
    }
    
    fn display() {
        println("Circle with radius: " + str(this.radius))
        println("Area: " + str(this.getArea()))
        println("Circumference: " + str(this.getCircumference()))
    }
}

# Test
let circle = new Circle()
circle.setRadius(5.0)
circle.display()
```

### Problem 2: Counter with Min/Max Limits
**Question:** Create a `Counter` class that increments/decrements within specified min/max bounds.

**Solution:**
```klang
class Counter {
    private let value: int = 0
    private let min: int = 0
    private let max: int = 100
    
    fn setValue(val: int) {
        if val >= this.min && val <= this.max {
            this.value = val
        }
    }
    
    fn setLimits(minVal: int, maxVal: int) {
        this.min = minVal
        this.max = maxVal
        # Adjust current value if out of bounds
        if this.value < this.min {
            this.value = this.min
        }
        if this.value > this.max {
            this.value = this.max
        }
    }
    
    fn increment() {
        if this.value < this.max {
            this.value += 1
        } else {
            println("Cannot increment beyond max: " + str(this.max))
        }
    }
    
    fn decrement() {
        if this.value > this.min {
            this.value -= 1
        } else {
            println("Cannot decrement below min: " + str(this.min))
        }
    }
    
    fn getValue() {
        return this.value
    }
    
    fn reset() {
        this.value = this.min
    }
}

# Test
let counter = new Counter()
counter.setLimits(0, 10)
counter.setValue(5)
println(counter.getValue())  # 5
counter.increment()
println(counter.getValue())  # 6
```

### Problem 3: RGB Color Class
**Question:** Create a `Color` class to represent RGB colors with validation.

**Solution:**
```klang
class Color {
    private let red: int = 0
    private let green: int = 0
    private let blue: int = 0
    
    fn setRed(r: int) {
        this.red = this.clamp(r, 0, 255)
    }
    
    fn setGreen(g: int) {
        this.green = this.clamp(g, 0, 255)
    }
    
    fn setBlue(b: int) {
        this.blue = this.clamp(b, 0, 255)
    }
    
    fn setRGB(r: int, g: int, b: int) {
        this.setRed(r)
        this.setGreen(g)
        this.setBlue(b)
    }
    
    private fn clamp(value: int, min: int, max: int) {
        if value < min { return min }
        if value > max { return max }
        return value
    }
    
    fn getRed() { return this.red }
    fn getGreen() { return this.green }
    fn getBlue() { return this.blue }
    
    fn toHex() {
        return "#" + this.toHexByte(this.red) + 
               this.toHexByte(this.green) + 
               this.toHexByte(this.blue)
    }
    
    private fn toHexByte(value: int) {
        let hex = "0123456789ABCDEF"
        return str(hex[value / 16]) + str(hex[value % 16])
    }
    
    fn display() {
        println("RGB(" + str(this.red) + ", " + 
                str(this.green) + ", " + 
                str(this.blue) + ")")
        println("Hex: " + this.toHex())
    }
}

# Test
let color = new Color()
color.setRGB(255, 100, 50)
color.display()
```

### Problem 4: Queue Implementation
**Question:** Implement a `Queue` class with enqueue, dequeue, and peek operations.

**Solution:**
```klang
class Queue {
    private let items: array = []
    
    fn enqueue(item) {
        this.items.push(item)
    }
    
    fn dequeue() {
        if this.isEmpty() {
            println("Queue is empty!")
            return null
        }
        return this.items.shift()
    }
    
    fn peek() {
        if this.isEmpty() {
            return null
        }
        return this.items[0]
    }
    
    fn isEmpty() {
        return len(this.items) == 0
    }
    
    fn size() {
        return len(this.items)
    }
    
    fn display() {
        println("Queue: " + str(this.items))
    }
}

# Test
let queue = new Queue()
queue.enqueue("First")
queue.enqueue("Second")
queue.enqueue("Third")
queue.display()
println("Dequeue: " + queue.dequeue())
println("Peek: " + queue.peek())
queue.display()
```

### Problem 5: Bank Account with Transaction History
**Question:** Create a `BankAccount` class that tracks all transactions.

**Solution:**
```klang
class BankAccount {
    private let accountNumber: string = ""
    private let balance: float = 0.0
    private let transactions: array = []
    
    fn initialize(accNum: string, initialBalance: float) {
        this.accountNumber = accNum
        this.balance = initialBalance
        this.addTransaction("Account opened", initialBalance)
    }
    
    fn deposit(amount: float) {
        if amount <= 0 {
            println("Deposit amount must be positive!")
            return false
        }
        
        this.balance += amount
        this.addTransaction("Deposit", amount)
        println("Deposited $" + str(amount))
        return true
    }
    
    fn withdraw(amount: float) {
        if amount <= 0 {
            println("Withdrawal amount must be positive!")
            return false
        }
        
        if amount > this.balance {
            println("Insufficient funds!")
            return false
        }
        
        this.balance -= amount
        this.addTransaction("Withdrawal", -amount)
        println("Withdrew $" + str(amount))
        return true
    }
    
    private fn addTransaction(type: string, amount: float) {
        let transaction = {
            "type": type,
            "amount": amount,
            "balance": this.balance
        }
        this.transactions.push(transaction)
    }
    
    fn getBalance() {
        return this.balance
    }
    
    fn displayTransactions() {
        println("\n=== Transaction History ===")
        println("Account: " + this.accountNumber)
        for txn in this.transactions {
            println(txn["type"] + ": $" + str(txn["amount"]) + 
                   " | Balance: $" + str(txn["balance"]))
        }
        println("===========================\n")
    }
}

# Test
let account = new BankAccount()
account.initialize("ACC001", 1000.0)
account.deposit(500.0)
account.withdraw(200.0)
account.withdraw(2000.0)  # Should fail
println("Current balance: $" + str(account.getBalance()))
account.displayTransactions()
```

---

- **Classes** are blueprints for creating objects
- **Objects** are instances of classes
- Use `new ClassName()` to create objects
- Access members with dot notation: `object.property` or `object.method()`
- Use `this` keyword inside methods to refer to current object

Happy coding! 🚀
