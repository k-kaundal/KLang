# OOP Runtime Implementation Summary

## Overview
Implemented complete Object-Oriented Programming runtime support in the KLang interpreter (src/interpreter.c and src/interpreter.h).

## New Value Types Added

### 1. VAL_CLASS
Represents a class definition with:
- `name`: Class name
- `parent_name`: Parent class name (for inheritance)
- `methods`: Environment storing class methods
- `fields`: Environment storing default field values

### 2. VAL_OBJECT
Represents an instance of a class with:
- `class_name`: Name of the class this object belongs to
- `fields`: Environment storing instance field values
- `methods`: Reference to class methods (shared across all instances)

### 3. VAL_METHOD
Represents a bound method (method + receiver) with:
- `receiver`: Pointer to the object (`this`)
- `method`: Pointer to the function to call

## Implementation Details

### Class Definition (NODE_CLASS_DEF)
- Creates a ClassVal with methods and fields environments
- Handles inheritance by deep-copying parent methods and fields
- Processes class members:
  - Functions become methods
  - Let statements become default field values
- Stores class in environment

### Object Instantiation (NODE_NEW via NODE_CALL)
- Special handling when callee is NODE_NEW
- Creates ObjectVal with class methods reference
- Initializes fields with deep-copied default values from class
- Calls `init()` method if it exists with constructor arguments
- Binds `this` to the object in init method scope

### Member Access (NODE_MEMBER_ACCESS)
- Accesses fields and methods on objects
- Returns field values directly
- Returns bound methods (VAL_METHOD) for method access
- Handles class static access

### This Keyword (NODE_THIS)
- Returns the current `this` object from environment
- Used within method bodies to access instance fields/methods
- Error if used outside class method context

### Super Keyword (NODE_SUPER)
- Accesses parent class methods
- Looks up parent class from current class definition
- Returns bound method from parent class
- Error if class has no parent or method not found

### Method Calls (NODE_CALL with VAL_METHOD)
- Detects bound method calls
- Binds `this` to receiver in method environment
- Executes method body with proper scope
- Returns method result

### Member Assignment (NODE_BINOP with "=")
- Handles `object.field = value` assignments
- Special case in BINOP evaluation
- Updates field in object's fields environment
- Does not free shared object/class values

## Memory Management Considerations

### Deep Copying Strategy
To avoid double-free errors, the implementation uses deep copying in several places:

1. **Function param_names**: Deep-copied when:
   - Copying parent methods to child class
   - Creating bound methods
   - Returning functions from NODE_IDENT

2. **String values**: Deep-copied when:
   - Storing in environments (env_set_local)
   - Copying parent fields to child class
   - Initializing object fields from class defaults

3. **Object/Class values**: Shared Env pointers, minimal copying
   - class_name and fields/methods Envs are NOT freed in value_free for objects
   - This prevents double-free but causes memory leaks
   - TODO: Implement proper reference counting

### Known Limitations
- Object and class Env pointers are leaked to avoid double-free
- No reference counting system implemented yet
- Memory will leak for long-running programs with many objects

## Example Usage

```klang
class Animal {
    let name: string = "Unknown"
    let age: int = 0
    
    fn init(n: string, a: int) {
        this.name = n
        this.age = a
    }
    
    fn speak() {
        println("Animal speaks")
    }
}

class Dog extends Animal {
    let breed: string = "Mixed"
    
    fn init(n: string, a: int, b: string) {
        super.init(n, a)
        this.breed = b
    }
    
    fn speak() {
        println("Woof!")
    }
}

let dog = new Dog("Buddy", 3, "Golden Retriever")
dog.speak()  # Prints: Woof!
```

## Testing
All 110 existing tests continue to pass. OOP functionality tested with:
- Basic class definition and instantiation
- Field access and modification
- Method calls with `this`
- Constructor (`init`) methods
- Inheritance with `extends`
- Parent method calls with `super`
- Method overriding
