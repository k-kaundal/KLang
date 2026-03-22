# KLang Tuple Implementation - Visual Guide

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                        KLang Compiler Pipeline                       │
├─────────────────────────────────────────────────────────────────────┤
│                                                                       │
│  Source Code  →  Lexer  →  Parser  →  AST  →  Interpreter  →  Output│
│     (1,2,3)      Tokens    ASTNode   Tree     Evaluation    (1, 2, 3)│
│                                                                       │
└─────────────────────────────────────────────────────────────────────┘
```

## Current Collection Types Flow

### Arrays/Lists: `[1, 2, 3]`
```
┌────────────────┐      ┌──────────────┐      ┌──────────────┐      ┌──────────────┐
│ Lexer          │      │ Parser       │      │ AST          │      │ Runtime      │
├────────────────┤      ├──────────────┤      ├──────────────┤      ├──────────────┤
│ TOKEN_LBRACKET │  →   │ parse_       │  →   │ NODE_LIST    │  →   │ VAL_LIST     │
│ TOKEN_NUMBER   │      │   primary()  │      │              │      │              │
│ TOKEN_COMMA    │      │              │      │ data.list.   │      │ ListVal {    │
│ TOKEN_NUMBER   │      │ Recognizes [ │      │   elements   │      │   items[]    │
│ TOKEN_RBRACKET │      │              │      │              │      │   count      │
│                │      │              │      │              │      │   capacity   │
│                │      │              │      │              │      │ }            │
└────────────────┘      └──────────────┘      └──────────────┘      └──────────────┘
```

### Objects: `{key: value}`
```
┌────────────────┐      ┌──────────────┐      ┌──────────────┐      ┌──────────────┐
│ Lexer          │      │ Parser       │      │ AST          │      │ Runtime      │
├────────────────┤      ├──────────────┤      ├──────────────┤      ├──────────────┤
│ TOKEN_LBRACE   │  →   │ parse_       │  →   │ NODE_OBJECT  │  →   │ VAL_OBJECT   │
│ TOKEN_IDENT    │      │   primary()  │      │              │      │              │
│ TOKEN_COLON    │      │              │      │ data.object. │      │ ObjectVal {  │
│ TOKEN_NUMBER   │      │ Recognizes { │      │   props[]    │      │   fields     │
│ TOKEN_RBRACE   │      │              │      │              │      │   methods    │
│                │      │              │      │              │      │ }            │
└────────────────┘      └──────────────┘      └──────────────┘      └──────────────┘
```

### Grouped Expression (Current): `(1 + 2)`
```
┌────────────────┐      ┌──────────────┐      ┌──────────────┐      ┌──────────────┐
│ Lexer          │      │ Parser       │      │ AST          │      │ Runtime      │
├────────────────┤      ├──────────────┤      ├──────────────┤      ├──────────────┤
│ TOKEN_LPAREN   │  →   │ parse_       │  →   │ NODE_BINOP   │  →   │ VAL_INT      │
│ TOKEN_NUMBER   │      │   primary()  │      │              │      │              │
│ TOKEN_PLUS     │      │              │      │ (unwrapped   │      │ int_val = 3  │
│ TOKEN_NUMBER   │      │ Just grouping│      │  expression) │      │              │
│ TOKEN_RPAREN   │      │              │      │              │      │              │
└────────────────┘      └──────────────┘      └──────────────┘      └──────────────┘
```

## NEW: Tuple Implementation `(1, 2, 3)`

```
┌────────────────┐      ┌──────────────┐      ┌──────────────┐      ┌──────────────┐
│ Lexer          │      │ Parser       │      │ AST          │      │ Runtime      │
│ (No changes)   │      │ (MODIFIED)   │      │ (NEW)        │      │ (NEW)        │
├────────────────┤      ├──────────────┤      ├──────────────┤      ├──────────────┤
│ TOKEN_LPAREN   │  →   │ parse_       │  →   │ NODE_TUPLE   │  →   │ VAL_TUPLE    │
│ TOKEN_NUMBER   │      │   primary()  │      │              │      │              │
│ TOKEN_COMMA ✓  │      │              │      │ data.tuple.  │      │ TupleVal {   │
│ TOKEN_NUMBER   │      │ Check comma! │      │   elements   │      │   items[]    │
│ TOKEN_COMMA    │      │ If yes→tuple │      │              │      │   count      │
│ TOKEN_NUMBER   │      │ If no→group  │      │              │      │ }            │
│ TOKEN_RPAREN   │      │              │      │              │      │              │
└────────────────┘      └──────────────┘      └──────────────┘      └──────────────┘
```

## Key Decision Point in Parser

```
parse_primary() at line 182:
    if (TOKEN_LPAREN) {
        │
        ├─── Next token is RPAREN? ──→ Empty tuple ()
        │
        ├─── Parse first expression
        │    │
        │    ├─── Next token is COMMA? ──→ TUPLE (1,) or (1, 2)
        │    │                                │
        │    │                                └─→ Continue parsing elements
        │    │
        │    └─── No comma? ──→ GROUPED (1 + 2)
        │                         │
        │                         └─→ Return expression unwrapped
        │
        └─── Consume RPAREN
    }
```

## Data Structure Comparison

```
┌─────────────────────────────────────────────────────────────────────┐
│                         Memory Layout                                 │
├─────────────────────────────────────────────────────────────────────┤

List: [1, 2, 3]
┌──────────────────────────────────────┐
│ Value (on stack)                     │
├──────────────────────────────────────┤
│ type = VAL_LIST                      │
│ as.list_val:                         │
│   ├─ items ───┐                      │
│   ├─ count = 3│                      │
│   └─ capacity = 8                    │
└────────────────│──────────────────────┘
                 │
                 └──→ ┌───────────────────────────┐
                      │ Heap array (8 slots)      │
                      ├───────────────────────────┤
                      │ [0]: Value{type=VAL_INT}  │
                      │ [1]: Value{type=VAL_INT}  │
                      │ [2]: Value{type=VAL_INT}  │
                      │ [3]: (unused)             │
                      │ ...                       │
                      └───────────────────────────┘
                      
Tuple: (1, 2, 3)
┌──────────────────────────────────────┐
│ Value (on stack)                     │
├──────────────────────────────────────┤
│ type = VAL_TUPLE                     │
│ as.tuple_val:                        │
│   ├─ items ───┐                      │
│   └─ count = 3│ (no capacity field)  │
└────────────────│──────────────────────┘
                 │
                 └──→ ┌───────────────────────────┐
                      │ Heap array (exact size)   │
                      ├───────────────────────────┤
                      │ [0]: Value{type=VAL_INT}  │
                      │ [1]: Value{type=VAL_INT}  │
                      │ [2]: Value{type=VAL_INT}  │
                      └───────────────────────────┘
```

## AST Node Structure

```
Current: ASTNode for NODE_LIST
┌─────────────────────────────────────┐
│ ASTNode                             │
├─────────────────────────────────────┤
│ type = NODE_LIST                    │
│ line = 42                           │
│ data.list:                          │
│   └─ elements ───┐                  │
└───────────────────│──────────────────┘
                    │
                    └──→ NodeList {
                           items = [ptr, ptr, ptr, ...]
                           count = 3
                           capacity = 8
                         }
                         Each ptr points to an ASTNode

New: ASTNode for NODE_TUPLE (same structure!)
┌─────────────────────────────────────┐
│ ASTNode                             │
├─────────────────────────────────────┤
│ type = NODE_TUPLE                   │
│ line = 42                           │
│ data.tuple:                         │
│   └─ elements ───┐                  │
└───────────────────│──────────────────┘
                    │
                    └──→ NodeList {
                           items = [ptr, ptr, ptr, ...]
                           count = 3
                           capacity = 8
                         }
                         Each ptr points to an ASTNode
```

## Evaluation Flow

```
NODE_TUPLE Evaluation (interpreter.c)

Step 1: Count elements
┌────────────────────────────────────────┐
│ for each element in tuple AST:        │
│   if element is NODE_SPREAD:          │
│     ├─ Evaluate spread expression     │
│     ├─ Get count from result          │
│     └─ Add to total_count             │
│   else:                                │
│     └─ Add 1 to total_count           │
└────────────────────────────────────────┘
         │
         ▼
Step 2: Allocate exact size
┌────────────────────────────────────────┐
│ tuple.type = VAL_TUPLE                │
│ tuple.as.tuple_val.count = total_count│
│ tuple.as.tuple_val.items = malloc()   │
└────────────────────────────────────────┘
         │
         ▼
Step 3: Fill elements
┌────────────────────────────────────────┐
│ idx = 0                                │
│ for each element in tuple AST:        │
│   if element is NODE_SPREAD:          │
│     ├─ Evaluate spread expression     │
│     ├─ Copy each item (deep copy str) │
│     └─ Increment idx                  │
│   else:                                │
│     ├─ Evaluate element               │
│     ├─ Store at tuple.items[idx]      │
│     └─ Increment idx                  │
└────────────────────────────────────────┘
         │
         ▼
Step 4: Return tuple value
┌────────────────────────────────────────┐
│ return tuple                           │
└────────────────────────────────────────┘
```

## Immutability Enforcement

```
Assignment Check Flow

let t = (1, 2, 3)
t[0] = 99  ← This line

┌──────────────────────────────────────────┐
│ Parser: NODE_ASSIGN                      │
│   target: NODE_INDEX                     │
│            obj: NODE_IDENT (t)           │
│            index: NODE_NUMBER (0)        │
│   value: NODE_NUMBER (99)                │
└──────────────────────────────────────────┘
         │
         ▼
┌──────────────────────────────────────────┐
│ Interpreter: eval NODE_ASSIGN            │
│                                           │
│ 1. Check if target is NODE_INDEX         │
│    ├─ Yes: evaluate obj                  │
│    └─ Check obj.type                     │
│        │                                  │
│        ├─ VAL_LIST? Allow assignment     │
│        │                                  │
│        └─ VAL_TUPLE? ────→ ERROR!        │
│           │                               │
│           └─ "cannot assign to tuple     │
│               element (immutable)"       │
└──────────────────────────────────────────┘
```

## Destructuring Flow

```
let (x, y) = (10, 20)

Step 1: Parser creates
┌─────────────────────────────────────────┐
│ NODE_DESTRUCTURE_ARRAY                  │
│   elements: [                           │
│     NODE_DESTRUCTURE_ELEMENT {          │
│       name: "x"                         │
│     },                                  │
│     NODE_DESTRUCTURE_ELEMENT {          │
│       name: "y"                         │
│     }                                   │
│   ]                                     │
│   source: NODE_TUPLE {                  │
│     elements: [NODE_NUMBER(10),         │
│                NODE_NUMBER(20)]         │
│   }                                     │
└─────────────────────────────────────────┘
         │
         ▼
Step 2: Interpreter evaluates
┌─────────────────────────────────────────┐
│ 1. Evaluate source → VAL_TUPLE          │
│                                          │
│ 2. Check type: VAL_LIST or VAL_TUPLE?   │
│    ├─ If VAL_LIST: use list_val.items   │
│    └─ If VAL_TUPLE: use tuple_val.items │
│                                          │
│ 3. Extract items pointer and count      │
│                                          │
│ 4. For each destructure element:        │
│    ├─ Get value from items[i]           │
│    └─ Bind to variable name             │
│                                          │
│ Result: x = 10, y = 20                  │
└─────────────────────────────────────────┘
```

## Type System Integration

```
┌────────────────────────────────────────────────────────────────┐
│                    ValueType Enum                               │
├────────────────────────────────────────────────────────────────┤
│ VAL_INT      → long long int_val                               │
│ VAL_FLOAT    → double float_val                                │
│ VAL_STRING   → char *str_val                                   │
│ VAL_BOOL     → int bool_val                                    │
│ VAL_NULL     → (no data)                                       │
│ VAL_FUNCTION → FunctionVal func_val                            │
│ VAL_LIST     → ListVal list_val {items, count, capacity}       │
│ VAL_TUPLE    → TupleVal tuple_val {items, count} ← NEW         │
│ VAL_BUILTIN  → BuiltinFn builtin                               │
│ VAL_CLASS    → ClassVal class_val                              │
│ VAL_OBJECT   → ObjectVal object_val                            │
│ VAL_METHOD   → MethodVal method_val                            │
│ VAL_PROMISE  → PromiseVal promise_val                          │
│ VAL_MODULE   → ModuleVal module_val                            │
│ VAL_GENERATOR→ GeneratorVal generator_val                      │
└────────────────────────────────────────────────────────────────┘
```

## Function Return Pattern

```
Before (workaround with array):
┌──────────────────────────────────────┐
│ fn get_point() {                     │
│     return [10, 20]  // Array        │
│ }                                    │
│                                      │
│ let [x, y] = get_point()             │
└──────────────────────────────────────┘

After (proper with tuple):
┌──────────────────────────────────────┐
│ fn get_point() {                     │
│     return (10, 20)  // Tuple        │
│ }                                    │
│                                      │
│ let (x, y) = get_point()             │
└──────────────────────────────────────┘
```

## Complete Example Flow

```
Source: let point = (10, 20)

LEXER
┌──────────────────────────────────────┐
│ TOKEN_LET                            │
│ TOKEN_IDENT("point")                 │
│ TOKEN_ASSIGN                         │
│ TOKEN_LPAREN                         │
│ TOKEN_NUMBER(10)                     │
│ TOKEN_COMMA          ← Key!          │
│ TOKEN_NUMBER(20)                     │
│ TOKEN_RPAREN                         │
└──────────────────────────────────────┘
         │
         ▼
PARSER
┌──────────────────────────────────────┐
│ NODE_LET {                           │
│   name: "point"                      │
│   value: NODE_TUPLE {                │
│     elements: [                      │
│       NODE_NUMBER(10),               │
│       NODE_NUMBER(20)                │
│     ]                                │
│   }                                  │
│ }                                    │
└──────────────────────────────────────┘
         │
         ▼
INTERPRETER
┌──────────────────────────────────────┐
│ 1. Evaluate NODE_TUPLE               │
│    ├─ Count: 2 elements              │
│    ├─ Allocate: 2 Values             │
│    └─ Fill: [VAL_INT(10),            │
│              VAL_INT(20)]            │
│                                      │
│ 2. Result: Value {                   │
│      type: VAL_TUPLE                 │
│      as.tuple_val: {                 │
│        items: [...]                  │
│        count: 2                      │
│      }                               │
│    }                                 │
│                                      │
│ 3. Bind to variable "point"          │
└──────────────────────────────────────┘
         │
         ▼
OUTPUT
┌──────────────────────────────────────┐
│ println(point) → "(10, 20)"          │
│ println(point[0]) → "10"             │
│ let (x, y) = point                   │
│ println(x) → "10"                    │
│ println(y) → "20"                    │
└──────────────────────────────────────┘
```

## Code Modifications Summary

```
┌─────────────────────────────────────────────────────────────┐
│ File: src/ast.h                                             │
├─────────────────────────────────────────────────────────────┤
│ Line 9: Add NODE_TUPLE to enum                             │
│ Line 69: Add struct { NodeList elements; } tuple;          │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ File: src/interpreter.h                                     │
├─────────────────────────────────────────────────────────────┤
│ Line 7: Add VAL_TUPLE to enum                              │
│ Line 27-31: Add TupleVal struct definition                 │
│ Line 108: Add TupleVal tuple_val; to Value union           │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ File: src/ast.c                                             │
├─────────────────────────────────────────────────────────────┤
│ Add ast_new_tuple() function (~10 lines)                   │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ File: src/parser.c                                          │
├─────────────────────────────────────────────────────────────┤
│ Line 182-190: Replace simple grouping logic                │
│               with tuple detection (~40 lines)              │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ File: src/interpreter.c                                     │
├─────────────────────────────────────────────────────────────┤
│ Add NODE_TUPLE eval case (~60 lines)                       │
│ Update value_print() (~10 lines)                           │
│ Update value_to_string() (~15 lines)                       │
│ Update value_free() (~10 lines)                            │
│ Update value_equals() (~10 lines)                          │
│ Update NODE_INDEX case (~15 lines)                         │
│ Update NODE_ASSIGN case (~10 lines)                        │
│ Update NODE_DESTRUCTURE_ARRAY (~10 lines)                  │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ File: src/runtime.c                                         │
├─────────────────────────────────────────────────────────────┤
│ Add builtin_tuple_len() (~15 lines)                        │
│ Add builtin_tuple_to_list() (~20 lines)                    │
│ Add builtin_list_to_tuple() (~15 lines)                    │
│ Add builtin_is_tuple() (~5 lines)                          │
│ Register builtins in runtime_init() (~4 lines)             │
└─────────────────────────────────────────────────────────────┘

TOTAL: ~355 lines across 6 files
```

## Syntax Examples with AST

```klang
// Example 1: Empty tuple
let t = ()

AST: NODE_LET {
  name: "t",
  value: NODE_TUPLE { elements: [] }
}
Runtime: VAL_TUPLE { items: NULL, count: 0 }

// Example 2: Single element
let t = (42,)

AST: NODE_LET {
  name: "t",
  value: NODE_TUPLE { 
    elements: [NODE_NUMBER(42)]
  }
}
Runtime: VAL_TUPLE { items: [VAL_INT(42)], count: 1 }

// Example 3: Multiple elements
let t = (1, "hello", true)

AST: NODE_LET {
  name: "t",
  value: NODE_TUPLE {
    elements: [
      NODE_NUMBER(1),
      NODE_STRING("hello"),
      NODE_BOOL(true)
    ]
  }
}
Runtime: VAL_TUPLE {
  items: [
    VAL_INT(1),
    VAL_STRING("hello"),
    VAL_BOOL(true)
  ],
  count: 3
}

// Example 4: With spread
let t = (1, ...(2, 3), 4)

AST: NODE_LET {
  name: "t",
  value: NODE_TUPLE {
    elements: [
      NODE_NUMBER(1),
      NODE_SPREAD {
        argument: NODE_TUPLE {
          elements: [NODE_NUMBER(2), NODE_NUMBER(3)]
        }
      },
      NODE_NUMBER(4)
    ]
  }
}
Runtime: VAL_TUPLE {
  items: [VAL_INT(1), VAL_INT(2), VAL_INT(3), VAL_INT(4)],
  count: 4
}
```

This visual guide should help you understand exactly how tuples will integrate into KLang's architecture!
