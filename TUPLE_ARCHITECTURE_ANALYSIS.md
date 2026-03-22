# KLang Architecture Analysis: Python Tuple Implementation Guide

## Executive Summary

KLang is a C-based interpreter with full support for modern language features (async/await, generators, destructuring). It currently uses **lists** (arrays) as its primary collection type but lacks tuple support. The architecture is well-designed for adding tuples as an immutable collection variant.

---

## 1. TYPE DEFINITIONS AND AST STRUCTURE

### Current NodeType Enum (ast.h:4-16)
```c
typedef enum {
    NODE_NUMBER, NODE_STRING, NODE_BOOL, NODE_IDENT, NODE_NULL,
    NODE_BINOP, NODE_UNOP, NODE_CALL, NODE_INDEX,
    NODE_LET, NODE_ASSIGN, NODE_IF, NODE_WHILE, NODE_FOR, NODE_FOR_OF,
    NODE_RETURN, NODE_BREAK, NODE_CONTINUE, NODE_BLOCK, NODE_FUNC_DEF, NODE_STRUCT_DEF, NODE_IMPORT,
    NODE_LIST, NODE_OBJECT, NODE_SPREAD,           // ← Collections
    NODE_CLASS_DEF, NODE_NEW, NODE_MEMBER_ACCESS, NODE_THIS, NODE_SUPER,
    NODE_TEMPLATE_LITERAL, NODE_TERNARY, NODE_SWITCH, NODE_CASE,
    NODE_AWAIT, NODE_YIELD,
    NODE_EXPORT, NODE_IMPORT_NAMED, NODE_IMPORT_DEFAULT, NODE_IMPORT_NAMESPACE,
    NODE_DESTRUCTURE_ARRAY, NODE_DESTRUCTURE_OBJECT, NODE_DESTRUCTURE_ELEMENT,
    NODE_TRY_CATCH, NODE_THROW
} NodeType;
```

**Key Observation:** No `NODE_TUPLE` exists yet. The parser currently distinguishes:
- `NODE_LIST` - for `[1, 2, 3]` (arrays/lists)
- `NODE_OBJECT` - for `{key: value}` (objects)
- `NODE_SPREAD` - for `...arr` (spread operator)

### ASTNode Structure (ast.h:46-133)

The `ASTNode` uses a **tagged union** pattern:
```c
struct ASTNode {
    NodeType type;                    // Discriminant
    int line;                         // Line number
    union {
        double num_val;
        char *str_val;
        int bool_val;
        struct { ... } ident;
        struct { ASTNode *obj; ASTNode *index; } index_expr;
        struct { NodeList elements; } list;           // ← Array structure
        struct { ObjectProperty *props; int count; int capacity; } object;
        struct { ASTNode *argument; } spread;
        // ... 20+ other variant structs
    } data;
};
```

**For Lists (NODE_LIST):**
```c
struct { NodeList elements; } list;  // NodeList is a dynamic array of ASTNode*

typedef struct {
    ASTNode **items;
    int count;
    int capacity;
} NodeList;
```

---

## 2. ARRAY/LIST IMPLEMENTATION

### Parser: List Literal Parsing (parser.c:192-214)

The parser recognizes `[` and creates a `NODE_LIST`:
```c
if (check(parser, TOKEN_LBRACKET)) {
    Token t = advance(parser);
    ASTNode *list;
    token_free(&t);
    list = ast_new_list(line);
    
    while (!check(parser, TOKEN_RBRACKET) && !check(parser, TOKEN_EOF)) {
        /* Check for spread element */
        if (check(parser, TOKEN_SPREAD)) {
            Token spread_tok = advance(parser);
            token_free(&spread_tok);
            ASTNode *spread_expr = parse_expression(parser);
            ASTNode *spread_node = ast_new_spread(spread_expr, line);
            nodelist_push(&list->data.list.elements, spread_node);
        } else {
            nodelist_push(&list->data.list.elements, parse_expression(parser));
        }
        if (!match(parser, TOKEN_COMMA)) break;
    }
    // Consume RBRACKET
    return list;
}
```

**Key Features:**
- Supports comma-separated elements
- Supports spread syntax `[...array, item]`
- No tuple syntax `(1, 2, 3)` implemented

### Runtime: List Storage - ListVal (interpreter.h:27-31)

```c
typedef struct {
    Value *items;      // Pointer to array of Values
    int count;         // Number of elements
    int capacity;      // Allocated capacity
} ListVal;
```

**Important:** Lists are **mutable** - no protection against modification.

### AST to Runtime: NODE_LIST Evaluation (interpreter.c:1405-1470)

When a `NODE_LIST` is evaluated, it:
1. **Two-pass evaluation** (count then fill)
2. Handles spread operator `...arr` expansion
3. Deep copies strings and lists to prevent aliasing issues
4. Allocates exact capacity needed (no over-allocation)

---

## 3. PARSER LITERAL PATTERN

### Literal Parsing in parse_primary() (parser.c:147-350)

The parser distinguishes literals by **token type**:

| Syntax | Token | Node Type | Handler |
|--------|-------|-----------|---------|
| `123` | `TOKEN_NUMBER` | `NODE_NUMBER` | `ast_new_number()` |
| `"text"` | `TOKEN_STRING` | `NODE_STRING` | `ast_new_string()` |
| `` `template ${expr}` `` | `TOKEN_TEMPLATE_LITERAL` | `NODE_TEMPLATE_LITERAL` | `parse_template_literal()` |
| `true/false` | `TOKEN_TRUE/FALSE` | `NODE_BOOL` | `ast_new_bool()` |
| `ident` | `TOKEN_IDENT` | `NODE_IDENT` | `ast_new_ident()` |
| `[...]` | `TOKEN_LBRACKET` | `NODE_LIST` | `ast_new_list()` |
| `{...}` | `TOKEN_LBRACE` | `NODE_OBJECT` | `ast_new_object()` |
| `(expr)` | `TOKEN_LPAREN` | Depends on content | Parenthesized expression |

**Critical Observation:** Parentheses `(...)` are currently **parsed as grouping operators**, not as tuple constructors. This is the key differentiator from Python.

---

## 4. RUNTIME VALUE STRUCTURE

### Value Type - Complete Definition (interpreter.h:93-110)

```c
struct Value {
    ValueType type;          // Discriminant (tagged union)
    union {
        long long int_val;           // VAL_INT
        double float_val;            // VAL_FLOAT
        char *str_val;               // VAL_STRING
        int bool_val;                // VAL_BOOL
        FunctionVal func_val;        // VAL_FUNCTION
        ListVal list_val;            // VAL_LIST ← Arrays only
        BuiltinFn builtin;           // VAL_BUILTIN
        ClassVal class_val;          // VAL_CLASS
        ObjectVal object_val;        // VAL_OBJECT
        MethodVal method_val;        // VAL_METHOD
        PromiseVal promise_val;      // VAL_PROMISE
        ModuleVal module_val;        // VAL_MODULE
        GeneratorVal generator_val;  // VAL_GENERATOR
    } as;
};
```

### ValueType Enum (interpreter.h:6-9)

```c
typedef enum {
    VAL_INT, VAL_FLOAT, VAL_STRING, VAL_BOOL, VAL_NULL, VAL_FUNCTION, VAL_LIST, VAL_BUILTIN,
    VAL_CLASS, VAL_OBJECT, VAL_METHOD, VAL_PROMISE, VAL_MODULE, VAL_GENERATOR
} ValueType;
```

**Missing:** `VAL_TUPLE` - no tuple variant exists

### Value Constructors (interpreter.c:212-320)

All values created via factory functions:
```c
Value make_int(long long v)
Value make_float(double v)
Value make_string(const char *s)
Value make_bool(int v)
Value make_null(void)
// ... etc
```

**Missing:** `make_tuple()` function

---

## 5. MULTIPLE RETURN VALUES SUPPORT

### Destructuring Assignment - Array Pattern

KLang **already supports destructuring arrays!** (ast.h:104-122)

```c
typedef struct {
    NodeList elements;      // Array of destructure elements
    ASTNode *source;        // Expression to destructure from
    DeclType decl_type;     // let/const/var
} destructure_array;

typedef struct {
    char *name;             // Variable name to bind to
    char *key;              // Object key (for object destructuring)
    ASTNode *default_value; // Default value if undefined
    int is_rest;            // Is this a rest element (...rest)
    int is_hole;            // Is this a hole/skip in array destructuring
    ASTNode *nested;        // Nested destructuring pattern
} destructure_element;
```

**Key Insight:** Destructuring is already fully implemented, so tuples can leverage this:

```klang
fn get_point() {
    return (10, 20)  // Return tuple
}

let (x, y) = get_point()  // Destructure on assignment
```

This pattern **will work automatically** once tuples are added!

---

## 6. IMPLEMENTATION ROADMAP

### Phase 1: Core Type Addition

**Step 1.1 - Add to ast.h:**
```c
// In NodeType enum (line ~4-16)
NODE_TUPLE,  // Add tuple literal node type

// In ASTNode union (line ~69)
struct { NodeList elements; } tuple;  // Same structure as list
```

**Step 1.2 - Add to interpreter.h:**
```c
// In ValueType enum (line ~6-9)
VAL_TUPLE,  // Add tuple value type

// Create TupleVal struct (line ~27-31):
typedef struct {
    Value *items;      // Pointer to array of Values
    int count;         // Number of elements (immutable count)
} TupleVal;

// In Value union (line ~93-110)
TupleVal tuple_val;    // VAL_TUPLE
```

**Step 1.3 - Add factory function in interpreter.c:**
```c
Value make_tuple(Value *items, int count) {
    Value val;
    val.type = VAL_TUPLE;
    val.as.tuple_val.count = count;
    val.as.tuple_val.items = malloc(count * sizeof(Value));
    for (int i = 0; i < count; i++) {
        // Deep copy strings to prevent aliasing
        if (items[i].type == VAL_STRING)
            val.as.tuple_val.items[i] = make_string(items[i].as.str_val);
        else
            val.as.tuple_val.items[i] = items[i];
    }
    return val;
}
```

### Phase 2: Parser Changes

**Modify parse_primary() in parser.c (around line 182):**

The key challenge: distinguish between:
- `(expr)` - grouped expression
- `(1,)` - single element tuple
- `(1, 2)` - multi-element tuple
- `()` - empty tuple

```c
if (check(parser, TOKEN_LPAREN)) {
    Token t = advance(parser);
    int line = t.line;
    token_free(&t);
    
    // Check for empty tuple ()
    if (check(parser, TOKEN_RPAREN)) {
        Token t2 = consume(parser, TOKEN_RPAREN);
        token_free(&t2);
        ASTNode *tuple = ast_new_tuple(line);
        return tuple;
    }
    
    // Parse first expression
    ASTNode *first_expr = parse_expression(parser);
    
    // Check if there's a comma (makes it a tuple)
    if (check(parser, TOKEN_COMMA)) {
        // It's a tuple
        ASTNode *tuple = ast_new_tuple(line);
        nodelist_push(&tuple->data.tuple.elements, first_expr);
        
        // Parse remaining elements
        while (match(parser, TOKEN_COMMA) && !check(parser, TOKEN_RPAREN)) {
            if (check(parser, TOKEN_SPREAD)) {
                Token spread_tok = advance(parser);
                token_free(&spread_tok);
                ASTNode *spread_expr = parse_expression(parser);
                ASTNode *spread_node = ast_new_spread(spread_expr, line);
                nodelist_push(&tuple->data.tuple.elements, spread_node);
            } else {
                nodelist_push(&tuple->data.tuple.elements, parse_expression(parser));
            }
        }
        
        Token t2 = consume(parser, TOKEN_RPAREN);
        token_free(&t2);
        return tuple;
    } else {
        // It's just a grouped expression
        Token t2 = consume(parser, TOKEN_RPAREN);
        token_free(&t2);
        return first_expr;
    }
}
```

**Add ast_new_tuple() in ast.c:**
```c
ASTNode *ast_new_tuple(int line) {
    ASTNode *node = ast_alloc(NODE_TUPLE, line);
    nodelist_init(&node->data.tuple.elements);
    return node;
}
```

### Phase 3: Interpreter Evaluation

**Add NODE_TUPLE case in eval_node() (interpreter.c, around line 1405):**

```c
case NODE_TUPLE: {
    Value tuple;
    int i, j;
    int total_count = 0;
    
    // First pass: count total elements including spread arrays
    for (i = 0; i < node->data.tuple.elements.count; i++) {
        ASTNode *elem = node->data.tuple.elements.items[i];
        if (elem->type == NODE_SPREAD) {
            Value spread_val = eval_node_env(interp, elem->data.spread.argument, env);
            if (spread_val.type == VAL_LIST) {
                total_count += spread_val.as.list_val.count;
            } else if (spread_val.type == VAL_TUPLE) {
                total_count += spread_val.as.tuple_val.count;
            }
            value_free(&spread_val);
        } else {
            total_count++;
        }
    }
    
    // Create tuple
    tuple.type = VAL_TUPLE;
    tuple.as.tuple_val.count = total_count;
    tuple.as.tuple_val.items = malloc(total_count * sizeof(Value));
    
    // Second pass: fill the tuple
    int idx = 0;
    for (i = 0; i < node->data.tuple.elements.count; i++) {
        ASTNode *elem = node->data.tuple.elements.items[i];
        if (elem->type == NODE_SPREAD) {
            Value spread_val = eval_node_env(interp, elem->data.spread.argument, env);
            
            Value *items = NULL;
            int count = 0;
            if (spread_val.type == VAL_LIST) {
                items = spread_val.as.list_val.items;
                count = spread_val.as.list_val.count;
            } else if (spread_val.type == VAL_TUPLE) {
                items = spread_val.as.tuple_val.items;
                count = spread_val.as.tuple_val.count;
            }
            
            for (j = 0; j < count; j++) {
                if (items[j].type == VAL_STRING) {
                    tuple.as.tuple_val.items[idx++] = make_string(items[j].as.str_val);
                } else {
                    tuple.as.tuple_val.items[idx++] = items[j];
                }
            }
            value_free(&spread_val);
        } else {
            tuple.as.tuple_val.items[idx++] = eval_node_env(interp, elem, env);
        }
    }
    
    return tuple;
}
```

### Phase 4: Value Operations

**Update value_print() in interpreter.c:**
```c
case VAL_TUPLE: {
    printf("(");
    for (int i = 0; i < v->as.tuple_val.count; i++) {
        if (i > 0) printf(", ");
        value_print(&v->as.tuple_val.items[i]);
    }
    // Print trailing comma for single-element tuples
    if (v->as.tuple_val.count == 1) printf(",");
    printf(")");
    break;
}
```

**Update value_to_string() in interpreter.c:**
```c
case VAL_TUPLE: {
    int total_len = 3; // "(" + ")"
    for (int i = 0; i < v->as.tuple_val.count; i++) {
        char *item_str = value_to_string(&v->as.tuple_val.items[i]);
        total_len += strlen(item_str) + 2; // item + ", "
        free(item_str);
    }
    
    char *result = malloc(total_len + 1);
    strcpy(result, "(");
    for (int i = 0; i < v->as.tuple_val.count; i++) {
        if (i > 0) strcat(result, ", ");
        char *item_str = value_to_string(&v->as.tuple_val.items[i]);
        strcat(result, item_str);
        free(item_str);
    }
    if (v->as.tuple_val.count == 1) strcat(result, ",");
    strcat(result, ")");
    return result;
}
```

**Update value_free() in interpreter.c:**
```c
if (v->type == VAL_TUPLE) {
    for (int i = 0; i < v->as.tuple_val.count; i++) {
        value_free(&v->as.tuple_val.items[i]);
    }
    free(v->as.tuple_val.items);
    v->as.tuple_val.items = NULL;
    v->as.tuple_val.count = 0;
}
```

**Update value_equals() in interpreter.c:**
```c
if (a->type == VAL_TUPLE && b->type == VAL_TUPLE) {
    if (a->as.tuple_val.count != b->as.tuple_val.count) return 0;
    for (int i = 0; i < a->as.tuple_val.count; i++) {
        if (!value_equals(&a->as.tuple_val.items[i], &b->as.tuple_val.items[i]))
            return 0;
    }
    return 1;
}
```

### Phase 5: Indexing Support

**Update NODE_INDEX case in interpreter.c:**
```c
case NODE_INDEX: {
    Value obj = eval_node_env(interp, node->data.index_expr.obj, env);
    Value idx = eval_node_env(interp, node->data.index_expr.index, env);
    Value result;
    
    // ... existing LIST handling ...
    
    if (obj.type == VAL_TUPLE) {
        if (idx.type == VAL_INT) {
            long long idx_val = idx.as.int_val;
            if (idx_val >= 0 && idx_val < obj.as.tuple_val.count) {
                result = obj.as.tuple_val.items[idx_val];
                if (result.type == VAL_STRING) {
                    result = make_string(result.as.str_val);
                }
            } else {
                fprintf(stderr, "Runtime error: tuple index out of range\n");
                result = make_null();
            }
        } else {
            fprintf(stderr, "Runtime error: tuple indices must be integers\n");
            result = make_null();
        }
    }
    
    value_free(&obj);
    value_free(&idx);
    return result;
}
```

### Phase 6: Immutability Enforcement

**Prevent tuple element assignment in NODE_ASSIGN case:**
```c
case NODE_ASSIGN: {
    // ... existing code ...
    
    if (node->data.assign.target->type == NODE_INDEX) {
        Value obj = eval_node_env(interp, node->data.assign.target->data.index_expr.obj, env);
        
        if (obj.type == VAL_TUPLE) {
            fprintf(stderr, "Runtime error: cannot assign to tuple element (tuples are immutable) at line %d\n", node->line);
            value_free(&obj);
            return make_null();
        }
        
        value_free(&obj);
    }
    
    // ... rest of assignment logic ...
}
```

### Phase 7: Destructuring Integration

**Update NODE_DESTRUCTURE_ARRAY in interpreter.c to support tuples:**
```c
case NODE_DESTRUCTURE_ARRAY: {
    Value source = eval_node_env(interp, node->data.destructure_array.source, env);
    
    // Allow both lists and tuples to be destructured
    if (source.type != VAL_LIST && source.type != VAL_TUPLE) {
        fprintf(stderr, "Runtime error: cannot destructure non-array/tuple\n");
        value_free(&source);
        return make_null();
    }
    
    Value *items;
    int count;
    if (source.type == VAL_LIST) {
        items = source.as.list_val.items;
        count = source.as.list_val.count;
    } else {
        items = source.as.tuple_val.items;
        count = source.as.tuple_val.count;
    }
    
    // ... rest of destructuring logic using items and count ...
}
```

### Phase 8: Built-in Functions

**Add tuple utility functions in runtime.c:**

```c
// Get tuple length
static Value builtin_tuple_len(Interpreter *interp, Value *args, int argc) {
    if (argc == 0) return make_int(0);
    if (args[0].type != VAL_TUPLE) {
        fprintf(stderr, "Error: tuple_len() expects a tuple\n");
        return make_null();
    }
    return make_int((long long)args[0].as.tuple_val.count);
}

// Convert tuple to list
static Value builtin_tuple_to_list(Interpreter *interp, Value *args, int argc) {
    if (argc == 0 || args[0].type != VAL_TUPLE) {
        fprintf(stderr, "Error: tuple_to_list() expects a tuple\n");
        return make_null();
    }
    
    Value list;
    list.type = VAL_LIST;
    list.as.list_val.count = args[0].as.tuple_val.count;
    list.as.list_val.capacity = args[0].as.tuple_val.count;
    list.as.list_val.items = malloc(list.as.list_val.count * sizeof(Value));
    
    for (int i = 0; i < args[0].as.tuple_val.count; i++) {
        if (args[0].as.tuple_val.items[i].type == VAL_STRING) {
            list.as.list_val.items[i] = make_string(args[0].as.tuple_val.items[i].as.str_val);
        } else {
            list.as.list_val.items[i] = args[0].as.tuple_val.items[i];
        }
    }
    
    return list;
}

// Convert list to tuple
static Value builtin_list_to_tuple(Interpreter *interp, Value *args, int argc) {
    if (argc == 0 || args[0].type != VAL_LIST) {
        fprintf(stderr, "Error: list_to_tuple() expects a list\n");
        return make_null();
    }
    
    return make_tuple(args[0].as.list_val.items, args[0].as.list_val.count);
}

// Check if value is a tuple
static Value builtin_is_tuple(Interpreter *interp, Value *args, int argc) {
    if (argc == 0) return make_bool(0);
    return make_bool(args[0].type == VAL_TUPLE);
}
```

**Register builtins in runtime_init():**
```c
void runtime_init(Interpreter *interp) {
    // ... existing builtins ...
    
    env_set(interp->global_env, "tuple_len", make_builtin(builtin_tuple_len));
    env_set(interp->global_env, "tuple_to_list", make_builtin(builtin_tuple_to_list));
    env_set(interp->global_env, "list_to_tuple", make_builtin(builtin_list_to_tuple));
    env_set(interp->global_env, "is_tuple", make_builtin(builtin_is_tuple));
}
```

---

## 7. TESTING STRATEGY

Create `tests/test_tuples.k`:

```klang
// Test 1: Empty tuple
let t1 = ()
println("Empty tuple:", t1)  // Expected: ()

// Test 2: Single element tuple (with trailing comma)
let t2 = (1,)
println("Single element:", t2)  // Expected: (1,)

// Test 3: Multi-element tuple
let t3 = (1, 2, 3)
println("Multi-element:", t3)  // Expected: (1, 2, 3)

// Test 4: Mixed types
let t4 = (1, "hello", true, null)
println("Mixed types:", t4)  // Expected: (1, "hello", true, null)

// Test 5: Nested tuples
let t5 = (1, (2, 3), 4)
println("Nested:", t5)  // Expected: (1, (2, 3), 4)

// Test 6: Tuple indexing
println("t3[0] =", t3[0])  // Expected: 1
println("t3[1] =", t3[1])  // Expected: 2
println("t3[2] =", t3[2])  // Expected: 3

// Test 7: Destructuring
let (a, b, c) = t3
println("Destructured:", a, b, c)  // Expected: 1 2 3

// Test 8: Rest element in destructuring
let (x, ...rest) = (1, 2, 3, 4, 5)
println("x =", x)  // Expected: 1
println("rest =", rest)  // Expected: [2, 3, 4, 5]

// Test 9: Spread in tuple literal
let t6 = (1, ...(2, 3), 4)
println("With spread:", t6)  // Expected: (1, 2, 3, 4)

// Test 10: Multiple returns via tuple
fn divmod(a, b) {
    return (a / b, a % b)
}
let (quotient, remainder) = divmod(17, 5)
println("17 / 5 =", quotient, "remainder", remainder)  // Expected: 3 remainder 2

// Test 11: Swap using tuples
fn swap(x, y) {
    return (y, x)
}
let (p, q) = swap(5, 10)
println("After swap:", p, q)  // Expected: 10 5

// Test 12: Immutability check (should error)
let t7 = (1, 2, 3)
// t7[0] = 99  // Uncomment to test - should error: "cannot assign to tuple element"

// Test 13: Comparison
let ta = (1, 2, 3)
let tb = (1, 2, 3)
let tc = (1, 2, 4)
println("ta == tb:", ta == tb)  // Expected: true
println("ta == tc:", ta == tc)  // Expected: false

// Test 14: Built-in functions
let t8 = (10, 20, 30)
println("Length:", tuple_len(t8))  // Expected: 3
let list_from_tuple = tuple_to_list(t8)
println("As list:", list_from_tuple)  // Expected: [10, 20, 30]
let tuple_from_list = list_to_tuple([100, 200])
println("From list:", tuple_from_list)  // Expected: (100, 200)
println("Is tuple?", is_tuple(t8))  // Expected: true
println("Is tuple?", is_tuple([1, 2]))  // Expected: false

// Test 15: Grouped expression (not a tuple)
let grouped = (1 + 2)
println("Grouped:", grouped, "type:", type(grouped))  // Expected: 3 type: number

println("\nAll tuple tests completed!")
```

---

## 8. FILES TO MODIFY (Priority Order)

| File | Changes | Priority | Approx Lines |
|------|---------|----------|--------------|
| `include/ast.h` | Add `NODE_TUPLE` to enum, add tuple struct to union | **Critical** | ~5 lines |
| `include/interpreter.h` | Add `VAL_TUPLE` to enum, add `TupleVal` struct, add to Value union | **Critical** | ~10 lines |
| `src/ast.c` | Add `ast_new_tuple()` function | **Critical** | ~10 lines |
| `src/parser.c` | Modify `parse_primary()` to distinguish tuples from grouped expressions | **Critical** | ~40 lines |
| `src/interpreter.c` | Add `NODE_TUPLE` case (~60 lines), update value operations (~50 lines), update destructuring (~10 lines) | **Critical** | ~120 lines |
| `src/runtime.c` | Add tuple built-in functions (~80 lines) | **High** | ~80 lines |
| `tests/test_tuples.k` | Create comprehensive test suite | **High** | ~80 lines |

**Total Estimated LOC:** ~355 lines

---

## 9. ARCHITECTURE STRENGTHS FOR TUPLES

✅ **Already in Place:**
1. **Tagged union Value structure** - easy to add `VAL_TUPLE` variant
2. **Destructuring support** - array patterns already implemented
3. **Spread operator** - already handled in AST evaluation
4. **Array literal parsing** - can be extended for tuple syntax
5. **Two-pass evaluation pattern** - established in NODE_LIST

✅ **Design Patterns to Follow:**
- Use `make_*()` factory functions for all value creation
- Two-pass evaluation for complex expressions (counting then filling)
- Deep copy strings to prevent aliasing bugs
- `value_free()` handles cleanup consistently

---

## 10. KEY DIFFERENCES: TUPLES vs LISTS

| Aspect | Lists `[1, 2, 3]` | Tuples `(1, 2, 3)` |
|--------|-------------------|---------------------|
| **Syntax** | Square brackets | Parentheses + comma |
| **Mutability** | Mutable | Immutable |
| **Storage** | `ListVal` with capacity | `TupleVal` without capacity |
| **Use Case** | Dynamic collections | Fixed structure data |
| **Assignment** | `list[0] = x` allowed | `tuple[0] = x` **error** |
| **Empty literal** | `[]` | `()` |
| **Single element** | `[1]` | `(1,)` (trailing comma required) |

---

## 11. POTENTIAL GOTCHAS

### 11.1 Parser Ambiguity

**Problem:** `(expr)` could be:
- Grouped expression: `(1 + 2)` → `3`
- Single-element tuple: `(1,)` → `(1,)`

**Solution:** Require trailing comma for single-element tuples (Python's approach).

### 11.2 Immutability Enforcement

**Problem:** Without const semantics, tuples could be modified via pointer manipulation.

**Solution:** Check at assignment time in `NODE_ASSIGN` case.

### 11.3 Type Checking

**Problem:** No compile-time type checking exists in KLang.

**Solution:** Runtime checks + clear error messages.

### 11.4 Memory Management

**Problem:** Need to ensure deep copies to prevent aliasing.

**Solution:** Follow existing pattern in `NODE_LIST` evaluation - deep copy strings and nested structures.

---

## 12. SUMMARY TABLE

| Aspect | Current Status | For Tuples | Status |
|--------|----------------|------------|--------|
| **Literals** | `[1,2,3]` → NODE_LIST | `(1,2,3)` → NODE_TUPLE | **To Add** |
| **Runtime Type** | VAL_LIST | VAL_TUPLE | **To Add** |
| **Storage** | ListVal (mutable) | TupleVal (immutable) | **To Add** |
| **Parser** | Complete | Extend parse_primary() | **To Add** |
| **Destructuring** | ✅ Works for arrays | Will work automatically | **Ready** |
| **Indexing** | ✅ Works | Add VAL_TUPLE case | **Minor addition** |
| **Spread** | ✅ Works | Handle in NODE_TUPLE case | **Minor addition** |
| **Comparison** | ✅ Works | Add VAL_TUPLE case | **Minor addition** |
| **Functions Return** | Single Value | Can return tuple | **Works** |

---

## Conclusion

The KLang architecture is **well-prepared** for tuple implementation. The main work involves:
1. Adding the `VAL_TUPLE` type to the runtime
2. Implementing parser logic to distinguish `(1,)` and `(1, 2)` from `(expr)`
3. Adapting the NODE_LIST evaluation pattern for NODE_TUPLE
4. Enforcing immutability at assignment time

The existing destructuring, spread operators, and indexing systems will largely work automatically once the core tuple type is added. Estimated implementation time: **4-6 hours** for a complete, well-tested implementation.
