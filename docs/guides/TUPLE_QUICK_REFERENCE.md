# KLang Tuple Implementation - Quick Reference

## Current Architecture Summary

### Type System (Tagged Union)

```c
// ast.h - AST Node Types
NODE_LIST     // [1, 2, 3]
NODE_OBJECT   // {key: value}
NODE_SPREAD   // ...arr
// Need: NODE_TUPLE

// interpreter.h - Runtime Value Types
VAL_LIST      // Mutable array
VAL_OBJECT    // Object/dictionary
VAL_STRING    // String
// Need: VAL_TUPLE
```

### List Implementation Pattern

```c
// AST Node
struct { NodeList elements; } list;

// Runtime Value
typedef struct {
    Value *items;
    int count;
    int capacity;  // For dynamic resizing
} ListVal;

// Parser recognizes: TOKEN_LBRACKET [ ... ]
// Evaluator: Two-pass (count → allocate → fill)
```

## What's Already Built

✅ **Destructuring** - `let [a, b] = array` works  
✅ **Spread Operator** - `[...arr, item]` works  
✅ **Indexing** - `array[0]` works  
✅ **Factory Functions** - `make_*()` pattern established  
✅ **Memory Management** - `value_free()` pattern established  

## What Needs Adding

### 1. Type Definitions (3 files)

**ast.h**
```c
// Line ~4: Add to enum
NODE_TUPLE,

// Line ~69: Add to union
struct { NodeList elements; } tuple;
```

**interpreter.h**
```c
// Line ~6: Add to enum
VAL_TUPLE,

// Line ~27: Add struct
typedef struct {
    Value *items;
    int count;
} TupleVal;

// Line ~93: Add to union
TupleVal tuple_val;
```

**ast.c**
```c
ASTNode *ast_new_tuple(int line) {
    ASTNode *node = ast_alloc(NODE_TUPLE, line);
    nodelist_init(&node->data.tuple.elements);
    return node;
}
```

### 2. Parser Logic (parser.c ~line 182)

**Key Challenge:** Distinguish:
- `(expr)` → grouped expression
- `(1,)` → single-element tuple
- `(1, 2)` → multi-element tuple
- `()` → empty tuple

**Solution:** Check for comma after first expression

```c
if (check(parser, TOKEN_LPAREN)) {
    // If empty: () → empty tuple
    // If comma after first expr: (x,) or (x, y) → tuple
    // Otherwise: (expr) → grouped expression
}
```

### 3. Evaluation (interpreter.c)

**NODE_TUPLE case (~60 lines)**
- Two-pass evaluation (like NODE_LIST)
- Handle spread: `(1, ...tuple, 3)`
- Deep copy strings

**Value operations (~50 lines)**
- `value_print()` - print `(1, 2, 3)`
- `value_to_string()` - convert to string
- `value_free()` - free memory
- `value_equals()` - compare tuples

**Indexing support (~10 lines)**
- Add `VAL_TUPLE` case to `NODE_INDEX`

**Immutability (~10 lines)**
- Block assignment: `tuple[0] = x` → error

### 4. Built-in Functions (runtime.c)

```c
tuple_len(t)        // Get length
tuple_to_list(t)    // Convert to list
list_to_tuple(l)    // Convert to tuple
is_tuple(v)         // Check type
```

## Syntax Examples

```klang
// Empty tuple
let empty = ()

// Single element (trailing comma required!)
let single = (1,)

// Multiple elements
let point = (10, 20)
let rgb = (255, 128, 0)

// Mixed types
let mixed = (1, "hello", true)

// Nested
let nested = (1, (2, 3), 4)

// With spread
let combined = (1, ...point, 5)

// Destructuring
let (x, y) = point
let (r, g, b) = rgb

// Multiple returns
fn divmod(a, b) {
    return (a / b, a % b)
}
let (q, r) = divmod(17, 5)

// Indexing (read-only)
println(point[0])  // 10
// point[0] = 99   // ERROR: immutable

// Grouping (NOT a tuple)
let x = (1 + 2)    // 3, not (3)
```

## Implementation Checklist

- [ ] Add `NODE_TUPLE` to `ast.h` NodeType enum
- [ ] Add tuple struct to `ast.h` ASTNode union
- [ ] Add `VAL_TUPLE` to `interpreter.h` ValueType enum
- [ ] Add `TupleVal` struct to `interpreter.h`
- [ ] Add `tuple_val` to `interpreter.h` Value union
- [ ] Add `ast_new_tuple()` to `ast.c`
- [ ] Modify `parse_primary()` in `parser.c` for tuple syntax
- [ ] Add `make_tuple()` factory function to `interpreter.c`
- [ ] Add `NODE_TUPLE` evaluation case to `interpreter.c`
- [ ] Update `value_print()` to handle `VAL_TUPLE`
- [ ] Update `value_to_string()` to handle `VAL_TUPLE`
- [ ] Update `value_free()` to handle `VAL_TUPLE`
- [ ] Update `value_equals()` to handle `VAL_TUPLE`
- [ ] Add `VAL_TUPLE` case to `NODE_INDEX` (indexing)
- [ ] Update `NODE_ASSIGN` to block tuple element assignment
- [ ] Update `NODE_DESTRUCTURE_ARRAY` to accept tuples
- [ ] Add tuple built-in functions to `runtime.c`
- [ ] Create test suite `tests/test_tuples.k`
- [ ] Update documentation

## Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| **Trailing comma for single element** | `(1,)` vs `(1)` - Python's approach, avoids ambiguity |
| **Immutable by design** | Enforced at assignment time, distinct from mutable lists |
| **Reuse NodeList structure** | Same AST structure as lists, simpler implementation |
| **No capacity field** | Tuples are fixed-size, don't need over-allocation |
| **Two-pass evaluation** | Count elements first (for spread), then allocate exact size |
| **Deep copy strings** | Prevent aliasing bugs, follow existing pattern |

## Testing Priority

1. **Core syntax** - Empty, single, multiple element tuples
2. **Indexing** - Read access works, write fails
3. **Destructuring** - Extract values into variables
4. **Spread** - Expand tuples into other tuples
5. **Multiple returns** - Functions returning tuples
6. **Nested tuples** - Tuples containing tuples
7. **Immutability** - Assignment errors correctly
8. **Built-ins** - Utility functions work
9. **Comparison** - Equality testing
10. **Edge cases** - Grouped expressions vs tuples

## Estimated Effort

- **Lines of Code:** ~355 lines total
- **Implementation Time:** 4-6 hours
- **Files Modified:** 6 files
- **Complexity:** Medium (follows established patterns)

## Common Pitfalls

⚠️ **Parser ambiguity** - Must check for comma to distinguish tuples  
⚠️ **Memory leaks** - Must deep copy strings in tuple items  
⚠️ **Immutability** - Must check at assignment time  
⚠️ **Spread support** - Must handle both lists and tuples in spread  
⚠️ **Single element** - Must require trailing comma `(1,)`  

## Next Steps

1. Read `TUPLE_ARCHITECTURE_ANALYSIS.md` for detailed implementation
2. Start with type definitions in headers
3. Add parser support for tuple syntax
4. Implement evaluation logic
5. Add value operations
6. Write comprehensive tests
7. Document in user-facing docs
