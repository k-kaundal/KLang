# KLang OOP Parsing - Quick Reference

## Parser Functions

### `parse_class_def(Parser *parser)`
Parses a complete class definition.

**Grammar:**
```
class_def := 'class' IDENT ['extends' IDENT] '{' class_member* '}'
class_member := let_stmt | func_def
```

**Returns:** `NODE_CLASS_DEF` with name, parent_name, and members list

---

### `parse_primary()` - Enhanced
Added support for OOP primary expressions.

**New Cases:**
- `TOKEN_NEW` → `NODE_NEW` with class name
- `TOKEN_THIS` → `NODE_THIS`
- `TOKEN_SUPER` → `NODE_SUPER` (empty member, filled by postfix)

---

### `parse_postfix()` - Enhanced
Added member access support.

**New Case:**
- `TOKEN_DOT` → `NODE_MEMBER_ACCESS`
  - Special handling: If left side is `NODE_SUPER`, updates it with member name

**Grammar:**
```
postfix := primary ( '(' args ')' | '[' expr ']' | '.' IDENT )*
```

---

### `parse_statement()` - Enhanced
Added class statement and member assignment support.

**New Cases:**
- `TOKEN_CLASS` → Calls `parse_class_def()`
- Assignment to `NODE_MEMBER_ACCESS` → Creates `NODE_BINOP` with op="="

---

## AST Nodes

### NODE_CLASS_DEF
```c
{
    char *name;          // "Point"
    char *parent_name;   // "Object" or NULL
    NodeList members;    // [NODE_LET, NODE_FUNC_DEF, ...]
}
```

### NODE_NEW
```c
{
    char *class_name;    // "Point"
    NodeList args;       // Initially empty, filled by CALL wrapping
}
```

### NODE_MEMBER_ACCESS
```c
{
    ASTNode *obj;        // Left side (can be THIS, IDENT, etc.)
    char *member;        // "fieldName"
}
```

### NODE_THIS
```c
{ }  // No data
```

### NODE_SUPER
```c
{
    char *member;        // "methodName"
}
```

---

## Examples

### Parse Class Definition
```klang
class Point {
    let x: int = 0
    let y: int = 0
}
```
→ `NODE_CLASS_DEF("Point", NULL, [NODE_LET("x"), NODE_LET("y")])`

### Parse Inheritance
```klang
class Point3D extends Point {
    let z: int = 0
}
```
→ `NODE_CLASS_DEF("Point3D", "Point", [NODE_LET("z")])`

### Parse New Expression
```klang
let p = new Point(3, 4)
```
→ `NODE_LET("p", NODE_CALL(NODE_NEW("Point"), [3, 4]))`

### Parse Member Access
```klang
this.x
```
→ `NODE_MEMBER_ACCESS(NODE_THIS, "x")`

### Parse Super Call
```klang
super.method()
```
→ `NODE_CALL(NODE_SUPER("method"), [])`

### Parse Member Assignment
```klang
this.x = 42
```
→ `NODE_BINOP("=", NODE_MEMBER_ACCESS(NODE_THIS, "x"), 42)`

---

## Testing

Run OOP tests:
```bash
make test
```

Parse OOP example:
```bash
./klang examples/oop_no_comments.klang
```

---

## Common Patterns

### Constructor Pattern
```klang
class MyClass {
    fn init(value: int) {
        this.field = value
    }
}

let obj = new MyClass()
obj.init(42)
```

### Method with This
```klang
class Counter {
    let count: int = 0
    
    fn increment() {
        this.count = this.count + 1
    }
}
```

### Inheritance with Super
```klang
class Child extends Parent {
    fn method() {
        super.method()
    }
}
```

---

## Error Handling

The parser detects and reports:
- Invalid class member (not `let` or `fn`)
- Missing class name
- Missing extends target
- Invalid assignment target (not identifier or member access)
- Malformed member access

---

## Limitations

Current implementation:
- ✅ Parses all OOP syntax correctly
- ✅ Generates proper AST nodes
- ❌ Runtime execution not yet implemented
- ❌ No static type checking
- ❌ No access modifiers (private/public)

---

## File Locations

- **Implementation**: `src/parser.c`
- **Tests**: `tests/test_parser.c`
- **Documentation**: `docs/OOP_PARSING.md`
- **Examples**: `examples/oop_no_comments.klang`
