# KLang Tuple Implementation - Documentation Index

## Overview

This collection of documents provides a complete analysis of KLang's architecture for implementing Python's tuple type. The exploration was conducted to understand the compiler pipeline, type system, and runtime before implementing tuples.

## Documents

### 1. **TUPLE_IMPLEMENTATION_SUMMARY.md** 📋
**Start here!** High-level overview and executive summary.

**Contents:**
- Key discoveries from architecture analysis
- Implementation strategy (7 phases)
- Syntax examples and use cases
- Tuples vs Lists comparison
- Estimated effort (355 lines, 4-6 hours)
- Critical implementation notes
- All questions answered

**Best for:** Getting a quick understanding of what needs to be done.

---

### 2. **TUPLE_ARCHITECTURE_ANALYSIS.md** 📚
**Comprehensive deep-dive.** Complete analysis of KLang's architecture.

**Contents:**
- Type definitions and AST structure (NodeType enum, ASTNode union)
- Array/List implementation details (parser, runtime, evaluation)
- Parser literal patterns (how literals are distinguished)
- Runtime Value structure (14 value types)
- Existing multiple return value support (destructuring)
- Complete implementation plan (all 7 phases with code)
- Testing strategy (10 test cases)
- Files to modify with line numbers
- Limitations and considerations

**Best for:** Understanding the full picture and implementing the feature.

---

### 3. **TUPLE_QUICK_REFERENCE.md** ⚡
**Quick lookup guide.** Condensed reference for rapid implementation.

**Contents:**
- Current architecture summary
- What's already built (destructuring, spread, indexing)
- What needs adding (checklist format)
- Syntax examples
- Key design decisions
- Common pitfalls
- Implementation checklist

**Best for:** Quick lookups during implementation.

---

### 4. **TUPLE_IMPLEMENTATION_VISUAL.md** 🎨
**Visual diagrams and flows.** Understand through pictures and charts.

**Contents:**
- Compiler pipeline overview
- Collection types flow diagrams
- Parser decision tree
- Memory layout comparisons
- AST node structure diagrams
- Evaluation flow (step-by-step)
- Immutability enforcement flow
- Destructuring flow
- Complete example with all phases
- Code modifications summary with visual blocks

**Best for:** Visual learners and understanding data flow.

---

## Quick Navigation

### By Task

**Want to understand the architecture?**
→ Start with `TUPLE_IMPLEMENTATION_SUMMARY.md`
→ Deep dive with `TUPLE_ARCHITECTURE_ANALYSIS.md`

**Ready to implement?**
→ Use `TUPLE_QUICK_REFERENCE.md` for checklist
→ Follow code in `TUPLE_ARCHITECTURE_ANALYSIS.md`

**Need to understand data flow?**
→ See diagrams in `TUPLE_IMPLEMENTATION_VISUAL.md`

**Looking for specific information?**
→ Use `TUPLE_QUICK_REFERENCE.md` for quick lookup

### By Question

**"How do I distinguish tuples from grouped expressions?"**
→ `TUPLE_ARCHITECTURE_ANALYSIS.md` - Phase 2: Parser Changes
→ `TUPLE_IMPLEMENTATION_VISUAL.md` - Key Decision Point in Parser

**"What's the memory layout?"**
→ `TUPLE_IMPLEMENTATION_VISUAL.md` - Data Structure Comparison

**"How does evaluation work?"**
→ `TUPLE_ARCHITECTURE_ANALYSIS.md` - Phase 3: Interpreter Changes
→ `TUPLE_IMPLEMENTATION_VISUAL.md` - Evaluation Flow

**"What files do I need to modify?"**
→ All documents have file lists, but see:
→ `TUPLE_QUICK_REFERENCE.md` - Implementation Checklist
→ `TUPLE_ARCHITECTURE_ANALYSIS.md` - Section 9: Files to Modify

**"How do I test this?"**
→ `TUPLE_ARCHITECTURE_ANALYSIS.md` - Section 10: Testing Strategy

**"What are the common pitfalls?"**
→ `TUPLE_QUICK_REFERENCE.md` - Common Pitfalls section
→ `TUPLE_ARCHITECTURE_ANALYSIS.md` - Section 11: Limitations

## Key Findings Summary

### Architecture
- **Type System**: Tagged unions (discriminated unions)
- **Value Types**: 14 types currently, adding VAL_TUPLE will be 15th
- **AST Pattern**: NodeType enum + union of structs
- **Runtime Pattern**: ValueType enum + union of values
- **Factory Functions**: All values created via `make_*()` functions

### Lists (Model for Tuples)
- **Syntax**: `[1, 2, 3]`
- **Parser**: TOKEN_LBRACKET → NODE_LIST
- **Runtime**: VAL_LIST with ListVal{items, count, capacity}
- **Evaluation**: Two-pass (count → allocate → fill)
- **Mutability**: Mutable, resizable

### Tuples (To Implement)
- **Syntax**: `(1, 2, 3)` (comma required)
- **Parser**: TOKEN_LPAREN + comma → NODE_TUPLE
- **Runtime**: VAL_TUPLE with TupleVal{items, count}
- **Evaluation**: Two-pass (same as lists)
- **Mutability**: Immutable, fixed-size

### Critical Differences
| Aspect | Current `(expr)` | New `(1, 2)` |
|--------|------------------|--------------|
| Purpose | Grouping | Tuple literal |
| Detection | Always | If comma present |
| Result | Unwrapped expression | NODE_TUPLE |
| Single element | `(1)` → `1` | `(1,)` → tuple |

## Implementation Phases

1. **Type Definitions** (ast.h, interpreter.h, ast.c) - Add enums and structs
2. **Parser Logic** (parser.c) - Detect tuples vs grouping
3. **Evaluation** (interpreter.c) - Evaluate NODE_TUPLE nodes
4. **Value Operations** (interpreter.c) - print, free, equals, to_string
5. **Indexing** (interpreter.c) - Read access, block write access
6. **Destructuring** (interpreter.c) - Accept tuples in destructuring
7. **Built-ins** (runtime.c) - tuple_len, conversions, is_tuple

## Code Stats

```
Total Implementation: ~355 lines

src/ast.h:          ~5 lines
src/interpreter.h:  ~10 lines
src/ast.c:          ~10 lines
src/parser.c:       ~40 lines
src/interpreter.c:  ~120 lines
src/runtime.c:      ~80 lines
tests/test_tuples.k: ~80 lines
```

## Testing Coverage

```
Core Syntax:
  ✓ Empty tuple: ()
  ✓ Single element: (1,)
  ✓ Multiple elements: (1, 2, 3)
  ✓ Mixed types: (1, "hello", true)
  ✓ Nested: (1, (2, 3), 4)

Operations:
  ✓ Indexing: tuple[0]
  ✓ Destructuring: let (x, y) = tuple
  ✓ Spread: (1, ...tuple, 4)
  ✓ Comparison: tuple1 == tuple2

Functions:
  ✓ Multiple returns: return (a, b)
  ✓ Parameter unpacking: fn(a, b) { ... }

Immutability:
  ✓ Read access works
  ✓ Write access errors

Built-ins:
  ✓ tuple_len(t)
  ✓ tuple_to_list(t)
  ✓ list_to_tuple(l)
  ✓ is_tuple(v)

Edge Cases:
  ✓ Grouped vs tuple: (1+2) vs (1,)
  ✓ Trailing commas: (1,) vs (1)
  ✓ Empty vs grouped empty: () vs ???
```

## Key Code Locations

### Headers
```
src/ast.h:
  Line 4-16:  NodeType enum (add NODE_TUPLE)
  Line 69:    ASTNode union (add tuple struct)

src/interpreter.h:
  Line 6-9:   ValueType enum (add VAL_TUPLE)
  Line 27-31: ListVal struct (model for TupleVal)
  Line 93-110: Value union (add tuple_val)
```

### Implementation
```
src/parser.c:
  Line 182-190: Parenthesis handling (modify)

src/interpreter.c:
  Line ~1405: NODE_LIST evaluation (model for NODE_TUPLE)
  Line ~322:  value_print (add VAL_TUPLE case)
  Line ~212:  Factory functions (add make_tuple)

src/runtime.c:
  Line ~200:  Builtin functions (add tuple functions)
```

## Usage Examples

### Simple Tuple
```klang
let point = (10, 20)
println(point)      // (10, 20)
println(point[0])   // 10
```

### Multiple Returns
```klang
fn get_user() {
    return (42, "Alice", "admin")
}

let (id, name, role) = get_user()
println(name)  // Alice
```

### Immutability
```klang
let coords = (5, 10)
coords[0] = 99  // ERROR: cannot assign to tuple element
```

### Type Conversion
```klang
let t = (1, 2, 3)
let l = tuple_to_list(t)  // [1, 2, 3]
l[0] = 99                  // OK, lists are mutable

let t2 = list_to_tuple(l)  // (99, 2, 3)
```

## Design Rationale

### Why Trailing Comma for Single Element?
- Avoids ambiguity with grouped expressions
- Follows Python's convention
- `(1)` → `1` (grouped)
- `(1,)` → `(1,)` (tuple)

### Why Separate from Lists?
- Semantic difference (immutable vs mutable)
- Performance (no capacity overhead)
- Type safety (clear intent)
- Pattern matching (different use cases)

### Why Two-Pass Evaluation?
- Handles spread operator correctly
- Allocates exact size needed
- Matches existing list pattern
- Clean separation of concerns

### Why Deep Copy Strings?
- Prevents aliasing bugs
- Consistent with list behavior
- Safe memory management
- Clear ownership semantics

## Next Steps

1. ✅ Architecture analysis complete
2. ✅ Documentation created
3. ⏭️ Implement Phase 1: Type definitions
4. ⏭️ Implement Phase 2: Parser changes
5. ⏭️ Implement Phase 3: Evaluation
6. ⏭️ Implement Phase 4: Value operations
7. ⏭️ Implement Phase 5: Indexing & immutability
8. ⏭️ Implement Phase 6: Destructuring
9. ⏭️ Implement Phase 7: Built-in functions
10. ⏭️ Write comprehensive tests
11. ⏭️ Update user documentation

## Resources

- **KLang Repository**: `/home/runner/work/KLang/KLang`
- **Key Files**:
  - `src/ast.h` - AST definitions
  - `src/interpreter.h` - Runtime value definitions
  - `src/parser.c` - Parser implementation
  - `src/interpreter.c` - Evaluation and runtime
  - `src/runtime.c` - Built-in functions

## Success Criteria

✅ Tuple syntax works: `(1, 2, 3)`
✅ Indexing works: `tuple[0]`
✅ Destructuring works: `let (x, y) = tuple`
✅ Immutability enforced: Assignment errors
✅ Spread operator works: `(1, ...tuple, 4)`
✅ Multiple returns work: `fn() { return (a, b) }`
✅ Built-in functions work: `tuple_len()`, conversions
✅ Comparison works: `tuple1 == tuple2`
✅ All tests pass
✅ No memory leaks

## Conclusion

KLang's architecture is **well-prepared** for tuple implementation. The tagged union design, existing list implementation, and comprehensive destructuring support provide a solid foundation. The main challenge is parser disambiguation, which has a clear solution. Implementation should take 4-6 hours for a complete, well-tested feature.

**All questions answered. Ready to implement! 🚀**
