# KLang Tuple Implementation - Summary

## Investigation Complete ✓

I've analyzed KLang's architecture to understand how to implement Python's tuple type. Here are the findings:

## Key Discoveries

### 1. **Type System Structure**
- **Tagged Union Pattern**: Both AST and Runtime use discriminated unions
- **AST**: `NodeType` enum determines which struct in ASTNode union is active
- **Runtime**: `ValueType` enum determines which field in Value union is active
- **Factory Pattern**: All values created via `make_*()` functions

### 2. **Array/List Implementation** (Model for Tuples)
- **Parser**: Recognizes `[` token → creates `NODE_LIST`
- **AST Storage**: `NodeList elements` - dynamic array of ASTNode pointers
- **Runtime Storage**: `ListVal {items, count, capacity}` - mutable, resizable
- **Evaluation**: Two-pass algorithm (count elements → allocate → fill)
- **Spread Support**: Already handles `[...arr, item]` expansion

### 3. **Parser Literal Patterns**
- **Token-based dispatch**: Each literal type identified by its starting token
- **Lists**: `TOKEN_LBRACKET` → parse comma-separated expressions → `TOKEN_RBRACKET`
- **Objects**: `TOKEN_LBRACE` → parse key:value pairs → `TOKEN_RBRACE`
- **Grouping**: `TOKEN_LPAREN` → parse single expression → `TOKEN_RPAREN`
- **Key Insight**: Parentheses currently used only for grouping, not tuples!

### 4. **Runtime Value Structure**
```c
struct Value {
    ValueType type;   // Discriminant (14 types currently)
    union {
        long long int_val;
        double float_val;
        char *str_val;
        ListVal list_val;     // Arrays (mutable)
        ObjectVal object_val;
        PromiseVal promise_val;
        GeneratorVal generator_val;
        // ... etc
        // Missing: TupleVal tuple_val
    } as;
};
```

### 5. **Destructuring Already Works!**
- **NODE_DESTRUCTURE_ARRAY**: Fully implemented for arrays
- **Pattern**: `let [x, y] = array` extracts elements
- **Features**: Supports rest elements `...rest`, holes, nested patterns
- **Bonus**: Will work automatically with tuples once added!

## Implementation Strategy

### Phase 1: Add Type Definitions
1. `ast.h`: Add `NODE_TUPLE` to enum, add tuple struct to union
2. `interpreter.h`: Add `VAL_TUPLE` to enum, define `TupleVal` struct
3. `ast.c`: Add `ast_new_tuple()` factory function

### Phase 2: Modify Parser
- **Challenge**: Distinguish `(expr)` from `(1,)` and `(1, 2)`
- **Solution**: Check for comma after first expression
  - No comma → grouped expression (return unwrapped)
  - Comma present → tuple (collect all elements)
  - Empty parens `()` → empty tuple

### Phase 3: Implement Evaluation
- Add `NODE_TUPLE` case in `interpreter.c`
- Follow `NODE_LIST` pattern: two-pass evaluation
- Handle spread operator: `(1, ...tuple, 3)`
- Deep copy strings to prevent aliasing

### Phase 4: Value Operations
- `value_print()`: Print as `(1, 2, 3)`
- `value_to_string()`: Convert to string representation
- `value_free()`: Free items array and contained values
- `value_equals()`: Compare tuple contents

### Phase 5: Indexing & Immutability
- Add `VAL_TUPLE` case to `NODE_INDEX` for reading
- Block assignment in `NODE_ASSIGN` for tuple elements
- Error message: "cannot assign to tuple element (tuples are immutable)"

### Phase 6: Destructuring Integration
- Modify `NODE_DESTRUCTURE_ARRAY` to accept both lists and tuples
- Extract items pointer and count based on type
- Rest of logic works unchanged

### Phase 7: Built-in Functions
- `tuple_len(t)` - Get tuple length
- `tuple_to_list(t)` - Convert tuple to mutable list
- `list_to_tuple(l)` - Convert list to immutable tuple
- `is_tuple(v)` - Type checking

## Syntax Examples

```klang
// Empty tuple
let empty = ()

// Single element (trailing comma required)
let single = (42,)

// Multiple elements
let point = (10, 20)
let rgb = (255, 128, 0)

// Mixed types
let mixed = (1, "hello", true, null)

// Nested
let nested = (1, (2, 3), 4)

// With spread
let combined = (1, ...point, 5)  // (1, 10, 20, 5)

// Indexing (read-only)
println(point[0])  // 10
println(point[1])  // 20

// Destructuring
let (x, y) = point
println(x, y)  // 10 20

// Multiple returns
fn divmod(a, b) {
    return (a / b, a % b)
}
let (quotient, remainder) = divmod(17, 5)

// Immutability (will error)
point[0] = 99  // ERROR: cannot assign to tuple element

// Comparison
let a = (1, 2, 3)
let b = (1, 2, 3)
println(a == b)  // true
```

## Key Differences: Tuples vs Lists

| Feature | Lists `[1, 2, 3]` | Tuples `(1, 2, 3)` |
|---------|-------------------|---------------------|
| **Syntax** | Square brackets | Parentheses + comma |
| **Mutability** | Mutable | Immutable |
| **Storage** | `ListVal` with capacity | `TupleVal` without capacity |
| **Single element** | `[1]` | `(1,)` - comma required |
| **Empty** | `[]` | `()` |
| **Use case** | Dynamic collections | Fixed structure data |
| **Assignment** | `list[0] = x` ✓ | `tuple[0] = x` ✗ |

## Architecture Strengths

✅ **Already in place:**
- Tagged union value system
- Factory function pattern
- Destructuring support
- Spread operator handling
- Two-pass evaluation pattern
- Memory management (value_free)

✅ **Easy additions:**
- Add enum values (2 lines)
- Add struct definitions (2 structs)
- Reuse NodeList structure for AST
- Follow existing patterns

## Estimated Effort

- **Total Lines**: ~355 lines across 6 files
- **Time**: 4-6 hours for complete implementation
- **Complexity**: Medium (follows established patterns)
- **Risk**: Low (well-understood architecture)

## Files to Modify

| File | Changes | Lines |
|------|---------|-------|
| `src/ast.h` | Add NODE_TUPLE, tuple struct | ~5 |
| `src/interpreter.h` | Add VAL_TUPLE, TupleVal struct | ~10 |
| `src/ast.c` | Add ast_new_tuple() | ~10 |
| `src/parser.c` | Modify parse_primary() | ~40 |
| `src/interpreter.c` | Add tuple evaluation & operations | ~120 |
| `src/runtime.c` | Add tuple built-in functions | ~80 |
| `tests/test_tuples.k` | Comprehensive test suite | ~80 |

## Next Steps

1. Review the detailed analysis: `TUPLE_ARCHITECTURE_ANALYSIS.md`
2. Check the visual guide: `TUPLE_IMPLEMENTATION_VISUAL.md`
3. Use the quick reference: `TUPLE_QUICK_REFERENCE.md`
4. Start implementation following the phase-by-phase plan
5. Create test suite early and test incrementally

## Critical Implementation Notes

### Parser Ambiguity Resolution
```c
if (TOKEN_LPAREN) {
    parse first expression
    if (TOKEN_COMMA) {
        // It's a tuple - collect all elements
        return NODE_TUPLE
    } else {
        // It's just grouping - return expression
        return expr
    }
}
```

### Memory Management
- Deep copy strings: `make_string(item.as.str_val)`
- Free items array: `free(tuple.as.tuple_val.items)`
- Free each element: Loop and call `value_free(&items[i])`

### Immutability Enforcement
```c
// In NODE_ASSIGN evaluation
if (target is NODE_INDEX) {
    eval target object
    if (obj.type == VAL_TUPLE) {
        error: "cannot assign to tuple element"
        return
    }
}
```

## Resources Created

1. **TUPLE_ARCHITECTURE_ANALYSIS.md** - Comprehensive 700+ line analysis
2. **TUPLE_QUICK_REFERENCE.md** - Quick reference guide
3. **TUPLE_IMPLEMENTATION_VISUAL.md** - Visual diagrams and flows

## Questions Answered

✓ How are types currently defined? → Tagged unions with discriminated enums  
✓ How are arrays implemented? → NODE_LIST + ListVal with two-pass evaluation  
✓ How does parser distinguish literals? → Token-based dispatch in parse_primary()  
✓ What is the Value structure? → Tagged union with 14 types currently  
✓ How are multiple returns handled? → Destructuring already works for arrays  

## Conclusion

KLang's architecture is **exceptionally well-suited** for tuple implementation:
- Clean tagged union design makes adding types straightforward
- Existing list implementation provides a proven pattern
- Destructuring infrastructure already in place
- Parser is modular and easy to extend
- Memory management patterns are consistent

The main work is extending the parser's parenthesis handling to detect tuples and implementing the evaluation logic following the established list pattern.

**Ready to implement!** 🚀
