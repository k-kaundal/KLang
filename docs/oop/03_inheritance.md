# Inheritance in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What is Inheritance?](#what-is-inheritance)
3. [The `extends` Keyword](#the-extends-keyword)
4. [Basic Syntax](#basic-syntax)
5. [Inherited Members](#inherited-members)
6. [Method Overriding](#method-overriding)
7. [The `super` Keyword](#the-super-keyword)
8. [Constructor Chaining](#constructor-chaining)
9. [Types of Inheritance](#types-of-inheritance)
10. [Best Practices](#best-practices)
11. [Examples](#examples)

---

## Introduction

**Inheritance** is a fundamental OOP concept that allows a class to inherit properties and methods from another class. This promotes code reuse and establishes a natural hierarchy between classes.

### Why Use Inheritance?

- **Code Reuse**: Don't repeat common functionality
- **Hierarchy**: Model "is-a" relationships naturally
- **Extensibility**: Add new features to existing code
- **Maintainability**: Update common code in one place

---

## What is Inheritance?

Inheritance creates a parent-child relationship between classes:
- **Parent Class** (Base/Super class): The class being inherited from
- **Child Class** (Derived/Sub class): The class that inherits

### Real-World Analogy

```
Animal (Parent)
  ├── Dog (Child) - is an Animal
  ├── Cat (Child) - is an Animal
  └── Bird (Child) - is an Animal
```

---

## The `extends` Keyword

Use the `extends` keyword to create a child class that inherits from a parent class.

### Syntax

```klang
class ChildClass extends ParentClass {
    # Child-specific members
}
```

### Example

```klang
class Animal {
    let name: string = ""
    
    fn speak() {
        println("Some sound")
    }
}

class Dog extends Animal {
    # Dog inherits name and speak() from Animal
    let breed: string = ""
}

let dog = new Dog()
dog.name = "Buddy"  # Inherited property
dog.speak()          # Inherited method - Output: Some sound
```

---

## Basic Syntax

### Simple Inheritance

```klang
# Parent class
class Vehicle {
    let brand: string = ""
    let year: int = 0
    
    fn init(b: string, y: int) {
        this.brand = b
        this.year = y
    }
    
    fn displayInfo() {
        println("Brand: " + this.brand)
        println("Year: " + str(this.year))
    }
}

# Child class
class Car extends Vehicle {
    let doors: int = 4
    
    fn init(b: string, y: int, d: int) {
        super.init(b, y)  # Call parent constructor
        this.doors = d
    }
    
    fn displayInfo() {
        super.displayInfo()  # Call parent method
        println("Doors: " + str(this.doors))
    }
}

# Usage
let car = new Car("Toyota", 2024, 4)
car.displayInfo()
# Output:
# Brand: Toyota
# Year: 2024
# Doors: 4
```

---

## Inherited Members

Child classes inherit all parent properties and methods (except private members).

### Accessing Inherited Properties

```klang
class Person {
    let name: string = ""
    let age: int = 0
    
    fn init(n: string, a: int) {
        this.name = n
        this.age = a
    }
}

class Student extends Person {
    let studentId: int = 0
    let grade: float = 0.0
    
    fn init(n: string, a: int, id: int, g: float) {
        super.init(n, a)
        this.studentId = id
        this.grade = g
    }
    
    fn displayAll() {
        # Access inherited properties
        println("Name: " + this.name)
        println("Age: " + str(this.age))
        # Access own properties
        println("Student ID: " + str(this.studentId))
        println("Grade: " + str(this.grade))
    }
}
```

### Accessing Inherited Methods

```klang
class Shape {
    let color: string = "black"
    
    fn setColor(c: string) {
        this.color = c
    }
    
    fn getColor() -> string {
        return this.color
    }
}

class Circle extends Shape {
    let radius: float = 0.0
    
    fn init(r: float) {
        this.radius = r
    }
    
    fn describe() {
        # Call inherited method
        let c = this.getColor()
        println("A " + c + " circle with radius " + str(this.radius))
    }
}

let circle = new Circle(5.0)
circle.setColor("red")  # Call inherited method
circle.describe()        # Output: A red circle with radius 5
```

---

## Method Overriding

Child classes can **override** (replace) parent methods with their own implementation.

### Basic Override

```klang
class Animal {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
    }
    
    fn speak() {
        println("Some generic animal sound")
    }
}

class Dog extends Animal {
    fn init(n: string) {
        super.init(n)
    }
    
    # Override speak method
    fn speak() {
        println("Woof! Woof!")
    }
}

class Cat extends Animal {
    fn init(n: string) {
        super.init(n)
    }
    
    # Override speak method
    fn speak() {
        println("Meow!")
    }
}

let dog = new Dog("Buddy")
let cat = new Cat("Whiskers")

dog.speak()  # Output: Woof! Woof!
cat.speak()  # Output: Meow!
```

---

## The `super` Keyword

The `super` keyword accesses parent class members from the child class.

### Calling Parent Methods

```klang
class Employee {
    let name: string = ""
    let salary: float = 0.0
    
    fn init(n: string, s: float) {
        this.name = n
        this.salary = s
    }
    
    fn displayInfo() {
        println("Name: " + this.name)
        println("Salary: $" + str(this.salary))
    }
}

class Manager extends Employee {
    let department: string = ""
    
    fn init(n: string, s: float, dept: string) {
        super.init(n, s)
        this.department = dept
    }
    
    fn displayInfo() {
        super.displayInfo()  # Call parent method
        println("Department: " + this.department)
    }
}
```

### When to Use `super`

1. **Call parent constructor**: Always in child constructor
2. **Call parent method**: When extending functionality
3. **Access parent version**: When method is overridden

---

## Constructor Chaining

Child constructors should call parent constructors using `super.init()`.

### Pattern

```klang
class Parent {
    let parentProp: type = value
    
    fn init(param: type) {
        this.parentProp = param
    }
}

class Child extends Parent {
    let childProp: type = value
    
    fn init(parentParam: type, childParam: type) {
        super.init(parentParam)  # MUST call parent constructor first
        this.childProp = childParam
    }
}
```

### Multi-Level Inheritance

```klang
class Grandparent {
    let level: int = 1
    
    fn init() {
        this.level = 1
        println("Grandparent constructed")
    }
}

class Parent extends Grandparent {
    fn init() {
        super.init()
        this.level = 2
        println("Parent constructed")
    }
}

class Child extends Parent {
    fn init() {
        super.init()
        this.level = 3
        println("Child constructed")
    }
}

let child = new Child()
# Output:
# Grandparent constructed
# Parent constructed
# Child constructed
```

---

## Types of Inheritance

### 1. Single Inheritance

One child inherits from one parent.

```klang
class Parent {
    let value: int = 0
}

class Child extends Parent {
    # Inherits value from Parent
}
```

### 2. Multi-Level Inheritance

A chain of inheritance.

```klang
class A {
    let a: int = 1
}

class B extends A {
    let b: int = 2
}

class C extends B {
    let c: int = 3
    # C has a, b, and c
}
```

### 3. Hierarchical Inheritance

Multiple children inherit from one parent.

```klang
class Animal {
    fn eat() {
        println("Eating...")
    }
}

class Dog extends Animal {
    # Dog has eat()
}

class Cat extends Animal {
    # Cat has eat()
}

class Bird extends Animal {
    # Bird has eat()
}
```

---

## Best Practices

### 1. Use "Is-A" Test

Only use inheritance when child "is a" parent.

```klang
# Good: A Dog IS AN Animal
class Dog extends Animal { }

# Bad: A Car HAS AN Engine (use composition instead)
# Don't: class Car extends Engine { }
```

### 2. Always Call Parent Constructor

```klang
class Child extends Parent {
    fn init(params) {
        super.init(params)  # Always call parent constructor
        # Then initialize child-specific properties
    }
}
```

### 3. Override Meaningfully

Only override when the child needs different behavior.

```klang
class Animal {
    fn move() {
        println("Moving...")
    }
}

class Fish extends Animal {
    # Override because fish move differently
    fn move() {
        println("Swimming...")
    }
}

class Dog extends Animal {
    # Override because dogs move differently
    fn move() {
        println("Running...")
    }
}
```

### 4. Don't Break Parent Contract

Child classes should not break parent class expectations.

```klang
# Parent expects area() to return positive value
class Shape {
    fn area() -> float {
        return 0.0  # Placeholder
    }
}

class Rectangle extends Shape {
    let width: float = 0.0
    let height: float = 0.0
    
    # Good: Returns positive value
    fn area() -> float {
        return this.width * this.height
    }
}
```

---

## Examples

### Example 1: Vehicle Hierarchy

```klang
class Vehicle {
    let brand: string = ""
    let speed: float = 0.0
    
    fn init(b: string) {
        this.brand = b
    }
    
    fn accelerate(amount: float) {
        this.speed = this.speed + amount
        println("Speed: " + str(this.speed) + " km/h")
    }
    
    fn displayInfo() {
        println("Brand: " + this.brand)
        println("Speed: " + str(this.speed) + " km/h")
    }
}

class Car extends Vehicle {
    let doors: int = 4
    
    fn init(b: string, d: int) {
        super.init(b)
        this.doors = d
    }
    
    fn displayInfo() {
        super.displayInfo()
        println("Doors: " + str(this.doors))
    }
}

class Motorcycle extends Vehicle {
    let hasSidecar: bool = false
    
    fn init(b: string, sidecar: bool) {
        super.init(b)
        this.hasSidecar = sidecar
    }
    
    fn displayInfo() {
        super.displayInfo()
        println("Sidecar: " + str(this.hasSidecar))
    }
}

# Usage
let car = new Car("Honda", 4)
car.accelerate(60.0)
car.displayInfo()

let bike = new Motorcycle("Yamaha", false)
bike.accelerate(80.0)
bike.displayInfo()
```

### Example 2: Employee Hierarchy

```klang
class Employee {
    let name: string = ""
    let id: int = 0
    let baseSalary: float = 0.0
    
    fn init(n: string, empId: int, salary: float) {
        this.name = n
        this.id = empId
        this.baseSalary = salary
    }
    
    fn calculateSalary() -> float {
        return this.baseSalary
    }
    
    fn displayInfo() {
        println("Name: " + this.name)
        println("ID: " + str(this.id))
        println("Salary: $" + str(this.calculateSalary()))
    }
}

class Manager extends Employee {
    let bonus: float = 0.0
    
    fn init(n: string, empId: int, salary: float, b: float) {
        super.init(n, empId, salary)
        this.bonus = b
    }
    
    fn calculateSalary() -> float {
        return this.baseSalary + this.bonus
    }
}

class Developer extends Employee {
    let overtimeHours: float = 0.0
    let hourlyRate: float = 50.0
    
    fn init(n: string, empId: int, salary: float) {
        super.init(n, empId, salary)
    }
    
    fn addOvertime(hours: float) {
        this.overtimeHours = this.overtimeHours + hours
    }
    
    fn calculateSalary() -> float {
        let overtime = this.overtimeHours * this.hourlyRate
        return this.baseSalary + overtime
    }
}

# Usage
let manager = new Manager("Alice", 101, 80000.0, 10000.0)
manager.displayInfo()

let dev = new Developer("Bob", 102, 70000.0)
dev.addOvertime(10.0)
dev.displayInfo()
```

---

## Summary

- **Inheritance** allows classes to inherit from parent classes
- Use `extends` keyword to create inheritance
- Child classes inherit parent properties and methods
- Use `super` to access parent members
- Always call `super.init()` in child constructors
- Override methods to provide specialized behavior
- Use inheritance for "is-a" relationships

---

## Next Steps

Continue to:
- [Method Overriding in Detail](04_method_overriding.md)
- [The Super Keyword](05_super_keyword.md)
- [Abstract Classes](06_abstract_classes.md)

Happy coding! 🚀
