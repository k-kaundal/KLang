# Python Features Quick Reference for KLang

**Quick lookup guide for Python features to add to KLang**

---

## ✅ Already Supported (Different Syntax)

| Python | KLang | Status |
|--------|-------|--------|
| `def func():` | `fn func()` | ✅ |
| `class MyClass:` | `class MyClass {}` | ✅ |
| `self` | `this` | ✅ |
| `__init__` | `init` | ✅ |
| `lambda x: x*2` | `x => x*2` | ✅ |
| `async/await` | `async/await` | ✅ |
| `f"{x}"` | `` `${x}` `` | ✅ |
| `for x in list:` | `for (x of list)` | ✅ |
| Type hints | Type annotations | ✅ |

---

## 🎯 Priority Features to Add

### Priority 1: Essential (Do First)

| Feature | Syntax | Effort | Phase |
|---------|--------|--------|-------|
| **Tuple** | `(1, 2, 3)` | 2w | 1 |
| **Dict** | `{"a": 1, "b": 2}` | 2w | 1 |
| **List Comp** | `[x*2 for x in range(10)]` | 2w | 2 |
| **range()** | `range(0, 10, 2)` | 1w | 2 |
| **File I/O** | `open("file.txt")` | 2w | 4 |
| **JSON** | `json.parse(text)` | 3w | 4 |

### Priority 2: High Value

| Feature | Syntax | Effort | Phase |
|---------|--------|--------|-------|
| **Set** | `new Set([1, 2, 3])` | 1.5w | 1 |
| **None** | `let x = None` | 1w | 1 |
| **enumerate** | `enumerate(list)` | 1w | 2 |
| **Slice** | `list[1:5]`, `list[::-1]` | 2w | 3 |
| **Regex** | `regex.match(/\d+/, text)` | 4w | 4 |
| **Math** | `math.sin()`, `math.PI` | 1w | 4 |

### Priority 3: Nice to Have

| Feature | Syntax | Effort | Phase |
|---------|--------|--------|-------|
| **zip** | `zip(list1, list2)` | 1w | 2 |
| **with** | `with (open(f) as x) {}` | 2w | 3 |
| **@property** | `@property fn area()` | 2w | 3 |
| **match** | `match x { case 1 => }` | 2w | 3 |
| **DateTime** | `datetime.now()` | 2w | 4 |
| **Random** | `random.randint(1, 10)` | 1w | 4 |

---

## 📚 Feature Categories

### Data Structures
- ✅ **List**: `[1, 2, 3]` - Already supported
- ❌ **Tuple**: `(1, 2, 3)` - Need to add
- ❌ **Dict**: `{"a": 1}` - Need true hash maps (objects are limited)
- ❌ **Set**: `{1, 2, 3}` - Need to add
- ❌ **None**: Explicit null value

### Syntax Sugar
- ❌ **List comprehensions**: `[x*2 for x in list if x > 0]`
- ❌ **Dict comprehensions**: `{k: v*2 for k, v in dict}`
- ❌ **Slice notation**: `list[1:5]`, `list[::2]`, `list[::-1]`
- ✅ **Template literals**: `` `Hello ${name}` `` (instead of f-strings)

### Built-in Functions
- ❌ **range**: `range(10)`, `range(0, 10, 2)`
- ❌ **enumerate**: `enumerate(list, start=0)`
- ❌ **zip**: `zip(list1, list2)`
- ✅ **map/filter**: Via array methods `.map()`, `.filter()`
- ✅ **len**: `len(list)` - Already supported
- ✅ **print**: `print(x)` - Already supported

### Control Flow
- ✅ **if/else**: Supported
- ✅ **while/for**: Supported
- ✅ **for-of**: Supported (Python's `for x in list`)
- ⚠️ **try-catch-finally**: Partial (needs completion)
- ❌ **with statement**: Context managers
- ❌ **match-case**: Pattern matching (Python 3.10+)

### OOP Features
- ✅ **Classes**: Supported
- ✅ **Inheritance**: Single inheritance supported
- ✅ **Abstract classes**: Supported
- ✅ **Access modifiers**: public/private/protected
- ❌ **Properties**: `@property` decorator
- ❌ **Operator overload**: `operator+()`, etc.
- ❌ **Magic methods**: `__str__`, `__repr__`, etc.

### Standard Library
- ❌ **File I/O**: `open()`, `read()`, `write()`, `close()`
- ❌ **JSON**: `json.parse()`, `json.stringify()`
- ❌ **Regex**: `regex.match()`, `regex.search()`
- ❌ **Math**: `math.sin()`, `math.sqrt()`, `math.PI`
- ❌ **DateTime**: `datetime.now()`, `datetime.parse()`
- ❌ **Random**: `random.random()`, `random.randint()`
- ❌ **HTTP**: `requests.get()` (low priority)

### Type System
- ✅ **Type hints**: `x: int` - Enforced in KLang
- ✅ **Static typing**: KLang is statically typed
- ❌ **None type**: Explicit null
- ⚠️ **isinstance**: `type()` exists but limited
- ❌ **Optional types**: `Type?` or `Optional<Type>`

---

## 🚫 Features NOT to Add

| Feature | Why Not |
|---------|---------|
| **Dynamic typing** | KLang is statically typed by design |
| **Duck typing** | Breaks type safety |
| **Multiple inheritance** | Too complex, prefer interfaces |
| **Metaclasses** | Too dynamic for static typing |
| **eval()/exec()** | Security risk |
| **Monkey patching** | Breaks static analysis |
| **Indentation syntax** | KLang uses braces |

---

## 📖 Syntax Examples

### Tuples (To Add)
```python
# Python → KLang (planned)
coords = (10, 20)              let coords = (10, 20)
x, y = coords                  let (x, y) = coords
def f(): return 1, 2           fn f() -> (int, int) { return (1, 2) }
```

### Dictionaries (To Add)
```python
# Python → KLang (planned)
scores = {"Alice": 95}         let scores = {"Alice": 95}
scores["Bob"] = 87             scores["Bob"] = 87
if "Alice" in scores:          if (scores.has("Alice"))
keys = scores.keys()           let keys = scores.keys()
```

### List Comprehensions (To Add)
```python
# Python → KLang (planned)
squares = [x*x for x in range(10)]
evens = [x for x in range(20) if x % 2 == 0]
matrix = [[i*j for j in range(5)] for i in range(5)]
```

### Slice Notation (To Add)
```python
# Python → KLang (planned)
list[1:5]                      list[1:5]
list[:3]                       list[:3]
list[::2]                      list[::2]
list[::-1]                     list[::-1]
```

### File I/O (To Add)
```python
# Python → KLang (planned)
with open("file.txt") as f:    with (open("file.txt") as f) {
    content = f.read()             let content = f.read()
                                }
```

### Pattern Matching (To Add)
```python
# Python → KLang (planned)
match status:                  match status {
    case 200:                      case 200 => print("OK")
        print("OK")                case 404 => print("Not Found")
    case 404:                      case _ => print("Unknown")
        print("Not Found")      }
    case _:
        print("Unknown")
```

---

## 🗓️ Implementation Timeline

### Weeks 1-6: Phase 1 (Data Structures)
- Week 1-2: Tuples
- Week 3-4: Dictionaries
- Week 5: Sets
- Week 6: None type

### Weeks 7-10: Phase 2 (Syntax Sugar)
- Week 7-8: List comprehensions
- Week 9: range() function
- Week 10: enumerate() + zip()

### Weeks 11-14: Phase 3 (Advanced)
- Week 11-12: Slice notation
- Week 13: With statement
- Week 14: Match statement

### Weeks 15-22: Phase 4 (Standard Library)
- Week 15-16: File I/O
- Week 17-19: JSON parser
- Week 20-23: Regex (PCRE2)
- Week 24: Math module
- Week 25: DateTime + Random

**Total**: ~25 weeks (6 months) for full Python feature support

---

## 🎯 Quick Start Guide

### What to Do First

1. **Complete existing JavaScript features** (1-2 weeks)
   - Finish try-catch-finally
   - Finish switch-case

2. **Add Tuples** (2 weeks)
   - Essential for multi-return values
   - Enables cleaner APIs

3. **Add Dictionaries** (2 weeks)
   - Most important data structure
   - Required for many algorithms

4. **Add List Comprehensions** (2 weeks)
   - Most Pythonic syntax feature
   - High developer satisfaction

5. **Add range()** (1 week)
   - Completes basic iteration patterns
   - Works with existing for-of loops

6. **Add File I/O** (2 weeks)
   - Essential for real programs
   - Opens door to practical applications

### After 10 Weeks
You'll have:
- Tuples, dicts, sets, None
- List comprehensions
- range(), enumerate(), zip()
- File I/O

**Result**: KLang will feel significantly more Python-like!

---

## 📊 Comparison Matrix

| Feature | Python | KLang Now | After Phase 1-2 | After Phase 4 |
|---------|--------|-----------|-----------------|---------------|
| **Data Structures** |
| Lists | ✅ | ✅ | ✅ | ✅ |
| Tuples | ✅ | ❌ | ✅ | ✅ |
| Dicts | ✅ | ⚠️ | ✅ | ✅ |
| Sets | ✅ | ❌ | ✅ | ✅ |
| **Syntax** |
| Comprehensions | ✅ | ❌ | ✅ | ✅ |
| Slice notation | ✅ | ⚠️ | ✅ | ✅ |
| f-strings | ✅ | ✅ | ✅ | ✅ |
| **Functions** |
| range() | ✅ | ❌ | ✅ | ✅ |
| enumerate() | ✅ | ❌ | ✅ | ✅ |
| zip() | ✅ | ❌ | ✅ | ✅ |
| **Stdlib** |
| File I/O | ✅ | ❌ | ❌ | ✅ |
| JSON | ✅ | ❌ | ❌ | ✅ |
| Regex | ✅ | ❌ | ❌ | ✅ |
| Math | ✅ | ⚠️ | ⚠️ | ✅ |

**Legend**: ✅ Full support, ⚠️ Partial, ❌ Not supported

---

## 💡 Design Principles

1. **Maintain static typing** - All Python features must work with KLang's type system
2. **Performance matters** - Features should compile efficiently
3. **Type safety first** - Prefer compile-time errors over runtime errors
4. **Syntax compatibility** - Make Python code easy to translate
5. **No compromises** - Don't add features that break type safety (dynamic typing, duck typing)

---

## 📚 See Also

- [PYTHON_FEATURES_ANALYSIS.md](./PYTHON_FEATURES_ANALYSIS.md) - Detailed analysis
- [PYTHON_FEATURES_ROADMAP.md](./PYTHON_FEATURES_ROADMAP.md) - Complete roadmap
- [JAVASCRIPT_FEATURES_ROADMAP.md](./JAVASCRIPT_FEATURES_ROADMAP.md) - JS features
- [README.md](./README.md) - Project overview

---

**Quick answer**: Start with Tuples (2w) → Dicts (2w) → List Comprehensions (2w) → range() (1w) → File I/O (2w). After 9 weeks, KLang will have essential Python features!
