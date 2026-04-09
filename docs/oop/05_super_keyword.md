# The Super Keyword in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What is the Super Keyword?](#what-is-the-super-keyword)
3. [Uses of Super](#uses-of-super)
4. [Super in Constructors](#super-in-constructors)
5. [Super in Methods](#super-in-methods)
6. [Accessing Parent Properties](#accessing-parent-properties)
7. [Super vs This](#super-vs-this)
8. [Constructor Chaining](#constructor-chaining)
9. [Practical Examples](#practical-examples)
10. [Best Practices](#best-practices)
11. [Common Mistakes](#common-mistakes)
12. [Interview Problems](#interview-problems)

---

## Introduction

The **super keyword** is a special reference in KLang that allows a child class to access members (methods, properties, and constructors) of its parent class. It's essential for extending parent functionality, calling parent constructors, and maintaining proper inheritance chains.

### Why Super is Needed

- **Constructor Chaining**: Initialize parent state before child state
- **Method Augmentation**: Extend parent behavior without replacing it
- **Explicit Parent Access**: Call parent methods even when overridden
- **Proper Initialization**: Ensure parent class is properly set up
- **Code Reuse**: Leverage parent implementation while adding new features

### Key Concept

```
Parent Class defines base functionality
        ↓
Child Class uses super to access parent
        ↓
Extends parent behavior while maintaining inheritance
```

---

## What is the Super Keyword?

The `super` keyword is a reference to the parent class that allows:
1. **Calling parent constructors** - `super.init()`
2. **Calling parent methods** - `super.methodName()`
3. **Accessing parent properties** - `super.propertyName`

### Visual Representation

```
┌─────────────────────┐
│   Parent Class      │
│  + property         │
│  + init()           │
│  + method()         │
└─────────────────────┘
          ↑
          │ super refers to parent
          │
┌─────────────────────┐
│   Child Class       │
│  + init() {         │
│      super.init()   │ ← Calls parent constructor
│    }                │
│  + method() {       │
│      super.method() │ ← Calls parent method
│    }                │
└─────────────────────┘
```

### Simple Example

```klang
class Animal {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
        println("Animal created: " + n)
    }
    
    fn speak() {
        println("Animal makes a sound")
    }
}

class Dog extends Animal {
    let breed: string = ""
    
    fn init(n: string, b: string) {
        super.init(n)  # Call parent constructor
        this.breed = b
        println("Dog breed: " + b)
    }
    
    fn speak() {
        super.speak()  # Call parent method
        println("Dog barks: Woof!")
    }
}

let dog = new Dog("Buddy", "Golden Retriever")
# Output:
# Animal created: Buddy
# Dog breed: Golden Retriever

dog.speak()
# Output:
# Animal makes a sound
# Dog barks: Woof!
```

---

## Uses of Super

The super keyword has three primary uses in KLang:

### 1. Calling Parent Constructor

```klang
class Person {
    let name: string = ""
    let age: int = 0
    
    fn init(n: string, a: int) {
        this.name = n
        this.age = a
    }
}

class Employee extends Person {
    let employeeId: string = ""
    
    fn init(n: string, a: int, id: string) {
        super.init(n, a)  # Initialize parent state
        this.employeeId = id
    }
}
```

### 2. Calling Parent Methods

```klang
class Logger {
    fn log(message: string) {
        println("[LOG] " + message)
    }
}

class TimestampLogger extends Logger {
    fn log(message: string) {
        super.log(message)  # Call parent log
        println("[TIME] Current timestamp logged")
    }
}
```

### 3. Accessing Parent Properties

```klang
class Counter {
    let count: int = 0
    
    fn increment() {
        this.count = this.count + 1
    }
}

class DoubleCounter extends Counter {
    fn increment() {
        super.increment()  # Call parent increment
        super.increment()  # Call again to double
    }
}
```

---

## Super in Constructors

When using constructors in inheritance, calling the parent constructor with `super.init()` is crucial for proper initialization.

### Rule: Super Must Be First

The call to `super.init()` should be the **first statement** in the child constructor to ensure the parent is fully initialized before the child adds its own state.

```klang
class Vehicle {
    let brand: string = ""
    let year: int = 0
    
    fn init(b: string, y: int) {
        this.brand = b
        this.year = y
        println("Vehicle initialized")
    }
}

class Car extends Vehicle {
    let model: string = ""
    let doors: int = 0
    
    fn init(b: string, y: int, m: string, d: int) {
        super.init(b, y)  # ✓ First statement - Good!
        this.model = m
        this.doors = d
        println("Car initialized")
    }
}

let car = new Car("Toyota", 2023, "Camry", 4)
# Output:
# Vehicle initialized
# Car initialized
```

### Without Super - Incomplete Initialization

```klang
class Base {
    let baseValue: int = 0
    
    fn init(val: int) {
        this.baseValue = val
        println("Base initialized with: " + str(val))
    }
}

class Derived extends Base {
    let derivedValue: int = 0
    
    fn init(val: int) {
        # Missing super.init() - parent not initialized!
        this.derivedValue = val
        # baseValue remains 0 (default)
    }
}
```

### Constructor with Multiple Parameters

```klang
class Person {
    let firstName: string = ""
    let lastName: string = ""
    let age: int = 0
    
    fn init(first: string, last: string, a: int) {
        this.firstName = first
        this.lastName = last
        this.age = a
    }
    
    fn getFullName() -> string {
        return this.firstName + " " + this.lastName
    }
}

class Student extends Person {
    let studentId: string = ""
    let grade: float = 0.0
    
    fn init(first: string, last: string, a: int, id: string, g: float) {
        super.init(first, last, a)  # Pass parameters to parent
        this.studentId = id
        this.grade = g
    }
    
    fn displayInfo() {
        println("Student: " + this.getFullName())
        println("ID: " + this.studentId)
        println("Grade: " + str(this.grade))
    }
}

let student = new Student("John", "Doe", 20, "S12345", 3.8)
student.displayInfo()
# Output:
# Student: John Doe
# ID: S12345
# Grade: 3.8
```

---

## Super in Methods

Using super in method overriding allows you to extend parent behavior rather than completely replacing it.

### Basic Method Extension

```klang
class Shape {
    let name: string = "Shape"
    
    fn draw() {
        println("Drawing a shape")
    }
}

class Circle extends Shape {
    let radius: float = 0.0
    
    fn init(r: float) {
        this.name = "Circle"
        this.radius = r
    }
    
    fn draw() {
        super.draw()  # Call parent draw first
        println("Drawing circle with radius: " + str(this.radius))
    }
}

let circle = new Circle(5.0)
circle.draw()
# Output:
# Drawing a shape
# Drawing circle with radius: 5
```

### Adding Pre and Post Behavior

```klang
class DatabaseConnection {
    let connected: bool = false
    
    fn connect() {
        this.connected = true
        println("Database connected")
    }
    
    fn disconnect() {
        this.connected = false
        println("Database disconnected")
    }
}

class PooledConnection extends DatabaseConnection {
    let poolId: int = 0
    
    fn init(id: int) {
        this.poolId = id
    }
    
    fn connect() {
        println("Acquiring connection from pool " + str(this.poolId))
        super.connect()  # Call parent connect
        println("Connection acquired")
    }
    
    fn disconnect() {
        println("Returning connection to pool " + str(this.poolId))
        super.disconnect()  # Call parent disconnect
        println("Connection returned")
    }
}

let conn = new PooledConnection(1)
conn.connect()
# Output:
# Acquiring connection from pool 1
# Database connected
# Connection acquired

conn.disconnect()
# Output:
# Returning connection to pool 1
# Database disconnected
# Connection returned
```

### Conditional Super Calls

```klang
class Validator {
    fn validate(input: string) -> bool {
        if input == "" {
            println("Validation failed: empty input")
            return false
        }
        println("Basic validation passed")
        return true
    }
}

class EmailValidator extends Validator {
    fn validate(input: string) -> bool {
        # First validate with parent
        if !super.validate(input) {
            return false
        }
        
        # Add email-specific validation
        let hasAt = false
        let i = 0
        while i < len(input) {
            if charAt(input, i) == "@" {
                hasAt = true
            }
            i = i + 1
        }
        
        if !hasAt {
            println("Email validation failed: missing @")
            return false
        }
        
        println("Email validation passed")
        return true
    }
}

let validator = new EmailValidator()
println(validator.validate("user@example.com"))
# Output:
# Basic validation passed
# Email validation passed
# true

println(validator.validate("invalid-email"))
# Output:
# Basic validation passed
# Email validation failed: missing @
# false
```

---

## Accessing Parent Properties

The super keyword can also be used to access parent properties, especially when they might be shadowed by child properties.

### Example with Property Access

```klang
class Counter {
    let value: int = 0
    
    fn init(v: int) {
        this.value = v
    }
    
    fn getValue() -> int {
        return this.value
    }
}

class DoubleCounter extends Counter {
    fn init(v: int) {
        super.init(v)
    }
    
    fn doubleValue() {
        # Access parent's value through method
        let current = super.getValue()
        this.value = current * 2
    }
}

let counter = new DoubleCounter(5)
println("Initial: " + str(counter.getValue()))  # Output: Initial: 5
counter.doubleValue()
println("Doubled: " + str(counter.getValue()))  # Output: Doubled: 10
```

### Multiple Level Access

```klang
class GrandParent {
    let generation: int = 1
    
    fn introduce() {
        println("I am generation " + str(this.generation))
    }
}

class Parent extends GrandParent {
    fn init() {
        this.generation = 2
    }
    
    fn introduce() {
        super.introduce()  # Call grandparent's introduce
        println("Parent generation")
    }
}

class Child extends Parent {
    fn init() {
        super.init()
        this.generation = 3
    }
    
    fn introduce() {
        super.introduce()  # Call parent's introduce
        println("Child generation")
    }
}

let child = new Child()
child.introduce()
# Output:
# I am generation 3
# Parent generation
# Child generation
```

---

## Super vs This

Understanding the difference between `super` and `this` is crucial for effective inheritance.

### Key Differences

| Aspect | `this` | `super` |
|--------|--------|---------|
| **Reference** | Current object | Parent class |
| **Methods** | Current class (overridden) | Parent class (original) |
| **Properties** | Current object's properties | Parent's properties |
| **Constructor** | Current class constructor | Parent constructor |
| **Usage** | Access own members | Access parent members |

### Comparison Example

```klang
class Animal {
    let type: string = "Animal"
    
    fn getType() -> string {
        return this.type
    }
    
    fn speak() {
        println("Animal sound")
    }
}

class Dog extends Animal {
    fn init() {
        this.type = "Dog"
    }
    
    fn speak() {
        println("Woof!")
    }
    
    fn demonstrateDifference() {
        println("Using this.speak():")
        this.speak()  # Calls Dog's speak
        
        println("Using super.speak():")
        super.speak()  # Calls Animal's speak
        
        println("Type via this: " + this.getType())
        println("Type via super: " + super.getType())
    }
}

let dog = new Dog()
dog.demonstrateDifference()
# Output:
# Using this.speak():
# Woof!
# Using super.speak():
# Animal sound
# Type via this: Dog
# Type via super: Dog
```

### When to Use Which

```klang
class Account {
    let balance: float = 0.0
    
    fn init(initial: float) {
        this.balance = initial  # Use this for current object
    }
    
    fn deposit(amount: float) {
        this.balance = this.balance + amount
    }
    
    fn getBalance() -> float {
        return this.balance
    }
}

class SavingsAccount extends Account {
    let interestRate: float = 0.0
    
    fn init(initial: float, rate: float) {
        super.init(initial)  # Use super to call parent constructor
        this.interestRate = rate  # Use this for current object
    }
    
    fn deposit(amount: float) {
        super.deposit(amount)  # Use super to call parent method
        println("Deposited to savings: " + str(amount))
    }
    
    fn addInterest() {
        let interest = this.getBalance() * this.interestRate  # Use this for current methods
        this.deposit(interest)  # Calls SavingsAccount's deposit
    }
}

let savings = new SavingsAccount(1000.0, 0.05)
savings.addInterest()
# Output:
# Deposited to savings: 50
```

---

## Constructor Chaining

Constructor chaining is the process of calling constructors up the inheritance hierarchy, ensuring each level is properly initialized.

### Single Level Chaining

```klang
class Vehicle {
    let brand: string = ""
    
    fn init(b: string) {
        this.brand = b
        println("1. Vehicle constructor called")
    }
}

class Car extends Vehicle {
    let model: string = ""
    
    fn init(b: string, m: string) {
        println("2. Car constructor started")
        super.init(b)  # Chain to parent
        this.model = m
        println("3. Car constructor finished")
    }
}

let car = new Car("Toyota", "Camry")
# Output:
# 2. Car constructor started
# 1. Vehicle constructor called
# 3. Car constructor finished
```

### Multi-Level Chaining

```klang
class Employee {
    let name: string = ""
    let id: string = ""
    
    fn init(n: string, empId: string) {
        this.name = n
        this.id = empId
        println("Employee initialized: " + n)
    }
}

class Manager extends Employee {
    let department: string = ""
    
    fn init(n: string, empId: string, dept: string) {
        super.init(n, empId)
        this.department = dept
        println("Manager initialized: " + dept)
    }
}

class SeniorManager extends Manager {
    let teamSize: int = 0
    
    fn init(n: string, empId: string, dept: string, size: int) {
        super.init(n, empId, dept)
        this.teamSize = size
        println("SeniorManager initialized: team size " + str(size))
    }
    
    fn displayInfo() {
        println("Name: " + this.name)
        println("ID: " + this.id)
        println("Department: " + this.department)
        println("Team Size: " + str(this.teamSize))
    }
}

let seniorMgr = new SeniorManager("Alice", "M123", "Engineering", 15)
# Output:
# Employee initialized: Alice
# Manager initialized: Engineering
# SeniorManager initialized: team size 15

seniorMgr.displayInfo()
# Output:
# Name: Alice
# ID: M123
# Department: Engineering
# Team Size: 15
```

### Complete Initialization Chain

```klang
class LivingBeing {
    let alive: bool = true
    
    fn init() {
        this.alive = true
        println("1. LivingBeing: Alive status set")
    }
}

class Animal extends LivingBeing {
    let species: string = ""
    
    fn init(s: string) {
        super.init()
        this.species = s
        println("2. Animal: Species set to " + s)
    }
}

class Mammal extends Animal {
    let warmBlooded: bool = true
    
    fn init(s: string) {
        super.init(s)
        this.warmBlooded = true
        println("3. Mammal: Warm-blooded trait set")
    }
}

class Dog extends Mammal {
    let breed: string = ""
    
    fn init(s: string, b: string) {
        super.init(s)
        this.breed = b
        println("4. Dog: Breed set to " + b)
    }
}

let dog = new Dog("Canis familiaris", "Labrador")
# Output:
# 1. LivingBeing: Alive status set
# 2. Animal: Species set to Canis familiaris
# 3. Mammal: Warm-blooded trait set
# 4. Dog: Breed set to Labrador
```

---

## Practical Examples

### Example 1: Employee Hierarchy with Super

```klang
class Person {
    let name: string = ""
    let age: int = 0
    
    fn init(n: string, a: int) {
        this.name = n
        this.age = a
    }
    
    fn introduce() {
        println("Hi, I'm " + this.name + ", age " + str(this.age))
    }
}

class Employee extends Person {
    let employeeId: string = ""
    let salary: float = 0.0
    
    fn init(n: string, a: int, id: string, sal: float) {
        super.init(n, a)
        this.employeeId = id
        this.salary = sal
    }
    
    fn introduce() {
        super.introduce()  # Call parent introduce
        println("Employee ID: " + this.employeeId)
        println("Salary: $" + str(this.salary))
    }
    
    fn work() {
        println(this.name + " is working")
    }
}

class Developer extends Employee {
    let programmingLanguage: string = ""
    let projectCount: int = 0
    
    fn init(n: string, a: int, id: string, sal: float, lang: string) {
        super.init(n, a, id, sal)
        this.programmingLanguage = lang
        this.projectCount = 0
    }
    
    fn introduce() {
        super.introduce()  # Call Employee's introduce
        println("Programming Language: " + this.programmingLanguage)
        println("Projects Completed: " + str(this.projectCount))
    }
    
    fn work() {
        super.work()  # Call Employee's work
        println("Coding in " + this.programmingLanguage)
    }
    
    fn completeProject() {
        this.projectCount = this.projectCount + 1
        println("Project completed! Total: " + str(this.projectCount))
    }
}

# Usage
let dev = new Developer("Bob", 30, "E456", 85000.0, "KLang")
dev.introduce()
# Output:
# Hi, I'm Bob, age 30
# Employee ID: E456
# Salary: $85000
# Programming Language: KLang
# Projects Completed: 0

dev.work()
# Output:
# Bob is working
# Coding in KLang

dev.completeProject()
# Output:
# Project completed! Total: 1
```

### Example 2: Vehicle Initialization Chain

```klang
class Vehicle {
    let registrationNumber: string = ""
    let yearManufactured: int = 0
    
    fn init(regNum: string, year: int) {
        this.registrationNumber = regNum
        this.yearManufactured = year
        println("Vehicle registered: " + regNum)
    }
    
    fn start() {
        println("Vehicle starting...")
    }
    
    fn displayInfo() {
        println("Registration: " + this.registrationNumber)
        println("Year: " + str(this.yearManufactured))
    }
}

class MotorVehicle extends Vehicle {
    let engineCapacity: float = 0.0
    let fuelType: string = ""
    
    fn init(regNum: string, year: int, engine: float, fuel: string) {
        super.init(regNum, year)
        this.engineCapacity = engine
        this.fuelType = fuel
        println("Motor vehicle initialized")
    }
    
    fn start() {
        super.start()
        println("Engine starting with " + this.fuelType)
    }
    
    fn displayInfo() {
        super.displayInfo()
        println("Engine: " + str(this.engineCapacity) + "L")
        println("Fuel: " + this.fuelType)
    }
}

class Car extends MotorVehicle {
    let brand: string = ""
    let model: string = ""
    let doors: int = 0
    
    fn init(regNum: string, year: int, engine: float, fuel: string, 
            b: string, m: string, d: int) {
        super.init(regNum, year, engine, fuel)
        this.brand = b
        this.model = m
        this.doors = d
        println("Car initialized: " + b + " " + m)
    }
    
    fn start() {
        super.start()
        println(this.brand + " " + this.model + " ready to drive")
    }
    
    fn displayInfo() {
        super.displayInfo()
        println("Brand: " + this.brand)
        println("Model: " + this.model)
        println("Doors: " + str(this.doors))
    }
}

# Usage
let car = new Car("ABC123", 2023, 2.0, "Gasoline", "Honda", "Civic", 4)
# Output:
# Vehicle registered: ABC123
# Motor vehicle initialized
# Car initialized: Honda Civic

println("\n--- Starting Car ---")
car.start()
# Output:
# Vehicle starting...
# Engine starting with Gasoline
# Honda Civic ready to drive

println("\n--- Car Information ---")
car.displayInfo()
# Output:
# Registration: ABC123
# Year: 2023
# Engine: 2L
# Fuel: Gasoline
# Brand: Honda
# Model: Civic
# Doors: 4
```

### Example 3: Shape with Extended Functionality

```klang
class Shape {
    let color: string = ""
    let filled: bool = false
    
    fn init(c: string, f: bool) {
        this.color = c
        this.filled = f
    }
    
    fn area() -> float {
        return 0.0
    }
    
    fn perimeter() -> float {
        return 0.0
    }
    
    fn describe() {
        println("Shape color: " + this.color)
        if this.filled {
            println("Shape is filled")
        } else {
            println("Shape is not filled")
        }
    }
}

class Rectangle extends Shape {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(c: string, f: bool, w: float, h: float) {
        super.init(c, f)
        this.width = w
        this.height = h
    }
    
    fn area() -> float {
        return this.width * this.height
    }
    
    fn perimeter() -> float {
        return 2.0 * (this.width + this.height)
    }
    
    fn describe() {
        super.describe()  # Call parent describe
        println("Rectangle dimensions: " + str(this.width) + " x " + str(this.height))
        println("Area: " + str(this.area()))
        println("Perimeter: " + str(this.perimeter()))
    }
}

class Square extends Rectangle {
    fn init(c: string, f: bool, side: float) {
        super.init(c, f, side, side)  # Pass same value for width and height
    }
    
    fn describe() {
        super.describe()  # Call Rectangle's describe
        println("This is a square with side: " + str(this.width))
    }
    
    fn isSquare() -> bool {
        return this.width == this.height
    }
}

# Usage
let square = new Square("blue", true, 5.0)
square.describe()
# Output:
# Shape color: blue
# Shape is filled
# Rectangle dimensions: 5 x 5
# Area: 25
# Perimeter: 20
# This is a square with side: 5

println("Is square? " + str(square.isSquare()))
# Output: Is square? true
```

### Example 4: Logger Hierarchy

```klang
class Logger {
    let logLevel: string = "INFO"
    
    fn init(level: string) {
        this.logLevel = level
    }
    
    fn log(message: string) {
        println("[" + this.logLevel + "] " + message)
    }
}

class FileLogger extends Logger {
    let filename: string = ""
    
    fn init(level: string, file: string) {
        super.init(level)
        this.filename = file
    }
    
    fn log(message: string) {
        super.log(message)  # Log to console via parent
        println("Written to file: " + this.filename)
    }
}

class TimestampLogger extends FileLogger {
    fn init(level: string, file: string) {
        super.init(level, file)
    }
    
    fn log(message: string) {
        println("[TIMESTAMP] 2024-01-15 10:30:00")
        super.log(message)  # Call FileLogger's log
    }
}

# Usage
let logger = new TimestampLogger("ERROR", "app.log")
logger.log("Application crashed")
# Output:
# [TIMESTAMP] 2024-01-15 10:30:00
# [ERROR] Application crashed
# Written to file: app.log
```

### Example 5: Banking System

```klang
class Account {
    let accountNumber: string = ""
    let balance: float = 0.0
    
    fn init(accNum: string, initialBalance: float) {
        this.accountNumber = accNum
        this.balance = initialBalance
        println("Account created: " + accNum)
    }
    
    fn deposit(amount: float) {
        this.balance = this.balance + amount
        println("Deposited: $" + str(amount))
    }
    
    fn withdraw(amount: float) -> bool {
        if amount > this.balance {
            println("Insufficient funds")
            return false
        }
        this.balance = this.balance - amount
        println("Withdrawn: $" + str(amount))
        return true
    }
    
    fn getBalance() -> float {
        return this.balance
    }
}

class SavingsAccount extends Account {
    let interestRate: float = 0.0
    let minimumBalance: float = 0.0
    
    fn init(accNum: string, initialBalance: float, rate: float, minBal: float) {
        super.init(accNum, initialBalance)
        this.interestRate = rate
        this.minimumBalance = minBal
        println("Savings account with " + str(rate * 100.0) + "% interest")
    }
    
    fn withdraw(amount: float) -> bool {
        let newBalance = this.balance - amount
        if newBalance < this.minimumBalance {
            println("Cannot withdraw: minimum balance requirement")
            return false
        }
        return super.withdraw(amount)  # Call parent withdraw
    }
    
    fn addInterest() {
        let interest = this.balance * this.interestRate
        super.deposit(interest)  # Use parent's deposit
        println("Interest added: $" + str(interest))
    }
}

class CheckingAccount extends Account {
    let overdraftLimit: float = 0.0
    
    fn init(accNum: string, initialBalance: float, overdraft: float) {
        super.init(accNum, initialBalance)
        this.overdraftLimit = overdraft
        println("Checking account with $" + str(overdraft) + " overdraft")
    }
    
    fn withdraw(amount: float) -> bool {
        let availableAmount = this.balance + this.overdraftLimit
        if amount > availableAmount {
            println("Exceeds overdraft limit")
            return false
        }
        # Call parent withdraw for normal case
        if amount <= this.balance {
            return super.withdraw(amount)
        }
        # Handle overdraft case
        this.balance = this.balance - amount
        println("Withdrawn: $" + str(amount) + " (using overdraft)")
        return true
    }
}

# Usage
let savings = new SavingsAccount("SAV001", 1000.0, 0.05, 500.0)
# Output:
# Account created: SAV001
# Savings account with 5% interest

savings.addInterest()
# Output:
# Deposited: $50
# Interest added: $50

println("Balance: $" + str(savings.getBalance()))
# Output: Balance: $1050

let checking = new CheckingAccount("CHK001", 500.0, 200.0)
# Output:
# Account created: CHK001
# Checking account with $200 overdraft

checking.withdraw(600.0)
# Output:
# Withdrawn: $600 (using overdraft)
```

---

## Best Practices

### 1. Always Call Super in Constructor

Always call the parent constructor to ensure proper initialization.

```klang
# ✓ Good
class Child extends Parent {
    fn init(param: string) {
        super.init(param)  # Always call parent constructor
        # Child-specific initialization
    }
}

# ✗ Bad
class Child extends Parent {
    fn init(param: string) {
        # Missing super.init() - parent not initialized!
        # Child-specific initialization
    }
}
```

### 2. Call Super.init() First

Place the super.init() call as the first statement in the constructor.

```klang
# ✓ Good
class Employee extends Person {
    fn init(name: string, id: string) {
        super.init(name)  # First statement
        this.employeeId = id
    }
}

# ⚠ Not Recommended
class Employee extends Person {
    fn init(name: string, id: string) {
        this.employeeId = id  # Using this before super
        super.init(name)  # Super should be first
    }
}
```

### 3. Use Super to Extend, Not Replace

When you want to keep parent behavior, use super to extend it.

```klang
# ✓ Good - Extends parent behavior
class ExtendedLogger extends Logger {
    fn log(message: string) {
        super.log(message)  # Keep parent functionality
        # Add extra functionality
        println("Additional logging info")
    }
}

# ✓ Also Good - Completely new behavior
class CustomLogger extends Logger {
    fn log(message: string) {
        # Don't call super - complete replacement
        println("CUSTOM: " + message)
    }
}
```

### 4. Document Super Usage

Make it clear when and why you're using super.

```klang
class ValidatingInput extends BasicInput {
    fn setValue(val: string) {
        # First validate the input
        if this.validate(val) {
            # Then use parent's setValue for actual storage
            super.setValue(val)
        } else {
            println("Validation failed")
        }
    }
}
```

### 5. Maintain Consistent Signatures

When overriding methods that use super, maintain the same signature.

```klang
# ✓ Good
class Parent {
    fn process(data: string) -> bool {
        return true
    }
}

class Child extends Parent {
    fn process(data: string) -> bool {  # Same signature
        let result = super.process(data)
        # Additional processing
        return result
    }
}
```

### 6. Chain Initialization Properly

Ensure the entire initialization chain is maintained.

```klang
class Base {
    let baseValue: int = 0
    fn init(val: int) {
        this.baseValue = val
    }
}

class Middle extends Base {
    let middleValue: int = 0
    fn init(base: int, mid: int) {
        super.init(base)  # Chain to Base
        this.middleValue = mid
    }
}

class Top extends Middle {
    let topValue: int = 0
    fn init(base: int, mid: int, top: int) {
        super.init(base, mid)  # Chain to Middle (which chains to Base)
        this.topValue = top
    }
}
```

---

## Common Mistakes

### Mistake 1: Forgetting Super in Constructor

**Problem**: Parent class is not properly initialized.

```klang
class Parent {
    let value: int = 0
    
    fn init(v: int) {
        this.value = v
        println("Parent initialized")
    }
}

# ✗ Wrong
class Child extends Parent {
    let childValue: int = 0
    
    fn init(v: int, cv: int) {
        # Missing super.init(v)!
        this.childValue = cv
        # Parent's value remains 0
    }
}

# ✓ Correct
class Child extends Parent {
    let childValue: int = 0
    
    fn init(v: int, cv: int) {
        super.init(v)  # Initialize parent
        this.childValue = cv
    }
}
```

### Mistake 2: Incorrect Super Placement

**Problem**: Using super after child initialization can cause issues.

```klang
# ✗ Wrong - Super called too late
class Vehicle extends Base {
    let speed: float = 0.0
    
    fn init(s: float) {
        this.speed = s  # Using this before super
        super.init()    # Should be first!
    }
}

# ✓ Correct
class Vehicle extends Base {
    let speed: float = 0.0
    
    fn init(s: float) {
        super.init()    # Initialize parent first
        this.speed = s
    }
}
```

### Mistake 3: Circular Super Calls

**Problem**: Creating infinite recursion with super.

```klang
# ✗ Wrong - Infinite recursion
class Parent {
    fn process() {
        println("Parent processing")
    }
}

class Child extends Parent {
    fn process() {
        this.process()  # ✗ Calls itself, not parent!
        # Infinite recursion
    }
}

# ✓ Correct
class Child extends Parent {
    fn process() {
        super.process()  # ✓ Calls parent
        println("Child processing")
    }
}
```

### Mistake 4: Wrong Super Method Call

**Problem**: Calling wrong parent method or with wrong parameters.

```klang
class Parent {
    fn calculate(a: int, b: int) -> int {
        return a + b
    }
}

# ✗ Wrong
class Child extends Parent {
    fn calculate(a: int, b: int) -> int {
        # Wrong: missing parameter
        let result = super.calculate(a)  # ✗ Error!
        return result * 2
    }
}

# ✓ Correct
class Child extends Parent {
    fn calculate(a: int, b: int) -> int {
        let result = super.calculate(a, b)  # ✓ Correct parameters
        return result * 2
    }
}
```

### Mistake 5: Confusing Super with This

**Problem**: Using super when this is needed, or vice versa.

```klang
class Animal {
    fn speak() {
        println("Animal sound")
    }
}

class Dog extends Animal {
    fn speak() {
        println("Woof!")
    }
    
    fn makeSound() {
        # ✗ Wrong - calls parent's speak
        super.speak()  # Output: Animal sound
        
        # ✓ Correct - calls Dog's speak
        this.speak()   # Output: Woof!
    }
}
```

### Mistake 6: Multiple Super Calls Without Reason

**Problem**: Calling super multiple times unnecessarily.

```klang
# ✗ Wrong - Redundant super calls
class Child extends Parent {
    fn init(value: int) {
        super.init(value)
        super.init(value)  # ✗ Redundant!
        super.init(value)  # ✗ Redundant!
    }
}

# ✓ Correct - Call super once
class Child extends Parent {
    fn init(value: int) {
        super.init(value)  # ✓ Once is enough
    }
}

# ✓ Correct - Multiple calls with purpose
class DoubleCounter extends Counter {
    fn increment() {
        super.increment()  # ✓ Increment once
        super.increment()  # ✓ Increment twice (intentional)
    }
}
```

---

## Interview Problems

### Problem 1: Constructor Chain Analysis

**Question**: What will be the output of this code?

```klang
class A {
    fn init() {
        println("A constructor")
    }
}

class B extends A {
    fn init() {
        println("B constructor start")
        super.init()
        println("B constructor end")
    }
}

class C extends B {
    fn init() {
        println("C constructor start")
        super.init()
        println("C constructor end")
    }
}

let obj = new C()
```

**Answer**:
```
C constructor start
B constructor start
A constructor
B constructor end
C constructor end
```

**Explanation**: The constructor chain goes: C.init() → B.init() → A.init(). Each super.init() call goes up one level, and execution returns down the chain.

---

### Problem 2: Method Override with Super

**Question**: Implement a class hierarchy where each level multiplies the result by a factor.

```klang
class Base {
    fn calculate(value: int) -> int {
        return value
    }
}

class Double extends Base {
    # Double the result
    fn calculate(value: int) -> int {
        # Your code here
    }
}

class Quadruple extends Double {
    # Quadruple the result (2x of Double)
    fn calculate(value: int) -> int {
        # Your code here
    }
}

# Test
let quad = new Quadruple()
println(quad.calculate(5))  # Should output: 20
```

**Solution**:

```klang
class Base {
    fn calculate(value: int) -> int {
        return value
    }
}

class Double extends Base {
    fn calculate(value: int) -> int {
        return super.calculate(value) * 2
    }
}

class Quadruple extends Double {
    fn calculate(value: int) -> int {
        return super.calculate(value) * 2
    }
}

# Test
let quad = new Quadruple()
println(quad.calculate(5))  # Output: 20
# Explanation: 5 * 2 (from Double) * 2 (from Quadruple) = 20
```

---

### Problem 3: Validation Chain

**Question**: Create a validation chain where each level adds validation rules.

```klang
# Implement this hierarchy:
# - BaseValidator: checks if input is not empty
# - LengthValidator: checks if length >= 5 (extends BaseValidator)
# - AlphaValidator: checks if all characters are letters (extends LengthValidator)

class BaseValidator {
    fn validate(input: string) -> bool {
        # Your code here
    }
}

class LengthValidator extends BaseValidator {
    fn validate(input: string) -> bool {
        # Your code here - use super.validate()
    }
}

class AlphaValidator extends LengthValidator {
    fn validate(input: string) -> bool {
        # Your code here - use super.validate()
    }
}

# Test
let validator = new AlphaValidator()
println(validator.validate("Hello"))   # true
println(validator.validate("Hi"))      # false (too short)
println(validator.validate("Hello1"))  # false (has digit)
```

**Solution**:

```klang
class BaseValidator {
    fn validate(input: string) -> bool {
        if input == "" {
            println("Validation failed: empty")
            return false
        }
        println("Base validation passed")
        return true
    }
}

class LengthValidator extends BaseValidator {
    fn validate(input: string) -> bool {
        if !super.validate(input) {
            return false
        }
        if len(input) < 5 {
            println("Validation failed: too short")
            return false
        }
        println("Length validation passed")
        return true
    }
}

class AlphaValidator extends LengthValidator {
    fn validate(input: string) -> bool {
        if !super.validate(input) {
            return false
        }
        let i = 0
        while i < len(input) {
            let ch = charAt(input, i)
            # Simple check: A-Z or a-z
            if !((ch >= "A" && ch <= "Z") || (ch >= "a" && ch <= "z")) {
                println("Validation failed: non-alphabetic character")
                return false
            }
            i = i + 1
        }
        println("Alpha validation passed")
        return true
    }
}

# Test
let validator = new AlphaValidator()
println(validator.validate("Hello"))   # true
println(validator.validate("Hi"))      # false
println(validator.validate("Hello1"))  # false
```

---

### Problem 4: Shape Area Calculator

**Question**: What's wrong with this code and how to fix it?

```klang
class Shape {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
    }
    
    fn area() -> float {
        return 0.0
    }
}

class Rectangle extends Shape {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(w: float, h: float) {
        # Bug is here
        this.width = w
        this.height = h
    }
    
    fn area() -> float {
        return this.width * this.height
    }
}

let rect = new Rectangle(5.0, 3.0)
println("Shape: " + rect.name)  # What prints?
println("Area: " + str(rect.area()))
```

**Problem**: The parent constructor is not called, so `name` is never set.

**Solution**:

```klang
class Shape {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
    }
    
    fn area() -> float {
        return 0.0
    }
}

class Rectangle extends Shape {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(w: float, h: float) {
        super.init("Rectangle")  # ✓ Call parent constructor
        this.width = w
        this.height = h
    }
    
    fn area() -> float {
        return this.width * this.height
    }
}

let rect = new Rectangle(5.0, 3.0)
println("Shape: " + rect.name)  # Output: Shape: Rectangle
println("Area: " + str(rect.area()))  # Output: Area: 15
```

---

### Problem 5: Complex Initialization Order

**Question**: Predict the output and explain the initialization order.

```klang
class A {
    let valueA: int = 0
    
    fn init(v: int) {
        this.valueA = v
        println("A: " + str(this.valueA))
        this.display()
    }
    
    fn display() {
        println("A.display: " + str(this.valueA))
    }
}

class B extends A {
    let valueB: int = 0
    
    fn init(v: int) {
        super.init(v * 2)
        this.valueB = v
        println("B: " + str(this.valueB))
    }
    
    fn display() {
        println("B.display: A=" + str(this.valueA) + ", B=" + str(this.valueB))
    }
}

let obj = new B(5)
```

**Answer**:
```
A: 10
B.display: A=10, B=0
B: 5
```

**Explanation**:
1. `new B(5)` calls B's constructor with v=5
2. B's constructor calls `super.init(v * 2)` which is `super.init(10)`
3. A's constructor sets valueA=10, prints "A: 10"
4. A's constructor calls `this.display()` - but since obj is type B, it calls B's display
5. B.display prints, but valueB is still 0 (not initialized yet)
6. Control returns to B's constructor
7. B's constructor sets valueB=5, prints "B: 5"

**Key Insight**: When parent constructor calls a method that is overridden, the child's version is called, even though child's initialization isn't complete!

---

## Summary

The **super keyword** is essential for:

1. **Calling Parent Constructors**: `super.init()` ensures proper initialization chain
2. **Calling Parent Methods**: `super.methodName()` extends parent functionality
3. **Accessing Parent Properties**: `super.propertyName` when needed
4. **Constructor Chaining**: Maintaining proper initialization order
5. **Code Reuse**: Leveraging parent implementation while adding new features

### Key Points to Remember

- ✓ Always call `super.init()` in child constructors
- ✓ Call `super.init()` as the first statement
- ✓ Use super to extend parent behavior, not just replace it
- ✓ Understand difference between `super` and `this`
- ✓ Maintain consistent method signatures when using super
- ✗ Don't forget super.init() in constructors
- ✗ Don't confuse super with this
- ✗ Don't create circular super calls

### When to Use Super

| Scenario | Use Super? |
|----------|-----------|
| Initialize parent state | ✓ Yes - super.init() |
| Extend parent method | ✓ Yes - super.method() |
| Replace parent behavior completely | ✗ No - don't call super |
| Access overridden parent method | ✓ Yes - super.method() |
| Access current object's method | ✗ No - use this |

---

## Next Steps

- Learn about [Polymorphism](06_polymorphism.md) to see how super enables dynamic behavior
- Explore [Method Overriding](04_method_overriding.md) for more override patterns
- Study [Inheritance](03_inheritance.md) for the foundation of super
- Practice with [OOP Examples](../../examples/oop/) for real-world usage

---

**Happy Coding with KLang! 🚀**
