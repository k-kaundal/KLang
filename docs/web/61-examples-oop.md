# OOP Examples

## Introduction

This page demonstrates Object-Oriented Programming in KLang through practical examples. You'll learn about classes, inheritance, encapsulation, and polymorphism.

## Basic Classes

### Example 1: Simple Person Class

```klang
class Person {
    constructor(name, age) {
        this.name = name
        this.age = age
    }
    
    greet() {
        println("Hello, I'm " + this.name)
    }
    
    haveBirthday() {
        this.age = this.age + 1
        println(this.name + " is now " + str(this.age))
    }
}

let person = Person("Alice", 30)
person.greet()           # "Hello, I'm Alice"
person.haveBirthday()    # "Alice is now 31"
println(person.age)      # 31
```

### Example 2: Rectangle Class

```klang
class Rectangle {
    constructor(width, height) {
        this.width = width
        this.height = height
    }
    
    area() {
        return this.width * this.height
    }
    
    perimeter() {
        return 2 * (this.width + this.height)
    }
    
    scale(factor) {
        this.width = this.width * factor
        this.height = this.height * factor
    }
    
    describe() {
        println("Rectangle: " + str(this.width) + "x" + str(this.height))
        println("Area: " + str(this.area()))
        println("Perimeter: " + str(this.perimeter()))
    }
}

let rect = Rectangle(10, 5)
rect.describe()
rect.scale(2)
rect.describe()
```

## Inheritance

### Example 3: Animal Hierarchy

```klang
class Animal {
    constructor(name, species) {
        this.name = name
        this.species = species
    }
    
    speak() {
        println(this.name + " makes a sound")
    }
    
    info() {
        println(this.name + " is a " + this.species)
    }
}

class Dog extends Animal {
    constructor(name, breed) {
        super(name, "Dog")
        this.breed = breed
    }
    
    speak() {
        println(this.name + " barks: Woof! Woof!")
    }
    
    fetch() {
        println(this.name + " is fetching the ball!")
    }
}

class Cat extends Animal {
    constructor(name) {
        super(name, "Cat")
    }
    
    speak() {
        println(this.name + " meows: Meow!")
    }
    
    scratch() {
        println(this.name + " is scratching the furniture!")
    }
}

let dog = Dog("Buddy", "Golden Retriever")
dog.info()      # "Buddy is a Dog"
dog.speak()     # "Buddy barks: Woof! Woof!"
dog.fetch()     # "Buddy is fetching the ball!"

let cat = Cat("Whiskers")
cat.info()      # "Whiskers is a Cat"
cat.speak()     # "Whiskers meows: Meow!"
cat.scratch()   # "Whiskers is scratching the furniture!"
```

### Example 4: Vehicle System

```klang
class Vehicle {
    constructor(brand, model, year) {
        this.brand = brand
        this.model = model
        this.year = year
        this.speed = 0
    }
    
    accelerate(amount) {
        this.speed = this.speed + amount
        println(this.brand + " " + this.model + " accelerating to " + str(this.speed) + " mph")
    }
    
    brake(amount) {
        this.speed = max(0, this.speed - amount)
        println("Braking to " + str(this.speed) + " mph")
    }
    
    info() {
        return this.year + " " + this.brand + " " + this.model
    }
}

class Car extends Vehicle {
    constructor(brand, model, year, doors) {
        super(brand, model, year)
        this.doors = doors
        this.passengers = 0
    }
    
    addPassenger() {
        this.passengers = this.passengers + 1
        println("Passenger added. Total: " + str(this.passengers))
    }
}

class Truck extends Vehicle {
    constructor(brand, model, year, cargo_capacity) {
        super(brand, model, year)
        this.cargo_capacity = cargo_capacity
        this.cargo = 0
    }
    
    loadCargo(weight) {
        if this.cargo + weight <= this.cargo_capacity {
            this.cargo = this.cargo + weight
            println("Loaded " + str(weight) + " tons")
        } else {
            println("Cannot load: exceeds capacity!")
        }
    }
}

let car = Car("Toyota", "Camry", 2024, 4)
println(car.info())
car.accelerate(30)
car.addPassenger()

let truck = Truck("Ford", "F-150", 2024, 5)
println(truck.info())
truck.loadCargo(3)
truck.accelerate(20)
```

## Encapsulation

### Example 5: Bank Account

```klang
class BankAccount {
    constructor(owner, initial_balance) {
        this.owner = owner
        this.balance = initial_balance
        this.transactions = []
    }
    
    deposit(amount) {
        if amount <= 0 {
            println("Error: Amount must be positive")
            return false
        }
        
        this.balance = this.balance + amount
        this.transactions.push({
            type: "deposit",
            amount: amount,
            balance: this.balance
        })
        println("Deposited $" + str(amount))
        return true
    }
    
    withdraw(amount) {
        if amount <= 0 {
            println("Error: Amount must be positive")
            return false
        }
        
        if amount > this.balance {
            println("Error: Insufficient funds")
            return false
        }
        
        this.balance = this.balance - amount
        this.transactions.push({
            type: "withdrawal",
            amount: amount,
            balance: this.balance
        })
        println("Withdrew $" + str(amount))
        return true
    }
    
    getBalance() {
        return this.balance
    }
    
    statement() {
        println("\n=== Account Statement ===")
        println("Owner: " + this.owner)
        println("Balance: $" + str(this.balance))
        println("\nRecent Transactions:")
        this.transactions.forEach(t => {
            println("- " + t.type + ": $" + str(t.amount) + 
                    " (Balance: $" + str(t.balance) + ")")
        })
    }
}

let account = BankAccount("Alice", 1000)
account.deposit(500)
account.withdraw(200)
account.withdraw(2000)  # Error
account.statement()
```

### Example 6: Library System

```klang
class Book {
    constructor(title, author, isbn) {
        this.title = title
        this.author = author
        this.isbn = isbn
        this.available = true
    }
    
    info() {
        return this.title + " by " + this.author
    }
}

class Library {
    constructor(name) {
        this.name = name
        this.books = []
        this.borrowed = []
    }
    
    addBook(book) {
        this.books.push(book)
        println("Added: " + book.info())
    }
    
    findBook(title) {
        return this.books.find(b => 
            lowercase(b.title) == lowercase(title)
        )
    }
    
    borrowBook(title) {
        let book = this.findBook(title)
        
        if book == null {
            println("Book not found")
            return false
        }
        
        if !book.available {
            println("Book already borrowed")
            return false
        }
        
        book.available = false
        this.borrowed.push(book)
        println("Borrowed: " + book.info())
        return true
    }
    
    returnBook(title) {
        let book = this.findBook(title)
        
        if book == null {
            println("Book not found")
            return false
        }
        
        book.available = true
        this.borrowed = this.borrowed.filter(b => b.isbn != book.isbn)
        println("Returned: " + book.info())
        return true
    }
    
    listAvailable() {
        println("\nAvailable Books:")
        let available = this.books.filter(b => b.available)
        available.forEach(b => {
            println("- " + b.info())
        })
    }
}

let library = Library("City Library")

library.addBook(Book("1984", "George Orwell", "123"))
library.addBook(Book("To Kill a Mockingbird", "Harper Lee", "456"))
library.addBook(Book("Pride and Prejudice", "Jane Austen", "789"))

library.listAvailable()
library.borrowBook("1984")
library.listAvailable()
library.returnBook("1984")
library.listAvailable()
```

## Polymorphism

### Example 7: Shape System

```klang
class Shape {
    constructor(name) {
        this.name = name
    }
    
    area() {
        return 0  # Override in subclasses
    }
    
    describe() {
        println(this.name + " with area: " + str(this.area()))
    }
}

class Circle extends Shape {
    constructor(radius) {
        super("Circle")
        this.radius = radius
    }
    
    area() {
        return PI * pow(this.radius, 2)
    }
}

class Square extends Shape {
    constructor(side) {
        super("Square")
        this.side = side
    }
    
    area() {
        return pow(this.side, 2)
    }
}

class Triangle extends Shape {
    constructor(base, height) {
        super("Triangle")
        this.base = base
        this.height = height
    }
    
    area() {
        return 0.5 * this.base * this.height
    }
}

let shapes = [
    Circle(5),
    Square(10),
    Triangle(8, 6)
]

println("Shape Areas:")
shapes.forEach(shape => {
    shape.describe()
})
```

### Example 8: Payment System

```klang
class Payment {
    constructor(amount) {
        this.amount = amount
        this.processed = false
    }
    
    process() {
        println("Processing payment of $" + str(this.amount))
        this.processed = true
    }
    
    receipt() {
        return "Payment: $" + str(this.amount)
    }
}

class CreditCardPayment extends Payment {
    constructor(amount, card_number) {
        super(amount)
        this.card_number = card_number
    }
    
    process() {
        println("Processing credit card: ****" + substr(this.card_number, -4))
        super.process()
    }
    
    receipt() {
        return super.receipt() + " (Credit Card)"
    }
}

class PayPalPayment extends Payment {
    constructor(amount, email) {
        super(amount)
        this.email = email
    }
    
    process() {
        println("Processing PayPal: " + this.email)
        super.process()
    }
    
    receipt() {
        return super.receipt() + " (PayPal)"
    }
}

let payments = [
    CreditCardPayment(99.99, "1234567890123456"),
    PayPalPayment(49.99, "user@example.com")
]

payments.forEach(payment => {
    payment.process()
    println(payment.receipt())
    println("---")
})
```

## Static Members

### Example 9: Counter Class with Static

```klang
class Counter {
    static total_count = 0
    
    constructor(name) {
        this.name = name
        this.count = 0
        Counter.total_count = Counter.total_count + 1
    }
    
    increment() {
        this.count = this.count + 1
    }
    
    static getTotalCounters() {
        return Counter.total_count
    }
}

let counter1 = Counter("First")
let counter2 = Counter("Second")

counter1.increment()
counter1.increment()
counter2.increment()

println("Counter1: " + str(counter1.count))  # 2
println("Counter2: " + str(counter2.count))  # 1
println("Total counters created: " + str(Counter.getTotalCounters()))  # 2
```

### Example 10: ID Generator

```klang
class User {
    static next_id = 1
    
    constructor(username, email) {
        this.id = User.next_id
        User.next_id = User.next_id + 1
        this.username = username
        this.email = email
    }
    
    info() {
        println("User #" + str(this.id) + ": " + this.username)
    }
}

let users = [
    User("alice", "alice@example.com"),
    User("bob", "bob@example.com"),
    User("charlie", "charlie@example.com")
]

users.forEach(user => user.info())
# User #1: alice
# User #2: bob
# User #3: charlie
```

## Design Patterns

### Example 11: Singleton Pattern

```klang
class Database {
    static instance = null
    
    constructor() {
        if Database.instance != null {
            println("Warning: Database instance already exists")
            return Database.instance
        }
        
        this.connected = false
        this.data = []
        Database.instance = this
    }
    
    static getInstance() {
        if Database.instance == null {
            Database.instance = Database()
        }
        return Database.instance
    }
    
    connect() {
        this.connected = true
        println("Database connected")
    }
    
    insert(record) {
        if this.connected {
            this.data.push(record)
            println("Record inserted")
        } else {
            println("Error: Not connected")
        }
    }
}

let db1 = Database.getInstance()
let db2 = Database.getInstance()

db1.connect()
db1.insert({id: 1, name: "Alice"})

# db2 is the same instance
db2.insert({id: 2, name: "Bob"})
println("Total records: " + str(len(db1.data)))  # 2
```

### Example 12: Factory Pattern

```klang
class Employee {
    constructor(name, position) {
        this.name = name
        this.position = position
    }
    
    work() {
        println(this.name + " is working as " + this.position)
    }
}

class Manager extends Employee {
    constructor(name) {
        super(name, "Manager")
        this.team = []
    }
    
    addTeamMember(employee) {
        this.team.push(employee)
        println("Added " + employee.name + " to team")
    }
    
    work() {
        println(this.name + " is managing " + str(len(this.team)) + " team members")
    }
}

class Developer extends Employee {
    constructor(name, language) {
        super(name, "Developer")
        this.language = language
    }
    
    work() {
        println(this.name + " is coding in " + this.language)
    }
}

class EmployeeFactory {
    static create(type, name, extra) {
        if type == "manager" {
            return Manager(name)
        } else if type == "developer" {
            return Developer(name, extra)
        } else {
            return Employee(name, type)
        }
    }
}

let employees = [
    EmployeeFactory.create("manager", "Alice", null),
    EmployeeFactory.create("developer", "Bob", "KLang"),
    EmployeeFactory.create("developer", "Charlie", "Python")
]

employees.forEach(emp => emp.work())
```

## Composition

### Example 13: Game Character with Components

```klang
class Position {
    constructor(x, y) {
        this.x = x
        this.y = y
    }
    
    move(dx, dy) {
        this.x = this.x + dx
        this.y = this.y + dy
    }
    
    distanceTo(other) {
        let dx = this.x - other.x
        let dy = this.y - other.y
        return sqrt(pow(dx, 2) + pow(dy, 2))
    }
}

class Health {
    constructor(max_health) {
        this.max = max_health
        this.current = max_health
    }
    
    damage(amount) {
        this.current = max(0, this.current - amount)
        println("Took " + str(amount) + " damage. HP: " + str(this.current))
    }
    
    heal(amount) {
        this.current = min(this.max, this.current + amount)
        println("Healed " + str(amount) + ". HP: " + str(this.current))
    }
    
    isAlive() {
        return this.current > 0
    }
}

class Character {
    constructor(name, x, y) {
        this.name = name
        this.position = Position(x, y)
        this.health = Health(100)
    }
    
    moveTo(x, y) {
        this.position.move(x - this.position.x, y - this.position.y)
        println(this.name + " moved to (" + str(x) + ", " + str(y) + ")")
    }
    
    attack(target) {
        let distance = this.position.distanceTo(target.position)
        if distance > 10 {
            println("Target too far!")
            return
        }
        
        println(this.name + " attacks " + target.name)
        target.health.damage(20)
    }
    
    status() {
        println(this.name + " at (" + str(this.position.x) + ", " + 
                str(this.position.y) + ") HP: " + str(this.health.current))
    }
}

let player = Character("Hero", 0, 0)
let enemy = Character("Goblin", 5, 5)

player.status()
enemy.status()

player.attack(enemy)
enemy.attack(player)

player.status()
enemy.status()
```

## Advanced Patterns

### Example 14: Shopping Cart with Products

```klang
class Product {
    constructor(id, name, price, quantity) {
        this.id = id
        this.name = name
        this.price = price
        this.quantity = quantity
    }
    
    totalPrice() {
        return this.price * this.quantity
    }
    
    info() {
        return this.name + " x" + str(this.quantity) + " = $" + str(this.totalPrice())
    }
}

class ShoppingCart {
    constructor() {
        this.items = []
        this.next_id = 1
    }
    
    addItem(name, price, quantity) {
        let product = Product(this.next_id, name, price, quantity)
        this.items.push(product)
        this.next_id = this.next_id + 1
        println("Added: " + product.info())
    }
    
    removeItem(id) {
        let original_length = len(this.items)
        this.items = this.items.filter(item => item.id != id)
        
        if len(this.items) < original_length {
            println("Item removed")
        } else {
            println("Item not found")
        }
    }
    
    updateQuantity(id, quantity) {
        let item = this.items.find(i => i.id == id)
        if item != null {
            item.quantity = quantity
            println("Updated quantity for " + item.name)
        }
    }
    
    getTotal() {
        return this.items.reduce((sum, item) => sum + item.totalPrice(), 0)
    }
    
    summary() {
        println("\n=== Cart Summary ===")
        if len(this.items) == 0 {
            println("Cart is empty")
            return
        }
        
        this.items.forEach(item => {
            println(item.info())
        })
        
        println("---")
        println("Total: $" + str(this.getTotal()))
    }
}

let cart = ShoppingCart()
cart.addItem("Laptop", 999, 1)
cart.addItem("Mouse", 25, 2)
cart.addItem("Keyboard", 75, 1)
cart.summary()

cart.updateQuantity(2, 3)
cart.summary()
```

### Example 15: Task Manager

```klang
class Task {
    constructor(id, title, priority) {
        this.id = id
        this.title = title
        this.priority = priority  # 1=high, 2=medium, 3=low
        this.completed = false
        this.created_at = "2024-01-15"  # Simplified
    }
    
    complete() {
        this.completed = true
        println("✓ Completed: " + this.title)
    }
    
    priorityLabel() {
        if this.priority == 1 {
            return "HIGH"
        } else if this.priority == 2 {
            return "MEDIUM"
        } else {
            return "LOW"
        }
    }
}

class TaskManager {
    constructor() {
        this.tasks = []
        this.next_id = 1
    }
    
    addTask(title, priority) {
        let task = Task(this.next_id, title, priority)
        this.tasks.push(task)
        this.next_id = this.next_id + 1
        println("Added task #" + str(task.id) + ": " + title)
    }
    
    completeTask(id) {
        let task = this.tasks.find(t => t.id == id)
        if task != null {
            task.complete()
        } else {
            println("Task not found")
        }
    }
    
    listTasks(filter) {
        let filtered = this.tasks
        
        if filter == "pending" {
            filtered = filtered.filter(t => !t.completed)
        } else if filter == "completed" {
            filtered = filtered.filter(t => t.completed)
        } else if filter == "high" {
            filtered = filtered.filter(t => t.priority == 1)
        }
        
        println("\nTasks (" + filter + "):")
        filtered.forEach(task => {
            let status = task.completed ? "✓" : " "
            println("[" + status + "] " + str(task.id) + ". " + 
                    task.title + " [" + task.priorityLabel() + "]")
        })
    }
    
    statistics() {
        let total = len(this.tasks)
        let completed = this.tasks.filter(t => t.completed)
        let pending = total - len(completed)
        
        println("\n=== Statistics ===")
        println("Total tasks: " + str(total))
        println("Completed: " + str(len(completed)))
        println("Pending: " + str(pending))
    }
}

let manager = TaskManager()

manager.addTask("Learn KLang", 1)
manager.addTask("Build project", 1)
manager.addTask("Write docs", 2)
manager.addTask("Deploy app", 2)

manager.listTasks("all")
manager.completeTask(1)
manager.listTasks("pending")
manager.statistics()
```

## Method Chaining

### Example 16: Query Builder

```klang
class QueryBuilder {
    constructor() {
        this.data = []
        this.filters = []
        this.sort_field = null
        this.limit_value = null
    }
    
    from(data) {
        this.data = data
        return this  # Return this for chaining
    }
    
    where(predicate) {
        this.filters.push(predicate)
        return this
    }
    
    sortBy(field) {
        this.sort_field = field
        return this
    }
    
    limit(n) {
        this.limit_value = n
        return this
    }
    
    execute() {
        let result = this.data
        
        # Apply filters
        this.filters.forEach(filter => {
            result = result.filter(filter)
        })
        
        # Apply sort
        if this.sort_field != null {
            result = result.sort((a, b) => {
                if a[this.sort_field] < b[this.sort_field] {
                    return -1
                } else if a[this.sort_field] > b[this.sort_field] {
                    return 1
                }
                return 0
            })
        }
        
        # Apply limit
        if this.limit_value != null {
            result = result.slice(0, this.limit_value)
        }
        
        return result
    }
}

let users = [
    {name: "Alice", age: 30, city: "NYC"},
    {name: "Bob", age: 25, city: "LA"},
    {name: "Charlie", age: 35, city: "NYC"},
    {name: "Diana", age: 28, city: "Chicago"}
]

let query = QueryBuilder()
let result = query
    .from(users)
    .where(u => u.age >= 28)
    .where(u => u.city == "NYC")
    .sortBy("age")
    .limit(5)
    .execute()

println("Query results:")
result.forEach(user => {
    println(user.name + ", " + str(user.age) + ", " + user.city)
})
```

## Next Steps

Explore more advanced topics:

- [AI Examples](62-examples-ai.md) - AI-powered applications
- [Project Examples](63-examples-projects.md) - Complete projects
- [Classes & OOP](20-classes-oop.md) - Detailed OOP guide
- [Design Patterns](docs/patterns.md) - Common patterns

---

**Master OOP for building scalable applications!** 🏗️
