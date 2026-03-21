# KLang OOP Language Specification

## Overview

KLang now supports Object-Oriented Programming (OOP) with classes, objects, inheritance, and polymorphism. This document describes the OOP features and syntax.

## Classes

### Class Definition

```klang
class ClassName {
    # Fields (instance variables)
    let field1: type = default_value
    let field2: type = default_value
    
    # Constructor (optional)
    fn init(param1: type, param2: type) {
        this.field1 = param1
        this.field2 = param2
    }
    
    # Methods
    fn method_name(param: type) -> return_type {
        # method body
        return value
    }
}
```

### Features

- Classes can have multiple fields and methods
- Fields can have default values
- The `init` method is the constructor (called when creating objects)
- Methods can access instance fields using `this` keyword
- Methods can have parameters and return values

## Object Instantiation

### Creating Objects

```klang
let obj = new ClassName(arg1, arg2)
```

- Use `new` keyword followed by class name
- Pass arguments if the class has an `init` constructor
- Returns a new object instance

### Accessing Members

```klang
# Access fields
let value = obj.field_name
obj.field_name = new_value

# Call methods
obj.method_name(args)
let result = obj.method_name(args)
```

## The `this` Keyword

The `this` keyword refers to the current object instance inside methods.

```klang
class Counter {
    let count: int = 0
    
    fn increment() {
        this.count = this.count + 1
    }
    
    fn get_count() {
        return this.count
    }
}

let c = new Counter()
c.increment()
println(c.get_count())  # 1
```

## Inheritance

### Single Inheritance

KLang supports single inheritance using the `extends` keyword.

```klang
class Parent {
    let parent_field: type = value
    
    fn parent_method() {
        # implementation
    }
}

class Child extends Parent {
    let child_field: type = value
    
    fn child_method() {
        # implementation
    }
}
```

### Features

- Child class inherits all fields and methods from parent
- Child can override parent methods
- Child can add new fields and methods

### The `super` Keyword

Use `super` to call parent class methods from child class:

```klang
class Animal {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
    }
    
    fn speak() {
        println("Animal speaks")
    }
}

class Dog extends Animal {
    let breed: string = ""
    
    fn init(n: string, b: string) {
        super.init(n)  # Call parent constructor
        this.breed = b
    }
    
    fn speak() {
        super.speak()  # Call parent method
        println("Woof!")
    }
}
```

## Method Overriding

Child classes can override parent methods:

```klang
class Parent {
    fn greet() {
        println("Hello from Parent")
    }
}

class Child extends Parent {
    fn greet() {
        println("Hello from Child")
    }
}

let c = new Child()
c.greet()  # "Hello from Child"
```

## Complete Example

```klang
# Define a base class
class Shape {
    let color: string = "white"
    
    fn init(c: string) {
        this.color = c
    }
    
    fn describe() {
        println("A ")
        println(this.color)
        println(" shape")
    }
}

# Define a derived class
class Circle extends Shape {
    let radius: int = 0
    
    fn init(c: string, r: int) {
        super.init(c)
        this.radius = r
    }
    
    fn describe() {
        super.describe()
        println(" with radius ")
        println(this.radius)
    }
    
    fn area() {
        return 3.14 * this.radius * this.radius
    }
}

# Create and use objects
let circle = new Circle("red", 5)
circle.describe()
println("Area: ")
println(circle.area())
```

## Limitations

### Current Limitations

1. **Multi-level inheritance** (3+ levels) may have stability issues
2. **No interfaces or abstract classes** yet
3. **No static methods or class variables** yet
4. **No access modifiers** (public/private) - all members are public
5. **No method overloading** - method names must be unique
6. **No multiple inheritance** - only single inheritance supported

### Memory Management

- Objects are managed by the runtime
- Circular references may cause memory leaks (GC not implemented yet)
- Be cautious with complex object graphs

## Best Practices

1. **Always define constructors** for classes with fields
2. **Use `this` explicitly** to access instance members
3. **Call `super.init()`** in child constructors before setting child fields
4. **Override methods carefully** - ensure parent behavior is preserved if needed
5. **Keep inheritance hierarchies shallow** (1-2 levels recommended)

## Syntax Summary

| Feature | Syntax |
|---------|--------|
| Class definition | `class Name { }` |
| Inheritance | `class Child extends Parent { }` |
| Constructor | `fn init(params) { }` |
| Field declaration | `let field: type = value` |
| Method definition | `fn method(params) -> type { }` |
| Object creation | `let obj = new ClassName(args)` |
| Field access | `obj.field` |
| Method call | `obj.method(args)` |
| Self-reference | `this.member` |
| Parent access | `super.method(args)` |

## Examples Available

See the `examples/` directory for working OOP examples:

- `examples/oop_demo.kl` - Basic OOP with inheritance
- `examples/point_class.kl` - Point class with methods
- `examples/calculator_class.kl` - Method chaining example
- `examples/inheritance_demo.kl` - Inheritance demonstration

## Testing OOP Features

Run examples:
```bash
./klang run examples/oop_demo.kl
./klang run examples/point_class.kl
./klang run examples/calculator_class.kl
```

Or use the REPL:
```bash
./klang repl
>> class Point { let x: int = 0 let y: int = 0 }
>> let p = new Point()
>> p.x = 10
>> println(p.x)
10
```
