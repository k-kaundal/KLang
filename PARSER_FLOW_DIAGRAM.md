# Parser Flow Diagram - Issue Analysis

## Normal Parsing Flow (Working Case)

```
Code: if (x < 0) { return }

┌─────────────────────────────────────────────────────────┐
│ parse_if()                                              │
│                                                         │
│  1. Consume TOKEN_IF                                   │
│  2. Call parse_expression() → parses (x < 0)          │
│  3. Call parse_block()                                 │
│     ┌───────────────────────────────────────┐         │
│     │ parse_block()                          │         │
│     │  - Consume TOKEN_LBRACE '{'          │         │
│     │  - Parse statements in loop          │         │
│     │    └─ parse_statement() → return    │         │
│     │  - Consume TOKEN_RBRACE '}'          │         │
│     └───────────────────────────────────────┘         │
│  4. Return if_node ✓                                   │
└─────────────────────────────────────────────────────────┘
```

## Broken Parsing Flow (Failing Case)

```
Code: if (x < 0) return

┌─────────────────────────────────────────────────────────┐
│ parse_if()                                              │
│                                                         │
│  1. Consume TOKEN_IF                                   │
│  2. Call parse_expression() → parses (x < 0)          │
│  3. Call parse_block()                                 │
│     ┌───────────────────────────────────────┐         │
│     │ parse_block()                          │         │
│     │  - Call consume(TOKEN_LBRACE)         │         │
│     │    Current token: TOKEN_RETURN       │         │
│     │    Expected: TOKEN_LBRACE            │         │
│     │    ❌ ERROR!                         │         │
│     │    "expected LBRACE, got RETURN"     │         │
│     └───────────────────────────────────────┘         │
│  4. Error propagates ❌                                │
└─────────────────────────────────────────────────────────┘
```

## Memory Error Flow

```
Normal Token Lifecycle:
┌──────────┐    ┌──────────┐    ┌──────────┐
│ advance()│ → │ Caller   │ → │token_free│
│ returns  │    │ uses     │    │ called   │
│ old tok  │    │ token    │    │ once ✓   │
└──────────┘    └──────────┘    └──────────┘

Error Token Lifecycle:
┌──────────┐    ┌──────────┐    ┌──────────┐
│ consume()│ → │ Caller   │ → │token_free│
│ returns  │    │ calls    │    │ on same  │
│ current  │    │token_free│    │ token!   │
│ (error)  │    │          │    │ ❌ ERROR │
└──────────┘    └──────────┘    └──────────┘
                                      ↓
                            ┌──────────────────┐
                            │ parser_free()    │
                            │ tries to free    │
                            │ current again    │
                            │ ❌ DOUBLE FREE   │
                            └──────────────────┘
```

## Detailed consume() Flow

### Current Implementation (BROKEN)

```c
Token consume(Parser *parser, TokenType type) {
    if (check(parser, type)) {
        return advance(parser);  // ✓ Good: returns old current
    }
    
    // Error case:
    fprintf(stderr, "Parse error...");
    parser->had_error = 1;
    return parser->current;  // ❌ BAD: returns live token
}                            //    that's still in parser!

// Caller:
Token t = consume(parser, TOKEN_LBRACE);
token_free(&t);  // ❌ Frees parser->current.value!

// Later:
parser_free(parser);
token_free(&parser->current);  // ❌ DOUBLE FREE!
```

### Fixed Implementation

```c
Token consume(Parser *parser, TokenType type) {
    if (check(parser, type)) {
        return advance(parser);  // ✓ Good: returns old current
    }
    
    // Error case:
    fprintf(stderr, "Parse error...");
    parser->had_error = 1;
    
    // Return independent dummy token
    Token dummy;
    dummy.type = TOKEN_ERROR;
    dummy.value = strdup("");  // ✓ Safe to free
    dummy.line = parser->current.line;
    return dummy;  // ✓ Independent allocation
}

// Caller:
Token t = consume(parser, TOKEN_LBRACE);
token_free(&t);  // ✓ Frees dummy.value, not parser->current!

// Later:
parser_free(parser);
token_free(&parser->current);  // ✓ First and only free!
```

## Class Method Parsing Flow

### Context: stdlib/ui/server.kl line 118-119

```
Code:
  class ScreenBuffer {
      write(x, y, text, style) {
          if (y < 0 || y >= this.height) return
          //...
      }
  }
```

### Parsing Steps:

```
┌─────────────────────────────────────────────────────────────────┐
│ parse_class_def()                                               │
│                                                                 │
│  1. Consume "class" keyword                                    │
│  2. Consume "ScreenBuffer" (class name)                        │
│  3. Consume "{"                                                │
│  4. Enter member parsing loop                                  │
│                                                                 │
│     ┌─────────────────────────────────────────────────┐       │
│     │ Found: IDENT("write") followed by LPAREN       │       │
│     │ → This is method shorthand syntax              │       │
│     │                                                 │       │
│     │ Parse method:                                   │       │
│     │  - Consume "write"                             │       │
│     │  - Consume "(" and parse parameters            │       │
│     │  - Consume ")"                                 │       │
│     │  - Call parse_block() at line 2368            │       │
│     │                                                 │       │
│     │    ┌─────────────────────────────────┐        │       │
│     │    │ parse_block()                    │        │       │
│     │    │  - Consume "{"                  │        │       │
│     │    │  - Parse statements:            │        │       │
│     │    │    └─ parse_statement()        │        │       │
│     │    │       ├─ Found TOKEN_IF         │        │       │
│     │    │       └─ Call parse_if()       │        │       │
│     │    │                                  │        │       │
│     │    │          ┌───────────────────┐  │        │       │
│     │    │          │ parse_if()        │  │        │       │
│     │    │          │  - Consume "if"   │  │        │       │
│     │    │          │  - Parse (y < 0..│  │        │       │
│     │    │          │  - Call           │  │        │       │
│     │    │          │    parse_block() │  │        │       │
│     │    │          │    ❌ ERROR!     │  │        │       │
│     │    │          │    Expected: '{'  │  │        │       │
│     │    │          │    Found: return  │  │        │       │
│     │    │          └───────────────────┘  │        │       │
│     │    └─────────────────────────────────┘        │       │
│     └─────────────────────────────────────────────────┘       │
└─────────────────────────────────────────────────────────────────┘
```

## Fix Strategy

### For parse_if() and parse_while()

**Before**:
```c
static ASTNode *parse_if(Parser *parser) {
    // ...
    cond = parse_expression(parser);
    then_block = parse_block(parser);  // ❌ Always requires braces
    // ...
}
```

**After**:
```c
static ASTNode *parse_if(Parser *parser) {
    // ...
    cond = parse_expression(parser);
    
    // ✓ Accept block or single statement
    ASTNode *then_block;
    if (check(parser, TOKEN_LBRACE)) {
        then_block = parse_block(parser);
    } else {
        // Wrap single statement in block
        ASTNode *stmt = parse_statement(parser);
        then_block = ast_new_block(line);
        if (stmt) nodelist_push(&then_block->data.block.stmts, stmt);
    }
    // ...
}
```

## Cascading Error Example

```
File: stdlib/ui/server.kl

Line 118: write(x, y, text, style) {
Line 119:     if (y < 0 || y >= this.height) return  ← ❌ First error
Line 120: 
Line 121:     for (let i = 0; i < len(text); i++) {  ← ❌ Parser in bad state
Line 122:         let col = x + i
Line 123:         if (col >= 0 && col < this.width) {
Line 124:             this.buffer[y][col] = {
...

Error messages:
  Parse error at line 119: expected LBRACE, got RETURN ('return')
  Parse error at line 121: unexpected token FOR ('for')
  Parse error at line 121: unexpected token LET ('�vB�UV')  ← Memory corruption!
  Parse error at line 121: expected RPAREN, got IDENT ('i')
  ...
  free(): double free detected in tcache 2  ← Memory error
```

### Why Cascading Happens:

1. First error at line 119
2. Parser tries to continue but state is invalid
3. Token values become corrupted (memory issue)
4. Parser makes wrong assumptions about context
5. More errors pile up
6. Eventually double-free causes crash

### Solution: Error Recovery

Add synchronization after errors:
```c
static void synchronize(Parser *parser) {
    // Skip to next statement boundary or declaration keyword
    while (!check(parser, TOKEN_EOF)) {
        if (check(parser, TOKEN_SEMICOLON)) {
            advance(parser);
            return;
        }
        
        switch (parser->current.type) {
            case TOKEN_CLASS:
            case TOKEN_FN:
            case TOKEN_LET:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_FOR:
            case TOKEN_RETURN:
                return;  // Found safe point
            default:
                advance(parser);  // Keep skipping
        }
    }
}
```

Call after each error:
```c
if (error_detected) {
    synchronize(parser);
    return error_node;  // or NULL
}
```

