# Phase 1 Implementation Guide: Foundation Features

This document provides **detailed, step-by-step instructions** for implementing Phase 1 JavaScript features in KLang (Weeks 1-3).

---

## Phase 1 Overview

**Duration**: 3 weeks  
**Features**: var/let/const, arrow functions, template literals, object literals  
**Goal**: Enable modern JavaScript syntax for variables, functions, strings, and objects  
**Primary Agent**: `compiler-agent`  
**Supporting Agents**: `testing-agent`, `docs-agent`

---

## Week 1: Variables & Arrow Functions

### Feature 1.1: Variable Declarations (var/let/const)

#### Current State
KLang already has `let` declarations with type annotations:
```javascript
let x: int = 10;
```

#### Goal
Add JavaScript-style variable declarations:
```javascript
var x = 10;           // Function-scoped, hoisted
let y = 20;           // Block-scoped (upgrade existing)
const Z = 30;         // Immutable binding
```

---

#### Implementation Steps

##### Step 1: Update Lexer (Day 1 Morning)
**Agent**: `compiler-agent`

**File**: `src/lexer.h`

Add token types (check if already exist):
```c
// Around line 40-60 in TokenType enum
TOKEN_VAR,      // 'var' keyword
TOKEN_CONST,    // 'const' keyword
// TOKEN_LET already exists
```

**File**: `src/lexer.c`

Update keyword recognition (around line 165-186):
```c
static Token make_identifier(Lexer *lexer, int start) {
    // ... existing code ...
    
    // Add to keyword checks
    if (strcmp(lexer->source + start, "var") == 0) {
        return (Token){TOKEN_VAR, start, 3, lexer->line};
    }
    if (strcmp(lexer->source + start, "const") == 0) {
        return (Token){TOKEN_CONST, start, 5, lexer->line};
    }
    // let already handled
    
    // ... rest of function ...
}
```

**Verification**:
```bash
# Test lexer recognizes keywords
echo 'var x = 10; let y = 20; const Z = 30;' | ./klang --lex
# Should output TOKEN_VAR, TOKEN_LET, TOKEN_CONST
```

---

##### Step 2: Update Parser (Day 1 Afternoon)
**Agent**: `compiler-agent`

**File**: `src/parser.c`

Modify variable declaration parsing (find `parse_let_statement` or similar):

```c
// Around line 300-400
static ASTNode *parse_var_declaration(Parser *parser) {
    // Consume var/let/const
    TokenType decl_type = parser->current.type;
    advance(parser);
    
    // Parse identifier
    Token identifier = expect(parser, TOKEN_IDENTIFIER, "Expected variable name");
    
    // Optional type annotation (keep for backward compatibility)
    ASTNode *type_hint = NULL;
    if (match(parser, TOKEN_COLON)) {
        // Parse type annotation (but don't enforce yet)
        type_hint = parse_type_annotation(parser);
    }
    
    // Require initializer for const, optional for var/let
    ASTNode *initializer = NULL;
    if (match(parser, TOKEN_EQUAL)) {
        initializer = parse_expression(parser);
    } else if (decl_type == TOKEN_CONST) {
        error(parser, "const declaration must have initializer");
    }
    
    expect(parser, TOKEN_SEMICOLON, "Expected ';' after declaration");
    
    // Create variable declaration node
    ASTNode *node = ast_variable_decl(
        identifier.start,
        identifier.length,
        initializer,
        decl_type,  // Store declaration type
        type_hint
    );
    
    return node;
}
```

**Update AST Node** (if needed):

**File**: `src/ast.h`

```c
// Add field to VariableDecl structure
typedef struct {
    char *name;
    ASTNode *initializer;
    TokenType decl_type;  // TOKEN_VAR, TOKEN_LET, or TOKEN_CONST
    ASTNode *type_hint;   // Optional
} VariableDecl;
```

---

##### Step 3: Update Interpreter (Day 2 Morning)
**Agent**: `compiler-agent`

**File**: `src/interpreter.c`

Implement scoping rules:

```c
// Around line 500-600 in eval_variable_decl or similar
static Value eval_variable_decl(Interpreter *interp, ASTNode *node) {
    VariableDecl *decl = &node->as.var_decl;
    
    // Evaluate initializer
    Value value = decl->initializer 
        ? eval(interp, decl->initializer)
        : make_null();
    
    // Handle scoping based on declaration type
    switch (decl->decl_type) {
        case TOKEN_VAR: {
            // Function scope: add to nearest function environment
            Env *func_env = find_function_scope(interp->env);
            env_define(func_env, decl->name, value, ACCESS_PUBLIC);
            
            // TODO: Implement hoisting (later)
            break;
        }
        
        case TOKEN_LET: {
            // Block scope: add to current environment
            env_define(interp->env, decl->name, value, ACCESS_PUBLIC);
            break;
        }
        
        case TOKEN_CONST: {
            // Block scope, mark as immutable
            env_define_const(interp->env, decl->name, value);
            break;
        }
    }
    
    return make_null();
}

// Helper: Find nearest function scope for var
static Env *find_function_scope(Env *env) {
    // Walk up environments until we find function boundary
    while (env != NULL && env->type != ENV_FUNCTION) {
        env = env->parent;
    }
    return env ? env : env; // Return global if no function scope
}
```

**Add const enforcement**:

**File**: `src/interpreter.h`

```c
// Add to EnvEntry structure
typedef struct EnvEntry {
    char *name;
    Value value;
    AccessModifier access;
    bool is_const;  // NEW: Track if binding is const
    struct EnvEntry *next;
} EnvEntry;
```

**File**: `src/interpreter.c`

```c
// Modify env_define to support const
void env_define_const(Env *env, const char *name, Value value) {
    EnvEntry *entry = malloc(sizeof(EnvEntry));
    entry->name = strdup(name);
    entry->value = value;
    entry->is_const = true;  // Mark as const
    entry->next = env->entries;
    env->entries = entry;
}

// Modify env_set to check const
bool env_set(Env *env, const char *name, Value value) {
    Env *current = env;
    while (current != NULL) {
        EnvEntry *e = current->entries;
        while (e != NULL) {
            if (strcmp(e->name, name) == 0) {
                if (e->is_const) {
                    fprintf(stderr, "TypeError: Cannot assign to const variable '%s'\n", name);
                    return false;
                }
                value_free(&e->value);
                e->value = value;
                return true;
            }
            e = e->next;
        }
        current = current->parent;
    }
    return false;  // Variable not found
}
```

---

##### Step 4: Testing (Day 2 Afternoon)
**Agent**: `testing-agent`

Create test file: `tests/js_features/test_variables.kl`

```javascript
// Test var (function scope)
function test_var() {
    var x = 10;
    if (true) {
        var x = 20;  // Same variable (function-scoped)
        println(x);  // Should print 20
    }
    println(x);  // Should print 20 (not 10)
}

// Test let (block scope)
function test_let() {
    let x = 10;
    if (true) {
        let x = 20;  // Different variable (block-scoped)
        println(x);  // Should print 20
    }
    println(x);  // Should print 10
}

// Test const (immutable)
function test_const() {
    const X = 10;
    // X = 20;  // Should error: Cannot assign to const
    println(X);
}

// Test const requires initializer
// const Y;  // Should error: const must have initializer

test_var();
test_let();
test_const();
```

**Run tests**:
```bash
make test-js-vars
# Or manually:
./klang tests/js_features/test_variables.kl
```

---

### Feature 1.2: Arrow Functions

#### Goal
```javascript
let double = x => x * 2;
let add = (a, b) => a + b;
let greet = () => println("Hello!");
let complex = (x) => {
    let y = x * 2;
    return y + 1;
};
```

---

#### Implementation Steps

##### Step 1: Lexer (Already Done)
**File**: `src/lexer.h`

Verify `TOKEN_ARROW` exists (it should based on analysis):
```c
TOKEN_ARROW,  // '=>'
```

If not, add it and update lexer.c to recognize `=>`.

---

##### Step 2: Update AST (Day 3 Morning)
**Agent**: `compiler-agent`

**File**: `src/ast.h`

Add arrow function node type:
```c
typedef enum {
    // ... existing types ...
    NODE_ARROW_FUNC,
    // ...
} NodeType;

// Add to AST union
typedef struct {
    char **params;        // Parameter names
    int param_count;
    ASTNode *body;        // Expression or block statement
    bool is_expression;   // true if implicit return, false if block
    Env *closure_env;     // Capture lexical scope
} ArrowFunction;

// In ASTNode union:
union {
    // ... existing ...
    ArrowFunction arrow_func;
    // ...
} as;
```

**File**: `src/ast.c`

Add constructor:
```c
ASTNode *ast_arrow_function(char **params, int param_count, 
                            ASTNode *body, bool is_expression) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_ARROW_FUNC;
    node->as.arrow_func.params = params;
    node->as.arrow_func.param_count = param_count;
    node->as.arrow_func.body = body;
    node->as.arrow_func.is_expression = is_expression;
    node->as.arrow_func.closure_env = NULL;  // Set during eval
    return node;
}
```

---

##### Step 3: Update Parser (Day 3 Afternoon + Day 4 Morning)
**Agent**: `compiler-agent`

**File**: `src/parser.c`

Add arrow function parsing in expression parser:

```c
// Add at appropriate precedence level (assignment or higher)
static ASTNode *parse_arrow_or_assignment(Parser *parser) {
    int checkpoint = parser->current_pos;  // Save position
    
    // Try to parse as arrow function
    ASTNode *arrow = try_parse_arrow_function(parser);
    if (arrow != NULL) {
        return arrow;
    }
    
    // Backtrack and parse as regular assignment
    parser->current_pos = checkpoint;
    return parse_assignment(parser);
}

static ASTNode *try_parse_arrow_function(Parser *parser) {
    // Case 1: Single parameter without parens: x => expr
    if (check(parser, TOKEN_IDENTIFIER)) {
        Token param = advance(parser);
        if (match(parser, TOKEN_ARROW)) {
            char **params = malloc(sizeof(char*));
            params[0] = strndup(param.start, param.length);
            
            ASTNode *body = parse_arrow_body(parser);
            return ast_arrow_function(params, 1, body, is_expression_body(body));
        }
        // Not an arrow function, backtrack
        return NULL;
    }
    
    // Case 2: Multiple parameters or zero: (a, b) => expr or () => expr
    if (match(parser, TOKEN_LEFT_PAREN)) {
        // Parse parameter list
        char **params = NULL;
        int param_count = 0;
        
        if (!check(parser, TOKEN_RIGHT_PAREN)) {
            // Parse comma-separated identifiers
            do {
                Token param = expect(parser, TOKEN_IDENTIFIER, "Expected parameter name");
                params = realloc(params, sizeof(char*) * (param_count + 1));
                params[param_count++] = strndup(param.start, param.length);
            } while (match(parser, TOKEN_COMMA));
        }
        
        expect(parser, TOKEN_RIGHT_PAREN, "Expected ')' after parameters");
        
        // Must see arrow
        if (!match(parser, TOKEN_ARROW)) {
            // Not an arrow function, free params and backtrack
            for (int i = 0; i < param_count; i++) free(params[i]);
            free(params);
            return NULL;
        }
        
        ASTNode *body = parse_arrow_body(parser);
        return ast_arrow_function(params, param_count, body, is_expression_body(body));
    }
    
    return NULL;  // Not an arrow function
}

static ASTNode *parse_arrow_body(Parser *parser) {
    // Block body: { ... }
    if (check(parser, TOKEN_LEFT_BRACE)) {
        return parse_block_statement(parser);
    }
    // Expression body (implicit return)
    else {
        return parse_expression(parser);
    }
}

static bool is_expression_body(ASTNode *body) {
    return body->type != NODE_BLOCK;
}
```

---

##### Step 4: Update Interpreter (Day 4 Afternoon + Day 5)
**Agent**: `compiler-agent`

**File**: `src/interpreter.c`

Evaluate arrow functions:

```c
static Value eval_arrow_function(Interpreter *interp, ASTNode *node) {
    ArrowFunction *arrow = &node->as.arrow_func;
    
    // Capture current environment (lexical closure)
    arrow->closure_env = interp->env;
    
    // Create function value (reuse VAL_FUNCTION)
    FunctionValue func;
    func.params = arrow->params;
    func.param_count = arrow->param_count;
    func.body = arrow->body;
    func.closure = arrow->closure_env;
    func.is_arrow = true;  // Mark as arrow function
    func.is_expression = arrow->is_expression;
    
    return make_function(func);
}

// Modify function call to handle expression bodies
static Value call_function(Interpreter *interp, FunctionValue *func, Value *args, int argc) {
    // Check argument count
    if (argc != func->param_count) {
        fprintf(stderr, "ArgumentError: Expected %d arguments, got %d\n", 
                func->param_count, argc);
        return make_null();
    }
    
    // Create new environment for function execution
    Env *func_env = env_new(func->closure);
    
    // Bind parameters
    for (int i = 0; i < func->param_count; i++) {
        env_define(func_env, func->params[i], args[i], ACCESS_PUBLIC);
    }
    
    // If arrow function with lexical 'this', bind it
    if (func->is_arrow && func->closure != NULL) {
        // Capture 'this' from enclosing scope
        Value this_val = env_get(func->closure, "this");
        if (this_val.type != VAL_NULL) {
            env_define(func_env, "this", this_val, ACCESS_PUBLIC);
        }
    }
    
    // Save and set environment
    Env *prev_env = interp->env;
    interp->env = func_env;
    
    // Execute body
    Value result;
    if (func->is_expression) {
        // Expression body: implicit return
        result = eval(interp, func->body);
    } else {
        // Block body: explicit return
        result = eval(interp, func->body);
        // Check if return was called
        if (interp->return_flag) {
            result = interp->return_value;
            interp->return_flag = false;
        }
    }
    
    // Restore environment
    interp->env = prev_env;
    // Don't free func_env yet (it's in closures)
    
    return result;
}
```

**Update Value structure**:

**File**: `src/interpreter.h`

```c
typedef struct {
    char **params;
    int param_count;
    ASTNode *body;
    Env *closure;
    bool is_arrow;        // NEW: Is this an arrow function?
    bool is_expression;   // NEW: Is body an expression (implicit return)?
} FunctionValue;
```

---

##### Step 5: Testing (Day 5)
**Agent**: `testing-agent`

Create test file: `tests/js_features/test_arrows.kl`

```javascript
// Test basic arrow with single param
let double = x => x * 2;
println(double(5));  // Should print 10

// Test arrow with multiple params
let add = (a, b) => a + b;
println(add(3, 7));  // Should print 10

// Test arrow with no params
let greet = () => "Hello!";
println(greet());  // Should print "Hello!"

// Test arrow with block body
let complex = (x) => {
    let y = x * 2;
    return y + 1;
};
println(complex(5));  // Should print 11

// Test lexical this binding
class Counter {
    init() {
        this.count = 0;
    }
    
    increment() {
        // Arrow function captures 'this' from enclosing scope
        let incrementer = () => {
            this.count = this.count + 1;
        };
        incrementer();
    }
}

let counter = new Counter();
counter.increment();
println(counter.count);  // Should print 1

// Test arrow as callback
let numbers = [1, 2, 3];
// Assuming we have forEach (implement in Phase 2)
// numbers.forEach(x => println(x));
```

**Run tests**:
```bash
make test-js-arrows
./klang tests/js_features/test_arrows.kl
```

---

## Week 2: Template Literals

### Feature 1.3: Template Literals

#### Goal
```javascript
let name = "World";
let greeting = `Hello, ${name}!`;
let multi = `Line 1
Line 2`;
let expr = `Sum: ${1 + 2}`;
```

---

#### Implementation Steps

##### Step 1: Update Lexer (Day 6-7)
**Agent**: `compiler-agent`

**File**: `src/lexer.c`

Add template literal parsing:

```c
// Add new lexer state
typedef enum {
    LEX_NORMAL,
    LEX_TEMPLATE,        // Inside `...`
    LEX_TEMPLATE_EXPR    // Inside ${...}
} LexerState;

// Add to Lexer structure
typedef struct {
    // ... existing fields ...
    LexerState state;
    int brace_depth;  // Track nested braces in ${...}
} Lexer;

// Add token type
typedef enum {
    // ... existing ...
    TOKEN_TEMPLATE_START,     // ` at beginning
    TOKEN_TEMPLATE_MIDDLE,    // ` ${expr} ` middle part
    TOKEN_TEMPLATE_END,       // ` at end
    TOKEN_TEMPLATE_STRING,    // String portion
} TokenType;

// Parse template literal
static Token scan_template_literal(Lexer *lexer) {
    int start = lexer->current;
    lexer->current++;  // Skip opening `
    
    StringBuilder *sb = sb_new();
    
    while (lexer->current < lexer->length && 
           lexer->source[lexer->current] != '`') {
        
        // Check for ${
        if (lexer->source[lexer->current] == '$' &&
            lexer->current + 1 < lexer->length &&
            lexer->source[lexer->current + 1] == '{') {
            
            // Found interpolation
            // Return string part so far
            Token string_tok = {
                TOKEN_TEMPLATE_STRING,
                start,
                sb->length,
                lexer->line
            };
            // Save string
            // ... handle interpolation start
            
            lexer->state = LEX_TEMPLATE_EXPR;
            return string_tok;
        }
        
        // Regular character
        char c = lexer->source[lexer->current++];
        if (c == '\\') {
            // Handle escape sequences
            c = lexer->source[lexer->current++];
            switch (c) {
                case 'n': sb_append(sb, '\n'); break;
                case 't': sb_append(sb, '\t'); break;
                case '\\': sb_append(sb, '\\'); break;
                default: sb_append(sb, c); break;
            }
        } else {
            sb_append(sb, c);
        }
    }
    
    // End of template
    lexer->current++;  // Skip closing `
    
    Token tok = {
        TOKEN_TEMPLATE_END,
        start,
        sb->length,
        lexer->line
    };
    tok.string_value = sb_to_string(sb);
    sb_free(sb);
    
    return tok;
}
```

**Note**: Template literal lexing is complex. Consider a simpler approach:

**Simplified Approach** (Recommended for Phase 1):

```c
// Just recognize backticks and parse manually in parser
static Token scan_token(Lexer *lexer) {
    // ... existing code ...
    
    case '`':
        return scan_full_template(lexer);
    
    // ...
}

static Token scan_full_template(Lexer *lexer) {
    int start = lexer->current;
    lexer->current++;  // Skip `
    
    // Store entire template content (including ${...})
    int content_start = lexer->current;
    while (lexer->current < lexer->length &&
           lexer->source[lexer->current] != '`') {
        if (lexer->source[lexer->current] == '\\') {
            lexer->current += 2;  // Skip escaped char
        } else {
            lexer->current++;
        }
    }
    
    int content_length = lexer->current - content_start;
    lexer->current++;  // Skip closing `
    
    Token tok = {
        TOKEN_TEMPLATE_LITERAL,
        content_start,
        content_length,
        lexer->line
    };
    
    return tok;
}
```

---

##### Step 2: Update Parser (Day 7)
**Agent**: `compiler-agent`

**File**: `src/parser.c`

Parse template literal into AST:

```c
static ASTNode *parse_template_literal(Parser *parser) {
    Token tok = advance(parser);  // TOKEN_TEMPLATE_LITERAL
    
    // Extract template content
    char *content = strndup(tok.start, tok.length);
    
    // Parse ${...} expressions
    ASTNode **parts = NULL;
    int part_count = 0;
    
    char *ptr = content;
    char *start = ptr;
    
    while (*ptr != '\0') {
        if (*ptr == '$' && *(ptr + 1) == '{') {
            // Found expression start
            
            // Add string part before ${
            if (ptr > start) {
                ASTNode *str = ast_string_literal(start, ptr - start);
                parts = realloc(parts, sizeof(ASTNode*) * (part_count + 1));
                parts[part_count++] = str;
            }
            
            // Parse expression
            ptr += 2;  // Skip ${
            char *expr_start = ptr;
            int brace_depth = 1;
            
            while (*ptr != '\0' && brace_depth > 0) {
                if (*ptr == '{') brace_depth++;
                else if (*ptr == '}') brace_depth--;
                ptr++;
            }
            
            // Extract expression string
            char *expr_str = strndup(expr_start, ptr - expr_start - 1);
            
            // Parse expression (create mini-parser)
            ASTNode *expr = parse_expression_string(parser, expr_str);
            parts = realloc(parts, sizeof(ASTNode*) * (part_count + 1));
            parts[part_count++] = expr;
            
            free(expr_str);
            start = ptr;
        } else {
            ptr++;
        }
    }
    
    // Add final string part
    if (*start != '\0') {
        ASTNode *str = ast_string_literal(start, ptr - start);
        parts = realloc(parts, sizeof(ASTNode*) * (part_count + 1));
        parts[part_count++] = str;
    }
    
    free(content);
    
    // Create template literal node
    return ast_template_literal(parts, part_count);
}

// Helper: Parse expression from string
static ASTNode *parse_expression_string(Parser *parser, const char *expr_str) {
    // Create temporary lexer and parser for expression
    Lexer expr_lexer;
    lexer_init(&expr_lexer, expr_str);
    
    Parser expr_parser;
    parser_init(&expr_parser, &expr_lexer);
    
    ASTNode *expr = parse_expression(&expr_parser);
    
    return expr;
}
```

**Update AST**:

**File**: `src/ast.h`

```c
typedef struct {
    ASTNode **parts;  // Alternating string literals and expressions
    int part_count;
} TemplateLiteral;

// Add to union
union {
    // ...
    TemplateLiteral template_literal;
    // ...
} as;
```

---

##### Step 3: Update Interpreter (Day 8)
**Agent**: `compiler-agent`

**File**: `src/interpreter.c`

Evaluate template literal:

```c
static Value eval_template_literal(Interpreter *interp, ASTNode *node) {
    TemplateLiteral *tmpl = &node->as.template_literal;
    
    // Build result string
    StringBuilder *sb = sb_new();
    
    for (int i = 0; i < tmpl->part_count; i++) {
        Value part_val = eval(interp, tmpl->parts[i]);
        
        // Convert to string
        char *str = value_to_string(&part_val);
        sb_append_str(sb, str);
        free(str);
        
        value_free(&part_val);
    }
    
    char *result = sb_to_string(sb);
    sb_free(sb);
    
    return make_string(result);
}

// Helper: StringBuilder (if not already exists)
typedef struct {
    char *buffer;
    int length;
    int capacity;
} StringBuilder;

StringBuilder *sb_new() {
    StringBuilder *sb = malloc(sizeof(StringBuilder));
    sb->capacity = 64;
    sb->length = 0;
    sb->buffer = malloc(sb->capacity);
    sb->buffer[0] = '\0';
    return sb;
}

void sb_append_str(StringBuilder *sb, const char *str) {
    int len = strlen(str);
    while (sb->length + len >= sb->capacity) {
        sb->capacity *= 2;
        sb->buffer = realloc(sb->buffer, sb->capacity);
    }
    strcpy(sb->buffer + sb->length, str);
    sb->length += len;
}

char *sb_to_string(StringBuilder *sb) {
    return strdup(sb->buffer);
}

void sb_free(StringBuilder *sb) {
    free(sb->buffer);
    free(sb);
}
```

---

##### Step 4: Testing (Day 8)
**Agent**: `testing-agent`

Create test file: `tests/js_features/test_templates.kl`

```javascript
// Basic interpolation
let name = "World";
let greeting = `Hello, ${name}!`;
println(greeting);  // Hello, World!

// Expression interpolation
let x = 5;
let y = 10;
let msg = `Sum: ${x + y}`;
println(msg);  // Sum: 15

// Multi-line
let multi = `Line 1
Line 2
Line 3`;
println(multi);

// Nested expressions
let a = 2;
let b = 3;
let result = `${a} * ${b} = ${a * b}`;
println(result);  // 2 * 3 = 6

// Function calls in templates
function double(x) {
    return x * 2;
}
println(`Double of 5 is ${double(5)}`);  // Double of 5 is 10
```

---

## Week 3: Object Literals

### Feature 1.4: Enhanced Object Literals

#### Goal
```javascript
let point = { x: 10, y: 20 };
let user = { name, age };  // Shorthand
let obj = {
    method() { return this.value; }
};
let dynamic = { [expr]: value };
```

---

#### Implementation Steps

##### Step 1: Update AST (Day 9)
**Agent**: `compiler-agent`

**File**: `src/ast.h`

Add object literal node:

```c
typedef struct {
    char **keys;           // Property keys
    ASTNode **values;      // Property values
    bool *is_computed;     // Is key computed? [expr]
    bool *is_method;       // Is value a method?
    int property_count;
} ObjectLiteral;

// Add to union
union {
    // ...
    ObjectLiteral object_literal;
    // ...
} as;
```

---

##### Step 2: Update Parser (Day 9-10)
**Agent**: `compiler-agent`

**File**: `src/parser.c`

Parse object literals:

```c
static ASTNode *parse_object_literal(Parser *parser) {
    expect(parser, TOKEN_LEFT_BRACE, "Expected '{'");
    
    char **keys = NULL;
    ASTNode **values = NULL;
    bool *is_computed = NULL;
    bool *is_method = NULL;
    int count = 0;
    
    while (!check(parser, TOKEN_RIGHT_BRACE) && !is_at_end(parser)) {
        // Parse property
        
        bool computed = false;
        char *key = NULL;
        
        // Computed property: [expr]
        if (match(parser, TOKEN_LEFT_BRACKET)) {
            computed = true;
            ASTNode *key_expr = parse_expression(parser);
            expect(parser, TOKEN_RIGHT_BRACKET, "Expected ']'");
            
            // Store expression as key (special handling)
            // TODO: Store expr node, evaluate at runtime
            key = "<computed>";  // Placeholder
        }
        // Regular property
        else {
            Token key_tok = expect(parser, TOKEN_IDENTIFIER, "Expected property name");
            key = strndup(key_tok.start, key_tok.length);
        }
        
        // Check for shorthand: { name } instead of { name: name }
        if (check(parser, TOKEN_COMMA) || check(parser, TOKEN_RIGHT_BRACE)) {
            // Shorthand property
            ASTNode *value = ast_identifier(key);
            
            keys = realloc(keys, sizeof(char*) * (count + 1));
            values = realloc(values, sizeof(ASTNode*) * (count + 1));
            is_computed = realloc(is_computed, sizeof(bool) * (count + 1));
            is_method = realloc(is_method, sizeof(bool) * (count + 1));
            
            keys[count] = key;
            values[count] = value;
            is_computed[count] = computed;
            is_method[count] = false;
            count++;
        }
        // Method shorthand: { method() { } }
        else if (match(parser, TOKEN_LEFT_PAREN)) {
            // Parse method
            ASTNode *method = parse_function_params_and_body(parser);
            
            keys = realloc(keys, sizeof(char*) * (count + 1));
            values = realloc(values, sizeof(ASTNode*) * (count + 1));
            is_computed = realloc(is_computed, sizeof(bool) * (count + 1));
            is_method = realloc(is_method, sizeof(bool) * (count + 1));
            
            keys[count] = key;
            values[count] = method;
            is_computed[count] = computed;
            is_method[count] = true;
            count++;
        }
        // Regular property: { key: value }
        else {
            expect(parser, TOKEN_COLON, "Expected ':'");
            ASTNode *value = parse_expression(parser);
            
            keys = realloc(keys, sizeof(char*) * (count + 1));
            values = realloc(values, sizeof(ASTNode*) * (count + 1));
            is_computed = realloc(is_computed, sizeof(bool) * (count + 1));
            is_method = realloc(is_method, sizeof(bool) * (count + 1));
            
            keys[count] = key;
            values[count] = value;
            is_computed[count] = computed;
            is_method[count] = false;
            count++;
        }
        
        // Optional comma
        if (!check(parser, TOKEN_RIGHT_BRACE)) {
            expect(parser, TOKEN_COMMA, "Expected ',' or '}'");
        }
    }
    
    expect(parser, TOKEN_RIGHT_BRACE, "Expected '}'");
    
    return ast_object_literal(keys, values, is_computed, is_method, count);
}
```

---

##### Step 3: Update Interpreter (Day 11-12)
**Agent**: `compiler-agent`

**File**: `src/interpreter.h`

Add object value type:

```c
typedef struct {
    char **keys;
    Value *values;
    int property_count;
} ObjectValue;

typedef enum {
    // ... existing ...
    VAL_OBJECT_LITERAL,
    // ...
} ValueType;

// Add to Value union
union {
    // ...
    ObjectValue *object;
    // ...
} as;
```

**File**: `src/interpreter.c`

Evaluate object literal:

```c
static Value eval_object_literal(Interpreter *interp, ASTNode *node) {
    ObjectLiteral *obj_lit = &node->as.object_literal;
    
    // Create object value
    ObjectValue *obj = malloc(sizeof(ObjectValue));
    obj->property_count = obj_lit->property_count;
    obj->keys = malloc(sizeof(char*) * obj->property_count);
    obj->values = malloc(sizeof(Value) * obj->property_count);
    
    for (int i = 0; i < obj->property_count; i++) {
        // Evaluate key (if computed)
        char *key;
        if (obj_lit->is_computed[i]) {
            // Evaluate key expression
            // TODO: Handle computed keys
            key = strdup("<computed>");
        } else {
            key = strdup(obj_lit->keys[i]);
        }
        
        // Evaluate value
        Value value = eval(interp, obj_lit->values[i]);
        
        // If method, bind 'this'
        if (obj_lit->is_method[i] && value.type == VAL_FUNCTION) {
            // Bind object as 'this' context
            // TODO: Create bound method
        }
        
        obj->keys[i] = key;
        obj->values[i] = value;
    }
    
    Value result;
    result.type = VAL_OBJECT_LITERAL;
    result.as.object = obj;
    return result;
}

// Property access: obj.prop or obj[prop]
static Value eval_member_access(Interpreter *interp, ASTNode *node) {
    MemberAccess *access = &node->as.member_access;
    
    Value object = eval(interp, access->object);
    
    if (object.type == VAL_OBJECT_LITERAL) {
        ObjectValue *obj = object.as.object;
        
        // Get property name
        char *prop_name;
        if (access->is_computed) {
            // obj[expr]
            Value key_val = eval(interp, access->property);
            prop_name = value_to_string(&key_val);
            value_free(&key_val);
        } else {
            // obj.prop
            prop_name = access->property_name;
        }
        
        // Find property
        for (int i = 0; i < obj->property_count; i++) {
            if (strcmp(obj->keys[i], prop_name) == 0) {
                return obj->values[i];  // Return copy
            }
        }
        
        // Property not found
        return make_null();
    }
    
    // Handle other object types (classes, etc.)
    // ...
    
    return make_null();
}
```

---

##### Step 4: Testing (Day 12-13)
**Agent**: `testing-agent`

Create test file: `tests/js_features/test_objects.kl`

```javascript
// Basic object literal
let point = { x: 10, y: 20 };
println(point.x);  // 10
println(point.y);  // 20

// Shorthand properties
let name = "Alice";
let age = 30;
let user = { name, age };
println(user.name);  // Alice
println(user.age);   // 30

// Method shorthand
let calculator = {
    add(a, b) {
        return a + b;
    },
    multiply(a, b) {
        return a * b;
    }
};
println(calculator.add(5, 3));       // 8
println(calculator.multiply(4, 7));  // 28

// Nested objects
let config = {
    server: {
        host: "localhost",
        port: 8080
    }
};
println(config.server.host);  // localhost
println(config.server.port);  // 8080

// Dynamic property access
let key = "x";
println(point[key]);  // 10

// TODO: Computed property names
// let dynamicKey = "age";
// let obj = { [dynamicKey]: 25 };
```

**Run tests**:
```bash
make test-js-objects
./klang tests/js_features/test_objects.kl
```

---

## Phase 1 Completion Checklist

### Week 1 ✅
- [ ] var declarations (function-scoped)
- [ ] let declarations (block-scoped)
- [ ] const declarations (immutable)
- [ ] Hoisting behavior (optional, can defer)
- [ ] Arrow functions (single param)
- [ ] Arrow functions (multi param)
- [ ] Arrow functions (no params)
- [ ] Arrow functions (block body)
- [ ] Arrow functions (lexical this)
- [ ] Tests for variables
- [ ] Tests for arrow functions

### Week 2 ✅
- [ ] Template literal syntax (backticks)
- [ ] Expression interpolation (${expr})
- [ ] Multi-line strings
- [ ] Nested expressions
- [ ] Escape sequences in templates
- [ ] Tests for template literals

### Week 3 ✅
- [ ] Object literal syntax ({ key: value })
- [ ] Shorthand properties ({ x })
- [ ] Method shorthand ({ method() {} })
- [ ] Nested objects
- [ ] Dynamic property access (obj[key])
- [ ] Computed property names (optional)
- [ ] Tests for object literals

### Integration ✅
- [ ] All features work together
- [ ] No regressions in existing features
- [ ] Documentation updated
- [ ] Examples created
- [ ] Performance acceptable

---

## Common Issues & Solutions

### Issue: Parser Ambiguity (Arrow Functions)
**Problem**: `(x)` could be grouping or arrow params  
**Solution**: Use lookahead or backtracking

### Issue: Template Literal Escaping
**Problem**: Nested braces in ${...}  
**Solution**: Track brace depth

### Issue: Object vs. Block
**Problem**: `{ x: 1 }` could be object or labeled statement  
**Solution**: Context-dependent parsing

### Issue: Memory Leaks
**Problem**: Forgetting to free AST nodes  
**Solution**: Use Valgrind, implement proper cleanup

---

## Performance Tips

1. **String Builder**: Use for template literals (avoid repeated allocation)
2. **Object Property Lookup**: Consider hash map instead of linear search
3. **AST Node Reuse**: Share common nodes where possible
4. **Lazy Evaluation**: Don't evaluate unused parts

---

## Next Steps After Phase 1

1. **Code Review**: Have `qa-agent` review all changes
2. **Documentation**: Update language reference with `docs-agent`
3. **Performance Testing**: Benchmark with `performance-agent`
4. **Examples**: Create showcase programs
5. **Plan Phase 2**: Spread/rest, destructuring, array methods

---

**Document Status**: Ready for Implementation  
**Primary Agent**: `compiler-agent`  
**Estimated Duration**: 3 weeks  
**Last Updated**: 2024
