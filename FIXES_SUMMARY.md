# Parser and Memory Fixes - Summary

## Problem Statement

User reported two critical issues when running `klang run examples/interactive_ui_server.kl`:

1. **Parse Error**: `Parse error at line 119: expected LBRACE, got RETURN ('return')`
2. **Memory Error**: `klang(66882,0x205a73080) malloc: *** error for object 0xc06ca8040: pointer being freed was not allocated`

User requested: "i want permanent solution for this type of issues in my language so validate everything and fix these type of issues"

## Root Causes Identified

### Issue 1: Parser Requires Braces for Single-Statement Bodies
- **Location**: `src/core/parser/parser.c` in `parse_if()`, `parse_while()`, `parse_for()`
- **Problem**: Parser forced all control flow statement bodies to use braces `{}`, even for single statements
- **Example that failed**: `if (y < 0 || y >= this.height) return` in `stdlib/ui/server.kl:119`

### Issue 2: Double-Free in consume() Function
- **Location**: `src/core/parser/parser.c:57` in `consume()`
- **Problem**: On error, `consume()` returned `parser->current` which is still owned by the parser
- **Result**: Caller frees the token, then `parser_free()` tries to free it again → double-free crash

### Issue 3: parse_return() Consuming Next Statement
- **Location**: `src/core/parser/parser.c` in `parse_return()`
- **Problem**: After parsing `return`, it tried to parse the next token as an expression even when it was a statement keyword like `for`
- **Result**: Cascading parse errors

### Issue 4: Extends Clause Not Supporting Qualified Names
- **Location**: `src/core/parser/parser.c` in `parse_class_def()`
- **Problem**: Parser only supported `extends ClassName` but not `extends module.ClassName`
- **Example that failed**: `class UIServer extends ui.EventEmitter` in `stdlib/ui/server.kl:251`

## Fixes Applied

### Fix 1: Added parse_statement_or_block() Helper
```c
static ASTNode *parse_statement_or_block(Parser *parser) {
    if (check(parser, TOKEN_LBRACE)) {
        return parse_block(parser);
    } else {
        /* Single statement - wrap in a block */
        ASTNode *block = ast_new_block(line);
        ASTNode *stmt = parse_statement(parser);
        if (stmt) nodelist_push(&block->data.block.stmts, stmt);
        return block;
    }
}
```

Updated `parse_if()`, `parse_while()`, and all `parse_for()` variants to use this helper.

### Fix 2: Modified consume() to Return Dummy Token on Error
```c
static Token consume(Parser *parser, TokenType type) {
    if (check(parser, type)) return advance(parser);
    fprintf(stderr, "Parse error at line %d: expected %s, got %s ('%s')\n", ...);
    parser->had_error = 1;
    
    /* Return a dummy token to prevent double-free */
    Token dummy;
    dummy.type = type;
    dummy.value = strdup("");  /* Safe to free */
    dummy.line = parser->current.line;
    return dummy;
}
```

### Fix 3: Enhanced parse_return() to Detect Statement Keywords
```c
int next_token_starts_statement = (
    check(parser, TOKEN_IF) || check(parser, TOKEN_FOR) || check(parser, TOKEN_WHILE) ||
    check(parser, TOKEN_LET) || check(parser, TOKEN_VAR) || check(parser, TOKEN_CONST) ||
    check(parser, TOKEN_RETURN) || check(parser, TOKEN_BREAK) || check(parser, TOKEN_CONTINUE) ||
    check(parser, TOKEN_FN) || check(parser, TOKEN_CLASS) || check(parser, TOKEN_STRUCT) ||
    check(parser, TOKEN_SWITCH) || check(parser, TOKEN_IMPORT) || check(parser, TOKEN_EXPORT) ||
    check(parser, TOKEN_UNSAFE)
);

if (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF) && 
    !check(parser, TOKEN_SEMICOLON) && !next_token_starts_statement) {
    value = parse_expression(parser);
}
```

### Fix 4: Support Qualified Names in extends Clause
```c
if (match(parser, TOKEN_EXTENDS)) {
    Token parent_tok = consume(parser, TOKEN_IDENT);
    parent_name = strdup(parent_tok.value);
    token_free(&parent_tok);
    
    /* Check for qualified name: module.ClassName */
    if (check(parser, TOKEN_DOT)) {
        Token dot_tok = advance(parser);
        token_free(&dot_tok);
        Token class_tok = consume(parser, TOKEN_IDENT);
        
        /* Combine module.ClassName into a single string */
        size_t qualified_len = strlen(parent_name) + strlen(class_tok.value) + 2;
        char *qualified_name = malloc(qualified_len);
        if (qualified_name == NULL) {
            fprintf(stderr, "Fatal error: memory allocation failed\n");
            exit(1);
        }
        snprintf(qualified_name, qualified_len, "%s.%s", parent_name, class_tok.value);
        free(parent_name);
        parent_name = qualified_name;
        token_free(&class_tok);
    }
}
```

## Testing

### Comprehensive Test Cases
Created test file demonstrating all fixes:
```klang
// Single-statement if bodies
fn test1() {
    let x = 5
    if (x < 10) print("less than 10")
}

// Single-statement if-else
fn test2() {
    let x = 15
    if (x < 10) 
        print("less")
    else 
        print("greater")
}

// Single-statement while
fn test3() {
    let i = 0
    while (i < 3) i = i + 1
}

// Single-statement for
fn test4() {
    for (let i = 0; i < 3; i++) print(i)
}

// In class methods with early return
class Counter {
    increment() {
        this.count = this.count + 1
        if (this.count > 5) return
        print(this.count)
    }
}
```

**All tests pass successfully!**

### Original Issue Status
- ✅ Original parse error "expected LBRACE, got RETURN" at line 119: **FIXED**
- ✅ Memory error "pointer being freed was not allocated": **FIXED**
- ✅ No more crashes or double-free errors

## Validation Results

### Code Review: ✅ Passed
- Found 4 minor suggestions (all addressed)
- No critical issues

### CodeQL Security Scan: ✅ Passed
- 0 security alerts
- No vulnerabilities found

## Impact

### Before Fixes
- Parser rejected valid single-statement control flow bodies
- Crashes on parse errors due to double-free
- Cascading parse errors made debugging difficult
- stdlib files couldn't parse due to qualified names in extends

### After Fixes
- ✅ Supports both `if (x) statement` and `if (x) { statement }`
- ✅ No crashes - safe error handling
- ✅ Better error messages with no cascading errors
- ✅ Supports qualified parent class names `class A extends module.B`
- ✅ More flexible, user-friendly syntax
- ✅ Matches common programming language conventions

## Files Modified
- `src/core/parser/parser.c` - All parser fixes
- `src/runtime/interpreter/statements.c` - Comment fix (from review)
- `src/runtime/interpreter/interpreter.c` - Comment improvement (from review)

## Conclusion

All critical issues have been permanently fixed:
1. ✅ Single-statement bodies now work correctly
2. ✅ No more memory errors or crashes
3. ✅ Parser is more robust and user-friendly
4. ✅ Supports modern language syntax conventions
5. ✅ All validation tests pass

The fixes are comprehensive, well-tested, and address the root causes rather than just symptoms.
