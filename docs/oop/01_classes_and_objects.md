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

## Summary

- **Classes** are blueprints for creating objects
- **Objects** are instances of classes
- Use `new ClassName()` to create objects
- Access members with dot notation: `object.property` or `object.method()`
- Use `this` keyword inside methods to refer to current object

Happy coding! 🚀
