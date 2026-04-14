# Exact Code Locations for Parser Issues

## Issue #1: parse_if() Requires Braces

### File: src/core/parser/parser.c
### Lines: 1430-1447

```c
1430  static ASTNode *parse_if(Parser *parser) {
1431      int line = parser->current.line;
1432      Token t = consume(parser, TOKEN_IF);
1433      ASTNode *cond;
1434      ASTNode *then_block;
1435      ASTNode *else_block = NULL;
1436      token_free(&t);
1437      cond = parse_expression(parser);
1438      then_block = parse_block(parser);  // ❌ PROBLEM LINE
1439      if (match(parser, TOKEN_ELSE)) {
1440          if (check(parser, TOKEN_IF)) {
1441              else_block = parse_if(parser);
1442          } else {
1443              else_block = parse_block(parser);  // ❌ ALSO A PROBLEM
1444          }
1445      }
1446      return ast_new_if(cond, then_block, else_block, line);
1447  }
```

**Fix Required**: Replace lines 1437-1445 with:

```c
1437      cond = parse_expression(parser);
1438      
1439      // Accept either block or single statement for then branch
1440      ASTNode *then_block;
1441      if (check(parser, TOKEN_LBRACE)) {
1442          then_block = parse_block(parser);
1443      } else {
1444          ASTNode *stmt = parse_statement(parser);
1445          then_block = ast_new_block(line);
1446          if (stmt) nodelist_push(&then_block->data.block.stmts, stmt);
1447      }
1448      
1449      // Handle else branch
1450      ASTNode *else_block = NULL;
1451      if (match(parser, TOKEN_ELSE)) {
1452          if (check(parser, TOKEN_IF)) {
1453              // else if
1454              else_block = parse_if(parser);
1455          } else if (check(parser, TOKEN_LBRACE)) {
1456              // else with block
1457              else_block = parse_block(parser);
1458          } else {
1459              // else with single statement
1460              ASTNode *stmt = parse_statement(parser);
1461              else_block = ast_new_block(line);
1462              if (stmt) nodelist_push(&else_block->data.block.stmts, stmt);
1463          }
1464      }
```

---

## Issue #1b: parse_while() Requires Braces

### File: src/core/parser/parser.c
### Lines: 1449-1458

```c
1449  static ASTNode *parse_while(Parser *parser) {
1450      int line = parser->current.line;
1451      Token t = consume(parser, TOKEN_WHILE);
1452      ASTNode *cond;
1453      ASTNode *body;
1454      token_free(&t);
1455      cond = parse_expression(parser);
1456      body = parse_block(parser);  // ❌ PROBLEM LINE
1457      return ast_new_while(cond, body, line);
1458  }
```

**Fix Required**: Replace lines 1455-1456 with:

```c
1455      cond = parse_expression(parser);
1456      
1457      // Accept either block or single statement
1458      ASTNode *body;
1459      if (check(parser, TOKEN_LBRACE)) {
1460          body = parse_block(parser);
1461      } else {
1462          ASTNode *stmt = parse_statement(parser);
1463          body = ast_new_block(line);
1464          if (stmt) nodelist_push(&body->data.block.stmts, stmt);
1465      }
```

---

## Issue #2: consume() Returns Live Token on Error

### File: src/core/parser/parser.c
### Lines: 51-58

```c
51   static Token consume(Parser *parser, TokenType type) {
52       if (check(parser, type)) return advance(parser);
53       fprintf(stderr, "Parse error at line %d: expected %s, got %s ('%s')\n",
54               parser->current.line, token_type_name(type),
55               token_type_name(parser->current.type), parser->current.value);
56       parser->had_error = 1;
57       return parser->current;  // ❌ PROBLEM LINE
58   }
```

**Fix Required**: Replace line 57 with:

```c
56       parser->had_error = 1;
57       
58       // Return independent dummy token that caller can safely free
59       Token dummy;
60       dummy.type = TOKEN_ERROR;
61       dummy.value = strdup("");
62       dummy.line = parser->current.line;
63       return dummy;
64   }
```

---

## Supporting Changes

### Add TOKEN_ERROR to Token Types

**File**: Need to check which file defines TokenType enum (likely `src/core/lexer/lexer.h` or `include/core/lexer/lexer.h`)

Look for:
```c
typedef enum {
    TOKEN_EOF,
    TOKEN_NUMBER,
    TOKEN_STRING,
    // ... other tokens ...
```

Add:
```c
typedef enum {
    TOKEN_EOF,
    TOKEN_ERROR,    // ✓ ADD THIS
    TOKEN_NUMBER,
    TOKEN_STRING,
    // ... other tokens ...
```

### Add token_type_name Case for TOKEN_ERROR

Find `token_type_name()` function and add:
```c
const char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_ERROR: return "ERROR";  // ✓ ADD THIS
        case TOKEN_EOF: return "EOF";
        // ... other cases ...
```

### Make token_free() Defensive

**File**: Need to find where `token_free()` is defined (likely in `src/core/lexer/lexer.c`)

Current implementation (typical):
```c
void token_free(Token *token) {
    if (token->value) {
        free(token->value);
    }
}
```

**Fix**:
```c
void token_free(Token *token) {
    if (token && token->value) {
        free(token->value);
        token->value = NULL;  // ✓ Prevent double-free
    }
}
```

---

## Issue #3: Add Error Recovery

### Add synchronize() Function

**File**: src/core/parser/parser.c
**Location**: After utility functions (around line 60, after consume())

```c
/* Error recovery: skip tokens until we find a synchronization point */
static void synchronize(Parser *parser) {
    Token t;
    
    while (!check(parser, TOKEN_EOF)) {
        // Synchronize at semicolon
        if (check(parser, TOKEN_SEMICOLON)) {
            t = advance(parser);
            token_free(&t);
            return;
        }
        
        // Synchronize at statement/declaration keywords
        switch (parser->current.type) {
            case TOKEN_CLASS:
            case TOKEN_STRUCT:
            case TOKEN_FN:
            case TOKEN_LET:
            case TOKEN_CONST:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_FOR:
            case TOKEN_RETURN:
            case TOKEN_BREAK:
            case TOKEN_CONTINUE:
            case TOKEN_IMPORT:
            case TOKEN_EXPORT:
                return;
            default:
                ; // Continue skipping
        }
        
        t = advance(parser);
        token_free(&t);
    }
}
```

### Use synchronize() in Error Paths

Find error handling locations and add synchronize() calls. Examples:

**In parse_statement()** (around line 1856):
```c
static ASTNode *parse_statement(Parser *parser) {
    // ... existing code ...
    
    // If we get here, it's an unexpected token
    fprintf(stderr, "Parse error at line %d: unexpected token %s ('%s')\n",
            line, token_type_name(parser->current.type), parser->current.value);
    parser->had_error = 1;
    synchronize(parser);  // ✓ ADD THIS
    return NULL;
}
```

**In parse_class_def()** (around line 2375):
```c
} else {
    fprintf(stderr, "Parse error at line %d: expected 'let', 'fn', or 'constructor' in class body, got %s\n",
            parser->current.line, token_type_name(parser->current.type));
    parser->had_error = 1;
    synchronize(parser);  // ✓ ADD THIS
    Token t = advance(parser);
    token_free(&t);
}
```

---

## Triggering Code Location

### File: stdlib/ui/server.kl
### Lines: 118-119

```klang
118      write(x, y, text, style) {
119          if (y < 0 || y >= this.height) return
             ^^                               ^^^^^^
             Parser calls parse_if()       No braces!
             which calls parse_block()     Parser finds 'return'
             which expects '{'             instead of '{'
                                           → ERROR!
120          
121          for (let i = 0; i < len(text); i++) {
```

---

## Testing Locations

### Simple Test Cases

Create these files to test fixes:

**File**: `/tmp/test_if_single_stmt.kl`
```klang
fn test() {
    if (true) return
    print("Not reached")
}
test()
```

**File**: `/tmp/test_if_else_single_stmt.kl`
```klang
fn test(x) {
    if (x > 0) return 1
    else return 0
}
print(test(5))
```

**File**: `/tmp/test_while_single_stmt.kl`
```klang
fn test() {
    let x = 5
    while (x > 0) x = x - 1
    return x
}
print(test())
```

**File**: `/tmp/test_nested_single_stmt.kl`
```klang
fn test(x, y) {
    if (x > 0)
        if (y > 0)
            return 1
    return 0
}
print(test(1, 1))
```

---

## Verification Commands

```bash
# Test original failing case
./klang run examples/interactive_ui_server.kl

# Test simple cases
./klang run /tmp/test_if_single_stmt.kl
./klang run /tmp/test_if_else_single_stmt.kl
./klang run /tmp/test_while_single_stmt.kl
./klang run /tmp/test_nested_single_stmt.kl

# Test all stdlib files
for f in stdlib/**/*.kl; do
    echo "Checking $f"
    ./klang check "$f" 2>&1 | grep -i error && echo "FAILED: $f"
done

# Memory check
valgrind --leak-check=full --track-origins=yes \
         ./klang run examples/interactive_ui_server.kl 2>&1 | \
         grep -A5 "double free"
```

