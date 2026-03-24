# KLang OOP Implementation - Quick Start Guide

## 🎯 Executive Summary

KLang currently supports **functions with parameters** but has **no OOP features**. This guide provides a quick overview of what needs to be added.

---

## ✅ What Already Works

| Feature | Example | Status |
|---------|---------|--------|
| Functions with parameters | `fn factorial(n: int) -> int { ... }` | ✅ Complete |
| Function calls | `factorial(10)` | ✅ Complete |
| Closures | Functions capture environment | ✅ Complete |
| Arrays | `[1, 2, 3]` with indexing | ✅ Complete |
| Type annotations | `let x: int = 5` | ⚠️ Partial (not enforced) |

---

## ❌ What's Missing (OOP)

### Priority 1: Classes & Objects
- [ ] `class ClassName { ... }` syntax
- [ ] `let obj = new ClassName()` instantiation
- [ ] `obj.field` and `obj.method()` member access
- [ ] `this` keyword for self-reference
- [ ] Instance fields and methods

### Priority 2: Constructors
- [ ] `fn init(params) { ... }` constructor method
- [ ] Automatic init call on `new`

### Priority 3: Inheritance
- [ ] `class Child extends Parent { ... }` syntax
- [ ] Method inheritance
- [ ] Method overriding
- [ ] `super` keyword for parent access

---

## 📋 Quick Implementation Checklist

### Phase 1: Basic Classes (Week 1-2)

#### 1. Lexer (`src/lexer.h`, `src/lexer.c`)
- [ ] Add tokens: `TOKEN_CLASS`, `TOKEN_NEW`, `TOKEN_THIS`, `TOKEN_DOT`
- [ ] Add keyword recognition for "class", "new", "this"

#### 2. AST (`src/ast.h`, `src/ast.c`)
- [ ] Add node types: `NODE_CLASS_DEF`, `NODE_NEW`, `NODE_MEMBER_ACCESS`, `NODE_THIS`
- [ ] Add struct definitions for class data
- [ ] Add constructor functions: `ast_new_class_def()`, `ast_new_new()`, etc.
- [ ] Update `ast_free()` to handle new nodes

#### 3. Parser (`src/parser.h`, `src/parser.c`)
- [ ] Add `parse_class_def()` function
- [ ] Update `parse_statement()` to handle `TOKEN_CLASS`
- [ ] Update `parse_primary()` to handle `TOKEN_NEW` and `TOKEN_THIS`
- [ ] Update `parse_postfix()` to handle `TOKEN_DOT` (member access)

#### 4. Interpreter (`src/interpreter.h`, `src/interpreter.c`)
- [ ] Add value types: `VAL_CLASS`, `VAL_OBJECT`, `VAL_METHOD`
- [ ] Add struct definitions: `ClassVal`, `ObjectVal`, `MethodVal`
- [ ] Add helper functions: `make_class()`, `make_object()`, etc.
- [ ] Add eval cases for: `NODE_CLASS_DEF`, `NODE_NEW`, `NODE_MEMBER_ACCESS`, `NODE_THIS`
- [ ] Update `value_free()` and `value_print()` for new types

#### 5. Testing (`tests/test_oop.c`)
- [ ] Create test file with unit tests
- [ ] Test class parsing, object creation, member access
- [ ] Add to test runner

---

### Phase 2: Inheritance (Week 3)

#### 1. Lexer
- [ ] Add tokens: `TOKEN_EXTENDS`, `TOKEN_SUPER`

#### 2. AST
- [ ] Add `parent_name` field to `class_def` struct
- [ ] Add `NODE_SUPER` node type

#### 3. Parser
- [ ] Update `parse_class_def()` to handle `extends` clause
- [ ] Parse `super.method()` syntax

#### 4. Interpreter
- [ ] Implement method inheritance lookup
- [ ] Implement `super` resolution
- [ ] Test method overriding

---

## 🚀 Quick Start: First Steps

### Step 1: Add Basic Lexer Support (30 minutes)

```c
// In src/lexer.h, add to TokenType enum:
TOKEN_CLASS,
TOKEN_NEW,
TOKEN_THIS,
TOKEN_DOT,  // May already exist

// In src/lexer.c, in keyword recognition:
if (strcmp(lexeme, "class") == 0) return TOKEN_CLASS;
if (strcmp(lexeme, "new") == 0) return TOKEN_NEW;
if (strcmp(lexeme, "this") == 0) return TOKEN_THIS;
```

### Step 2: Test Lexer Changes (10 minutes)

```bash
# Build and test
make clean && make
./klang
# In REPL, type: class MyClass { }
# Should tokenize without error (parsing will fail, that's expected)
```

### Step 3: Add AST Nodes (1 hour)

See full implementation in `docs/OOP_IMPLEMENTATION_PLAN.md`, Step 2.

### Step 4: Continue with Parser (2-3 hours)

See full implementation in `docs/OOP_IMPLEMENTATION_PLAN.md`, Step 3.

---

## 📖 Example Target Syntax

```klang
class Point {
    let x: int = 0
    let y: int = 0
    
    fn init(x_val: int, y_val: int) {
        this.x = x_val
        this.y = y_val
    }
    
    fn distance() -> float {
        return sqrt(this.x * this.x + this.y * this.y)
    }
}

let p = new Point(3, 4)
println(p.distance())  # Output: 5.0
```

With inheritance:

```klang
class Animal {
    let name: string = ""
    
    fn speak() {
        println(this.name + " makes a sound")
    }
}

class Dog extends Animal {
    fn speak() {
        println(this.name + " says: Woof!")
    }
}

let dog = new Dog()
dog.name = "Buddy"
dog.speak()  # Output: Buddy says: Woof!
```

---

## 🔧 Key Design Decisions

### 1. Constructor Method
- Use special method name `init` (like Python's `__init__`)
- Automatically called on `new ClassName(args)`
- Optional - if not defined, creates object with default field values

### 2. This Keyword
- Implicitly passed to all methods
- Stored in environment as "this"
- Available in method scope only

### 3. Inheritance
- Single inheritance only (simpler to implement)
- Method lookup: instance → class → parent → parent's parent...
- Field inheritance: child gets all parent fields

### 4. Member Access
- Dot operator: `object.member`
- Fields accessed first, then methods
- Methods return bound method (method + instance)

### 5. Value Representation
- Classes are values (stored in environment)
- Objects are values (contain class reference + field values)
- Methods are values (contain instance + function)

---

## 📚 Full Documentation

For complete implementation details, see:
- **`docs/OOP_IMPLEMENTATION_PLAN.md`** - Comprehensive step-by-step guide with code examples
- **`docs/architecture.md`** - Overall architecture documentation

---

## 🏗️ Architecture Overview

```
┌─────────────────────────────────────────────┐
│              KLang OOP Pipeline              │
└─────────────────────────────────────────────┘

Source Code
    ↓
┌───────────┐
│  Lexer    │ → Tokens: class, new, this, extends, super, .
└───────────┘
    ↓
┌───────────┐
│  Parser   │ → AST Nodes: CLASS_DEF, NEW, MEMBER_ACCESS, THIS
└───────────┘
    ↓
┌───────────┐
│Interpreter│ → Values: CLASS, OBJECT, METHOD
└───────────┘
    ↓
┌───────────┐
│  Output   │ → Object creation, method calls, inheritance
└───────────┘
```

### Class Storage

```
Environment:
  ├─ "Point" → ClassVal {
  │      name: "Point"
  │      parent: NULL
  │      fields: ["x", "y"]
  │      methods: Env {
  │         "init" → FunctionVal
  │         "distance" → FunctionVal
  │      }
  │   }
  │
  └─ "p" → ObjectVal {
         class_ref: → (pointer to Point ClassVal)
         fields: [
            Value(int, 3),    // x
            Value(int, 4)     // y
         ]
      }
```

---

## ⚠️ Common Pitfalls

1. **Memory Management**
   - Objects contain pointers to class definitions
   - Don't free class while objects exist
   - Consider reference counting or GC integration

2. **This Binding**
   - Must bind "this" in method call environment
   - Must NOT bind "this" in regular function calls
   - Check environment chain for method vs function

3. **Method vs Function**
   - Methods are bound to instances (have "this")
   - Functions are standalone (no "this")
   - Member access returns bound method, not raw function

4. **Inheritance Lookup**
   - Methods: search class chain
   - Fields: all fields inherited, stored in object
   - Avoid infinite loops with circular inheritance

5. **Parser Precedence**
   - Dot operator is postfix (like `[]` and `()`)
   - Parse in `parse_postfix()`, not `parse_primary()`
   - Handle chaining: `obj.method().field.another()`

---

## 🧪 Testing Strategy

### Unit Tests Priority

1. **Lexer**: Token recognition
2. **Parser**: AST construction
3. **Interpreter**: Class registration
4. **Interpreter**: Object creation
5. **Interpreter**: Field access
6. **Interpreter**: Method calls
7. **Interpreter**: This keyword
8. **Interpreter**: Inheritance
9. **Integration**: Complete programs

### Test Coverage Goals

- Lexer: 100% (all new tokens)
- Parser: 90% (all syntax variations)
- Interpreter: 85% (core OOP features)
- Integration: 80% (example programs)

---

## 📊 Estimated Effort

| Phase | Component | Effort | Complexity |
|-------|-----------|--------|------------|
| 1 | Lexer | 2 hours | Low |
| 1 | AST | 3 hours | Low |
| 1 | Parser | 8 hours | Medium |
| 1 | Interpreter | 16 hours | High |
| 1 | Testing | 8 hours | Medium |
| **Phase 1 Total** | | **37 hours** | **~1 week** |
| 2 | Inheritance | 12 hours | Medium |
| 2 | Testing | 4 hours | Low |
| **Phase 2 Total** | | **16 hours** | **~2 days** |
| 3 | Advanced Features | 20 hours | High |
| **Total** | | **73 hours** | **~2 weeks** |

*Assuming 8-hour work days, experienced with C and compilers*

---

## 🎓 Learning Resources

### Understanding OOP Implementation

1. **Crafting Interpreters** by Bob Nystrom
   - Chapter 12: Classes
   - Chapter 13: Inheritance
   - Free online: https://craftinginterpreters.com/

2. **Similar Language Examples**
   - Python: Simple OOP model with `__init__` and `self`
   - JavaScript: Prototype-based but similar syntax
   - Ruby: Class-based with `initialize` and `@`

3. **Compiler Design**
   - Dragon Book: Chapter 6 (Type Checking)
   - Engineering a Compiler: Chapter 7 (Intermediate Representations)

---

## 🤝 Agent Assignment Recommendations

Based on KLang's agent architecture:

| Task | Recommended Agent | Reason |
|------|-------------------|--------|
| Lexer changes | **compiler-agent** | Tokenization specialist |
| AST design | **compiler-agent** | AST expert |
| Parser implementation | **compiler-agent** | Parser specialist |
| Interpreter changes | **runtime-agent** | Runtime specialist |
| Value types | **runtime-agent** | Value system expert |
| Testing | **qa-agent** or **testing-agent** | Test creation |
| Examples | **docs-agent** | Documentation |
| Integration | **integration-agent** | Cross-component |
| Memory management | **gc-agent** | Memory concerns |

### Workflow Suggestion

1. **planner-agent** (this agent) creates plan ✅ 
2. **compiler-agent** implements lexer, AST, parser
3. **runtime-agent** implements interpreter changes
4. **testing-agent** creates test suite
5. **integration-agent** ensures all components work together
6. **qa-agent** validates correctness
7. **docs-agent** updates documentation

---

## 🚦 Getting Started NOW

### Immediate Next Steps

1. **Review this plan** - Understand the scope (10 minutes)
2. **Read full plan** - `docs/OOP_IMPLEMENTATION_PLAN.md` (30 minutes)
3. **Start with lexer** - Easiest component (2 hours)
4. **Test lexer** - Verify tokens work (30 minutes)
5. **Move to AST** - Add node types (3 hours)
6. **Continue with parser** - Implement syntax (8 hours)
7. **Implement interpreter** - Core functionality (16 hours)
8. **Test everything** - Comprehensive testing (8 hours)

### First Commit Goals

After Phase 1, you should be able to:
```klang
class MyClass {
    let field: int = 42
    
    fn get_field() -> int {
        return this.field
    }
}

let obj = new MyClass()
println(obj.field)           # Output: 42
println(obj.get_field())     # Output: 42
```

---

## 📞 Questions or Issues?

Refer to:
- **Full Implementation Plan**: `docs/OOP_IMPLEMENTATION_PLAN.md`
- **Architecture Docs**: `docs/architecture.md`
- **Existing Code**: Study `src/parser.c` for function parsing patterns
- **Test Examples**: Look at `tests/test_parser.c` for testing patterns

---

**Good luck with the implementation!** 🚀
