# Method Overriding in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What is Method Overriding?](#what-is-method-overriding)
3. [Basic Syntax](#basic-syntax)
4. [Rules of Overriding](#rules-of-overriding)
5. [Override vs Overload](#override-vs-overload)
6. [Using Super](#using-super)
7. [Practical Examples](#practical-examples)
8. [Common Use Cases](#common-use-cases)
9. [Best Practices](#best-practices)
10. [Common Mistakes](#common-mistakes)
11. [Interview Problems](#interview-problems)

---

## Introduction

**Method Overriding** is a powerful OOP feature that allows a child class to provide its own implementation of a method that is already defined in its parent class. This enables **polymorphic behavior**, where the same method call can behave differently depending on the object type.

### Why Method Overriding?

- **Polymorphism**: Different behavior for different object types
- **Specialization**: Child classes can customize parent behavior
- **Flexibility**: Extend and modify inherited functionality
- **Clean Design**: Maintain consistent interfaces across hierarchy

### Key Concept

```
Parent Class defines general behavior
        ↓
Child Class overrides with specific behavior
        ↓
Same method name, different implementation
```

---

## What is Method Overriding?

Method overriding occurs when a child class provides a **new implementation** for a method that already exists in the parent class.

### Visual Example

```
┌─────────────────┐
│   Animal        │
│  - speak()      │ ← Original method
└─────────────────┘
         ↑
         │ extends
         │
┌─────────────────┐
│   Dog           │
│  - speak()      │ ← Overridden method (different implementation)
└─────────────────┘
```

### Simple Example

```klang
class Animal {
    fn speak() {
        println("Some generic sound")
    }
}

class Dog extends Animal {
    # Override speak() method
    fn speak() {
        println("Woof! Woof!")
    }
}

let animal = new Animal()
let dog = new Dog()

animal.speak()  # Output: Some generic sound
dog.speak()     # Output: Woof! Woof!
```

---

## Basic Syntax

### Override Pattern

```klang
class ParentClass {
    fn methodName(params) -> returnType {
        # Parent implementation
    }
}

class ChildClass extends ParentClass {
    # Override by redefining the method
    fn methodName(params) -> returnType {
        # Child implementation
    }
}
```

### Complete Example

```klang
class Shape {
    let name: string = "Shape"
    
    fn area() -> float {
        return 0.0  # Default implementation
    }
    
    fn describe() {
        println("This is a shape")
    }
}

class Rectangle extends Shape {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(w: float, h: float) {
        this.name = "Rectangle"
        this.width = w
        this.height = h
    }
    
    # Override area method
    fn area() -> float {
        return this.width * this.height
    }
    
    # Override describe method
    fn describe() {
        println("Rectangle with width " + str(this.width) + " and height " + str(this.height))
    }
}

let rect = new Rectangle(5.0, 3.0)
println("Area: " + str(rect.area()))  # Output: Area: 15
rect.describe()  # Output: Rectangle with width 5 and height 3
```

---

## Rules of Overriding

### Rule 1: Same Method Name

The overriding method **must** have the same name as the parent method.

```klang
class Parent {
    fn calculate() -> int {
        return 10
    }
}

class Child extends Parent {
    # ✓ Correct: Same name
    fn calculate() -> int {
        return 20
    }
    
    # ✗ Wrong: Different name (this is a new method, not an override)
    # fn compute() -> int {
    #     return 20
    # }
}
```

### Rule 2: Same Parameters

The overriding method must have the **same parameter list** (type and order).

```klang
class Calculator {
    fn add(a: int, b: int) -> int {
        return a + b
    }
}

class ScientificCalculator extends Calculator {
    # ✓ Correct: Same parameters
    fn add(a: int, b: int) -> int {
        println("Scientific add")
        return a + b
    }
    
    # ✗ Wrong: Different parameters (this is overloading, not overriding)
    # fn add(a: float, b: float) -> float {
    #     return a + b
    # }
}
```

### Rule 3: Compatible Return Type

The overriding method should have the **same or compatible** return type.

```klang
class DataSource {
    fn getData() -> string {
        return "data"
    }
    
    fn getCount() -> int {
        return 0
    }
}

class DatabaseSource extends DataSource {
    # ✓ Correct: Same return type
    fn getData() -> string {
        return "database data"
    }
    
    # ✓ Correct: Same return type
    fn getCount() -> int {
        return 100
    }
    
    # ✗ Wrong: Different return type
    # fn getData() -> int {
    #     return 42
    # }
}
```

### Rule 4: Access Modifiers

The overriding method **cannot be more restrictive** than the parent method.

```klang
class Parent {
    # Public method
    fn publicMethod() {
        println("Public in parent")
    }
}

class Child extends Parent {
    # ✓ Correct: Same or less restrictive access
    fn publicMethod() {
        println("Public in child")
    }
    
    # ✗ Wrong: Cannot make it private (more restrictive)
    # private fn publicMethod() {
    #     println("Private in child")
    # }
}
```

---

## Override vs Overload

**Method Overriding** and **Method Overloading** are different concepts:

### Method Overriding

- **Same class hierarchy** (parent-child)
- **Same method signature** (name + parameters)
- **Different implementation**
- **Runtime polymorphism**

```klang
class Parent {
    fn display() {
        println("Parent display")
    }
}

class Child extends Parent {
    # Overriding: Same signature, different implementation
    fn display() {
        println("Child display")
    }
}
```

### Method Overloading

- **Same class** (or hierarchy)
- **Same method name**
- **Different parameters**
- **Compile-time polymorphism**

```klang
class Calculator {
    # Overloading: Same name, different parameters
    fn add(a: int, b: int) -> int {
        return a + b
    }
    
    fn add(a: float, b: float) -> float {
        return a + b
    }
    
    fn add(a: int, b: int, c: int) -> int {
        return a + b + c
    }
}
```

### Comparison Table

| Feature | Overriding | Overloading |
|---------|-----------|-------------|
| Classes | Parent & Child | Same class |
| Method Name | Same | Same |
| Parameters | Same | Different |
| Return Type | Same/Compatible | Can differ |
| Purpose | Specialize behavior | Provide options |
| Binding | Runtime | Compile-time |

---

## Using Super

The `super` keyword allows you to call the parent class's version of an overridden method.

### Basic Super Usage

```klang
class Vehicle {
    let brand: string = ""
    
    fn init(b: string) {
        this.brand = b
    }
    
    fn start() {
        println("Vehicle " + this.brand + " starting...")
    }
}

class Car extends Vehicle {
    fn init(b: string) {
        super.init(b)
    }
    
    fn start() {
        super.start()  # Call parent method
        println("Engine ignited")
        println("Car is ready to drive")
    }
}

let car = new Car("Toyota")
car.start()
# Output:
# Vehicle Toyota starting...
# Engine ignited
# Car is ready to drive
```

### Extending Parent Behavior

Use `super` when you want to **extend** (not replace) parent functionality:

```klang
class Logger {
    fn log(message: string) {
        println("[LOG] " + message)
    }
}

class TimestampLogger extends Logger {
    fn getTimestamp() -> string {
        return "2024-01-15 10:30:00"
    }
    
    fn log(message: string) {
        let timestamp = this.getTimestamp()
        # Extend parent functionality
        super.log(timestamp + " - " + message)
    }
}

let logger = new TimestampLogger()
logger.log("System started")
# Output: [LOG] 2024-01-15 10:30:00 - System started
```

### When to Use Super

1. **Always in constructors**: `super.init()` to initialize parent state
2. **Extend behavior**: Keep parent functionality and add more
3. **Partial override**: Use parent logic in some cases
4. **Call parent validation**: Reuse parent checks

```klang
class Employee {
    let name: string = ""
    let salary: float = 0.0
    
    fn init(n: string, s: float) {
        this.name = n
        this.salary = s
    }
    
    fn validate() -> bool {
        if this.name == "" {
            return false
        }
        if this.salary <= 0.0 {
            return false
        }
        return true
    }
}

class Manager extends Employee {
    let department: string = ""
    
    fn init(n: string, s: float, dept: string) {
        super.init(n, s)
        this.department = dept
    }
    
    fn validate() -> bool {
        # Use parent validation first
        if !super.validate() {
            return false
        }
        # Add additional validation
        if this.department == "" {
            return false
        }
        return true
    }
}
```

---

## Practical Examples

### Example 1: Shape Hierarchy

```klang
class Shape {
    let color: string = "black"
    
    fn init(c: string) {
        this.color = c
    }
    
    fn area() -> float {
        return 0.0  # Default implementation
    }
    
    fn perimeter() -> float {
        return 0.0  # Default implementation
    }
    
    fn displayInfo() {
        println("Shape color: " + this.color)
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
    
    # Override area
    fn area() -> float {
        return this.width * this.height
    }
    
    # Override perimeter
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
    
    # Override area
    fn area() -> float {
        return 3.14159 * this.radius * this.radius
    }
    
    # Override perimeter
    fn perimeter() -> float {
        return 2.0 * 3.14159 * this.radius
    }
}

class Triangle extends Shape {
    let base: float = 0.0
    let height: float = 0.0
    let side1: float = 0.0
    let side2: float = 0.0
    let side3: float = 0.0
    
    fn init(c: string, b: float, h: float, s1: float, s2: float, s3: float) {
        super.init(c)
        this.base = b
        this.height = h
        this.side1 = s1
        this.side2 = s2
        this.side3 = s3
    }
    
    # Override area
    fn area() -> float {
        return 0.5 * this.base * this.height
    }
    
    # Override perimeter
    fn perimeter() -> float {
        return this.side1 + this.side2 + this.side3
    }
}

# Usage
let rectangle = new Rectangle("blue", 5.0, 3.0)
rectangle.displayInfo()
# Output:
# Shape color: blue
# Area: 15
# Perimeter: 16

let circle = new Circle("red", 4.0)
circle.displayInfo()
# Output:
# Shape color: red
# Area: 50.26544
# Perimeter: 25.13272

let triangle = new Triangle("green", 6.0, 4.0, 3.0, 4.0, 5.0)
triangle.displayInfo()
# Output:
# Shape color: green
# Area: 12
# Perimeter: 12
```

### Example 2: Vehicle Hierarchy

```klang
class Vehicle {
    let brand: string = ""
    let model: string = ""
    let isRunning: bool = false
    
    fn init(b: string, m: string) {
        this.brand = b
        this.model = m
    }
    
    fn start() {
        if this.isRunning {
            println("Already running")
            return
        }
        this.isRunning = true
        println("Vehicle started")
    }
    
    fn stop() {
        if !this.isRunning {
            println("Already stopped")
            return
        }
        this.isRunning = false
        println("Vehicle stopped")
    }
    
    fn honk() {
        println("Beep!")
    }
}

class Car extends Vehicle {
    let doors: int = 4
    
    fn init(b: string, m: string, d: int) {
        super.init(b, m)
        this.doors = d
    }
    
    # Override start
    fn start() {
        println("Unlocking car doors...")
        println("Checking seatbelts...")
        super.start()
        println("Ready to drive!")
    }
    
    # Override stop
    fn stop() {
        println("Applying brakes...")
        super.stop()
        println("Parking brake engaged")
        println("Locking doors...")
    }
    
    # Override honk
    fn honk() {
        println("Beep Beep! (Car horn)")
    }
}

class Motorcycle extends Vehicle {
    let hasHelmet: bool = false
    
    fn init(b: string, m: string) {
        super.init(b, m)
    }
    
    # Override start
    fn start() {
        if !this.hasHelmet {
            println("Please put on helmet first!")
            return
        }
        println("Kickstarting motorcycle...")
        super.start()
        println("Vroom vroom!")
    }
    
    # Override stop
    fn stop() {
        println("Slowing down...")
        super.stop()
        println("Putting down kickstand")
    }
    
    fn putHelmet() {
        this.hasHelmet = true
        println("Helmet on - safety first!")
    }
}

class Truck extends Vehicle {
    let cargoWeight: float = 0.0
    
    fn init(b: string, m: string) {
        super.init(b, m)
    }
    
    # Override start
    fn start() {
        if this.cargoWeight > 1000.0 {
            println("WARNING: Heavy load!")
        }
        println("Starting truck engine...")
        super.start()
        println("Truck ready for hauling")
    }
    
    fn loadCargo(weight: float) {
        this.cargoWeight = this.cargoWeight + weight
        println("Loaded " + str(weight) + " kg. Total: " + str(this.cargoWeight) + " kg")
    }
}

# Usage
let car = new Car("Toyota", "Camry", 4)
car.start()
println("")

let bike = new Motorcycle("Honda", "CBR")
bike.start()  # Won't start without helmet
bike.putHelmet()
bike.start()  # Now starts
println("")

let truck = new Truck("Ford", "F-150")
truck.loadCargo(800.0)
truck.start()
```

### Example 3: Employee Hierarchy with Salary Calculation

```klang
class Employee {
    let name: string = ""
    let id: int = 0
    let baseSalary: float = 0.0
    let yearsOfService: int = 0
    
    fn init(n: string, empId: int, salary: float, years: int) {
        this.name = n
        this.id = empId
        this.baseSalary = salary
        this.yearsOfService = years
    }
    
    fn calculateSalary() -> float {
        # Base implementation: just return base salary
        return this.baseSalary
    }
    
    fn displayInfo() {
        println("Employee: " + this.name)
        println("ID: " + str(this.id))
        println("Years of Service: " + str(this.yearsOfService))
        println("Total Salary: $" + str(this.calculateSalary()))
    }
}

class Manager extends Employee {
    let bonus: float = 0.0
    let teamSize: int = 0
    
    fn init(n: string, empId: int, salary: float, years: int, b: float, team: int) {
        super.init(n, empId, salary, years)
        this.bonus = b
        this.teamSize = team
    }
    
    # Override calculateSalary
    fn calculateSalary() -> float {
        let basePay = super.calculateSalary()
        let teamBonus = float(this.teamSize) * 500.0
        return basePay + this.bonus + teamBonus
    }
}

class Developer extends Employee {
    let overtimeHours: float = 0.0
    let hourlyRate: float = 50.0
    let projectBonus: float = 0.0
    
    fn init(n: string, empId: int, salary: float, years: int) {
        super.init(n, empId, salary, years)
    }
    
    fn addOvertime(hours: float) {
        this.overtimeHours = this.overtimeHours + hours
    }
    
    fn addProjectBonus(bonus: float) {
        this.projectBonus = this.projectBonus + bonus
    }
    
    # Override calculateSalary
    fn calculateSalary() -> float {
        let basePay = super.calculateSalary()
        let overtimePay = this.overtimeHours * this.hourlyRate
        let loyaltyBonus = float(this.yearsOfService) * 1000.0
        return basePay + overtimePay + this.projectBonus + loyaltyBonus
    }
}

class Intern extends Employee {
    let stipend: float = 1000.0
    let mentor: string = ""
    
    fn init(n: string, empId: int, mentorName: string) {
        super.init(n, empId, 0.0, 0)
        this.mentor = mentorName
    }
    
    # Override calculateSalary
    fn calculateSalary() -> float {
        # Interns get only stipend
        return this.stipend
    }
    
    # Override displayInfo
    fn displayInfo() {
        super.displayInfo()
        println("Mentor: " + this.mentor)
    }
}

# Usage
let emp = new Employee("John Doe", 1001, 50000.0, 3)
emp.displayInfo()
println("---")

let mgr = new Manager("Alice Smith", 2001, 80000.0, 5, 15000.0, 8)
mgr.displayInfo()
println("---")

let dev = new Developer("Bob Johnson", 3001, 70000.0, 4)
dev.addOvertime(20.0)
dev.addProjectBonus(5000.0)
dev.displayInfo()
println("---")

let intern = new Intern("Charlie Brown", 4001, "Alice Smith")
intern.displayInfo()
```

---

## Common Use Cases

### Use Case 1: toString() / display() Methods

Override to provide custom string representation:

```klang
class Object {
    fn toString() -> string {
        return "Object"
    }
}

class Person extends Object {
    let name: string = ""
    let age: int = 0
    
    fn init(n: string, a: int) {
        this.name = n
        this.age = a
    }
    
    # Override toString
    fn toString() -> string {
        return "Person(name=" + this.name + ", age=" + str(this.age) + ")"
    }
}

class Student extends Person {
    let grade: string = ""
    
    fn init(n: string, a: int, g: string) {
        super.init(n, a)
        this.grade = g
    }
    
    # Override toString
    fn toString() -> string {
        return "Student(name=" + this.name + ", age=" + str(this.age) + ", grade=" + this.grade + ")"
    }
}

let person = new Person("Alice", 30)
println(person.toString())  # Output: Person(name=Alice, age=30)

let student = new Student("Bob", 20, "A")
println(student.toString())  # Output: Student(name=Bob, age=20, grade=A)
```

### Use Case 2: Equality Checking

Override equals() method for custom comparison:

```klang
class Point {
    let x: int = 0
    let y: int = 0
    
    fn init(x1: int, y1: int) {
        this.x = x1
        this.y = y1
    }
    
    fn equals(other: Point) -> bool {
        return this.x == other.x && this.y == other.y
    }
}

class Point3D extends Point {
    let z: int = 0
    
    fn init(x1: int, y1: int, z1: int) {
        super.init(x1, y1)
        this.z = z1
    }
    
    # Override equals
    fn equals(other: Point3D) -> bool {
        return this.x == other.x && this.y == other.y && this.z == other.z
    }
}

let p1 = new Point3D(1, 2, 3)
let p2 = new Point3D(1, 2, 3)
let p3 = new Point3D(1, 2, 4)

println(p1.equals(p2))  # Output: true
println(p1.equals(p3))  # Output: false
```

### Use Case 3: Validation Methods

Override validation for specific requirements:

```klang
class FormField {
    let value: string = ""
    let required: bool = false
    
    fn init(req: bool) {
        this.required = req
    }
    
    fn setValue(val: string) {
        this.value = val
    }
    
    fn validate() -> bool {
        if this.required && this.value == "" {
            return false
        }
        return true
    }
}

class EmailField extends FormField {
    fn init(req: bool) {
        super.init(req)
    }
    
    # Override validate
    fn validate() -> bool {
        # Call parent validation
        if !super.validate() {
            return false
        }
        # Add email-specific validation
        if this.value != "" {
            # Simple email check: contains @ and .
            let hasAt = false
            let hasDot = false
            let i = 0
            while i < len(this.value) {
                if charAt(this.value, i) == "@" {
                    hasAt = true
                }
                if charAt(this.value, i) == "." {
                    hasDot = true
                }
                i = i + 1
            }
            return hasAt && hasDot
        }
        return true
    }
}

class PhoneField extends FormField {
    fn init(req: bool) {
        super.init(req)
    }
    
    # Override validate
    fn validate() -> bool {
        # Call parent validation
        if !super.validate() {
            return false
        }
        # Add phone-specific validation
        if this.value != "" {
            # Check if all characters are digits or hyphens
            let i = 0
            while i < len(this.value) {
                let ch = charAt(this.value, i)
                if ch != "0" && ch != "1" && ch != "2" && ch != "3" && 
                   ch != "4" && ch != "5" && ch != "6" && ch != "7" && 
                   ch != "8" && ch != "9" && ch != "-" {
                    return false
                }
                i = i + 1
            }
        }
        return true
    }
}

# Usage
let email = new EmailField(true)
email.setValue("user@example.com")
println(email.validate())  # Output: true

email.setValue("invalid-email")
println(email.validate())  # Output: false

let phone = new PhoneField(true)
phone.setValue("555-1234")
println(phone.validate())  # Output: true

phone.setValue("555-ABCD")
println(phone.validate())  # Output: false
```

---

## Best Practices

### 1. Use Overriding for Specialized Behavior

Override when child classes need different behavior for the same conceptual operation.

```klang
# Good: Each animal speaks differently
class Animal {
    fn speak() {
        println("Some sound")
    }
}

class Dog extends Animal {
    fn speak() {
        println("Woof!")
    }
}

class Cat extends Animal {
    fn speak() {
        println("Meow!")
    }
}
```

### 2. Call Super When Extending Functionality

Use `super` when you want to keep parent behavior and add more.

```klang
class Validator {
    fn validate(input: string) -> bool {
        # Basic validation: not empty
        return input != ""
    }
}

class LengthValidator extends Validator {
    let minLength: int = 0
    
    fn init(min: int) {
        this.minLength = min
    }
    
    # Good: Extend parent validation
    fn validate(input: string) -> bool {
        if !super.validate(input) {
            return false
        }
        return len(input) >= this.minLength
    }
}
```

### 3. Maintain Liskov Substitution Principle (LSP)

**Child classes should be substitutable for parent classes** without breaking the program.

```klang
# Good: Rectangle can be used anywhere Shape is expected
class Shape {
    fn area() -> float {
        return 0.0  # Returns valid float
    }
}

class Rectangle extends Shape {
    let width: float = 5.0
    let height: float = 3.0
    
    fn area() -> float {
        return this.width * this.height  # Returns valid float
    }
}

# Function that works with any Shape
fn printArea(shape: Shape) {
    println("Area: " + str(shape.area()))
}

let shape = new Shape()
let rect = new Rectangle()

printArea(shape)  # Works
printArea(rect)   # Also works - LSP satisfied
```

### 4. Don't Override Just to Throw Errors

If a child class doesn't support a parent method, reconsider your hierarchy.

```klang
# Bad: Bird doesn't support swim()
class Animal {
    fn swim() {
        println("Swimming...")
    }
}

class Bird extends Animal {
    fn swim() {
        # Bad: Breaks LSP
        println("ERROR: Birds can't swim!")
    }
}

# Better: Separate swimming animals
class SwimmingAnimal extends Animal {
    fn swim() {
        println("Swimming...")
    }
}

class Fish extends SwimmingAnimal {
    # Fish inherits swim() naturally
}
```

### 5. Keep Method Signatures Consistent

Don't change parameter meanings in overridden methods.

```klang
# Good: Consistent parameter meaning
class PaymentProcessor {
    fn processPayment(amount: float) {
        # amount is in dollars
        println("Processing $" + str(amount))
    }
}

class InternationalProcessor extends PaymentProcessor {
    fn processPayment(amount: float) {
        # amount is still in dollars
        # Convert to local currency internally
        let localAmount = amount * 0.85  # Example conversion
        println("Processing €" + str(localAmount))
    }
}

# Bad: Changing parameter meaning
# class ConfusingProcessor extends PaymentProcessor {
#     fn processPayment(amount: float) {
#         # amount is now in cents (different meaning!)
#         println("Processing $" + str(amount / 100.0))
#     }
# }
```

### 6. Document Override Behavior

Make it clear when and why a method is overridden.

```klang
class Report {
    fn generate() {
        println("Generating basic report")
    }
}

class PDFReport extends Report {
    # Override to generate PDF format
    fn generate() {
        println("Generating PDF report")
        # PDF-specific logic here
    }
}

class ExcelReport extends Report {
    # Override to generate Excel format
    fn generate() {
        println("Generating Excel report")
        # Excel-specific logic here
    }
}
```

---

## Common Mistakes

### Mistake 1: Breaking Parent Contract

**Problem**: Child method behaves completely differently than expected.

```klang
# Bad: Breaks parent contract
class BankAccount {
    let balance: float = 0.0
    
    fn deposit(amount: float) {
        # Contract: Add money to balance
        this.balance = this.balance + amount
    }
}

class BrokenAccount extends BankAccount {
    fn deposit(amount: float) {
        # Bad: Withdraws instead of deposits!
        this.balance = this.balance - amount
    }
}

# Good: Maintains contract
class SavingsAccount extends BankAccount {
    let interestRate: float = 0.05
    
    fn deposit(amount: float) {
        # Maintains contract: adds money
        super.deposit(amount)
        # Adds interest as bonus
        let interest = amount * this.interestRate
        this.balance = this.balance + interest
    }
}
```

### Mistake 2: Not Calling Super When Needed

**Problem**: Losing important parent functionality.

```klang
# Bad: Loses parent initialization
class User {
    let username: string = ""
    let isActive: bool = false
    
    fn init(name: string) {
        this.username = name
        this.isActive = true
        # Important: Log user creation
        println("User created: " + name)
    }
}

class AdminUser extends User {
    let permissions: string = "all"
    
    fn init(name: string) {
        # Bad: Doesn't call super.init()
        # Lost: username assignment, isActive flag, logging
        this.permissions = "all"
    }
}

# Good: Calls super
class ModeratorUser extends User {
    let permissions: string = "moderate"
    
    fn init(name: string) {
        super.init(name)  # Good: Preserves parent initialization
        this.permissions = "moderate"
    }
}
```

### Mistake 3: Changing Method Signature

**Problem**: Creating a new method instead of overriding.

```klang
class Calculator {
    fn calculate(a: int, b: int) -> int {
        return a + b
    }
}

# Bad: Not actually overriding (different parameters)
class WrongCalculator extends Calculator {
    fn calculate(a: float, b: float) -> float {
        # This is a NEW method, not an override
        return a + b
    }
}

# Usage shows the problem
let calc = new WrongCalculator()
# These call DIFFERENT methods!
let result1 = calc.calculate(5, 3)        # Calls parent (int version)
let result2 = calc.calculate(5.0, 3.0)    # Calls child (float version)

# Good: Same signature
class CorrectCalculator extends Calculator {
    fn calculate(a: int, b: int) -> int {
        # Properly overrides
        println("Enhanced calculation")
        return super.calculate(a, b) * 2
    }
}
```

### Mistake 4: Ignoring Return Type

**Problem**: Returning incompatible types.

```klang
# Bad: Incompatible return type
class DataProvider {
    fn getData() -> string {
        return "data"
    }
}

# Error: Cannot change return type
# class IntDataProvider extends DataProvider {
#     fn getData() -> int {
#         return 42  # Wrong type!
#     }
# }

# Good: Same return type
class JSONDataProvider extends DataProvider {
    fn getData() -> string {
        return "{\"data\": \"value\"}"  # Still a string
    }
}
```

---

## Interview Problems

### Problem 1: What will this print?

```klang
class A {
    fn show() {
        println("A")
    }
}

class B extends A {
    fn show() {
        println("B")
        super.show()
    }
}

class C extends B {
    fn show() {
        println("C")
        super.show()
    }
}

let obj = new C()
obj.show()
```

**Answer**: 
```
C
B
A
```

**Explanation**: C.show() prints "C" and calls B.show(), which prints "B" and calls A.show(), which prints "A".

---

### Problem 2: Find the bug

```klang
class Rectangle {
    let width: float = 0.0
    let height: float = 0.0
    
    fn area() -> float {
        return this.width * this.height
    }
}

class Square extends Rectangle {
    let side: float = 0.0
    
    fn init(s: float) {
        this.side = s
    }
    
    fn area() -> float {
        return this.side * this.side
    }
}

let square = new Square(5.0)
println(square.area())  # What's the output?
```

**Answer**: Output is 25.0 (correct).

**But**: This violates LSP! Square changes width/height to be equal, but users of Rectangle might set them differently. Better to have separate hierarchies or use composition.

---

### Problem 3: Design Challenge

**Question**: Design a hierarchy for different types of bank accounts (Savings, Checking, Business) where each has different withdrawal rules:
- Savings: Max 6 withdrawals per month
- Checking: Unlimited withdrawals
- Business: Unlimited withdrawals with transaction fees

**Solution**:

```klang
class BankAccount {
    let balance: float = 0.0
    let accountNumber: string = ""
    
    fn init(accNum: string, initialBalance: float) {
        this.accountNumber = accNum
        this.balance = initialBalance
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
        println("Withdrew: $" + str(amount))
        return true
    }
    
    fn getBalance() -> float {
        return this.balance
    }
}

class SavingsAccount extends BankAccount {
    let monthlyWithdrawals: int = 0
    let maxWithdrawals: int = 6
    
    fn init(accNum: string, initialBalance: float) {
        super.init(accNum, initialBalance)
    }
    
    # Override withdraw with monthly limit
    fn withdraw(amount: float) -> bool {
        if this.monthlyWithdrawals >= this.maxWithdrawals {
            println("Monthly withdrawal limit reached")
            return false
        }
        if super.withdraw(amount) {
            this.monthlyWithdrawals = this.monthlyWithdrawals + 1
            return true
        }
        return false
    }
    
    fn resetMonthlyLimit() {
        this.monthlyWithdrawals = 0
    }
}

class CheckingAccount extends BankAccount {
    fn init(accNum: string, initialBalance: float) {
        super.init(accNum, initialBalance)
    }
    
    # No override needed - unlimited withdrawals
}

class BusinessAccount extends BankAccount {
    let transactionFee: float = 2.5
    
    fn init(accNum: string, initialBalance: float) {
        super.init(accNum, initialBalance)
    }
    
    # Override withdraw with transaction fee
    fn withdraw(amount: float) -> bool {
        let totalAmount = amount + this.transactionFee
        if totalAmount > this.balance {
            println("Insufficient funds (including $" + str(this.transactionFee) + " fee)")
            return false
        }
        this.balance = this.balance - totalAmount
        println("Withdrew: $" + str(amount) + " (Fee: $" + str(this.transactionFee) + ")")
        return true
    }
}

# Usage
let savings = new SavingsAccount("SAV001", 1000.0)
let checking = new CheckingAccount("CHK001", 1000.0)
let business = new BusinessAccount("BUS001", 1000.0)

savings.withdraw(100.0)   # Works
checking.withdraw(100.0)  # Works
business.withdraw(100.0)  # Deducts 102.5
```

---

### Problem 4: Polymorphism Challenge

**Question**: What will this print?

```klang
class Animal {
    fn makeSound() {
        println("Some sound")
    }
    
    fn describe() {
        print("Animal makes: ")
        this.makeSound()
    }
}

class Dog extends Animal {
    fn makeSound() {
        println("Woof!")
    }
}

let dog = new Dog()
dog.describe()  # What's the output?
```

**Answer**: 
```
Animal makes: Woof!
```

**Explanation**: Even though `describe()` is called from Animal class, `this.makeSound()` calls the overridden version in Dog. This is **runtime polymorphism** - the actual method called depends on the object type, not the class where the calling method is defined.

---

## Summary

### Key Takeaways

1. **Method Overriding** allows child classes to provide specific implementations
2. **Same signature** required: name, parameters, and return type
3. **Use `super`** to access parent implementation
4. **Extend, don't break** parent contracts (LSP)
5. **Override for specialization**, not just for change

### When to Override

✓ Provide specialized behavior for child classes  
✓ Customize inherited methods for specific types  
✓ Extend parent functionality with additional features  
✓ Implement polymorphic behavior  

### When NOT to Override

✗ To break parent contracts  
✗ When composition would be clearer  
✗ Just to change method signature  
✗ To throw "not supported" errors  

---

## Next Steps

Continue learning:
- [Polymorphism](05_polymorphism.md)
- [Abstract Classes](06_abstract_classes.md)
- [Interfaces](07_interfaces.md)

---

Happy coding! 🚀
