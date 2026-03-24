# KLang OOP Implementation Plan

## Executive Summary

This document provides a detailed, step-by-step implementation plan for adding Object-Oriented Programming (OOP) features to KLang. Based on comprehensive codebase analysis, this plan outlines what needs to be added, what's already implemented, and the specific changes required for each component.

---

## Table of Contents

1. [Current State Analysis](#current-state-analysis)
2. [OOP Features to Add](#oop-features-to-add)
3. [Implementation Phases](#implementation-phases)
4. [Detailed Implementation Steps](#detailed-implementation-steps)
5. [Files to Modify](#files-to-modify)
6. [Testing Strategy](#testing-strategy)
7. [Example Code](#example-code)

---

## Current State Analysis

### ✅ What's Already Implemented

| Feature | Status | Details |
|---------|--------|---------|
| **Functions with Parameters** | ✅ Complete | Syntax: `fn name(param: type) -> type { ... }` |
| **Function Calls** | ✅ Complete | Supports recursive calls, closures |
| **Type Annotations** | ⚠️ Partial | String-based storage, no enforcement |
| **First-class Functions** | ✅ Complete | Functions as values, closures |
| **Lexical Scoping** | ✅ Complete | Environment chain with parent pointers |
| **Arrays/Lists** | ✅ Complete | Dynamic arrays with indexing |
| **Value Types** | ✅ Complete | 8 types: int, float, string, bool, null, function, list, builtin |

### ❌ What's Missing (OOP Features)

| Feature | Current Status | Implementation Needed |
|---------|----------------|----------------------|
| **Classes** | Not defined | Full implementation |
| **Objects/Instances** | Not defined | Full implementation |
| **Methods** | Not defined | Full implementation |
| **Constructors** | Not defined | Full implementation |
| **this/self keyword** | Not defined | Full implementation |
| **Instance Variables** | Not defined | Full implementation |
| **Inheritance** | Not defined | Full implementation |
| **Method Overriding** | Not defined | Full implementation |
| **Access Modifiers** | Not defined | Optional (public/private) |
| **Static Methods** | Not defined | Optional |
| **Struct Definition** | ⚠️ Placeholder only | Complete implementation |
| **Member Access (dot operator)** | Not defined | Full implementation |

### 🔍 Placeholder Items Found

- **NODE_STRUCT_DEF**: Defined in AST enum but no supporting infrastructure
- **NODE_IMPORT**: Defined in AST enum but not implemented
- No lexer tokens for: `class`, `struct`, `this`, `self`, `new`, `extends`
- No parser functions for OOP constructs
- No value types for objects/instances

---

## OOP Features to Add

### Phase 1: Basic Classes & Objects (Minimal OOP)

1. **Class Definition**
   - Syntax: `class ClassName { ... }`
   - Instance variables (fields)
   - Methods (functions bound to class)

2. **Object Instantiation**
   - Syntax: `new ClassName(args)`
   - Constructor method support
   - Memory allocation for instances

3. **Member Access**
   - Dot operator: `object.field`, `object.method(args)`
   - `this` keyword for self-reference

4. **Basic Methods**
   - Instance methods with implicit `this`
   - Method calls on objects

### Phase 2: Constructors & Initialization

1. **Constructor Syntax**
   - Special method: `init(params) { ... }`
   - Called automatically on instantiation
   - Parameter passing to new

2. **Field Initialization**
   - Default values
   - Constructor-based initialization

### Phase 3: Inheritance & Polymorphism

1. **Single Inheritance**
   - Syntax: `class Child extends Parent { ... }`
   - Method inheritance
   - Field inheritance

2. **Method Overriding**
   - Override parent methods
   - `super` keyword for parent access

3. **Type Checking**
   - Is-a relationships
   - Type compatibility checks

### Phase 4: Advanced Features (Optional)

1. **Static Members**
   - Class-level variables and methods
   - Access via `ClassName.member`

2. **Access Modifiers**
   - `private` fields/methods
   - `public` by default

3. **Abstract Classes/Interfaces** (Future)

---

## Implementation Phases

### Phase 1: Foundation (Weeks 1-2)
**Goal**: Basic class definition, object creation, and member access

- Add lexer tokens
- Extend AST with class nodes
- Implement parser for class syntax
- Add object value type
- Implement dot operator
- Basic object creation

### Phase 2: Methods & This (Week 3)
**Goal**: Instance methods and this keyword

- Bind methods to instances
- Implement `this` resolution
- Method call evaluation
- Constructor support

### Phase 3: Inheritance (Week 4)
**Goal**: Single inheritance and method overriding

- Extend class definition for parent
- Implement inheritance chain
- Method override mechanism
- `super` keyword support

### Phase 4: Testing & Refinement (Week 5)
**Goal**: Comprehensive testing and bug fixes

- Unit tests for all OOP features
- Integration tests
- Example programs
- Documentation updates

---

## Detailed Implementation Steps

### STEP 1: Lexer Changes (`src/lexer.h`, `src/lexer.c`)

#### 1.1 Add New Token Types (`lexer.h`)

```c
typedef enum {
    // ... existing tokens ...
    TOKEN_FN, TOKEN_LET, TOKEN_IF, TOKEN_ELSE, /* ... */
    
    // NEW OOP TOKENS
    TOKEN_CLASS,        // 'class'
    TOKEN_NEW,          // 'new'
    TOKEN_THIS,         // 'this'
    TOKEN_EXTENDS,      // 'extends'
    TOKEN_SUPER,        // 'super'
    TOKEN_STRUCT,       // 'struct' (for struct support)
    TOKEN_DOT,          // '.' (already exists? verify!)
    
    // ... rest of tokens ...
} TokenType;
```

#### 1.2 Update Keyword Recognition (`lexer.c`)

**Location**: In `lexer.c`, find the keyword checking function (likely in `lex_identifier()` or similar)

**Add**:
```c
if (strcmp(lexeme, "class") == 0) return TOKEN_CLASS;
if (strcmp(lexeme, "new") == 0) return TOKEN_NEW;
if (strcmp(lexeme, "this") == 0) return TOKEN_THIS;
if (strcmp(lexeme, "extends") == 0) return TOKEN_EXTENDS;
if (strcmp(lexeme, "super") == 0) return TOKEN_SUPER;
if (strcmp(lexeme, "struct") == 0) return TOKEN_STRUCT;
```

#### 1.3 Ensure Dot Operator Token Exists

**Check if TOKEN_DOT already exists**. If not, add:
```c
case '.':
    return make_token(lexer, TOKEN_DOT);
```

---

### STEP 2: AST Changes (`src/ast.h`, `src/ast.c`)

#### 2.1 Add New Node Types (`ast.h`)

```c
typedef enum {
    // ... existing nodes ...
    NODE_FUNC_DEF, NODE_STRUCT_DEF, NODE_IMPORT,
    
    // NEW OOP NODES
    NODE_CLASS_DEF,      // class ClassName extends Parent { ... }
    NODE_NEW,            // new ClassName(args)
    NODE_MEMBER_ACCESS,  // object.field or object.method
    NODE_THIS,           // this keyword
    NODE_SUPER,          // super keyword
    
    NODE_LIST
} NodeType;
```

#### 2.2 Add AST Node Structures (`ast.h`)

**Add to ASTNode union**:
```c
struct ASTNode {
    NodeType type;
    int line;
    union {
        // ... existing members ...
        
        // CLASS DEFINITION
        struct {
            char *name;              // Class name
            char *parent_name;       // Parent class (NULL if no inheritance)
            NodeList fields;         // Instance variable declarations
            NodeList methods;        // Method definitions (NODE_FUNC_DEF)
        } class_def;
        
        // OBJECT INSTANTIATION
        struct {
            char *class_name;        // Class to instantiate
            NodeList args;           // Constructor arguments
        } new_expr;
        
        // MEMBER ACCESS (dot operator)
        struct {
            ASTNode *object;         // Object expression (left of dot)
            char *member_name;       // Field or method name (right of dot)
        } member_access;
        
        // THIS KEYWORD
        struct {
            // No data needed, just marker
        } this_expr;
        
        // SUPER KEYWORD
        struct {
            char *method_name;       // For super.method() calls
        } super_expr;
        
        // STRUCT DEFINITION (implement the placeholder)
        struct {
            char *name;              // Struct name
            NodeList fields;         // Field declarations (NODE_LET)
        } struct_def;
        
    } data;
};
```

#### 2.3 Add AST Constructor Functions (`ast.h` and `ast.c`)

**Declarations in `ast.h`**:
```c
ASTNode *ast_new_class_def(const char *name, const char *parent_name, int line);
ASTNode *ast_new_new(const char *class_name, int line);
ASTNode *ast_new_member_access(ASTNode *object, const char *member_name, int line);
ASTNode *ast_new_this(int line);
ASTNode *ast_new_super(const char *method_name, int line);
ASTNode *ast_new_struct_def(const char *name, int line);
```

**Implementations in `ast.c`**:
```c
ASTNode *ast_new_class_def(const char *name, const char *parent_name, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_CLASS_DEF;
    node->line = line;
    node->data.class_def.name = strdup(name);
    node->data.class_def.parent_name = parent_name ? strdup(parent_name) : NULL;
    nodelist_init(&node->data.class_def.fields);
    nodelist_init(&node->data.class_def.methods);
    return node;
}

ASTNode *ast_new_new(const char *class_name, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_NEW;
    node->line = line;
    node->data.new_expr.class_name = strdup(class_name);
    nodelist_init(&node->data.new_expr.args);
    return node;
}

ASTNode *ast_new_member_access(ASTNode *object, const char *member_name, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_MEMBER_ACCESS;
    node->line = line;
    node->data.member_access.object = object;
    node->data.member_access.member_name = strdup(member_name);
    return node;
}

ASTNode *ast_new_this(int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_THIS;
    node->line = line;
    return node;
}

ASTNode *ast_new_super(const char *method_name, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_SUPER;
    node->line = line;
    node->data.super_expr.method_name = method_name ? strdup(method_name) : NULL;
    return node;
}

ASTNode *ast_new_struct_def(const char *name, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_STRUCT_DEF;
    node->line = line;
    node->data.struct_def.name = strdup(name);
    nodelist_init(&node->data.struct_def.fields);
    return node;
}
```

#### 2.4 Update ast_free() (`ast.c`)

**Add cases to handle new node types** in the `ast_free()` function:
```c
void ast_free(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        // ... existing cases ...
        
        case NODE_CLASS_DEF:
            free(node->data.class_def.name);
            if (node->data.class_def.parent_name)
                free(node->data.class_def.parent_name);
            nodelist_free(&node->data.class_def.fields);
            nodelist_free(&node->data.class_def.methods);
            break;
            
        case NODE_NEW:
            free(node->data.new_expr.class_name);
            nodelist_free(&node->data.new_expr.args);
            break;
            
        case NODE_MEMBER_ACCESS:
            ast_free(node->data.member_access.object);
            free(node->data.member_access.member_name);
            break;
            
        case NODE_THIS:
            // No data to free
            break;
            
        case NODE_SUPER:
            if (node->data.super_expr.method_name)
                free(node->data.super_expr.method_name);
            break;
            
        case NODE_STRUCT_DEF:
            free(node->data.struct_def.name);
            nodelist_free(&node->data.struct_def.fields);
            break;
            
        // ... rest of cases ...
    }
    free(node);
}
```

---

### STEP 3: Parser Changes (`src/parser.h`, `src/parser.c`)

#### 3.1 Add Parser Function Declarations (`parser.h`)

```c
ASTNode *parse_class_def(Parser *parser);
ASTNode *parse_struct_def(Parser *parser);
```

#### 3.2 Update parse_statement() (`parser.c`)

**Location**: Find `parse_statement()` function (around line 434)

**Add cases for new keywords**:
```c
static ASTNode *parse_statement(Parser *parser) {
    // ... existing code ...
    
    if (match(parser, TOKEN_CLASS)) {
        return parse_class_def(parser);
    }
    
    if (match(parser, TOKEN_STRUCT)) {
        return parse_struct_def(parser);
    }
    
    // ... rest of function ...
}
```

#### 3.3 Implement parse_class_def() (`parser.c`)

```c
// Parse: class ClassName [extends ParentName] { fields... methods... }
static ASTNode *parse_class_def(Parser *parser) {
    int line = parser->prev.line;
    
    // Expect class name
    if (!match(parser, TOKEN_IDENT)) {
        error(parser, "Expected class name");
        return NULL;
    }
    char *class_name = strdup(parser->prev.value);
    
    // Optional: extends ParentName
    char *parent_name = NULL;
    if (match(parser, TOKEN_EXTENDS)) {
        if (!match(parser, TOKEN_IDENT)) {
            error(parser, "Expected parent class name after 'extends'");
            free(class_name);
            return NULL;
        }
        parent_name = strdup(parser->prev.value);
    }
    
    // Create class definition node
    ASTNode *class_node = ast_new_class_def(class_name, parent_name, line);
    free(class_name);
    if (parent_name) free(parent_name);
    
    // Expect opening brace
    if (!match(parser, TOKEN_LBRACE)) {
        error(parser, "Expected '{' after class name");
        ast_free(class_node);
        return NULL;
    }
    
    // Parse class body: fields and methods
    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        if (match(parser, TOKEN_FN)) {
            // Parse method (function definition)
            ASTNode *method = parse_func_def(parser);
            if (method) {
                nodelist_push(&class_node->data.class_def.methods, method);
            }
        } else if (match(parser, TOKEN_LET)) {
            // Parse field declaration
            ASTNode *field = parse_let(parser);
            if (field) {
                nodelist_push(&class_node->data.class_def.fields, field);
            }
        } else {
            error(parser, "Expected method or field declaration in class body");
            advance(parser); // Skip problematic token
        }
    }
    
    // Expect closing brace
    if (!match(parser, TOKEN_RBRACE)) {
        error(parser, "Expected '}' after class body");
        ast_free(class_node);
        return NULL;
    }
    
    return class_node;
}
```

#### 3.4 Implement parse_struct_def() (`parser.c`)

```c
// Parse: struct StructName { fields... }
static ASTNode *parse_struct_def(Parser *parser) {
    int line = parser->prev.line;
    
    // Expect struct name
    if (!match(parser, TOKEN_IDENT)) {
        error(parser, "Expected struct name");
        return NULL;
    }
    char *struct_name = strdup(parser->prev.value);
    
    ASTNode *struct_node = ast_new_struct_def(struct_name, line);
    free(struct_name);
    
    // Expect opening brace
    if (!match(parser, TOKEN_LBRACE)) {
        error(parser, "Expected '{' after struct name");
        ast_free(struct_node);
        return NULL;
    }
    
    // Parse struct fields
    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        if (match(parser, TOKEN_LET)) {
            ASTNode *field = parse_let(parser);
            if (field) {
                nodelist_push(&struct_node->data.struct_def.fields, field);
            }
        } else {
            error(parser, "Expected field declaration in struct body");
            advance(parser);
        }
    }
    
    // Expect closing brace
    if (!match(parser, TOKEN_RBRACE)) {
        error(parser, "Expected '}' after struct body");
        ast_free(struct_node);
        return NULL;
    }
    
    return struct_node;
}
```

#### 3.5 Update parse_primary() for 'new' and 'this' (`parser.c`)

**Location**: Find `parse_primary()` function (handles literals and basic expressions)

**Add cases**:
```c
static ASTNode *parse_primary(Parser *parser) {
    int line = parser->current.line;
    
    // ... existing cases for numbers, strings, etc. ...
    
    // NEW keyword: new ClassName(args)
    if (match(parser, TOKEN_NEW)) {
        if (!match(parser, TOKEN_IDENT)) {
            error(parser, "Expected class name after 'new'");
            return NULL;
        }
        char *class_name = strdup(parser->prev.value);
        ASTNode *new_node = ast_new_new(class_name, line);
        free(class_name);
        
        // Parse constructor arguments
        if (match(parser, TOKEN_LPAREN)) {
            if (!check(parser, TOKEN_RPAREN)) {
                do {
                    ASTNode *arg = parse_expression(parser);
                    if (arg) {
                        nodelist_push(&new_node->data.new_expr.args, arg);
                    }
                } while (match(parser, TOKEN_COMMA));
            }
            
            if (!match(parser, TOKEN_RPAREN)) {
                error(parser, "Expected ')' after constructor arguments");
                ast_free(new_node);
                return NULL;
            }
        }
        
        return new_node;
    }
    
    // THIS keyword
    if (match(parser, TOKEN_THIS)) {
        return ast_new_this(line);
    }
    
    // SUPER keyword
    if (match(parser, TOKEN_SUPER)) {
        // super.methodName() syntax
        if (match(parser, TOKEN_DOT)) {
            if (!match(parser, TOKEN_IDENT)) {
                error(parser, "Expected method name after 'super.'");
                return NULL;
            }
            char *method_name = strdup(parser->prev.value);
            ASTNode *super_node = ast_new_super(method_name, line);
            free(method_name);
            return super_node;
        } else {
            error(parser, "Expected '.' after 'super'");
            return NULL;
        }
    }
    
    // ... rest of function ...
}
```

#### 3.6 Update parse_postfix() for Member Access (`parser.c`)

**Location**: Find `parse_postfix()` function (handles `obj[index]`, `func(args)`)

**Add dot operator handling**:
```c
static ASTNode *parse_postfix(Parser *parser) {
    ASTNode *expr = parse_primary(parser);
    
    while (1) {
        // ... existing code for function calls and indexing ...
        
        // MEMBER ACCESS: object.member
        if (match(parser, TOKEN_DOT)) {
            int line = parser->prev.line;
            
            if (!match(parser, TOKEN_IDENT)) {
                error(parser, "Expected member name after '.'");
                ast_free(expr);
                return NULL;
            }
            
            char *member_name = strdup(parser->prev.value);
            expr = ast_new_member_access(expr, member_name, line);
            free(member_name);
            continue;
        }
        
        break;
    }
    
    return expr;
}
```

---

### STEP 4: Interpreter/Runtime Changes (`src/interpreter.h`, `src/interpreter.c`)

#### 4.1 Add New Value Types (`interpreter.h`)

```c
typedef enum {
    VAL_INT, VAL_FLOAT, VAL_STRING, VAL_BOOL, VAL_NULL, 
    VAL_FUNCTION, VAL_LIST, VAL_BUILTIN,
    
    // NEW OOP VALUE TYPES
    VAL_CLASS,           // Class definition
    VAL_OBJECT,          // Object instance
    VAL_METHOD           // Bound method (object + method)
} ValueType;
```

#### 4.2 Add Value Structures (`interpreter.h`)

```c
// CLASS VALUE (stores class definition)
typedef struct {
    char *name;                  // Class name
    char *parent_name;           // Parent class (NULL if none)
    struct Value *parent_class;  // Pointer to parent class value
    Env *methods;                // Method environment
    char **field_names;          // Field names
    int field_count;             // Number of fields
} ClassVal;

// OBJECT VALUE (instance of a class)
typedef struct {
    ClassVal *class_ref;         // Pointer to class definition
    Value *fields;               // Array of field values
    int field_count;             // Number of fields
} ObjectVal;

// BOUND METHOD VALUE (method bound to instance)
typedef struct {
    Value *instance;             // Object instance ('this')
    FunctionVal method;          // Method function
} MethodVal;

// Update Value struct
struct Value {
    ValueType type;
    union {
        // ... existing members ...
        ClassVal class_val;
        ObjectVal object_val;
        MethodVal method_val;
    } as;
};
```

#### 4.3 Add Helper Functions (`interpreter.h` and `interpreter.c`)

**Declarations in `interpreter.h`**:
```c
Value make_class(ClassVal class_val);
Value make_object(ObjectVal object_val);
Value make_method(Value *instance, FunctionVal method);
Value *object_get_field(ObjectVal *obj, const char *field_name);
void object_set_field(ObjectVal *obj, const char *field_name, Value value);
```

**Implementations in `interpreter.c`**:
```c
Value make_class(ClassVal class_val) {
    Value v;
    v.type = VAL_CLASS;
    v.as.class_val = class_val;
    return v;
}

Value make_object(ObjectVal object_val) {
    Value v;
    v.type = VAL_OBJECT;
    v.as.object_val = object_val;
    return v;
}

Value make_method(Value *instance, FunctionVal method) {
    Value v;
    v.type = VAL_METHOD;
    v.as.method_val.instance = instance;
    v.as.method_val.method = method;
    return v;
}

Value *object_get_field(ObjectVal *obj, const char *field_name) {
    // Search for field by name
    ClassVal *cls = obj->class_ref;
    for (int i = 0; i < cls->field_count; i++) {
        if (strcmp(cls->field_names[i], field_name) == 0) {
            return &obj->fields[i];
        }
    }
    return NULL; // Field not found
}

void object_set_field(ObjectVal *obj, const char *field_name, Value value) {
    Value *field = object_get_field(obj, field_name);
    if (field) {
        *field = value;
    }
}
```

#### 4.4 Update eval_node_env() with New Cases (`interpreter.c`)

**Location**: Find the main evaluation function `eval_node_env()` (large switch statement)

**Add cases**:
```c
static Value eval_node_env(Interpreter *interp, ASTNode *node, Env *env) {
    // ... existing cases ...
    
    case NODE_CLASS_DEF: {
        // Register class in environment
        ClassVal class_val;
        class_val.name = strdup(node->data.class_def.name);
        class_val.parent_name = node->data.class_def.parent_name ? 
                                strdup(node->data.class_def.parent_name) : NULL;
        class_val.parent_class = NULL; // Resolve later if needed
        
        // Create method environment
        class_val.methods = env_new(env);
        
        // Collect field names
        class_val.field_count = node->data.class_def.fields.count;
        class_val.field_names = malloc(sizeof(char*) * class_val.field_count);
        for (int i = 0; i < class_val.field_count; i++) {
            ASTNode *field = node->data.class_def.fields.items[i];
            if (field->type == NODE_LET) {
                class_val.field_names[i] = strdup(field->data.let_stmt.name);
            }
        }
        
        // Register methods
        for (int i = 0; i < node->data.class_def.methods.count; i++) {
            ASTNode *method = node->data.class_def.methods.items[i];
            if (method->type == NODE_FUNC_DEF) {
                Value method_val = eval_node_env(interp, method, class_val.methods);
                env_set_local(class_val.methods, method->data.func_def.name, method_val);
            }
        }
        
        Value class_value = make_class(class_val);
        env_set_local(env, class_val.name, class_value);
        return class_value;
    }
    
    case NODE_NEW: {
        // Create object instance
        char *class_name = node->data.new_expr.class_name;
        Value *class_val_ptr = env_get(env, class_name);
        
        if (!class_val_ptr || class_val_ptr->type != VAL_CLASS) {
            fprintf(stderr, "Error: '%s' is not a defined class\n", class_name);
            return make_null();
        }
        
        ClassVal *class_def = &class_val_ptr->as.class_val;
        
        // Allocate object
        ObjectVal obj_val;
        obj_val.class_ref = class_def;
        obj_val.field_count = class_def->field_count;
        obj_val.fields = malloc(sizeof(Value) * obj_val.field_count);
        
        // Initialize fields to null
        for (int i = 0; i < obj_val.field_count; i++) {
            obj_val.fields[i] = make_null();
        }
        
        Value instance = make_object(obj_val);
        
        // Call constructor if exists (init method)
        Value *init_method = env_get(class_def->methods, "init");
        if (init_method && init_method->type == VAL_FUNCTION) {
            // Evaluate constructor arguments
            int argc = node->data.new_expr.args.count;
            Value *args = malloc(sizeof(Value) * (argc + 1));
            args[0] = instance; // 'this' is first argument
            for (int i = 0; i < argc; i++) {
                args[i + 1] = eval_node_env(interp, node->data.new_expr.args.items[i], env);
            }
            
            // Create call environment with 'this' bound
            Env *call_env = env_new(init_method->as.func_val.closure);
            env_set_local(call_env, "this", instance);
            
            // Bind other parameters
            for (int i = 0; i < init_method->as.func_val.param_count && i < argc; i++) {
                env_set_local(call_env, init_method->as.func_val.param_names[i], args[i + 1]);
            }
            
            // Execute constructor
            eval_block(interp, init_method->as.func_val.body, call_env);
            env_free(call_env);
            free(args);
        }
        
        return instance;
    }
    
    case NODE_MEMBER_ACCESS: {
        // Evaluate object expression
        Value obj = eval_node_env(interp, node->data.member_access.object, env);
        char *member_name = node->data.member_access.member_name;
        
        if (obj.type == VAL_OBJECT) {
            // Field access
            Value *field = object_get_field(&obj.as.object_val, member_name);
            if (field) {
                return *field;
            }
            
            // Method access - return bound method
            ClassVal *class_def = obj.as.object_val.class_ref;
            Value *method = env_get(class_def->methods, member_name);
            if (method && method->type == VAL_FUNCTION) {
                return make_method(&obj, method->as.func_val);
            }
            
            fprintf(stderr, "Error: Object has no member '%s'\n", member_name);
            return make_null();
        }
        
        fprintf(stderr, "Error: Cannot access member of non-object\n");
        return make_null();
    }
    
    case NODE_THIS: {
        // Lookup 'this' in current environment
        Value *this_val = env_get(env, "this");
        if (this_val) {
            return *this_val;
        }
        fprintf(stderr, "Error: 'this' used outside of class context\n");
        return make_null();
    }
    
    case NODE_STRUCT_DEF: {
        // Similar to class but simpler (no methods, just data)
        // Implementation similar to NODE_CLASS_DEF but skip method handling
        // ... (left as exercise, follows same pattern)
        return make_null();
    }
    
    // ... rest of cases ...
}
```

#### 4.5 Update Function Call Handling for Bound Methods (`interpreter.c`)

**Location**: Find the NODE_CALL case in `eval_node_env()`

**Update to handle bound methods**:
```c
case NODE_CALL: {
    Value callee = eval_node_env(interp, node->data.call.callee, env);
    
    // ... existing argument evaluation code ...
    
    if (callee.type == VAL_METHOD) {
        // Bound method call - inject 'this'
        Env *call_env = env_new(callee.as.method_val.method.closure);
        env_set_local(call_env, "this", *callee.as.method_val.instance);
        
        // Bind parameters
        for (int i = 0; i < callee.as.method_val.method.param_count && i < argc; i++) {
            env_set_local(call_env, callee.as.method_val.method.param_names[i], args[i]);
        }
        
        // Execute method body
        result = eval_block(interp, callee.as.method_val.method.body, call_env);
        env_free(call_env);
    }
    else if (callee.type == VAL_FUNCTION) {
        // ... existing function call code ...
    }
    // ... rest of call handling ...
}
```

#### 4.6 Update value_free() for New Types (`interpreter.c`)

```c
void value_free(Value *v) {
    if (!v) return;
    
    switch (v->type) {
        // ... existing cases ...
        
        case VAL_CLASS:
            free(v->as.class_val.name);
            if (v->as.class_val.parent_name)
                free(v->as.class_val.parent_name);
            for (int i = 0; i < v->as.class_val.field_count; i++)
                free(v->as.class_val.field_names[i]);
            free(v->as.class_val.field_names);
            env_free(v->as.class_val.methods);
            break;
            
        case VAL_OBJECT:
            for (int i = 0; i < v->as.object_val.field_count; i++)
                value_free(&v->as.object_val.fields[i]);
            free(v->as.object_val.fields);
            break;
            
        case VAL_METHOD:
            // Method shares instance and function data, don't double-free
            break;
            
        // ... rest of cases ...
    }
}
```

#### 4.7 Update value_print() for New Types (`interpreter.c`)

```c
void value_print(Value *v) {
    switch (v->type) {
        // ... existing cases ...
        
        case VAL_CLASS:
            printf("<class %s>", v->as.class_val.name);
            break;
            
        case VAL_OBJECT:
            printf("<object of %s>", v->as.object_val.class_ref->name);
            break;
            
        case VAL_METHOD:
            printf("<bound method>");
            break;
            
        // ... rest of cases ...
    }
}
```

---

### STEP 5: Testing (`tests/` directory)

#### 5.1 Create test_oop.c

```c
#include "test_runner.h"
#include "../src/parser.h"
#include "../src/interpreter.h"
#include "../src/lexer.h"

void test_class_definition() {
    const char *code = 
        "class Point {\n"
        "    let x: int = 0\n"
        "    let y: int = 0\n"
        "}\n";
    
    Lexer *lexer = lexer_new(code);
    Parser *parser = parser_new(lexer);
    ASTNode *ast = parse(parser);
    
    ASSERT(ast != NULL, "Class definition should parse");
    ASSERT(ast->type == NODE_CLASS_DEF, "Root should be class definition");
    ASSERT(strcmp(ast->data.class_def.name, "Point") == 0, "Class name should be Point");
    
    ast_free(ast);
    parser_free(parser);
}

void test_object_creation() {
    const char *code = 
        "class Point {\n"
        "    let x: int = 0\n"
        "    let y: int = 0\n"
        "}\n"
        "let p = new Point()\n";
    
    Interpreter *interp = interpreter_new();
    // ... evaluate and check ...
    interpreter_free(interp);
}

void test_member_access() {
    const char *code = 
        "class Point {\n"
        "    let x: int = 5\n"
        "}\n"
        "let p = new Point()\n"
        "println(p.x)\n";
    
    // Test that p.x resolves to 5
}

void test_method_call() {
    const char *code = 
        "class Counter {\n"
        "    let count: int = 0\n"
        "    fn increment() {\n"
        "        this.count = this.count + 1\n"
        "    }\n"
        "}\n"
        "let c = new Counter()\n"
        "c.increment()\n"
        "println(c.count)\n";
    
    // Test that count becomes 1
}

void test_inheritance() {
    const char *code = 
        "class Animal {\n"
        "    fn speak() {\n"
        "        println(\"Animal sound\")\n"
        "    }\n"
        "}\n"
        "class Dog extends Animal {\n"
        "    fn speak() {\n"
        "        println(\"Woof!\")\n"
        "    }\n"
        "}\n"
        "let d = new Dog()\n"
        "d.speak()\n";
    
    // Test that Dog's speak() is called, not Animal's
}

void run_oop_tests() {
    test_class_definition();
    test_object_creation();
    test_member_access();
    test_method_call();
    test_inheritance();
}
```

#### 5.2 Update test_runner.c

Add `run_oop_tests()` to the main test runner.

---

### STEP 6: Example Programs (`examples/` directory)

#### 6.1 Create examples/point.kl

```klang
# Point class example

class Point {
    let x: int = 0
    let y: int = 0
    
    fn init(x_val: int, y_val: int) {
        this.x = x_val
        this.y = y_val
    }
    
    fn distance_from_origin() -> float {
        let dx = this.x * this.x
        let dy = this.y * this.y
        return sqrt(dx + dy)
    }
    
    fn print_coords() {
        println("(" + str(this.x) + ", " + str(this.y) + ")")
    }
}

let p1 = new Point(3, 4)
p1.print_coords()
println("Distance from origin: " + str(p1.distance_from_origin()))
```

#### 6.2 Create examples/inheritance.kl

```klang
# Inheritance example

class Animal {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
    }
    
    fn speak() {
        println(this.name + " makes a sound")
    }
}

class Dog extends Animal {
    fn speak() {
        println(this.name + " says: Woof!")
    }
}

class Cat extends Animal {
    fn speak() {
        println(this.name + " says: Meow!")
    }
}

let dog = new Dog("Buddy")
let cat = new Cat("Whiskers")

dog.speak()  # Buddy says: Woof!
cat.speak()  # Whiskers says: Meow!
```

#### 6.3 Create examples/counter.kl

```klang
# Simple counter class

class Counter {
    let count: int = 0
    
    fn increment() {
        this.count = this.count + 1
    }
    
    fn decrement() {
        this.count = this.count - 1
    }
    
    fn get_value() -> int {
        return this.count
    }
}

let c = new Counter()
c.increment()
c.increment()
c.increment()
println("Count: " + str(c.get_value()))  # Count: 3
```

---

## Files to Modify

### Summary of All File Changes

| File | Changes Required | Priority |
|------|------------------|----------|
| **src/lexer.h** | Add 6 new token types (class, new, this, extends, super, struct) | HIGH |
| **src/lexer.c** | Add keyword recognition for new tokens | HIGH |
| **src/ast.h** | Add 6 new node types + 4 new structs in union | HIGH |
| **src/ast.c** | Add 6 constructor functions + update ast_free() | HIGH |
| **src/parser.h** | Add 2 new parser function declarations | HIGH |
| **src/parser.c** | Add 2 new parser functions + update 3 existing functions | HIGH |
| **src/interpreter.h** | Add 3 new value types + 3 new structs | HIGH |
| **src/interpreter.c** | Add 6 new eval cases + 5 helper functions + update 3 functions | HIGH |
| **tests/test_oop.c** | Create new test file with 5+ test functions | MEDIUM |
| **tests/test_runner.c** | Add call to run_oop_tests() | MEDIUM |
| **examples/point.kl** | Create example program | LOW |
| **examples/inheritance.kl** | Create example program | LOW |
| **examples/counter.kl** | Create example program | LOW |
| **README.md** | Update feature list and examples | LOW |
| **Makefile** | Add test_oop.c to build (if needed) | LOW |

---

## Testing Strategy

### Unit Tests

1. **Lexer Tests**
   - Verify all new tokens are recognized
   - Test keyword vs identifier distinction

2. **Parser Tests**
   - Test class definition parsing
   - Test object instantiation parsing
   - Test member access parsing
   - Test inheritance parsing

3. **Interpreter Tests**
   - Test class registration
   - Test object creation
   - Test field access
   - Test method calls
   - Test 'this' keyword
   - Test inheritance
   - Test method overriding

### Integration Tests

1. **Complete Programs**
   - Run example programs end-to-end
   - Verify output matches expected

2. **Error Handling**
   - Test undefined class
   - Test undefined member
   - Test 'this' outside class
   - Test inheritance cycles

### Performance Tests (Optional)

1. Object creation overhead
2. Method call performance vs function call
3. Memory usage with many objects

---

## Example Code

### What the Final Syntax Should Look Like

```klang
# Complete OOP example

class BankAccount {
    let balance: float = 0.0
    let account_number: string = ""
    
    fn init(acc_num: string, initial_balance: float) {
        this.account_number = acc_num
        this.balance = initial_balance
    }
    
    fn deposit(amount: float) {
        if amount > 0 {
            this.balance = this.balance + amount
            println("Deposited: $" + str(amount))
        }
    }
    
    fn withdraw(amount: float) {
        if amount > 0 and amount <= this.balance {
            this.balance = this.balance - amount
            println("Withdrew: $" + str(amount))
        } else {
            println("Insufficient funds")
        }
    }
    
    fn get_balance() -> float {
        return this.balance
    }
    
    fn print_statement() {
        println("Account: " + this.account_number)
        println("Balance: $" + str(this.balance))
    }
}

class SavingsAccount extends BankAccount {
    let interest_rate: float = 0.02
    
    fn add_interest() {
        let interest = this.balance * this.interest_rate
        this.deposit(interest)
        println("Interest added: $" + str(interest))
    }
}

# Usage
let checking = new BankAccount("CHK-001", 1000.0)
checking.deposit(500.0)
checking.withdraw(200.0)
checking.print_statement()

let savings = new SavingsAccount("SAV-001", 5000.0)
savings.add_interest()
savings.print_statement()
```

---

## Implementation Timeline

### Week 1: Foundation
- **Days 1-2**: Lexer and AST changes (Steps 1-2)
- **Days 3-5**: Parser implementation (Step 3)
- **Days 6-7**: Basic interpreter support (Step 4.1-4.3)

### Week 2: Core OOP
- **Days 8-10**: Complete interpreter implementation (Step 4.4-4.7)
- **Days 11-12**: Basic testing and debugging
- **Days 13-14**: Refinement and bug fixes

### Week 3: Inheritance
- **Days 15-17**: Implement inheritance mechanism
- **Days 18-19**: Implement super keyword
- **Days 20-21**: Testing inheritance

### Week 4: Polish
- **Days 22-24**: Comprehensive testing (Step 5)
- **Days 25-26**: Example programs (Step 6)
- **Days 27-28**: Documentation and final testing

---

## Risk Assessment

| Risk | Likelihood | Impact | Mitigation |
|------|------------|--------|------------|
| Memory leaks with objects | HIGH | HIGH | Implement proper cleanup, add GC integration later |
| 'this' binding complexity | MEDIUM | HIGH | Thorough testing, clear environment management |
| Inheritance bugs | MEDIUM | MEDIUM | Start with single inheritance, test extensively |
| Parser ambiguity | LOW | MEDIUM | Clear grammar rules, precedence handling |
| Performance degradation | MEDIUM | LOW | Profile and optimize later if needed |

---

## Future Enhancements (Beyond This Plan)

1. **Static Type Checking**
   - Enforce type compatibility
   - Compile-time error checking

2. **Abstract Classes & Interfaces**
   - Define contracts
   - Multiple interface implementation

3. **Access Modifiers**
   - Private/public fields and methods
   - Protected for inheritance

4. **Static Members**
   - Class-level fields and methods
   - Singleton pattern support

5. **Operator Overloading**
   - Custom operators for classes
   - More Pythonic feel

6. **Properties/Getters/Setters**
   - Computed properties
   - Validation on field access

7. **Multiple Inheritance** (Controversial)
   - Mixin support
   - Diamond problem resolution

---

## Conclusion

This plan provides a complete roadmap for adding OOP features to KLang. The implementation is broken down into manageable steps, with clear dependencies and priorities. By following this plan, KLang will support:

- ✅ Class definitions with fields and methods
- ✅ Object instantiation with constructors
- ✅ Member access (fields and methods)
- ✅ The `this` keyword for self-reference
- ✅ Single inheritance with `extends`
- ✅ Method overriding
- ✅ The `super` keyword for parent access
- ✅ Struct definitions (simple data containers)

This brings KLang from a functional programming language to a multi-paradigm language supporting both functional and object-oriented programming styles, similar to Python and JavaScript.

---

**Document Version**: 1.0  
**Last Updated**: 2025  
**Author**: KLang Development Team
