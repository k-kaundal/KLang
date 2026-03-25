# KLang OOP Documentation - Complete Guide

## Overview

This directory contains comprehensive documentation for Object-Oriented Programming (OOP) in KLang. Each document covers a specific OOP concept with theory, syntax, examples, and best practices.

## Documentation Structure

### Phase 1: Basic OOP (COMPLETE ✅)
- **01_classes_and_objects.md** - Introduction to classes and objects
- **02_constructors.md** - Object initialization with constructors

### Phase 2: Intermediate OOP (In Progress)
- **03_inheritance.md** - Inheritance and code reuse
- **04_method_overriding.md** - Overriding parent methods
- **05_super_keyword.md** - Calling parent class methods

### Phase 3: Advanced OOP (Planned)
- **06_abstract_classes.md** - Abstract classes and methods
- **07_access_modifiers.md** - Public, private, protected
- **08_static_members.md** - Static properties and methods
- **09_polymorphism.md** - Polymorphic behavior

### Phase 4: Design Patterns (Planned)
- **10_encapsulation.md** - Data hiding and encapsulation
- **11_design_patterns.md** - Common OOP patterns

## Quick Reference

### Class Definition
```klang
class MyClass {
    let property: type = value
    
    fn init(param: type) {
        this.property = param
    }
    
    fn method() {
        # Method body
    }
}
```

### Object Creation
```klang
let obj = new MyClass(value)
```

### Inheritance
```klang
class Child extends Parent {
    fn init(params) {
        super.init(params)
        # Additional initialization
    }
    
    fn method() {
        super.method()  # Call parent method
        # Additional behavior
    }
}
```

### Access Modifiers
```klang
class MyClass {
    public let publicProp: int = 0
    private let privateProp: int = 0
    protected let protectedProp: int = 0
}
```

### Static Members
```klang
class MyClass {
    public static let staticProp: int = 0
    
    public static fn staticMethod() {
        # Static method body
    }
}

# Access static members
let val = MyClass.staticProp
MyClass.staticMethod()
```

### Abstract Classes
```klang
abstract class AbstractClass {
    public abstract fn abstractMethod() -> type;
    
    public fn concreteMethod() {
        # Implementation
    }
}

class ConcreteClass extends AbstractClass {
    public fn abstractMethod() -> type {
        # Required implementation
    }
}
```

## Learning Path

### For Beginners
1. Start with **01_classes_and_objects.md**
2. Learn **02_constructors.md**
3. Move to **03_inheritance.md**
4. Practice with examples in `examples/oop/`

### For Intermediate Programmers
1. Review basic concepts
2. Study **04_method_overriding.md** and **05_super_keyword.md**
3. Learn **06_abstract_classes.md** and **07_access_modifiers.md**
4. Practice with real-world examples

### For Advanced Users
1. Master all basic and intermediate concepts
2. Study **08_static_members.md** and **09_polymorphism.md**
3. Learn design patterns from **11_design_patterns.md**
4. Build complex applications

## Related Resources

### Test Files
Located in `tests/oop/`:
- `test_oop_01_classes_objects.kl`
- `test_oop_02_constructors.kl`
- `test_oop_03_inheritance.kl`
- And more...

### Examples
Located in `examples/oop/`:
- `example_01_basic_class.kl` - Basic classes and objects
- `example_02_constructors.kl` - Constructor patterns
- `example_03_inheritance.kl` - Inheritance examples
- And more...

### Running Tests
```bash
# Run all OOP tests
./tests/oop/run_all_oop_tests.sh

# Run specific test
./klang run tests/oop/test_oop_01_classes_objects.kl
```

### Running Examples
```bash
# Run specific example
./klang run examples/oop/example_01_basic_class.kl
```

## Key Concepts Summary

### Classes and Objects
- Classes are blueprints
- Objects are instances
- Use `new` to create objects
- Access members with dot notation

### Constructors
- Use `init` method
- Initialize properties
- Validate input
- Support default parameters

### Inheritance
- Use `extends` keyword
- Child inherits parent members
- Can override parent methods
- Use `super` to access parent

### Access Control
- `public` - accessible everywhere
- `private` - only within class
- `protected` - class and subclasses

### Static Members
- Belong to class, not instances
- Access via class name
- Shared across all instances

### Abstract Classes
- Cannot be instantiated
- Define abstract methods
- Subclasses must implement
- Provide partial implementation

## Best Practices

1. **Use Clear Names**: Class names in PascalCase, methods in camelCase
2. **Initialize in Constructor**: Set up all properties in `init`
3. **Validate Input**: Check parameters in constructors
4. **Use Inheritance Wisely**: Only when "is-a" relationship exists
5. **Favor Composition**: Sometimes better than inheritance
6. **Keep Classes Focused**: Single Responsibility Principle
7. **Use Access Modifiers**: Protect internal implementation
8. **Document Complex Logic**: Help future developers

## Common Patterns

### Singleton Pattern
```klang
class Singleton {
    private static let instance: Singleton = null
    
    private fn init() {
        # Private constructor
    }
    
    public static fn getInstance() -> Singleton {
        if (Singleton.instance == null) {
            Singleton.instance = new Singleton()
        }
        return Singleton.instance
    }
}
```

### Factory Pattern
```klang
class ShapeFactory {
    public static fn createShape(type: string) -> Shape {
        if (type == "circle") {
            return new Circle()
        } else if (type == "rectangle") {
            return new Rectangle()
        }
        return null
    }
}
```

### Observer Pattern
```klang
class Observable {
    private let observers: array = []
    
    public fn addObserver(observer: Observer) {
        # Add to observers list
    }
    
    public fn notifyObservers() {
        # Notify all observers
    }
}
```

## Contributing

When adding new OOP documentation:
1. Follow the existing format
2. Include clear examples
3. Add test cases
4. Provide practical examples
5. Document best practices

## Support

For questions or issues:
- Check existing documentation
- Review test files for examples
- Open an issue on GitHub
- Consult the KLang community

---

**Happy Learning!** 🚀

*Last Updated: 2026-03-25*
*Version: 1.0*
