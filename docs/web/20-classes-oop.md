# Classes and Object-Oriented Programming

## Mastering OOP in KLang

Object-oriented programming (OOP) is a powerful paradigm that organizes code around objects and classes. KLang provides full support for OOP concepts including classes, inheritance, encapsulation, polymorphism, and abstraction. This guide covers everything you need to master OOP in KLang.

## Why Use Classes and OOP?

Classes and OOP help you:
- **Organize Complex Code**: Group related data and behavior together
- **Reuse Code**: Inherit from existing classes to avoid duplication
- **Model Real-World Entities**: Represent objects like users, products, or vehicles
- **Encapsulate Logic**: Hide implementation details and expose clean interfaces
- **Scale Applications**: Build large systems with maintainable, modular code
- **Improve Maintainability**: Changes to a class don't affect unrelated code

## Class Basics

### Simple Class Definition

Define a class with the `class` keyword:

```klang
# Basic class
class Person {
    fn constructor(name, age) {
        this.name = name
        this.age = age
    }
    
    fn greet() {
        println("Hello, I'm " + this.name)
    }
    
    fn describe() {
        return this.name + " is " + str(this.age) + " years old"
    }
}

# Create instances
let alice = Person("Alice", 25)
alice.greet()  # Hello, I'm Alice
println(alice.describe())  # Alice is 25 years old

let bob = Person("Bob", 30)
bob.greet()  # Hello, I'm Bob
```

### Class Properties

Classes can have properties that store data:

```klang
class Car {
    fn constructor(make, model, year) {
        this.make = make
        this.model = model
        this.year = year
        this.mileage = 0
    }
    
    fn drive(miles) {
        this.mileage = this.mileage + miles
        println("Drove " + str(miles) + " miles")
    }
    
    fn info() {
        return str(this.year) + " " + this.make + " " + this.model
    }
}

let myCar = Car("Toyota", "Camry", 2020)
println(myCar.info())  # 2020 Toyota Camry
myCar.drive(100)       # Drove 100 miles
myCar.drive(50)        # Drove 50 miles
println("Total mileage: " + str(myCar.mileage))  # Total mileage: 150
```

### Class Methods

Methods are functions that belong to a class:

```klang
class Calculator {
    fn constructor() {
        this.lastResult = 0
    }
    
    fn add(a, b) {
        this.lastResult = a + b
        return this.lastResult
    }
    
    fn subtract(a, b) {
        this.lastResult = a - b
        return this.lastResult
    }
    
    fn multiply(a, b) {
        this.lastResult = a * b
        return this.lastResult
    }
    
    fn divide(a, b) {
        if b == 0 {
            println("Error: Division by zero")
            return null
        }
        this.lastResult = a / b
        return this.lastResult
    }
    
    fn getLastResult() {
        return this.lastResult
    }
}

let calc = Calculator()
calc.add(10, 5)        # 15
calc.multiply(3, 4)    # 12
println(calc.getLastResult())  # 12
```

## Constructors

### Basic Constructors

Constructors initialize new instances:

```klang
class Book {
    fn constructor(title, author, pages) {
        this.title = title
        this.author = author
        this.pages = pages
        this.currentPage = 1
        println("Created book: " + title)
    }
    
    fn read(numPages) {
        this.currentPage = this.currentPage + numPages
        if this.currentPage > this.pages {
            this.currentPage = this.pages
        }
    }
    
    fn progress() {
        let percent = (this.currentPage / this.pages) * 100
        return str(percent) + "% complete"
    }
}

let book = Book("1984", "George Orwell", 328)
# Output: Created book: 1984
book.read(50)
println(book.progress())  # 15.54% complete
```

### Constructors with Default Values

```klang
class User {
    fn constructor(username, email, role) {
        this.username = username
        this.email = email
        this.role = role || "user"  # Default role
        this.active = true
        this.createdAt = Date.now()
    }
    
    fn deactivate() {
        this.active = false
        println(this.username + " has been deactivated")
    }
    
    fn isAdmin() {
        return this.role == "admin"
    }
}

let user1 = User("alice", "alice@example.com", "admin")
let user2 = User("bob", "bob@example.com", null)  # Uses default role

println(user1.isAdmin())  # true
println(user2.isAdmin())  # false
```

### Constructor Validation

```klang
class BankAccount {
    fn constructor(accountNumber, ownerName, initialBalance) {
        if initialBalance < 0 {
            throw "Initial balance cannot be negative"
        }
        
        this.accountNumber = accountNumber
        this.ownerName = ownerName
        this.balance = initialBalance
        this.transactions = []
    }
    
    fn deposit(amount) {
        if amount <= 0 {
            println("Deposit amount must be positive")
            return false
        }
        this.balance = this.balance + amount
        this.transactions.push({type: "deposit", amount: amount})
        return true
    }
    
    fn withdraw(amount) {
        if amount <= 0 {
            println("Withdrawal amount must be positive")
            return false
        }
        if amount > this.balance {
            println("Insufficient funds")
            return false
        }
        this.balance = this.balance - amount
        this.transactions.push({type: "withdrawal", amount: amount})
        return true
    }
    
    fn getBalance() {
        return this.balance
    }
}

let account = BankAccount("12345", "Alice", 1000)
account.deposit(500)    # Balance: 1500
account.withdraw(200)   # Balance: 1300
println("Balance: $" + str(account.getBalance()))
```

## Inheritance

### Basic Inheritance

Classes can inherit from other classes:

```klang
# Base class
class Animal {
    fn constructor(name, species) {
        this.name = name
        this.species = species
    }
    
    fn makeSound() {
        println(this.name + " makes a sound")
    }
    
    fn describe() {
        return this.name + " is a " + this.species
    }
}

# Derived class
class Dog extends Animal {
    fn constructor(name, breed) {
        super(name, "Dog")
        this.breed = breed
    }
    
    fn makeSound() {
        println(this.name + " barks: Woof!")
    }
    
    fn fetch() {
        println(this.name + " is fetching the ball")
    }
}

let genericAnimal = Animal("Generic", "Unknown")
genericAnimal.makeSound()  # Generic makes a sound

let dog = Dog("Buddy", "Golden Retriever")
dog.makeSound()   # Buddy barks: Woof!
dog.fetch()       # Buddy is fetching the ball
println(dog.describe())  # Buddy is a Dog
```

### Multi-Level Inheritance

```klang
# Level 1: Base class
class Vehicle {
    fn constructor(brand) {
        this.brand = brand
        this.speed = 0
    }
    
    fn accelerate(amount) {
        this.speed = this.speed + amount
        println(this.brand + " accelerating to " + str(this.speed) + " mph")
    }
    
    fn brake() {
        this.speed = 0
        println(this.brand + " stopped")
    }
}

# Level 2: Intermediate class
class MotorVehicle extends Vehicle {
    fn constructor(brand, fuelType) {
        super(brand)
        this.fuelType = fuelType
        this.fuelLevel = 100
    }
    
    fn refuel() {
        this.fuelLevel = 100
        println("Refueled " + this.brand)
    }
}

# Level 3: Most specific class
class ElectricCar extends MotorVehicle {
    fn constructor(brand, batteryCapacity) {
        super(brand, "Electric")
        this.batteryCapacity = batteryCapacity
    }
    
    fn charge() {
        this.fuelLevel = 100
        println("Charged " + this.brand + " to 100%")
    }
    
    fn autoPilot() {
        println(this.brand + " engaged autopilot")
    }
}

let tesla = ElectricCar("Tesla Model 3", 75)
tesla.accelerate(60)    # Tesla Model 3 accelerating to 60 mph
tesla.charge()          # Charged Tesla Model 3 to 100%
tesla.autoPilot()       # Tesla Model 3 engaged autopilot
```

### Overriding Methods

```klang
class Shape {
    fn constructor(name) {
        this.name = name
    }
    
    fn area() {
        return 0
    }
    
    fn describe() {
        println("This is a " + this.name)
    }
}

class Rectangle extends Shape {
    fn constructor(width, height) {
        super("Rectangle")
        this.width = width
        this.height = height
    }
    
    fn area() {
        return this.width * this.height
    }
    
    fn describe() {
        super.describe()  # Call parent method
        println("Width: " + str(this.width) + ", Height: " + str(this.height))
    }
}

class Circle extends Shape {
    fn constructor(radius) {
        super("Circle")
        this.radius = radius
    }
    
    fn area() {
        return 3.14159 * this.radius * this.radius
    }
    
    fn circumference() {
        return 2 * 3.14159 * this.radius
    }
}

let rect = Rectangle(10, 5)
println("Rectangle area: " + str(rect.area()))  # 50
rect.describe()

let circle = Circle(7)
println("Circle area: " + str(circle.area()))  # 153.938
println("Circle circumference: " + str(circle.circumference()))
```

## Polymorphism

### Method Polymorphism

Different classes can respond to the same method differently:

```klang
class Payment {
    fn constructor(amount) {
        this.amount = amount
    }
    
    fn process() {
        println("Processing payment of $" + str(this.amount))
    }
}

class CreditCardPayment extends Payment {
    fn constructor(amount, cardNumber) {
        super(amount)
        this.cardNumber = cardNumber
    }
    
    fn process() {
        println("Processing credit card payment of $" + str(this.amount))
        println("Card ending in " + this.cardNumber.slice(-4))
    }
}

class PayPalPayment extends Payment {
    fn constructor(amount, email) {
        super(amount)
        this.email = email
    }
    
    fn process() {
        println("Processing PayPal payment of $" + str(this.amount))
        println("PayPal account: " + this.email)
    }
}

class CryptoPayment extends Payment {
    fn constructor(amount, wallet) {
        super(amount)
        this.wallet = wallet
    }
    
    fn process() {
        println("Processing crypto payment of $" + str(this.amount))
        println("Wallet: " + this.wallet)
    }
}

# Polymorphic function
fn processPayments(payments) {
    for let payment in payments {
        payment.process()
        println("---")
    }
}

let payments = [
    CreditCardPayment(100, "1234567890123456"),
    PayPalPayment(50, "user@example.com"),
    CryptoPayment(200, "0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb")
]

processPayments(payments)
```

### Duck Typing

```klang
# Different classes with same interface
class Dog {
    fn constructor(name) {
        this.name = name
    }
    
    fn speak() {
        println(this.name + " says: Woof!")
    }
}

class Cat {
    fn constructor(name) {
        this.name = name
    }
    
    fn speak() {
        println(this.name + " says: Meow!")
    }
}

class Parrot {
    fn constructor(name) {
        this.name = name
    }
    
    fn speak() {
        println(this.name + " says: Hello!")
    }
}

# Function that works with any object that has a speak() method
fn makeAnimalSpeak(animal) {
    animal.speak()
}

let pets = [
    Dog("Buddy"),
    Cat("Whiskers"),
    Parrot("Polly")
]

for let pet in pets {
    makeAnimalSpeak(pet)
}
# Output:
# Buddy says: Woof!
# Whiskers says: Meow!
# Polly says: Hello!
```

## Access Modifiers

### Public and Private Members

```klang
class SecureAccount {
    fn constructor(username, password) {
        this.username = username          # Public
        this._password = password         # Private by convention
        this._loginAttempts = 0
    }
    
    # Public method
    fn login(password) {
        if password == this._password {
            println("Login successful!")
            this._loginAttempts = 0
            return true
        } else {
            this._loginAttempts = this._loginAttempts + 1
            println("Login failed. Attempt " + str(this._loginAttempts))
            return false
        }
    }
    
    # Public method
    fn changePassword(oldPassword, newPassword) {
        if oldPassword == this._password {
            if this._validatePassword(newPassword) {
                this._password = newPassword
                println("Password changed successfully")
                return true
            }
        }
        println("Password change failed")
        return false
    }
    
    # Private method (by convention)
    fn _validatePassword(password) {
        if password.length < 8 {
            println("Password must be at least 8 characters")
            return false
        }
        return true
    }
    
    # Getter
    fn getLoginAttempts() {
        return this._loginAttempts
    }
}

let account = SecureAccount("alice", "secret123")
account.login("wrong")        # Login failed
account.login("secret123")    # Login successful
account.changePassword("secret123", "newpass")  # Too short
account.changePassword("secret123", "newpass2024")  # Success
```

### Encapsulation with Getters and Setters

```klang
class Temperature {
    fn constructor(celsius) {
        this._celsius = celsius
    }
    
    # Getter
    fn getCelsius() {
        return this._celsius
    }
    
    # Setter with validation
    fn setCelsius(value) {
        if value < -273.15 {
            println("Temperature cannot be below absolute zero")
            return false
        }
        this._celsius = value
        return true
    }
    
    # Computed property
    fn getFahrenheit() {
        return (this._celsius * 9/5) + 32
    }
    
    fn setFahrenheit(value) {
        this._celsius = (value - 32) * 5/9
    }
    
    fn getKelvin() {
        return this._celsius + 273.15
    }
}

let temp = Temperature(25)
println("Celsius: " + str(temp.getCelsius()))       # 25
println("Fahrenheit: " + str(temp.getFahrenheit())) # 77
println("Kelvin: " + str(temp.getKelvin()))         # 298.15

temp.setFahrenheit(68)
println("New Celsius: " + str(temp.getCelsius()))   # 20
```

## Abstract Classes

### Abstract Base Class Pattern

```klang
class Database {
    fn constructor(connectionString) {
        this.connectionString = connectionString
        this.connected = false
    }
    
    fn connect() {
        throw "connect() must be implemented by subclass"
    }
    
    fn disconnect() {
        throw "disconnect() must be implemented by subclass"
    }
    
    fn query(sql) {
        throw "query() must be implemented by subclass"
    }
}

class MySQLDatabase extends Database {
    fn constructor(connectionString) {
        super(connectionString)
    }
    
    fn connect() {
        println("Connecting to MySQL: " + this.connectionString)
        this.connected = true
    }
    
    fn disconnect() {
        println("Disconnecting from MySQL")
        this.connected = false
    }
    
    fn query(sql) {
        if !this.connected {
            println("Not connected to database")
            return null
        }
        println("Executing MySQL query: " + sql)
        return []  # Return results
    }
}

class PostgreSQLDatabase extends Database {
    fn constructor(connectionString) {
        super(connectionString)
    }
    
    fn connect() {
        println("Connecting to PostgreSQL: " + this.connectionString)
        this.connected = true
    }
    
    fn disconnect() {
        println("Disconnecting from PostgreSQL")
        this.connected = false
    }
    
    fn query(sql) {
        if !this.connected {
            println("Not connected to database")
            return null
        }
        println("Executing PostgreSQL query: " + sql)
        return []  # Return results
    }
}

let db = MySQLDatabase("localhost:3306/mydb")
db.connect()
db.query("SELECT * FROM users")
db.disconnect()
```

## Real-World Use Cases

### E-commerce System

```klang
class Product {
    fn constructor(id, name, price, stock) {
        this.id = id
        this.name = name
        this.price = price
        this.stock = stock
    }
    
    fn isAvailable() {
        return this.stock > 0
    }
    
    fn reduceStock(quantity) {
        if quantity > this.stock {
            return false
        }
        this.stock = this.stock - quantity
        return true
    }
}

class ShoppingCart {
    fn constructor() {
        this.items = []
    }
    
    fn addItem(product, quantity) {
        if !product.isAvailable() {
            println(product.name + " is out of stock")
            return false
        }
        
        this.items.push({
            product: product,
            quantity: quantity
        })
        println("Added " + str(quantity) + "x " + product.name)
        return true
    }
    
    fn removeItem(productId) {
        let newItems = []
        for let item in this.items {
            if item.product.id != productId {
                newItems.push(item)
            }
        }
        this.items = newItems
    }
    
    fn getTotal() {
        let total = 0
        for let item in this.items {
            total = total + (item.product.price * item.quantity)
        }
        return total
    }
    
    fn checkout() {
        for let item in this.items {
            if !item.product.reduceStock(item.quantity) {
                println("Checkout failed: insufficient stock")
                return false
            }
        }
        
        let total = this.getTotal()
        println("Checkout successful! Total: $" + str(total))
        this.items = []
        return true
    }
}

let laptop = Product(1, "Laptop", 999.99, 5)
let mouse = Product(2, "Mouse", 29.99, 10)

let cart = ShoppingCart()
cart.addItem(laptop, 1)
cart.addItem(mouse, 2)
println("Cart total: $" + str(cart.getTotal()))
cart.checkout()
```

### Game Character System

```klang
class Character {
    fn constructor(name, health, attackPower) {
        this.name = name
        this.health = health
        this.maxHealth = health
        this.attackPower = attackPower
        this.level = 1
        this.experience = 0
    }
    
    fn attack(target) {
        println(this.name + " attacks " + target.name)
        target.takeDamage(this.attackPower)
    }
    
    fn takeDamage(damage) {
        this.health = this.health - damage
        if this.health < 0 {
            this.health = 0
        }
        println(this.name + " takes " + str(damage) + " damage (HP: " + str(this.health) + ")")
        
        if this.health == 0 {
            this.die()
        }
    }
    
    fn heal(amount) {
        this.health = this.health + amount
        if this.health > this.maxHealth {
            this.health = this.maxHealth
        }
        println(this.name + " healed for " + str(amount) + " (HP: " + str(this.health) + ")")
    }
    
    fn die() {
        println(this.name + " has been defeated!")
    }
    
    fn gainExperience(xp) {
        this.experience = this.experience + xp
        if this.experience >= this.level * 100 {
            this.levelUp()
        }
    }
    
    fn levelUp() {
        this.level = this.level + 1
        this.maxHealth = this.maxHealth + 20
        this.health = this.maxHealth
        this.attackPower = this.attackPower + 5
        println(this.name + " leveled up to level " + str(this.level) + "!")
    }
}

class Warrior extends Character {
    fn constructor(name) {
        super(name, 150, 25)
        this.armor = 10
    }
    
    fn takeDamage(damage) {
        let reducedDamage = damage - this.armor
        if reducedDamage < 0 {
            reducedDamage = 0
        }
        super.takeDamage(reducedDamage)
    }
    
    fn shieldBash(target) {
        println(this.name + " shield bashes " + target.name)
        target.takeDamage(this.attackPower * 1.5)
    }
}

class Mage extends Character {
    fn constructor(name) {
        super(name, 80, 15)
        this.mana = 100
    }
    
    fn castFireball(target) {
        if this.mana >= 20 {
            this.mana = this.mana - 20
            println(this.name + " casts Fireball!")
            target.takeDamage(this.attackPower * 3)
        } else {
            println("Not enough mana!")
        }
    }
    
    fn meditate() {
        this.mana = 100
        println(this.name + " meditates and restores mana")
    }
}

let warrior = Warrior("Conan")
let mage = Mage("Gandalf")

warrior.attack(mage)
mage.castFireball(warrior)
warrior.shieldBash(mage)
```

### Task Management System

```klang
class Task {
    fn constructor(title, description, priority) {
        this.title = title
        this.description = description
        this.priority = priority || "medium"
        this.completed = false
        this.createdAt = Date.now()
        this.completedAt = null
    }
    
    fn complete() {
        this.completed = true
        this.completedAt = Date.now()
        println("Task completed: " + this.title)
    }
    
    fn info() {
        let status = this.completed ? "✓ Completed" : "○ Pending"
        println(status + " [" + this.priority + "] " + this.title)
    }
}

class Project {
    fn constructor(name, deadline) {
        this.name = name
        this.deadline = deadline
        this.tasks = []
    }
    
    fn addTask(task) {
        this.tasks.push(task)
        println("Added task to " + this.name + ": " + task.title)
    }
    
    fn removeTask(taskTitle) {
        let newTasks = []
        for let task in this.tasks {
            if task.title != taskTitle {
                newTasks.push(task)
            }
        }
        this.tasks = newTasks
    }
    
    fn completeTask(taskTitle) {
        for let task in this.tasks {
            if task.title == taskTitle {
                task.complete()
                return true
            }
        }
        return false
    }
    
    fn getProgress() {
        if this.tasks.length == 0 {
            return 0
        }
        
        let completed = 0
        for let task in this.tasks {
            if task.completed {
                completed = completed + 1
            }
        }
        
        return (completed / this.tasks.length) * 100
    }
    
    fn showStatus() {
        println("Project: " + this.name)
        println("Progress: " + str(this.getProgress()) + "%")
        println("Tasks:")
        for let task in this.tasks {
            task.info()
        }
    }
}

let project = Project("Website Redesign", "2024-12-31")
project.addTask(Task("Design mockups", "Create initial designs", "high"))
project.addTask(Task("Implement frontend", "Build React components", "high"))
project.addTask(Task("Write tests", "Unit and integration tests", "medium"))
project.addTask(Task("Deploy", "Deploy to production", "high"))

project.completeTask("Design mockups")
project.completeTask("Implement frontend")
project.showStatus()
```

## Common Patterns

### Singleton Pattern

```klang
class Singleton {
    fn constructor() {
        if Singleton._instance {
            return Singleton._instance
        }
        
        this.data = []
        Singleton._instance = this
    }
    
    fn addData(item) {
        this.data.push(item)
    }
    
    fn getData() {
        return this.data
    }
}

let instance1 = Singleton()
let instance2 = Singleton()

instance1.addData("Hello")
instance2.addData("World")

println(instance1.getData())  # ["Hello", "World"]
println(instance2.getData())  # ["Hello", "World"]
println(instance1 == instance2)  # true (same instance)
```

### Factory Pattern

```klang
class Vehicle {
    fn constructor(type) {
        this.type = type
    }
    
    fn start() {
        println(this.type + " started")
    }
}

class Car extends Vehicle {
    fn constructor() {
        super("Car")
    }
}

class Truck extends Vehicle {
    fn constructor() {
        super("Truck")
    }
}

class Motorcycle extends Vehicle {
    fn constructor() {
        super("Motorcycle")
    }
}

class VehicleFactory {
    fn createVehicle(type) {
        if type == "car" {
            return Car()
        } else if type == "truck" {
            return Truck()
        } else if type == "motorcycle" {
            return Motorcycle()
        }
        return null
    }
}

let factory = VehicleFactory()
let car = factory.createVehicle("car")
let truck = factory.createVehicle("truck")

car.start()    # Car started
truck.start()  # Truck started
```

### Observer Pattern

```klang
class Observable {
    fn constructor() {
        this.observers = []
    }
    
    fn subscribe(observer) {
        this.observers.push(observer)
    }
    
    fn unsubscribe(observer) {
        let newObservers = []
        for let obs in this.observers {
            if obs != observer {
                newObservers.push(obs)
            }
        }
        this.observers = newObservers
    }
    
    fn notify(data) {
        for let observer in this.observers {
            observer.update(data)
        }
    }
}

class NewsAgency extends Observable {
    fn publishNews(news) {
        println("Publishing: " + news)
        this.notify(news)
    }
}

class NewsSubscriber {
    fn constructor(name) {
        this.name = name
    }
    
    fn update(news) {
        println(this.name + " received news: " + news)
    }
}

let agency = NewsAgency()
let subscriber1 = NewsSubscriber("Alice")
let subscriber2 = NewsSubscriber("Bob")

agency.subscribe(subscriber1)
agency.subscribe(subscriber2)

agency.publishNews("Breaking: New KLang version released!")
# Output:
# Publishing: Breaking: New KLang version released!
# Alice received news: Breaking: New KLang version released!
# Bob received news: Breaking: New KLang version released!
```

## Best Practices

### 1. Use Clear, Descriptive Class Names

```klang
# Good
class UserAccountManager {
    # ...
}

class EmailNotificationService {
    # ...
}

# Avoid
class UAM {
    # ...
}

class Thing {
    # ...
}
```

### 2. Keep Classes Focused (Single Responsibility)

```klang
# Good: Each class has one responsibility
class User {
    fn constructor(name, email) {
        this.name = name
        this.email = email
    }
}

class UserValidator {
    fn validateEmail(email) {
        return email.contains("@")
    }
}

class UserRepository {
    fn save(user) {
        # Save to database
    }
}

# Avoid: Class doing too much
class User {
    fn constructor(name, email) {
        this.name = name
        this.email = email
    }
    
    fn validateEmail() {
        # Validation logic
    }
    
    fn saveToDatabase() {
        # Database logic
    }
    
    fn sendEmail() {
        # Email logic
    }
}
```

### 3. Use Inheritance Wisely

```klang
# Good: Clear "is-a" relationship
class Animal {
    # ...
}

class Dog extends Animal {
    # A Dog IS-A Animal
}

# Avoid: Forced inheritance
class Utility {
    # ...
}

class MathHelper extends Utility {
    # MathHelper is NOT-A Utility, use composition instead
}
```

### 4. Favor Composition Over Inheritance

```klang
# Good: Composition
class Engine {
    fn start() {
        println("Engine started")
    }
}

class Car {
    fn constructor() {
        this.engine = Engine()
    }
    
    fn start() {
        this.engine.start()
        println("Car is ready")
    }
}

# Instead of excessive inheritance chains
```

### 5. Initialize All Properties in Constructor

```klang
# Good
class Account {
    fn constructor(id) {
        this.id = id
        this.balance = 0
        this.transactions = []
        this.active = true
    }
}

# Avoid: Uninitialized properties
class Account {
    fn constructor(id) {
        this.id = id
        # Other properties not initialized
    }
}
```

## Common Mistakes to Avoid

### 1. Forgetting to Call super()

```klang
# Wrong
class Dog extends Animal {
    fn constructor(name) {
        this.name = name  # Parent constructor not called
    }
}

# Correct
class Dog extends Animal {
    fn constructor(name) {
        super(name, "Dog")
        this.breed = "Unknown"
    }
}
```

### 2. Modifying Inherited Methods Without Understanding

```klang
# Problematic
class BankAccount {
    fn withdraw(amount) {
        this.balance = this.balance - amount
        return true
    }
}

class SavingsAccount extends BankAccount {
    fn withdraw(amount) {
        # Forgot to check balance, breaking parent's contract
        this.balance = this.balance - amount
        return true
    }
}

# Better
class SavingsAccount extends BankAccount {
    fn withdraw(amount) {
        if this.balance < amount {
            return false
        }
        return super.withdraw(amount)
    }
}
```

### 3. Creating Too Deep Inheritance Hierarchies

```klang
# Avoid
class A {
}
class B extends A {
}
class C extends B {
}
class D extends C {
}
class E extends D {
}
# Too many levels!

# Better: Keep it shallow (2-3 levels max)
class Vehicle {
}
class Car extends Vehicle {
}
class ElectricCar extends Car {
}
# That's enough
```

## Next Steps

Now that you understand classes and OOP in KLang, explore these related topics:

- **[Arrow Functions](21-arrow-functions.md)** - Learn concise function syntax
- **[Closures](22-closures.md)** - Master scope and closure patterns
- **[Async/Await](23-async-await.md)** - Handle asynchronous operations
- **[Design Patterns](30-design-patterns.md)** - Learn common software patterns
- **[Error Handling](16-error-handling.md)** - Proper exception handling

Ready to write cleaner, more modular code? Practice building class hierarchies and applying OOP principles to your projects!
