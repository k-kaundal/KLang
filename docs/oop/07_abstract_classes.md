# Abstract Classes in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What are Abstract Classes?](#what-are-abstract-classes)
3. [Abstract Methods](#abstract-methods)
4. [Concrete Methods](#concrete-methods)
5. [Cannot Instantiate](#cannot-instantiate)
6. [Child Implementation Requirements](#child-implementation-requirements)
7. [Practical Examples](#practical-examples)
8. [Abstract vs Interface](#abstract-vs-interface)
9. [Abstract vs Concrete Classes](#abstract-vs-concrete-classes)
10. [Template Method Pattern](#template-method-pattern)
11. [Best Practices](#best-practices)
12. [Interview Problems](#interview-problems)

---

## Introduction

**Abstract classes** are incomplete classes that serve as blueprints for other classes. They define a contract that child classes must follow while providing common functionality that can be shared.

### Why Use Abstract Classes?

- **Define Contracts**: Enforce that child classes implement specific methods
- **Share Common Code**: Provide default implementations for shared behavior
- **Prevent Instantiation**: Ensure only concrete implementations are created
- **Template Pattern**: Define algorithms with customizable steps
- **Type Hierarchy**: Create meaningful class hierarchies with partial implementations

### Key Characteristics

```
Abstract Class
  ├── Cannot be instantiated directly
  ├── Can have abstract methods (no implementation)
  ├── Can have concrete methods (with implementation)
  ├── Can have properties and fields
  └── Child classes must implement all abstract methods
```

---

## What are Abstract Classes?

An abstract class is a class that cannot be instantiated and may contain abstract methods that must be implemented by child classes.

### Declaration Syntax

```klang
abstract class ClassName {
    # Properties (can be abstract or concrete)
    let property: type = value
    
    # Abstract methods (no implementation)
    abstract fn methodName(params) -> returnType
    
    # Concrete methods (with implementation)
    fn concreteMethod(params) -> returnType {
        # Implementation
    }
}
```

### Simple Example

```klang
abstract class Animal {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
    }
    
    # Abstract method - child must implement
    abstract fn makeSound() -> string
    
    # Concrete method - shared by all animals
    fn introduce() {
        println("I am " + this.name)
        println("I say: " + this.makeSound())
    }
}

# Concrete implementation
class Dog extends Animal {
    fn init(n: string) {
        super.init(n)
    }
    
    # Must implement abstract method
    fn makeSound() -> string {
        return "Woof!"
    }
}

let dog = new Dog("Buddy")
dog.introduce()
# Output:
# I am Buddy
# I say: Woof!
```

---

## Abstract Methods

Abstract methods are method declarations without implementations. Child classes **must** provide implementations for all abstract methods.

### Declaration

```klang
abstract class Shape {
    # Abstract method - just the signature
    abstract fn area() -> float
    abstract fn perimeter() -> float
}
```

### Rules for Abstract Methods

1. **No Implementation**: Abstract methods have no body
2. **Must Override**: Child classes must implement them
3. **In Abstract Classes**: Only abstract classes can have abstract methods
4. **Cannot be Private**: Abstract methods must be accessible to children

### Complete Example

```klang
abstract class Shape {
    let color: string = "black"
    
    fn init(c: string) {
        this.color = c
    }
    
    # Abstract methods - children must implement
    abstract fn area() -> float
    abstract fn perimeter() -> float
    
    # Concrete method using abstract methods
    fn describe() {
        println("Color: " + this.color)
        println("Area: " + str(this.area()))
        println("Perimeter: " + str(this.perimeter()))
    }
}

class Rectangle extends Shape {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(c: string, w: float, h: float) {
        super.init(c)
        this.width = w
        this.height = h
    }
    
    # Implement abstract methods
    fn area() -> float {
        return this.width * this.height
    }
    
    fn perimeter() -> float {
        return 2.0 * (this.width + this.height)
    }
}

class Circle extends Shape {
    let radius: float = 0.0
    
    fn init(c: string, r: float) {
        super.init(c)
        this.radius = r
    }
    
    # Implement abstract methods
    fn area() -> float {
        return 3.14159 * this.radius * this.radius
    }
    
    fn perimeter() -> float {
        return 2.0 * 3.14159 * this.radius
    }
}

# Usage
let rect = new Rectangle("blue", 5.0, 3.0)
rect.describe()
# Output:
# Color: blue
# Area: 15
# Perimeter: 16

let circle = new Circle("red", 4.0)
circle.describe()
# Output:
# Color: red
# Area: 50.26544
# Perimeter: 25.13272
```

---

## Concrete Methods

Abstract classes can (and should) contain concrete methods with full implementations. These methods provide shared functionality across all child classes.

### Purpose of Concrete Methods

1. **Code Reuse**: Share common logic without duplication
2. **Default Behavior**: Provide default implementations
3. **Template Methods**: Use abstract methods within concrete ones
4. **Helper Functions**: Provide utility functions to children

### Example: Mix of Abstract and Concrete

```klang
abstract class BankAccount {
    let accountNumber: string = ""
    let balance: float = 0.0
    let owner: string = ""
    
    fn init(accNum: string, own: string) {
        this.accountNumber = accNum
        this.owner = own
        this.balance = 0.0
    }
    
    # Concrete methods - shared by all accounts
    fn deposit(amount: float) {
        if (amount > 0.0) {
            this.balance = this.balance + amount
            println("Deposited: $" + str(amount))
            this.logTransaction("DEPOSIT", amount)
        }
    }
    
    fn getBalance() -> float {
        return this.balance
    }
    
    fn displayInfo() {
        println("Account: " + this.accountNumber)
        println("Owner: " + this.owner)
        println("Balance: $" + str(this.balance))
        println("Type: " + this.getAccountType())
    }
    
    # Abstract methods - each account type implements differently
    abstract fn withdraw(amount: float) -> bool
    abstract fn calculateInterest() -> float
    abstract fn getAccountType() -> string
    abstract fn logTransaction(type: string, amount: float)
}

class SavingsAccount extends BankAccount {
    let interestRate: float = 0.03
    let minBalance: float = 100.0
    
    fn init(accNum: string, own: string) {
        super.init(accNum, own)
    }
    
    fn withdraw(amount: float) -> bool {
        if (amount <= 0.0) {
            println("Invalid amount")
            return false
        }
        
        let newBalance = this.balance - amount
        if (newBalance < this.minBalance) {
            println("Cannot withdraw: minimum balance required")
            return false
        }
        
        this.balance = newBalance
        println("Withdrew: $" + str(amount))
        this.logTransaction("WITHDRAW", amount)
        return true
    }
    
    fn calculateInterest() -> float {
        return this.balance * this.interestRate
    }
    
    fn getAccountType() -> string {
        return "Savings Account"
    }
    
    fn logTransaction(type: string, amount: float) {
        println("[LOG] " + type + ": $" + str(amount) + " on " + this.accountNumber)
    }
}

class CheckingAccount extends BankAccount {
    let overdraftLimit: float = 500.0
    
    fn init(accNum: string, own: string) {
        super.init(accNum, own)
    }
    
    fn withdraw(amount: float) -> bool {
        if (amount <= 0.0) {
            println("Invalid amount")
            return false
        }
        
        let newBalance = this.balance - amount
        if (newBalance < -this.overdraftLimit) {
            println("Cannot withdraw: overdraft limit exceeded")
            return false
        }
        
        this.balance = newBalance
        println("Withdrew: $" + str(amount))
        this.logTransaction("WITHDRAW", amount)
        return true
    }
    
    fn calculateInterest() -> float {
        return 0.0  # No interest on checking
    }
    
    fn getAccountType() -> string {
        return "Checking Account"
    }
    
    fn logTransaction(type: string, amount: float) {
        println("[LOG] " + type + ": $" + str(amount) + " on " + this.accountNumber)
    }
}

# Usage
let savings = new SavingsAccount("SAV001", "Alice")
savings.deposit(1000.0)
savings.withdraw(50.0)
savings.displayInfo()

let checking = new CheckingAccount("CHK001", "Bob")
checking.deposit(500.0)
checking.withdraw(800.0)  # Uses overdraft
checking.displayInfo()
```

---

## Cannot Instantiate

Abstract classes **cannot be instantiated directly**. You can only create instances of concrete child classes.

### Why?

Abstract classes are incomplete - they have abstract methods without implementations. Creating an instance would result in an object with undefined behavior.

### Compilation Error Example

```klang
abstract class Vehicle {
    abstract fn startEngine()
}

# This will cause a compilation error:
# let vehicle = new Vehicle()
# Error: Cannot instantiate abstract class 'Vehicle'
```

### Correct Usage

```klang
abstract class Vehicle {
    let brand: string = ""
    
    fn init(b: string) {
        this.brand = b
    }
    
    abstract fn startEngine()
}

class Car extends Vehicle {
    fn init(b: string) {
        super.init(b)
    }
    
    fn startEngine() {
        println("Car engine started")
    }
}

# This is correct - instantiate concrete class
let car = new Car("Toyota")
car.startEngine()
```

### Polymorphic References

While you cannot instantiate abstract classes, you can use them as types for polymorphic references:

```klang
abstract class Animal {
    abstract fn makeSound()
}

class Dog extends Animal {
    fn makeSound() {
        println("Woof!")
    }
}

class Cat extends Animal {
    fn makeSound() {
        println("Meow!")
    }
}

# Abstract class used as type
let animals: array<Animal> = []
animals.push(new Dog())
animals.push(new Cat())

for (let i = 0; i < animals.length(); i = i + 1) {
    animals[i].makeSound()
}
# Output:
# Woof!
# Meow!
```

---

## Child Implementation Requirements

When a class extends an abstract class, it **must implement all abstract methods** unless it is also declared abstract.

### Rule 1: Implement All Abstract Methods

```klang
abstract class Database {
    abstract fn connect() -> bool
    abstract fn disconnect()
    abstract fn query(sql: string) -> string
}

# This class must implement all three abstract methods
class MySQLDatabase extends Database {
    fn connect() -> bool {
        println("Connecting to MySQL...")
        return true
    }
    
    fn disconnect() {
        println("Disconnecting from MySQL...")
    }
    
    fn query(sql: string) -> string {
        println("Executing: " + sql)
        return "Result data"
    }
}
```

### Rule 2: Incomplete Implementation Not Allowed

```klang
abstract class Shape {
    abstract fn area() -> float
    abstract fn perimeter() -> float
}

# Error: Must implement both area() AND perimeter()
class Triangle extends Shape {
    fn area() -> float {
        return 0.0
    }
    # Missing perimeter() - Compilation Error!
}
```

### Rule 3: Child Can Be Abstract

A child class can also be abstract if it doesn't implement all abstract methods:

```klang
abstract class Shape {
    abstract fn area() -> float
    abstract fn perimeter() -> float
    abstract fn draw()
}

# Implements only area and perimeter
abstract class Polygon extends Shape {
    let sides: int = 0
    
    fn init(s: int) {
        this.sides = s
    }
    
    fn area() -> float {
        return 0.0  # Default implementation
    }
    
    fn perimeter() -> float {
        return 0.0  # Default implementation
    }
    
    # draw() still abstract - must be implemented by concrete children
}

# Concrete class must implement draw()
class Triangle extends Polygon {
    fn init() {
        super.init(3)
    }
    
    fn draw() {
        println("Drawing triangle")
    }
}
```

### Rule 4: Match Method Signature

Implementations must exactly match the abstract method signature:

```klang
abstract class Processor {
    abstract fn process(data: string) -> bool
}

# Correct - exact signature match
class JSONProcessor extends Processor {
    fn process(data: string) -> bool {
        println("Processing JSON: " + data)
        return true
    }
}

# Error - signature mismatch
class XMLProcessor extends Processor {
    # Wrong: parameter type doesn't match
    fn process(data: int) -> bool {
        return true
    }
}
```

---

## Practical Examples

### Example 1: Shape Hierarchy

```klang
abstract class Shape {
    let color: string = "black"
    let filled: bool = false
    
    fn init(c: string, f: bool) {
        this.color = c
        this.filled = f
    }
    
    # Abstract methods
    abstract fn area() -> float
    abstract fn perimeter() -> float
    abstract fn getShapeName() -> string
    
    # Concrete methods
    fn display() {
        println("=== " + this.getShapeName() + " ===")
        println("Color: " + this.color)
        println("Filled: " + str(this.filled))
        println("Area: " + str(this.area()))
        println("Perimeter: " + str(this.perimeter()))
    }
    
    fn setColor(c: string) {
        this.color = c
    }
    
    fn isFilled() -> bool {
        return this.filled
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
    
    fn getShapeName() -> string {
        return "Rectangle"
    }
}

class Circle extends Shape {
    let radius: float = 0.0
    
    fn init(c: string, f: bool, r: float) {
        super.init(c, f)
        this.radius = r
    }
    
    fn area() -> float {
        return 3.14159 * this.radius * this.radius
    }
    
    fn perimeter() -> float {
        return 2.0 * 3.14159 * this.radius
    }
    
    fn getShapeName() -> string {
        return "Circle"
    }
}

class Triangle extends Shape {
    let base: float = 0.0
    let height: float = 0.0
    let side1: float = 0.0
    let side2: float = 0.0
    
    fn init(c: string, f: bool, b: float, h: float, s1: float, s2: float) {
        super.init(c, f)
        this.base = b
        this.height = h
        this.side1 = s1
        this.side2 = s2
    }
    
    fn area() -> float {
        return 0.5 * this.base * this.height
    }
    
    fn perimeter() -> float {
        return this.base + this.side1 + this.side2
    }
    
    fn getShapeName() -> string {
        return "Triangle"
    }
}

# Usage
let shapes: array<Shape> = []
shapes.push(new Rectangle("blue", true, 5.0, 3.0))
shapes.push(new Circle("red", false, 4.0))
shapes.push(new Triangle("green", true, 6.0, 4.0, 5.0, 5.0))

for (let i = 0; i < shapes.length(); i = i + 1) {
    shapes[i].display()
    println("")
}
```

### Example 2: Animal Hierarchy

```klang
abstract class Animal {
    let name: string = ""
    let age: int = 0
    let species: string = ""
    
    fn init(n: string, a: int, s: string) {
        this.name = n
        this.age = a
        this.species = s
    }
    
    # Abstract methods
    abstract fn makeSound() -> string
    abstract fn move() -> string
    abstract fn eat(food: string) -> bool
    
    # Concrete methods
    fn introduce() {
        println("Name: " + this.name)
        println("Age: " + str(this.age))
        println("Species: " + this.species)
        println("Sound: " + this.makeSound())
        println("Movement: " + this.move())
    }
    
    fn celebrate() {
        println(this.name + " says: " + this.makeSound())
    }
}

class Dog extends Animal {
    let breed: string = ""
    
    fn init(n: string, a: int, b: string) {
        super.init(n, a, "Dog")
        this.breed = b
    }
    
    fn makeSound() -> string {
        return "Woof! Woof!"
    }
    
    fn move() -> string {
        return "Running on four legs"
    }
    
    fn eat(food: string) -> bool {
        if (food == "bone" or food == "meat") {
            println(this.name + " happily eats " + food)
            return true
        }
        println(this.name + " doesn't like " + food)
        return false
    }
}

class Bird extends Animal {
    let canFly: bool = true
    
    fn init(n: string, a: int, fly: bool) {
        super.init(n, a, "Bird")
        this.canFly = fly
    }
    
    fn makeSound() -> string {
        return "Tweet! Chirp!"
    }
    
    fn move() -> string {
        if (this.canFly) {
            return "Flying in the sky"
        }
        return "Hopping on the ground"
    }
    
    fn eat(food: string) -> bool {
        if (food == "seeds" or food == "worms") {
            println(this.name + " pecks at " + food)
            return true
        }
        println(this.name + " ignores " + food)
        return false
    }
}

class Fish extends Animal {
    let waterType: string = "fresh"
    
    fn init(n: string, a: int, wt: string) {
        super.init(n, a, "Fish")
        this.waterType = wt
    }
    
    fn makeSound() -> string {
        return "Blub... blub..."
    }
    
    fn move() -> string {
        return "Swimming in " + this.waterType + " water"
    }
    
    fn eat(food: string) -> bool {
        if (food == "algae" or food == "insects") {
            println(this.name + " gobbles " + food)
            return true
        }
        println(this.name + " swims away from " + food)
        return false
    }
}

# Usage
let zoo: array<Animal> = []
zoo.push(new Dog("Buddy", 3, "Golden Retriever"))
zoo.push(new Bird("Tweety", 1, true))
zoo.push(new Fish("Nemo", 2, "salt"))

for (let i = 0; i < zoo.length(); i = i + 1) {
    zoo[i].introduce()
    zoo[i].eat("meat")
    println("---")
}
```

### Example 3: Payment Processor

```klang
abstract class PaymentProcessor {
    let merchantId: string = ""
    let transactionId: string = ""
    
    fn init(mid: string) {
        this.merchantId = mid
        this.transactionId = ""
    }
    
    # Abstract methods
    abstract fn authorize(amount: float) -> bool
    abstract fn capture(amount: float) -> bool
    abstract fn refund(amount: float) -> bool
    abstract fn getProcessorName() -> string
    
    # Template method - defines the payment flow
    fn processPayment(amount: float) -> bool {
        println("=== Processing Payment via " + this.getProcessorName() + " ===")
        
        # Step 1: Validate
        if (!this.validateAmount(amount)) {
            println("Payment failed: Invalid amount")
            return false
        }
        
        # Step 2: Authorize
        println("Authorizing payment...")
        if (!this.authorize(amount)) {
            println("Payment failed: Authorization declined")
            return false
        }
        
        # Step 3: Capture
        println("Capturing funds...")
        if (!this.capture(amount)) {
            println("Payment failed: Capture failed")
            return false
        }
        
        # Step 4: Generate transaction ID
        this.transactionId = this.generateTransactionId()
        println("Payment successful! Transaction ID: " + this.transactionId)
        
        return true
    }
    
    # Concrete helper methods
    fn validateAmount(amount: float) -> bool {
        return amount > 0.0
    }
    
    fn generateTransactionId() -> string {
        return "TXN-" + this.merchantId + "-12345"
    }
}

class CreditCardProcessor extends PaymentProcessor {
    let cardNumber: string = ""
    let cvv: string = ""
    
    fn init(mid: string, card: string, cvv: string) {
        super.init(mid)
        this.cardNumber = card
        this.cvv = cvv
    }
    
    fn authorize(amount: float) -> bool {
        println("Checking credit card: " + this.cardNumber)
        # Simulate authorization logic
        return true
    }
    
    fn capture(amount: float) -> bool {
        println("Charging card $" + str(amount))
        return true
    }
    
    fn refund(amount: float) -> bool {
        println("Refunding $" + str(amount) + " to card")
        return true
    }
    
    fn getProcessorName() -> string {
        return "Credit Card"
    }
}

class PayPalProcessor extends PaymentProcessor {
    let email: string = ""
    
    fn init(mid: string, email: string) {
        super.init(mid)
        this.email = email
    }
    
    fn authorize(amount: float) -> bool {
        println("Authorizing PayPal account: " + this.email)
        return true
    }
    
    fn capture(amount: float) -> bool {
        println("Transferring $" + str(amount) + " from PayPal")
        return true
    }
    
    fn refund(amount: float) -> bool {
        println("Refunding $" + str(amount) + " to PayPal account")
        return true
    }
    
    fn getProcessorName() -> string {
        return "PayPal"
    }
}

class BitcoinProcessor extends PaymentProcessor {
    let walletAddress: string = ""
    
    fn init(mid: string, wallet: string) {
        super.init(mid)
        this.walletAddress = wallet
    }
    
    fn authorize(amount: float) -> bool {
        println("Verifying Bitcoin wallet: " + this.walletAddress)
        return true
    }
    
    fn capture(amount: float) -> bool {
        println("Transferring " + str(amount) + " BTC")
        return true
    }
    
    fn refund(amount: float) -> bool {
        println("Refunding " + str(amount) + " BTC to wallet")
        return true
    }
    
    fn getProcessorName() -> string {
        return "Bitcoin"
    }
}

# Usage
let processors: array<PaymentProcessor> = []
processors.push(new CreditCardProcessor("MERCH001", "4111-1111-1111-1111", "123"))
processors.push(new PayPalProcessor("MERCH001", "user@example.com"))
processors.push(new BitcoinProcessor("MERCH001", "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa"))

for (let i = 0; i < processors.length(); i = i + 1) {
    processors[i].processPayment(99.99)
    println("")
}
```

### Example 4: Database Connection

```klang
abstract class DatabaseConnection {
    let host: string = ""
    let port: int = 0
    let database: string = ""
    let username: string = ""
    let connected: bool = false
    
    fn init(h: string, p: int, db: string, user: string) {
        this.host = h
        this.port = p
        this.database = db
        this.username = user
        this.connected = false
    }
    
    # Abstract methods
    abstract fn connect() -> bool
    abstract fn disconnect()
    abstract fn executeQuery(sql: string) -> string
    abstract fn executeUpdate(sql: string) -> int
    abstract fn beginTransaction()
    abstract fn commitTransaction()
    abstract fn rollbackTransaction()
    
    # Concrete template method
    fn executeTransaction(queries: array<string>) -> bool {
        if (!this.connected) {
            println("Error: Not connected to database")
            return false
        }
        
        println("Starting transaction...")
        this.beginTransaction()
        
        let success = true
        for (let i = 0; i < queries.length(); i = i + 1) {
            let result = this.executeUpdate(queries[i])
            if (result < 0) {
                println("Query failed, rolling back...")
                this.rollbackTransaction()
                success = false
                break
            }
        }
        
        if (success) {
            println("Committing transaction...")
            this.commitTransaction()
        }
        
        return success
    }
    
    fn isConnected() -> bool {
        return this.connected
    }
}

class MySQLConnection extends DatabaseConnection {
    let charset: string = "utf8"
    
    fn init(h: string, p: int, db: string, user: string) {
        super.init(h, p, db, user)
    }
    
    fn connect() -> bool {
        println("Connecting to MySQL at " + this.host + ":" + str(this.port))
        this.connected = true
        return true
    }
    
    fn disconnect() {
        println("Disconnecting from MySQL")
        this.connected = false
    }
    
    fn executeQuery(sql: string) -> string {
        println("MySQL Query: " + sql)
        return "query_result"
    }
    
    fn executeUpdate(sql: string) -> int {
        println("MySQL Update: " + sql)
        return 1  # Rows affected
    }
    
    fn beginTransaction() {
        println("MySQL: BEGIN TRANSACTION")
    }
    
    fn commitTransaction() {
        println("MySQL: COMMIT")
    }
    
    fn rollbackTransaction() {
        println("MySQL: ROLLBACK")
    }
}

class PostgreSQLConnection extends DatabaseConnection {
    let sslMode: string = "require"
    
    fn init(h: string, p: int, db: string, user: string) {
        super.init(h, p, db, user)
    }
    
    fn connect() -> bool {
        println("Connecting to PostgreSQL at " + this.host + ":" + str(this.port))
        println("SSL Mode: " + this.sslMode)
        this.connected = true
        return true
    }
    
    fn disconnect() {
        println("Disconnecting from PostgreSQL")
        this.connected = false
    }
    
    fn executeQuery(sql: string) -> string {
        println("PostgreSQL Query: " + sql)
        return "query_result"
    }
    
    fn executeUpdate(sql: string) -> int {
        println("PostgreSQL Update: " + sql)
        return 1
    }
    
    fn beginTransaction() {
        println("PostgreSQL: BEGIN")
    }
    
    fn commitTransaction() {
        println("PostgreSQL: COMMIT")
    }
    
    fn rollbackTransaction() {
        println("PostgreSQL: ROLLBACK")
    }
}

# Usage
let mysql = new MySQLConnection("localhost", 3306, "mydb", "root")
mysql.connect()
mysql.executeQuery("SELECT * FROM users")

let queries: array<string> = []
queries.push("INSERT INTO users VALUES (1, 'Alice')")
queries.push("UPDATE accounts SET balance = 100 WHERE id = 1")
mysql.executeTransaction(queries)

mysql.disconnect()
```

---

## Abstract vs Interface

Both abstract classes and interfaces define contracts, but they have important differences.

### Comparison Table

| Feature | Abstract Class | Interface |
|---------|---------------|-----------|
| **Instantiation** | Cannot instantiate | Cannot instantiate |
| **Abstract Methods** | Can have abstract methods | All methods are abstract |
| **Concrete Methods** | Can have concrete methods | Cannot have concrete methods |
| **Properties** | Can have properties | Cannot have properties |
| **Constructor** | Can have constructors | Cannot have constructors |
| **State** | Can maintain state | Cannot maintain state |
| **Multiple Inheritance** | Single inheritance only | Can implement multiple |
| **Use Case** | "is-a" relationship | "can-do" behavior |

### When to Use Abstract Classes

Use abstract classes when:

1. **Sharing Code**: You want to share implementation among related classes
2. **Common State**: You need to maintain common state (properties)
3. **Partial Implementation**: Some methods have default implementations
4. **Natural Hierarchy**: Classes have a natural "is-a" relationship

```klang
# Good use of abstract class
abstract class Vehicle {
    let speed: float = 0.0  # Shared state
    
    # Shared implementation
    fn accelerate(amount: float) {
        this.speed = this.speed + amount
    }
    
    # Must be implemented by children
    abstract fn startEngine()
}
```

### When to Use Interfaces

Use interfaces when:

1. **Unrelated Classes**: Classes are unrelated but share behavior
2. **Multiple Behaviors**: A class needs multiple capabilities
3. **Contract Only**: You only need to define what, not how
4. **Flexibility**: Maximum flexibility for implementations

```klang
# Good use of interface
interface Flyable {
    fn fly()
    fn land()
}

interface Swimmable {
    fn swim()
}

# Bird can both fly and swim
class Duck implements Flyable, Swimmable {
    fn fly() { println("Duck flying") }
    fn land() { println("Duck landing") }
    fn swim() { println("Duck swimming") }
}
```

### Side-by-Side Example

```klang
# Abstract class: Related classes with shared code
abstract class Employee {
    let name: string = ""
    let salary: float = 0.0
    
    fn init(n: string, s: float) {
        this.name = n
        this.salary = s
    }
    
    # Shared implementation
    fn displayBasicInfo() {
        println("Name: " + this.name)
        println("Salary: $" + str(this.salary))
    }
    
    # Different for each employee type
    abstract fn calculateBonus() -> float
}

# Interface: Unrelated classes with common capability
interface Printable {
    fn print()
}

class Manager extends Employee implements Printable {
    fn init(n: string, s: float) {
        super.init(n, s)
    }
    
    fn calculateBonus() -> float {
        return this.salary * 0.20  # 20% bonus
    }
    
    fn print() {
        println("=== Manager Report ===")
        this.displayBasicInfo()
        println("Bonus: $" + str(this.calculateBonus()))
    }
}

class Developer extends Employee implements Printable {
    fn init(n: string, s: float) {
        super.init(n, s)
    }
    
    fn calculateBonus() -> float {
        return this.salary * 0.15  # 15% bonus
    }
    
    fn print() {
        println("=== Developer Report ===")
        this.displayBasicInfo()
        println("Bonus: $" + str(this.calculateBonus()))
    }
}
```

---

## Abstract vs Concrete Classes

Understanding when to use abstract versus concrete classes is crucial for good design.

### Concrete Classes

**Concrete classes** are complete classes that can be instantiated directly.

```klang
# Concrete class - can be instantiated
class Rectangle {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(w: float, h: float) {
        this.width = w
        this.height = h
    }
    
    fn area() -> float {
        return this.width * this.height
    }
}

let rect = new Rectangle(5.0, 3.0)  # OK - concrete class
```

### Abstract Classes

**Abstract classes** are incomplete and cannot be instantiated.

```klang
# Abstract class - cannot be instantiated
abstract class Shape {
    abstract fn area() -> float
}

# let shape = new Shape()  # Error!
```

### When to Use Abstract Classes

Use abstract classes when:

1. **Prevent Direct Instantiation**

```klang
# Bad: Concrete class allows meaningless instantiation
class Shape {
    fn area() -> float {
        return 0.0  # What does this mean?
    }
}

let shape = new Shape()  # Creates a meaningless shape

# Good: Abstract class prevents this
abstract class Shape {
    abstract fn area() -> float
}

# Must create specific shape
let circle = new Circle(5.0)
```

2. **Force Implementation**

```klang
abstract class DatabaseDriver {
    # Forces all drivers to implement these
    abstract fn connect() -> bool
    abstract fn query(sql: string) -> string
    abstract fn disconnect()
}

# Each driver must implement all methods
class MySQLDriver extends DatabaseDriver {
    # Must implement connect(), query(), disconnect()
}
```

3. **Share Common Logic**

```klang
abstract class GameCharacter {
    let health: int = 100
    let level: int = 1
    
    # Shared logic
    fn takeDamage(amount: int) {
        this.health = this.health - amount
        if (this.health <= 0) {
            this.die()
        }
    }
    
    fn levelUp() {
        this.level = this.level + 1
        this.health = 100
    }
    
    # Character-specific behavior
    abstract fn attack() -> int
    abstract fn die()
}
```

### When to Use Concrete Classes

Use concrete classes when:

1. **Complete Implementation**: All functionality is defined
2. **Ready to Use**: Objects can be created and used immediately
3. **No Specialization Needed**: No need for child classes

```klang
# Concrete classes for specific, complete types
class Point {
    let x: float = 0.0
    let y: float = 0.0
    
    fn init(x: float, y: float) {
        this.x = x
        this.y = y
    }
    
    fn distance(other: Point) -> float {
        let dx = this.x - other.x
        let dy = this.y - other.y
        return sqrt(dx * dx + dy * dy)
    }
}

let p1 = new Point(0.0, 0.0)
let p2 = new Point(3.0, 4.0)
println(str(p1.distance(p2)))  # 5.0
```

### Hierarchy Example

```klang
# Abstract base - defines contract
abstract class Transport {
    abstract fn move()
    abstract fn stop()
}

# Abstract intermediate - adds partial implementation
abstract class LandTransport extends Transport {
    let wheels: int = 0
    
    fn stop() {
        println("Applying brakes")
    }
    
    # Still abstract - children implement
    abstract fn move()
}

# Concrete classes - complete implementation
class Car extends LandTransport {
    fn init() {
        this.wheels = 4
    }
    
    fn move() {
        println("Car driving on " + str(this.wheels) + " wheels")
    }
}

class Motorcycle extends LandTransport {
    fn init() {
        this.wheels = 2
    }
    
    fn move() {
        println("Motorcycle riding on " + str(this.wheels) + " wheels")
    }
}
```

---

## Template Method Pattern

The **Template Method Pattern** is a design pattern that uses abstract classes to define the skeleton of an algorithm, allowing child classes to customize specific steps.

### Pattern Structure

```klang
abstract class AbstractClass {
    # Template method - defines algorithm structure
    fn templateMethod() {
        this.step1()
        this.step2()
        this.step3()
    }
    
    # Concrete steps
    fn step1() {
        # Default implementation
    }
    
    # Abstract steps - children customize
    abstract fn step2()
    
    # Hook method - optional override
    fn step3() {
        # Default (do nothing)
    }
}
```

### Complete Example: Data Processing Pipeline

```klang
abstract class DataProcessor {
    # Template method - defines processing pipeline
    fn process(filename: string) {
        println("=== Starting Data Processing ===")
        
        # Step 1: Read data
        let data = this.readData(filename)
        if (data == "") {
            println("Error: Failed to read data")
            return
        }
        
        # Step 2: Parse data
        println("Parsing data...")
        this.parseData(data)
        
        # Step 3: Validate data
        if (!this.validateData()) {
            println("Error: Validation failed")
            return
        }
        
        # Step 4: Transform data
        println("Transforming data...")
        this.transformData()
        
        # Step 5: Save results
        this.saveResults()
        
        println("=== Processing Complete ===")
    }
    
    # Concrete method - same for all processors
    fn readData(filename: string) -> string {
        println("Reading file: " + filename)
        return "raw_data_content"
    }
    
    # Abstract methods - each processor implements differently
    abstract fn parseData(raw: string)
    abstract fn validateData() -> bool
    abstract fn transformData()
    abstract fn saveResults()
}

class CSVProcessor extends DataProcessor {
    let rows: int = 0
    let columns: int = 0
    
    fn parseData(raw: string) {
        println("Parsing CSV format")
        this.rows = 100
        this.columns = 5
    }
    
    fn validateData() -> bool {
        println("Validating CSV data")
        if (this.rows == 0 or this.columns == 0) {
            return false
        }
        return true
    }
    
    fn transformData() {
        println("Converting CSV to structured format")
        println("Rows: " + str(this.rows) + ", Columns: " + str(this.columns))
    }
    
    fn saveResults() {
        println("Saving to database")
    }
}

class JSONProcessor extends DataProcessor {
    let objects: int = 0
    
    fn parseData(raw: string) {
        println("Parsing JSON format")
        this.objects = 50
    }
    
    fn validateData() -> bool {
        println("Validating JSON schema")
        return this.objects > 0
    }
    
    fn transformData() {
        println("Normalizing JSON objects")
        println("Objects processed: " + str(this.objects))
    }
    
    fn saveResults() {
        println("Saving to NoSQL database")
    }
}

class XMLProcessor extends DataProcessor {
    let nodes: int = 0
    
    fn parseData(raw: string) {
        println("Parsing XML format")
        this.nodes = 75
    }
    
    fn validateData() -> bool {
        println("Validating XML against DTD")
        return this.nodes > 0
    }
    
    fn transformData() {
        println("Converting XML to objects")
        println("Nodes processed: " + str(this.nodes))
    }
    
    fn saveResults() {
        println("Saving to file system")
    }
}

# Usage
let processors: array<DataProcessor> = []
processors.push(new CSVProcessor())
processors.push(new JSONProcessor())
processors.push(new XMLProcessor())

for (let i = 0; i < processors.length(); i = i + 1) {
    processors[i].process("data_file_" + str(i))
    println("")
}
```

### Example: Report Generation

```klang
abstract class ReportGenerator {
    let title: string = ""
    let data: array<string> = []
    
    # Template method
    fn generateReport(t: string) {
        this.title = t
        
        println("Generating " + this.getReportType() + " Report")
        
        this.writeHeader()
        this.writeBody()
        this.writeFooter()
        this.finalizeReport()
        
        println("Report generation complete")
    }
    
    # Concrete methods
    fn writeHeader() {
        println("=".repeat(50))
        println(this.title)
        println("=".repeat(50))
    }
    
    fn writeFooter() {
        println("=".repeat(50))
        println("End of Report")
    }
    
    # Abstract methods - each report type implements
    abstract fn getReportType() -> string
    abstract fn writeBody()
    abstract fn finalizeReport()
}

class PDFReport extends ReportGenerator {
    fn getReportType() -> string {
        return "PDF"
    }
    
    fn writeBody() {
        println("Rendering PDF content...")
        println("Adding charts and graphs")
        println("Formatting tables")
    }
    
    fn finalizeReport() {
        println("Compressing PDF")
        println("Saving to: report.pdf")
    }
}

class HTMLReport extends ReportGenerator {
    fn getReportType() -> string {
        return "HTML"
    }
    
    fn writeBody() {
        println("<html><body>")
        println("<h1>" + this.title + "</h1>")
        println("<div>Report content here</div>")
        println("</body></html>")
    }
    
    fn finalizeReport() {
        println("Minifying HTML")
        println("Saving to: report.html")
    }
}

class ExcelReport extends ReportGenerator {
    fn getReportType() -> string {
        return "Excel"
    }
    
    fn writeBody() {
        println("Creating worksheets")
        println("Populating cells with data")
        println("Adding formulas and pivot tables")
    }
    
    fn finalizeReport() {
        println("Calculating formulas")
        println("Saving to: report.xlsx")
    }
}

# Usage
let pdf = new PDFReport()
pdf.generateReport("Q4 Sales Report")

let html = new HTMLReport()
html.generateReport("Monthly Analytics")

let excel = new ExcelReport()
excel.generateReport("Financial Summary")
```

---

## Best Practices

### 1. Use Abstract Classes for "Is-A" Relationships

Abstract classes model hierarchical relationships where child "is a" type of parent.

```klang
# Good: Natural hierarchy
abstract class Employee { }
class Manager extends Employee { }  # Manager IS AN Employee

# Bad: No natural relationship
abstract class Database { }
class Car extends Database { }  # Car is NOT a Database
```

### 2. Provide Meaningful Abstract Methods

Each abstract method should represent a clear contract that makes sense for all children.

```klang
# Good: All shapes have area
abstract class Shape {
    abstract fn area() -> float
    abstract fn perimeter() -> float
}

# Bad: Not all employees drive
abstract class Employee {
    abstract fn drive()  # What about remote workers?
}
```

### 3. Don't Over-Abstract

Don't make a class abstract if all methods can be implemented.

```klang
# Bad: No abstract methods, why is it abstract?
abstract class Point {
    let x: float = 0.0
    let y: float = 0.0
    
    fn distance(other: Point) -> float {
        return sqrt((this.x - other.x) * (this.x - other.x))
    }
}

# Good: Make it concrete
class Point {
    let x: float = 0.0
    let y: float = 0.0
    
    fn distance(other: Point) -> float {
        return sqrt((this.x - other.x) * (this.x - other.x))
    }
}
```

### 4. Keep Abstract Classes Focused

Each abstract class should have a single, clear purpose.

```klang
# Bad: Too many responsibilities
abstract class Animal {
    abstract fn eat()
    abstract fn sleep()
    abstract fn fly()      # Not all animals fly
    abstract fn swim()     # Not all animals swim
    abstract fn climb()    # Not all animals climb
}

# Good: Focused responsibilities
abstract class Animal {
    abstract fn eat()
    abstract fn sleep()
    abstract fn move()
}

# Separate capabilities
interface Flyable {
    fn fly()
}

interface Swimmable {
    fn swim()
}
```

### 5. Use Template Methods for Algorithms

When you have a standard algorithm with customizable steps, use template methods.

```klang
abstract class Authenticator {
    # Template method - standard flow
    fn authenticate(username: string, password: string) -> bool {
        if (!this.validateInput(username, password)) {
            return false
        }
        
        let user = this.findUser(username)
        if (user == "") {
            return false
        }
        
        return this.verifyPassword(user, password)
    }
    
    # Concrete validation
    fn validateInput(username: string, password: string) -> bool {
        return username != "" and password != ""
    }
    
    # Abstract steps - different for each auth system
    abstract fn findUser(username: string) -> string
    abstract fn verifyPassword(user: string, password: string) -> bool
}
```

### 6. Document Abstract Methods Clearly

Explain what each abstract method should do and what it should return.

```klang
abstract class Cache {
    # Store a value with the given key
    # Returns: true if successful, false otherwise
    abstract fn set(key: string, value: string) -> bool
    
    # Retrieve a value by key
    # Returns: the value if found, empty string if not found
    abstract fn get(key: string) -> string
    
    # Remove a value by key
    # Returns: true if the key existed and was removed, false otherwise
    abstract fn delete(key: string) -> bool
}
```

### 7. Prefer Composition Over Deep Hierarchies

Avoid creating deep inheritance trees. Prefer composition when appropriate.

```klang
# Bad: Deep hierarchy
abstract class Vehicle { }
abstract class LandVehicle extends Vehicle { }
abstract class MotorizedLandVehicle extends LandVehicle { }
class Car extends MotorizedLandVehicle { }

# Better: Shallow hierarchy with composition
abstract class Vehicle { }

class Car extends Vehicle {
    let engine: Engine      # Composition
    let wheels: array<Wheel>  # Composition
}
```

### 8. Initialize Properties in Abstract Classes

Provide sensible defaults for properties in abstract classes.

```klang
abstract class GameObject {
    let x: float = 0.0           # Default position
    let y: float = 0.0
    let isActive: bool = true    # Active by default
    let id: string = ""
    
    fn init() {
        this.id = this.generateId()
    }
    
    fn generateId() -> string {
        return "OBJ-" + str(System.currentTime())
    }
    
    abstract fn update()
    abstract fn render()
}
```

---

## Interview Problems

### Problem 1: Library System

**Question**: Design an abstract class `LibraryItem` for a library system. It should handle books, magazines, and DVDs. Include methods for checkout, return, and late fees.

**Solution**:

```klang
abstract class LibraryItem {
    let itemId: string = ""
    let title: string = ""
    let isCheckedOut: bool = false
    let dueDate: int = 0
    let dailyLateFee: float = 0.0
    
    fn init(id: string, title: string, fee: float) {
        this.itemId = id
        this.title = title
        this.dailyLateFee = fee
    }
    
    # Concrete methods
    fn checkout(days: int) {
        if (this.isCheckedOut) {
            println("Item already checked out")
            return
        }
        
        this.isCheckedOut = true
        this.dueDate = days
        println(this.title + " checked out for " + str(days) + " days")
    }
    
    fn returnItem(daysLate: int) {
        if (!this.isCheckedOut) {
            println("Item not checked out")
            return
        }
        
        this.isCheckedOut = false
        
        if (daysLate > 0) {
            let fee = this.calculateLateFee(daysLate)
            println("Late fee: $" + str(fee))
        }
        
        println(this.title + " returned")
    }
    
    fn calculateLateFee(daysLate: int) -> float {
        return float(daysLate) * this.dailyLateFee
    }
    
    # Abstract methods
    abstract fn getItemType() -> string
    abstract fn getMaxCheckoutDays() -> int
    abstract fn displayInfo()
}

class Book extends LibraryItem {
    let author: string = ""
    let isbn: string = ""
    
    fn init(id: string, title: string, author: string, isbn: string) {
        super.init(id, title, 0.50)
        this.author = author
        this.isbn = isbn
    }
    
    fn getItemType() -> string {
        return "Book"
    }
    
    fn getMaxCheckoutDays() -> int {
        return 14
    }
    
    fn displayInfo() {
        println("=== Book ===")
        println("Title: " + this.title)
        println("Author: " + this.author)
        println("ISBN: " + this.isbn)
        println("Status: " + (this.isCheckedOut ? "Checked Out" : "Available"))
    }
}

class Magazine extends LibraryItem {
    let issue: int = 0
    let month: string = ""
    
    fn init(id: string, title: string, issue: int, month: string) {
        super.init(id, title, 0.25)
        this.issue = issue
        this.month = month
    }
    
    fn getItemType() -> string {
        return "Magazine"
    }
    
    fn getMaxCheckoutDays() -> int {
        return 7
    }
    
    fn displayInfo() {
        println("=== Magazine ===")
        println("Title: " + this.title)
        println("Issue: " + str(this.issue))
        println("Month: " + this.month)
        println("Status: " + (this.isCheckedOut ? "Checked Out" : "Available"))
    }
}

class DVD extends LibraryItem {
    let director: string = ""
    let runtime: int = 0
    
    fn init(id: string, title: string, director: string, runtime: int) {
        super.init(id, title, 1.00)
        this.director = director
        this.runtime = runtime
    }
    
    fn getItemType() -> string {
        return "DVD"
    }
    
    fn getMaxCheckoutDays() -> int {
        return 3
    }
    
    fn displayInfo() {
        println("=== DVD ===")
        println("Title: " + this.title)
        println("Director: " + this.director)
        println("Runtime: " + str(this.runtime) + " minutes")
        println("Status: " + (this.isCheckedOut ? "Checked Out" : "Available"))
    }
}

# Usage
let book = new Book("B001", "Clean Code", "Robert Martin", "978-0132350884")
book.displayInfo()
book.checkout(14)
book.returnItem(3)  # 3 days late
```

### Problem 2: Notification System

**Question**: Create an abstract class for a notification system that supports email, SMS, and push notifications. Include rate limiting and retry logic.

**Solution**:

```klang
abstract class Notification {
    let recipient: string = ""
    let message: string = ""
    let priority: int = 1
    let maxRetries: int = 3
    let retryCount: int = 0
    
    fn init(recip: string, msg: string, prior: int) {
        this.recipient = recip
        this.message = msg
        this.priority = prior
    }
    
    # Template method
    fn send() -> bool {
        if (!this.validate()) {
            println("Validation failed")
            return false
        }
        
        if (!this.checkRateLimit()) {
            println("Rate limit exceeded")
            return false
        }
        
        let success = this.sendMessage()
        
        if (!success and this.retryCount < this.maxRetries) {
            this.retryCount = this.retryCount + 1
            println("Retrying... (" + str(this.retryCount) + "/" + str(this.maxRetries) + ")")
            return this.send()
        }
        
        if (success) {
            this.logSuccess()
        } else {
            this.logFailure()
        }
        
        return success
    }
    
    # Concrete methods
    fn validate() -> bool {
        return this.recipient != "" and this.message != ""
    }
    
    fn logSuccess() {
        println("[LOG] " + this.getNotificationType() + " sent to " + this.recipient)
    }
    
    fn logFailure() {
        println("[ERROR] Failed to send " + this.getNotificationType() + " to " + this.recipient)
    }
    
    # Abstract methods
    abstract fn sendMessage() -> bool
    abstract fn checkRateLimit() -> bool
    abstract fn getNotificationType() -> string
}

class EmailNotification extends Notification {
    let subject: string = ""
    let emailsSentToday: int = 0
    let dailyLimit: int = 100
    
    fn init(recip: string, msg: string, subj: string, prior: int) {
        super.init(recip, msg, prior)
        this.subject = subj
    }
    
    fn sendMessage() -> bool {
        println("Sending email to: " + this.recipient)
        println("Subject: " + this.subject)
        println("Message: " + this.message)
        this.emailsSentToday = this.emailsSentToday + 1
        return true
    }
    
    fn checkRateLimit() -> bool {
        return this.emailsSentToday < this.dailyLimit
    }
    
    fn getNotificationType() -> string {
        return "Email"
    }
}

class SMSNotification extends Notification {
    let phoneNumber: string = ""
    let smsSentThisHour: int = 0
    let hourlyLimit: int = 10
    
    fn init(phone: string, msg: string, prior: int) {
        super.init(phone, msg, prior)
        this.phoneNumber = phone
    }
    
    fn sendMessage() -> bool {
        if (this.message.length() > 160) {
            println("Message too long for SMS")
            return false
        }
        
        println("Sending SMS to: " + this.phoneNumber)
        println("Message: " + this.message)
        this.smsSentThisHour = this.smsSentThisHour + 1
        return true
    }
    
    fn checkRateLimit() -> bool {
        return this.smsSentThisHour < this.hourlyLimit
    }
    
    fn getNotificationType() -> string {
        return "SMS"
    }
}

class PushNotification extends Notification {
    let deviceToken: string = ""
    let pushCount: int = 0
    let maxPushPerDay: int = 50
    
    fn init(token: string, msg: string, prior: int) {
        super.init(token, msg, prior)
        this.deviceToken = token
    }
    
    fn sendMessage() -> bool {
        println("Sending push to device: " + this.deviceToken)
        println("Message: " + this.message)
        println("Priority: " + str(this.priority))
        this.pushCount = this.pushCount + 1
        return true
    }
    
    fn checkRateLimit() -> bool {
        return this.pushCount < this.maxPushPerDay
    }
    
    fn getNotificationType() -> string {
        return "Push Notification"
    }
}

# Usage
let email = new EmailNotification("user@example.com", "Hello!", "Welcome", 1)
email.send()

let sms = new SMSNotification("+1234567890", "Your code is 1234", 2)
sms.send()

let push = new PushNotification("device_token_123", "New message", 3)
push.send()
```

### Problem 3: File Compression

**Question**: Design an abstract class for file compression that supports ZIP, RAR, and TAR formats. Include compression ratio calculation.

**Solution**:

```klang
abstract class Compressor {
    let inputFile: string = ""
    let outputFile: string = ""
    let originalSize: float = 0.0
    let compressedSize: float = 0.0
    
    fn init(input: string, output: string) {
        this.inputFile = input
        this.outputFile = output
    }
    
    # Template method
    fn compress() -> bool {
        println("=== Starting Compression ===")
        println("Format: " + this.getFormat())
        println("Input: " + this.inputFile)
        
        # Read file
        this.originalSize = this.getFileSize(this.inputFile)
        println("Original size: " + str(this.originalSize) + " KB")
        
        # Compress
        if (!this.performCompression()) {
            println("Compression failed")
            return false
        }
        
        # Calculate results
        this.compressedSize = this.getFileSize(this.outputFile)
        let ratio = this.calculateCompressionRatio()
        let savings = this.calculateSpaceSavings()
        
        println("Compressed size: " + str(this.compressedSize) + " KB")
        println("Compression ratio: " + str(ratio) + "%")
        println("Space saved: " + str(savings) + " KB")
        println("Output: " + this.outputFile)
        
        return true
    }
    
    # Concrete methods
    fn getFileSize(filename: string) -> float {
        # Simulate file size
        return 1000.0
    }
    
    fn calculateCompressionRatio() -> float {
        if (this.originalSize == 0.0) {
            return 0.0
        }
        return (this.compressedSize / this.originalSize) * 100.0
    }
    
    fn calculateSpaceSavings() -> float {
        return this.originalSize - this.compressedSize
    }
    
    # Abstract methods
    abstract fn performCompression() -> bool
    abstract fn decompress() -> bool
    abstract fn getFormat() -> string
    abstract fn getCompressionLevel() -> int
}

class ZipCompressor extends Compressor {
    let compressionLevel: int = 6
    
    fn init(input: string, output: string, level: int) {
        super.init(input, output)
        this.compressionLevel = level
    }
    
    fn performCompression() -> bool {
        println("Applying ZIP compression (level " + str(this.compressionLevel) + ")...")
        println("Using DEFLATE algorithm")
        this.compressedSize = this.originalSize * 0.4  # 40% of original
        return true
    }
    
    fn decompress() -> bool {
        println("Extracting ZIP archive...")
        return true
    }
    
    fn getFormat() -> string {
        return "ZIP"
    }
    
    fn getCompressionLevel() -> int {
        return this.compressionLevel
    }
}

class RarCompressor extends Compressor {
    let solidMode: bool = false
    
    fn init(input: string, output: string, solid: bool) {
        super.init(input, output)
        this.solidMode = solid
    }
    
    fn performCompression() -> bool {
        println("Applying RAR compression...")
        if (this.solidMode) {
            println("Using solid archive mode")
            this.compressedSize = this.originalSize * 0.3  # Better compression
        } else {
            this.compressedSize = this.originalSize * 0.35
        }
        return true
    }
    
    fn decompress() -> bool {
        println("Extracting RAR archive...")
        return true
    }
    
    fn getFormat() -> string {
        return "RAR"
    }
    
    fn getCompressionLevel() -> int {
        return 5
    }
}

class TarCompressor extends Compressor {
    let useGzip: bool = false
    
    fn init(input: string, output: string, gzip: bool) {
        super.init(input, output)
        this.useGzip = gzip
    }
    
    fn performCompression() -> bool {
        println("Creating TAR archive...")
        if (this.useGzip) {
            println("Applying GZIP compression")
            this.compressedSize = this.originalSize * 0.45
        } else {
            println("No compression (TAR only)")
            this.compressedSize = this.originalSize * 0.95  # Minimal reduction
        }
        return true
    }
    
    fn decompress() -> bool {
        println("Extracting TAR archive...")
        return true
    }
    
    fn getFormat() -> string {
        return this.useGzip ? "TAR.GZ" : "TAR"
    }
    
    fn getCompressionLevel() -> int {
        return this.useGzip ? 6 : 0
    }
}

# Usage
let zip = new ZipCompressor("document.txt", "document.zip", 9)
zip.compress()

let rar = new RarCompressor("photos.folder", "photos.rar", true)
rar.compress()

let tar = new TarCompressor("project.folder", "project.tar.gz", true)
tar.compress()
```

### Problem 4: Sorting Algorithm Framework

**Question**: Create an abstract class for sorting algorithms that includes timing and comparison counting. Implement BubbleSort and QuickSort.

**Solution**:

```klang
abstract class SortAlgorithm {
    let comparisons: int = 0
    let swaps: int = 0
    let startTime: float = 0.0
    let endTime: float = 0.0
    
    # Template method
    fn sort(arr: array<int>) -> array<int> {
        println("=== " + this.getAlgorithmName() + " ===")
        println("Input size: " + str(arr.length()))
        
        this.resetCounters()
        this.startTime = this.getCurrentTime()
        
        let result = this.performSort(arr)
        
        this.endTime = this.getCurrentTime()
        this.displayStatistics()
        
        return result
    }
    
    # Concrete methods
    fn resetCounters() {
        this.comparisons = 0
        this.swaps = 0
    }
    
    fn getCurrentTime() -> float {
        return 0.0  # Simulated
    }
    
    fn displayStatistics() {
        let duration = this.endTime - this.startTime
        println("Comparisons: " + str(this.comparisons))
        println("Swaps: " + str(this.swaps))
        println("Time: " + str(duration) + " ms")
        println("Complexity: " + this.getTimeComplexity())
    }
    
    fn compare(a: int, b: int) -> bool {
        this.comparisons = this.comparisons + 1
        return a < b
    }
    
    fn swap(arr: array<int>, i: int, j: int) {
        this.swaps = this.swaps + 1
        let temp = arr[i]
        arr[i] = arr[j]
        arr[j] = temp
    }
    
    # Abstract methods
    abstract fn performSort(arr: array<int>) -> array<int>
    abstract fn getAlgorithmName() -> string
    abstract fn getTimeComplexity() -> string
    abstract fn getSpaceComplexity() -> string
}

class BubbleSort extends SortAlgorithm {
    fn performSort(arr: array<int>) -> array<int> {
        let n = arr.length()
        
        for (let i = 0; i < n - 1; i = i + 1) {
            for (let j = 0; j < n - i - 1; j = j + 1) {
                if (!this.compare(arr[j], arr[j + 1])) {
                    this.swap(arr, j, j + 1)
                }
            }
        }
        
        return arr
    }
    
    fn getAlgorithmName() -> string {
        return "Bubble Sort"
    }
    
    fn getTimeComplexity() -> string {
        return "O(n²)"
    }
    
    fn getSpaceComplexity() -> string {
        return "O(1)"
    }
}

class QuickSort extends SortAlgorithm {
    fn performSort(arr: array<int>) -> array<int> {
        this.quickSortHelper(arr, 0, arr.length() - 1)
        return arr
    }
    
    fn quickSortHelper(arr: array<int>, low: int, high: int) {
        if (low < high) {
            let pi = this.partition(arr, low, high)
            this.quickSortHelper(arr, low, pi - 1)
            this.quickSortHelper(arr, pi + 1, high)
        }
    }
    
    fn partition(arr: array<int>, low: int, high: int) -> int {
        let pivot = arr[high]
        let i = low - 1
        
        for (let j = low; j < high; j = j + 1) {
            if (this.compare(arr[j], pivot)) {
                i = i + 1
                this.swap(arr, i, j)
            }
        }
        
        this.swap(arr, i + 1, high)
        return i + 1
    }
    
    fn getAlgorithmName() -> string {
        return "Quick Sort"
    }
    
    fn getTimeComplexity() -> string {
        return "O(n log n) average, O(n²) worst"
    }
    
    fn getSpaceComplexity() -> string {
        return "O(log n)"
    }
}

# Usage
let data: array<int> = [64, 34, 25, 12, 22, 11, 90]

let bubble = new BubbleSort()
let sorted1 = bubble.sort(data)

let quick = new QuickSort()
let sorted2 = quick.sort(data)
```

### Problem 5: Game Character System

**Question**: Design an abstract class for RPG game characters with health, attacks, and special abilities. Implement Warrior, Mage, and Archer.

**Solution**:

```klang
abstract class GameCharacter {
    let name: string = ""
    let level: int = 1
    let health: int = 100
    let maxHealth: int = 100
    let mana: int = 50
    let maxMana: int = 50
    let isAlive: bool = true
    
    fn init(n: string) {
        this.name = n
        this.maxHealth = this.getBaseHealth()
        this.health = this.maxHealth
        this.maxMana = this.getBaseMana()
        this.mana = this.maxMana
    }
    
    # Template method for combat
    fn attack(target: GameCharacter) {
        if (!this.isAlive) {
            println(this.name + " is dead and cannot attack")
            return
        }
        
        println(this.name + " attacks " + target.name + "!")
        let damage = this.calculateDamage()
        target.takeDamage(damage)
        
        # Check for special ability
        if (this.canUseSpecialAbility()) {
            this.useSpecialAbility(target)
        }
    }
    
    fn takeDamage(amount: int) {
        this.health = this.health - amount
        println(this.name + " takes " + str(amount) + " damage! Health: " + str(this.health) + "/" + str(this.maxHealth))
        
        if (this.health <= 0) {
            this.health = 0
            this.isAlive = false
            println(this.name + " has been defeated!")
        }
    }
    
    fn heal(amount: int) {
        if (!this.isAlive) {
            return
        }
        
        this.health = this.health + amount
        if (this.health > this.maxHealth) {
            this.health = this.maxHealth
        }
        println(this.name + " heals " + str(amount) + " HP! Health: " + str(this.health) + "/" + str(this.maxHealth))
    }
    
    fn displayStats() {
        println("=== " + this.name + " (" + this.getClassName() + ") ===")
        println("Level: " + str(this.level))
        println("Health: " + str(this.health) + "/" + str(this.maxHealth))
        println("Mana: " + str(this.mana) + "/" + str(this.maxMana))
        println("Status: " + (this.isAlive ? "Alive" : "Dead"))
    }
    
    # Abstract methods
    abstract fn getClassName() -> string
    abstract fn getBaseHealth() -> int
    abstract fn getBaseMana() -> int
    abstract fn calculateDamage() -> int
    abstract fn canUseSpecialAbility() -> bool
    abstract fn useSpecialAbility(target: GameCharacter)
}

class Warrior extends GameCharacter {
    let rage: int = 0
    let maxRage: int = 100
    
    fn init(n: string) {
        super.init(n)
    }
    
    fn getClassName() -> string {
        return "Warrior"
    }
    
    fn getBaseHealth() -> int {
        return 150
    }
    
    fn getBaseMana() -> int {
        return 20
    }
    
    fn calculateDamage() -> int {
        let baseDamage = 25
        let rageDamage = this.rage / 10
        this.rage = this.rage + 10
        if (this.rage > this.maxRage) {
            this.rage = this.maxRage
        }
        return baseDamage + rageDamage
    }
    
    fn canUseSpecialAbility() -> bool {
        return this.rage >= 50
    }
    
    fn useSpecialAbility(target: GameCharacter) {
        println(this.name + " uses BERSERKER RAGE!")
        this.rage = this.rage - 50
        target.takeDamage(40)
    }
}

class Mage extends GameCharacter {
    let spellPower: int = 30
    
    fn init(n: string) {
        super.init(n)
    }
    
    fn getClassName() -> string {
        return "Mage"
    }
    
    fn getBaseHealth() -> int {
        return 80
    }
    
    fn getBaseMana() -> int {
        return 100
    }
    
    fn calculateDamage() -> int {
        return 15 + (this.spellPower / 2)
    }
    
    fn canUseSpecialAbility() -> bool {
        return this.mana >= 30
    }
    
    fn useSpecialAbility(target: GameCharacter) {
        println(this.name + " casts FIREBALL!")
        this.mana = this.mana - 30
        target.takeDamage(50)
    }
}

class Archer extends GameCharacter {
    let arrows: int = 20
    let critChance: int = 25
    
    fn init(n: string) {
        super.init(n)
    }
    
    fn getClassName() -> string {
        return "Archer"
    }
    
    fn getBaseHealth() -> int {
        return 100
    }
    
    fn getBaseMana() -> int {
        return 40
    }
    
    fn calculateDamage() -> int {
        if (this.arrows > 0) {
            this.arrows = this.arrows - 1
            return 20
        }
        println(this.name + " is out of arrows!")
        return 5  # Melee attack
    }
    
    fn canUseSpecialAbility() -> bool {
        return this.arrows >= 3
    }
    
    fn useSpecialAbility(target: GameCharacter) {
        println(this.name + " uses MULTI-SHOT!")
        this.arrows = this.arrows - 3
        target.takeDamage(60)
    }
}

# Usage
let warrior = new Warrior("Conan")
let mage = new Mage("Gandalf")
let archer = new Archer("Legolas")

warrior.displayStats()
mage.displayStats()
archer.displayStats()

println("\n=== Battle Begins! ===\n")

warrior.attack(mage)
mage.attack(warrior)
archer.attack(warrior)

warrior.attack(mage)
mage.attack(warrior)
```

### Problem 6: Cache Implementation

**Question**: Create an abstract cache class with get, set, and eviction policies. Implement LRU and LFU caches.

**Solution**:

```klang
abstract class Cache {
    let capacity: int = 0
    let currentSize: int = 0
    let hits: int = 0
    let misses: int = 0
    
    fn init(cap: int) {
        this.capacity = cap
        this.currentSize = 0
    }
    
    # Template method
    fn get(key: string) -> string {
        let value = this.retrieve(key)
        
        if (value != "") {
            this.hits = this.hits + 1
            this.onCacheHit(key)
            println("[HIT] Key: " + key + " = " + value)
        } else {
            this.misses = this.misses + 1
            println("[MISS] Key: " + key)
        }
        
        return value
    }
    
    fn put(key: string, value: string) {
        if (this.currentSize >= this.capacity) {
            let evicted = this.evict()
            println("[EVICT] Removed: " + evicted)
        }
        
        this.store(key, value)
        this.currentSize = this.currentSize + 1
        println("[PUT] Key: " + key + " = " + value)
    }
    
    fn displayStats() {
        println("=== Cache Statistics ===")
        println("Type: " + this.getCacheType())
        println("Capacity: " + str(this.capacity))
        println("Current Size: " + str(this.currentSize))
        println("Hits: " + str(this.hits))
        println("Misses: " + str(this.misses))
        let hitRate = this.calculateHitRate()
        println("Hit Rate: " + str(hitRate) + "%")
    }
    
    fn calculateHitRate() -> float {
        let total = this.hits + this.misses
        if (total == 0) {
            return 0.0
        }
        return (float(this.hits) / float(total)) * 100.0
    }
    
    # Abstract methods
    abstract fn retrieve(key: string) -> string
    abstract fn store(key: string, value: string)
    abstract fn evict() -> string
    abstract fn onCacheHit(key: string)
    abstract fn getCacheType() -> string
}

class LRUCache extends Cache {
    let data: map<string, string> = {}
    let accessOrder: array<string> = []
    
    fn init(cap: int) {
        super.init(cap)
    }
    
    fn retrieve(key: string) -> string {
        if (this.data.has(key)) {
            return this.data.get(key)
        }
        return ""
    }
    
    fn store(key: string, value: string) {
        this.data.set(key, value)
        this.accessOrder.push(key)
    }
    
    fn evict() -> string {
        # Remove least recently used (first in list)
        if (this.accessOrder.length() > 0) {
            let key = this.accessOrder[0]
            this.accessOrder.remove(0)
            this.data.remove(key)
            this.currentSize = this.currentSize - 1
            return key
        }
        return ""
    }
    
    fn onCacheHit(key: string) {
        # Move to end (most recently used)
        this.removeFromOrder(key)
        this.accessOrder.push(key)
    }
    
    fn removeFromOrder(key: string) {
        for (let i = 0; i < this.accessOrder.length(); i = i + 1) {
            if (this.accessOrder[i] == key) {
                this.accessOrder.remove(i)
                break
            }
        }
    }
    
    fn getCacheType() -> string {
        return "LRU (Least Recently Used)"
    }
}

class LFUCache extends Cache {
    let data: map<string, string> = {}
    let frequency: map<string, int> = {}
    
    fn init(cap: int) {
        super.init(cap)
    }
    
    fn retrieve(key: string) -> string {
        if (this.data.has(key)) {
            return this.data.get(key)
        }
        return ""
    }
    
    fn store(key: string, value: string) {
        this.data.set(key, value)
        this.frequency.set(key, 0)
    }
    
    fn evict() -> string {
        # Remove least frequently used
        let minFreq = 999999
        let evictKey = ""
        
        for (let key in this.frequency.keys()) {
            let freq = this.frequency.get(key)
            if (freq < minFreq) {
                minFreq = freq
                evictKey = key
            }
        }
        
        if (evictKey != "") {
            this.data.remove(evictKey)
            this.frequency.remove(evictKey)
            this.currentSize = this.currentSize - 1
        }
        
        return evictKey
    }
    
    fn onCacheHit(key: string) {
        # Increment frequency
        let freq = this.frequency.get(key)
        this.frequency.set(key, freq + 1)
    }
    
    fn getCacheType() -> string {
        return "LFU (Least Frequently Used)"
    }
}

# Usage
let lruCache = new LRUCache(3)
lruCache.put("a", "value_a")
lruCache.put("b", "value_b")
lruCache.put("c", "value_c")
lruCache.get("a")
lruCache.get("b")
lruCache.put("d", "value_d")  # Evicts "c" (least recently used)
lruCache.displayStats()

let lfuCache = new LFUCache(3)
lfuCache.put("x", "value_x")
lfuCache.put("y", "value_y")
lfuCache.put("z", "value_z")
lfuCache.get("x")
lfuCache.get("x")
lfuCache.get("y")
lfuCache.put("w", "value_w")  # Evicts "z" (least frequently used)
lfuCache.displayStats()
```

---

## Summary

- **Abstract classes** are incomplete classes that serve as blueprints
- Use `abstract` keyword to declare abstract classes and methods
- **Cannot instantiate** abstract classes directly
- **Abstract methods** have no implementation - children must implement them
- **Concrete methods** in abstract classes provide shared functionality
- Child classes must implement **all abstract methods** unless also abstract
- Use for **"is-a" relationships** with shared code and partial implementations
- **Template Method Pattern** uses abstract classes to define algorithm structure
- Abstract classes can have **state** (properties) unlike interfaces
- Choose abstract classes over interfaces when sharing implementation is important

---

## Next Steps

Continue to:
- [Interfaces](08_interfaces.md)
- [Polymorphism Deep Dive](06_polymorphism.md)
- [Design Patterns](../patterns/README.md)

Happy coding! 🚀
