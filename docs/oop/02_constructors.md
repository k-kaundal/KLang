# Constructors and Object Initialization in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What is a Constructor?](#what-is-a-constructor)
3. [The `init` Method](#the-init-method)
4. [Constructor Parameters](#constructor-parameters)
5. [Default Parameters in Constructors](#default-parameters-in-constructors)
6. [Best Practices](#best-practices)
7. [Common Patterns](#common-patterns)
8. [Examples](#examples)

---

## Introduction

While classes define blueprints for objects, **constructors** provide a way to initialize objects with specific values when they are created. In KLang, constructors are defined using the special `init` method.

### Why Use Constructors?

- **Initialize properties** with specific values
- **Validate input** before setting properties
- **Set up object state** in one place
- **Make object creation** more convenient

---

## What is a Constructor?

A **constructor** is a special method that is automatically called when you create a new object using the `new` keyword. It's used to set up the initial state of the object.

### Without Constructor (Manual Initialization)

```klang
class Person {
    let name: string = ""
    let age: int = 0
}

let person = new Person()
person.name = "Alice"    # Manual initialization
person.age = 30          # Manual initialization
```

### With Constructor (Automatic Initialization)

```klang
class Person {
    let name: string = ""
    let age: int = 0
    
    fn init(n: string, a: int) {
        this.name = n
        this.age = a
    }
}

let person = new Person("Alice", 30)  # Initialized automatically!
```

---

## The `init` Method

The `init` method is KLang's constructor method. It:
- Is called automatically when `new ClassName(...)` is used
- Takes parameters to initialize the object
- Uses `this` to refer to the current object
- Does not have a return type

### Basic Syntax

```klang
class ClassName {
    let property: type = defaultValue
    
    fn init(param1: type1, param2: type2) {
        this.property = param1
        # More initialization code
    }
}
```

### Example

```klang
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

# Create rectangle with constructor
let rect = new Rectangle(10.0, 5.0)
println("Area: " + str(rect.area()))  # Output: Area: 50
```

---

## Constructor Parameters

Constructors can take any number of parameters to initialize object properties.

### No Parameters

```klang
class Counter {
    let count: int = 0
    
    fn init() {
        this.count = 0  # Explicit initialization
        println("Counter created!")
    }
}

let counter = new Counter()  # Output: Counter created!
```

### One Parameter

```klang
class Circle {
    let radius: float = 1.0
    
    fn init(r: float) {
        this.radius = r
    }
}

let circle = new Circle(5.0)
```

### Multiple Parameters

```klang
class Student {
    let name: string = ""
    let id: int = 0
    let grade: float = 0.0
    
    fn init(n: string, i: int, g: float) {
        this.name = n
        this.id = i
        this.grade = g
    }
}

let student = new Student("Bob", 12345, 85.5)
```

### Parameter Validation

```klang
class BankAccount {
    let balance: float = 0.0
    let accountNumber: int = 0
    
    fn init(accNum: int, initialBalance: float) {
        this.accountNumber = accNum
        
        # Validate balance
        if (initialBalance >= 0.0) {
            this.balance = initialBalance
        } else {
            this.balance = 0.0
            println("Warning: Negative balance not allowed")
        }
    }
}

let account1 = new BankAccount(12345, 1000.0)  # Valid
let account2 = new BankAccount(12346, -500.0)  # Warning printed
```

---

## Default Parameters in Constructors

KLang supports default parameter values in constructors, making them more flexible.

### Basic Default Parameters

```klang
class Book {
    let title: string = ""
    let author: string = ""
    let pages: int = 0
    
    fn init(t: string, a: string = "Unknown", p: int = 0) {
        this.title = t
        this.author = a
        this.pages = p
    }
}

# Different ways to create books
let book1 = new Book("1984", "George Orwell", 328)
let book2 = new Book("Mystery Book", "Unknown", 200)
let book3 = new Book("New Release")  # Uses defaults for author and pages
```

### All Parameters with Defaults

```klang
class Point {
    let x: float = 0.0
    let y: float = 0.0
    
    fn init(xVal: float = 0.0, yVal: float = 0.0) {
        this.x = xVal
        this.y = yVal
    }
}

let origin = new Point()              # (0, 0)
let p1 = new Point(5.0)               # (5, 0)
let p2 = new Point(3.0, 4.0)          # (3, 4)
```

---

## Best Practices

### 1. Initialize All Properties

```klang
# Good: All properties initialized in constructor
class User {
    let username: string = ""
    let email: string = ""
    let age: int = 0
    
    fn init(user: string, mail: string, userAge: int) {
        this.username = user
        this.email = mail
        this.age = userAge
    }
}
```

### 2. Validate Input

```klang
class Product {
    let name: string = ""
    let price: float = 0.0
    let quantity: int = 0
    
    fn init(n: string, p: float, q: int) {
        this.name = n
        
        # Validate price
        if (p >= 0.0) {
            this.price = p
        } else {
            this.price = 0.0
        }
        
        # Validate quantity
        if (q >= 0) {
            this.quantity = q
        } else {
            this.quantity = 0
        }
    }
}
```

### 3. Use Clear Parameter Names

```klang
# Good: Clear parameter names
fn init(customerName: string, accountBalance: float) { }

# Bad: Unclear parameter names
fn init(n: string, b: float) { }
```

### 4. Provide Sensible Defaults

```klang
class Configuration {
    let timeout: int = 30
    let retries: int = 3
    let debug: bool = false
    
    fn init(t: int = 30, r: int = 3, d: bool = false) {
        this.timeout = t
        this.retries = r
        this.debug = d
    }
}

let defaultConfig = new Configuration()
let customConfig = new Configuration(60, 5, true)
```

---

## Common Patterns

### Pattern 1: Builder-Style Initialization

```klang
class Config {
    let host: string = "localhost"
    let port: int = 8080
    let ssl: bool = false
    
    fn init(h: string = "localhost", p: int = 8080, s: bool = false) {
        this.host = h
        this.port = p
        this.ssl = s
    }
}
```

### Pattern 2: Copy Constructor Pattern

```klang
class Point {
    let x: float = 0.0
    let y: float = 0.0
    
    fn init(xVal: float = 0.0, yVal: float = 0.0) {
        this.x = xVal
        this.y = yVal
    }
    
    fn copy() -> Point {
        return new Point(this.x, this.y)
    }
}

let p1 = new Point(10.0, 20.0)
let p2 = p1.copy()  # Create a copy
```

### Pattern 3: Factory Method Pattern

```klang
class Temperature {
    let celsius: float = 0.0
    
    fn init(c: float) {
        this.celsius = c
    }
    
    fn fromFahrenheit(f: float) -> Temperature {
        let c = (f - 32.0) * 5.0 / 9.0
        return new Temperature(c)
    }
    
    fn fromKelvin(k: float) -> Temperature {
        let c = k - 273.15
        return new Temperature(c)
    }
}

let temp1 = new Temperature(25.0)
# Note: fromFahrenheit would need to be static in real implementation
```

---

## Examples

### Example 1: Person Class

```klang
class Person {
    let firstName: string = ""
    let lastName: string = ""
    let age: int = 0
    
    fn init(first: string, last: string, personAge: int = 0) {
        this.firstName = first
        this.lastName = last
        this.age = personAge
    }
    
    fn getFullName() -> string {
        return this.firstName + " " + this.lastName
    }
    
    fn greet() {
        println("Hello, I'm " + this.getFullName())
    }
}

# Usage
let person1 = new Person("John", "Doe", 30)
person1.greet()  # Output: Hello, I'm John Doe

let person2 = new Person("Jane", "Smith")  # age defaults to 0
person2.greet()  # Output: Hello, I'm Jane Smith
```

### Example 2: Rectangle Class

```klang
class Rectangle {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(w: float, h: float) {
        if (w > 0.0 && h > 0.0) {
            this.width = w
            this.height = h
        } else {
            this.width = 1.0
            this.height = 1.0
            println("Invalid dimensions, using 1x1")
        }
    }
    
    fn area() -> float {
        return this.width * this.height
    }
    
    fn perimeter() -> float {
        return 2.0 * (this.width + this.height)
    }
}

# Usage
let rect1 = new Rectangle(10.0, 5.0)
println("Area: " + str(rect1.area()))

let rect2 = new Rectangle(-5.0, 10.0)  # Invalid, will use 1x1
```

### Example 3: Bank Account

```klang
class BankAccount {
    let accountNumber: int = 0
    let owner: string = ""
    let balance: float = 0.0
    
    fn init(accNum: int, ownerName: string, initialBalance: float = 0.0) {
        this.accountNumber = accNum
        this.owner = ownerName
        
        if (initialBalance >= 0.0) {
            this.balance = initialBalance
        } else {
            this.balance = 0.0
            println("Warning: Initial balance cannot be negative")
        }
    }
    
    fn displayInfo() {
        println("Account: " + str(this.accountNumber))
        println("Owner: " + this.owner)
        println("Balance: $" + str(this.balance))
    }
}

# Usage
let account1 = new BankAccount(12345, "Alice", 1000.0)
let account2 = new BankAccount(12346, "Bob")  # 0 balance
account1.displayInfo()
```

---

## Summary

- **Constructors** initialize objects when they are created
- Use the `init` method to define constructors
- Constructors can have parameters with default values
- Always validate input in constructors
- Use `this` to assign values to properties
- Constructors make object creation more convenient and safer

---

## Next Steps

Continue to:
- [Inheritance](04_inheritance.md)
- [Method Overriding](05_method_overriding.md)
- [The `super` Keyword](06_super_keyword.md)

Happy coding! 🚀
