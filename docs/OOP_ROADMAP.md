# KLang OOP Implementation Roadmap

## Visual Phase Breakdown

```
┌─────────────────────────────────────────────────────────────────────────┐
│                     KLang OOP Implementation Phases                      │
└─────────────────────────────────────────────────────────────────────────┘

                            START HERE ↓

╔═══════════════════════════════════════════════════════════════════════╗
║                        PHASE 1: FOUNDATION                             ║
║                         (Weeks 1-2, 37 hours)                         ║
╚═══════════════════════════════════════════════════════════════════════╝

    ┌─────────────┐
    │   Lexer     │  Add tokens: class, new, this, dot
    │  (2 hours)  │  ✓ TOKEN_CLASS
    └──────┬──────┘  ✓ TOKEN_NEW
           │         ✓ TOKEN_THIS
           ↓         ✓ TOKEN_DOT
    ┌─────────────┐
    │     AST     │  Add node types and structures
    │  (3 hours)  │  ✓ NODE_CLASS_DEF
    └──────┬──────┘  ✓ NODE_NEW
           │         ✓ NODE_MEMBER_ACCESS
           ↓         ✓ NODE_THIS
    ┌─────────────┐
    │   Parser    │  Implement parsing functions
    │  (8 hours)  │  ✓ parse_class_def()
    └──────┬──────┘  ✓ Update parse_statement()
           │         ✓ Update parse_primary()
           ↓         ✓ Update parse_postfix()
    ┌─────────────┐
    │ Interpreter │  Add value types and evaluation
    │ (16 hours)  │  ✓ VAL_CLASS, VAL_OBJECT
    └──────┬──────┘  ✓ Eval NODE_CLASS_DEF
           │         ✓ Eval NODE_NEW
           ↓         ✓ Eval NODE_MEMBER_ACCESS
    ┌─────────────┐  ✓ Eval NODE_THIS
    │   Testing   │
    │  (8 hours)  │  ✓ Unit tests for each component
    └──────┬──────┘  ✓ Integration tests
           │
           ↓
    
    🎯 MILESTONE 1: Basic classes and objects working!
    
    Example working:
    ╭───────────────────────────────────╮
    │ class Point {                     │
    │     let x: int = 0                │
    │     let y: int = 0                │
    │ }                                 │
    │ let p = new Point()               │
    │ println(p.x)                      │
    ╰───────────────────────────────────╯

╔═══════════════════════════════════════════════════════════════════════╗
║                       PHASE 2: CONSTRUCTORS                            ║
║                          (Week 3, 12 hours)                           ║
╚═══════════════════════════════════════════════════════════════════════╝

    ┌─────────────┐
    │ Interpreter │  Constructor support
    │  (8 hours)  │  ✓ Recognize 'init' method
    └──────┬──────┘  ✓ Call init on new
           │         ✓ Bind parameters
           ↓
    ┌─────────────┐
    │   Testing   │  ✓ Constructor tests
    │  (4 hours)  │  ✓ Parameter passing
    └──────┬──────┘  ✓ Field initialization
           │
           ↓
    
    🎯 MILESTONE 2: Constructors with parameters!
    
    Example working:
    ╭───────────────────────────────────╮
    │ class Point {                     │
    │     let x: int = 0                │
    │     let y: int = 0                │
    │     fn init(a: int, b: int) {     │
    │         this.x = a                │
    │         this.y = b                │
    │     }                             │
    │ }                                 │
    │ let p = new Point(3, 4)           │
    ╰───────────────────────────────────╯

╔═══════════════════════════════════════════════════════════════════════╗
║                       PHASE 3: INHERITANCE                             ║
║                          (Week 4, 16 hours)                           ║
╚═══════════════════════════════════════════════════════════════════════╝

    ┌─────────────┐
    │   Lexer     │  Add inheritance tokens
    │  (1 hour)   │  ✓ TOKEN_EXTENDS
    └──────┬──────┘  ✓ TOKEN_SUPER
           │
           ↓
    ┌─────────────┐
    │     AST     │  Extend class definition
    │  (1 hour)   │  ✓ parent_name field
    └──────┬──────┘  ✓ NODE_SUPER
           │
           ↓
    ┌─────────────┐
    │   Parser    │  Parse extends clause
    │  (2 hours)  │  ✓ Update parse_class_def()
    └──────┬──────┘  ✓ Parse super.method()
           │
           ↓
    ┌─────────────┐
    │ Interpreter │  Inheritance logic
    │  (8 hours)  │  ✓ Method lookup chain
    └──────┬──────┘  ✓ Field inheritance
           │         ✓ Method overriding
           ↓         ✓ Super resolution
    ┌─────────────┐
    │   Testing   │  ✓ Inheritance tests
    │  (4 hours)  │  ✓ Override tests
    └──────┬──────┘  ✓ Super tests
           │
           ↓
    
    🎯 MILESTONE 3: Full OOP with inheritance!
    
    Example working:
    ╭───────────────────────────────────╮
    │ class Animal {                    │
    │     fn speak() {                  │
    │         println("Sound")          │
    │     }                             │
    │ }                                 │
    │ class Dog extends Animal {        │
    │     fn speak() {                  │
    │         println("Woof!")          │
    │     }                             │
    │ }                                 │
    │ let d = new Dog()                 │
    │ d.speak()  # Woof!                │
    ╰───────────────────────────────────╯

╔═══════════════════════════════════════════════════════════════════════╗
║                    PHASE 4: POLISH & ADVANCED                          ║
║                          (Week 5, 20 hours)                           ║
╚═══════════════════════════════════════════════════════════════════════╝

    ┌─────────────┐
    │Comprehensive│  ✓ Full test suite
    │   Testing   │  ✓ Edge cases
    │  (8 hours)  │  ✓ Error handling
    └──────┬──────┘  ✓ Memory leak checks
           │
           ↓
    ┌─────────────┐
    │  Examples   │  ✓ point.kl
    │  (4 hours)  │  ✓ inheritance.kl
    └──────┬──────┘  ✓ counter.kl
           │         ✓ bank_account.kl
           ↓
    ┌─────────────┐
    │Documentation│  ✓ Update README
    │  (4 hours)  │  ✓ API documentation
    └──────┬──────┘  ✓ Tutorial
           │
           ↓
    ┌─────────────┐
    │ Optional:   │  ✓ Static methods
    │  Advanced   │  ✓ Access modifiers
    │  (4 hours)  │  ✓ Type checking
    └──────┬──────┘
           │
           ↓
    
    🎉 COMPLETE: Production-ready OOP system!

                            ↓
                         ┌─────┐
                         │ END │
                         └─────┘
```

---

## Component Dependency Graph

```
                    Lexer (Tokens)
                        ↓
                    AST (Nodes)
                        ↓
                    Parser (Syntax)
                        ↓
                Interpreter (Evaluation)
                        ↓
                    Runtime (Values)
                        ↓
                   Testing (QA)

    Legend:
    ↓  = Depends on
    Each layer requires the previous to be complete
```

---

## File Modification Map

```
KLang Repository
│
├── src/
│   ├── lexer.h          🔧 Add 6 tokens
│   ├── lexer.c          🔧 Add keyword recognition
│   ├── ast.h            🔧 Add 5 node types + structs
│   ├── ast.c            🔧 Add 6 constructors + update free
│   ├── parser.h         🔧 Add 2 function declarations
│   ├── parser.c         🔧 Add 2 functions + update 3
│   ├── interpreter.h    🔧 Add 3 value types + structs
│   └── interpreter.c    🔧 Add eval cases + helpers
│
├── tests/
│   ├── test_oop.c       ✨ CREATE NEW
│   └── test_runner.c    🔧 Update to include OOP tests
│
├── examples/
│   ├── point.kl         ✨ CREATE NEW
│   ├── inheritance.kl   ✨ CREATE NEW
│   ├── counter.kl       ✨ CREATE NEW
│   └── bank_account.kl  ✨ CREATE NEW
│
├── docs/
│   ├── OOP_IMPLEMENTATION_PLAN.md  ✅ CREATED
│   ├── OOP_QUICK_START.md          ✅ CREATED
│   └── OOP_ROADMAP.md              ✅ THIS FILE
│
└── README.md            🔧 Update features list

Legend:
🔧 = Modify existing file
✨ = Create new file
✅ = Already created (documentation)
```

---

## Implementation Complexity Matrix

```
                LOW        MEDIUM       HIGH
              ─────────────────────────────────
Lexer         │    ✓    │          │          │
              │─────────│──────────│──────────│
AST           │    ✓    │          │          │
              │─────────│──────────│──────────│
Parser        │         │     ✓    │          │
              │─────────│──────────│──────────│
Interpreter   │         │          │     ✓    │
              │─────────│──────────│──────────│
Testing       │         │     ✓    │          │
              ─────────────────────────────────

Complexity Factors:
- Lexer: Simple token addition
- AST: Straightforward struct definitions
- Parser: Moderate - need to handle grammar rules
- Interpreter: Complex - value types, environments, binding
- Testing: Moderate - comprehensive coverage needed
```

---

## Agent Workflow Diagram

```
┌──────────────────────────────────────────────────────────────┐
│                      Agent Orchestration                      │
└──────────────────────────────────────────────────────────────┘

    START
      │
      ↓
┌─────────────┐
│  Planner    │  Create detailed plan (✅ DONE)
│   Agent     │  Break into phases and tasks
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  Compiler   │  Implement lexer, AST, parser
│   Agent     │  • Add tokens
└──────┬──────┘  • Define node types
       │         • Write parsing functions
       ↓
┌─────────────┐
│  Runtime    │  Implement interpreter changes
│   Agent     │  • Add value types
└──────┬──────┘  • Add evaluation logic
       │         • Update helpers
       ↓
┌─────────────┐
│  Testing    │  Create comprehensive test suite
│   Agent     │  • Unit tests
└──────┬──────┘  • Integration tests
       │
       ↓
┌─────────────┐
│Integration  │  Ensure all components work together
│   Agent     │  • Cross-component testing
└──────┬──────┘  • End-to-end validation
       │
       ↓
┌─────────────┐
│     QA      │  Validate correctness
│   Agent     │  • Run all tests
└──────┬──────┘  • Check edge cases
       │         • Verify examples
       ↓
┌─────────────┐
│    Docs     │  Update documentation
│   Agent     │  • README updates
└──────┬──────┘  • Example programs
       │         • API docs
       ↓
     END
```

---

## Risk Mitigation Timeline

```
Week 1: Foundation
├── Risk: Parser ambiguity
│   └── Mitigation: Study existing function parsing, test incrementally
├── Risk: Memory leaks
│   └── Mitigation: Implement proper cleanup from start
└── Risk: Scope creep
    └── Mitigation: Stick to basic features, defer advanced

Week 2: Core Implementation  
├── Risk: This binding complexity
│   └── Mitigation: Thorough environment testing
├── Risk: Performance issues
│   └── Mitigation: Profile early, optimize if needed
└── Risk: Integration bugs
    └── Mitigation: Continuous testing during development

Week 3: Inheritance
├── Risk: Inheritance bugs
│   └── Mitigation: Start simple (single inheritance)
├── Risk: Circular inheritance
│   └── Mitigation: Add validation during class definition
└── Risk: Method resolution complexity
    └── Mitigation: Clear lookup algorithm, test thoroughly

Week 4: Polish
├── Risk: Incomplete testing
│   └── Mitigation: Systematic test matrix, edge case focus
├── Risk: Documentation lag
│   └── Mitigation: Document as you go
└── Risk: Last-minute changes
    └── Mitigation: Feature freeze, bug fixes only
```

---

## Success Criteria Checklist

### Phase 1 Success Criteria
- [ ] All new tokens recognized by lexer
- [ ] AST nodes created and freed correctly
- [ ] Class syntax parses without errors
- [ ] Simple class creates object instance
- [ ] Member access retrieves field values
- [ ] Methods can be called on objects
- [ ] `this` resolves to current instance
- [ ] No memory leaks in basic scenarios

### Phase 2 Success Criteria
- [ ] Constructor (`init`) automatically called
- [ ] Parameters passed to constructor correctly
- [ ] Fields initialized by constructor
- [ ] Constructor can reference `this`
- [ ] Multiple constructors with different arities (optional)

### Phase 3 Success Criteria
- [ ] Child class can extend parent class
- [ ] Child inherits parent methods
- [ ] Child can override parent methods
- [ ] `super` calls parent method
- [ ] Fields from parent accessible in child
- [ ] Multi-level inheritance works (grandparent → parent → child)

### Phase 4 Success Criteria
- [ ] All unit tests pass (100%)
- [ ] All integration tests pass (100%)
- [ ] Example programs run successfully
- [ ] Documentation complete and accurate
- [ ] No known critical bugs
- [ ] Performance acceptable (within 2x of function calls)

---

## Progress Tracking Table

Use this to track implementation progress:

| Component | Task | Started | Completed | Status | Notes |
|-----------|------|---------|-----------|--------|-------|
| Lexer | Add tokens | | | ⬜ | |
| Lexer | Test tokens | | | ⬜ | |
| AST | Add node types | | | ⬜ | |
| AST | Add constructors | | | ⬜ | |
| AST | Update ast_free | | | ⬜ | |
| Parser | parse_class_def | | | ⬜ | |
| Parser | Update parse_statement | | | ⬜ | |
| Parser | Update parse_primary | | | ⬜ | |
| Parser | Update parse_postfix | | | ⬜ | |
| Interpreter | Add value types | | | ⬜ | |
| Interpreter | Add ClassVal | | | ⬜ | |
| Interpreter | Add ObjectVal | | | ⬜ | |
| Interpreter | Eval NODE_CLASS_DEF | | | ⬜ | |
| Interpreter | Eval NODE_NEW | | | ⬜ | |
| Interpreter | Eval NODE_MEMBER_ACCESS | | | ⬜ | |
| Interpreter | Eval NODE_THIS | | | ⬜ | |
| Testing | Create test_oop.c | | | ⬜ | |
| Testing | Class definition tests | | | ⬜ | |
| Testing | Object creation tests | | | ⬜ | |
| Testing | Member access tests | | | ⬜ | |
| Testing | Method call tests | | | ⬜ | |
| Examples | point.kl | | | ⬜ | |
| Examples | inheritance.kl | | | ⬜ | |
| Examples | counter.kl | | | ⬜ | |
| Docs | Update README | | | ⬜ | |

Legend: ⬜ Not started | 🟦 In progress | ✅ Complete

---

## Quick Reference: What Goes Where

```
┌─────────────────────────────────────────────────────────┐
│                  Token → AST → Value                     │
└─────────────────────────────────────────────────────────┘

class MyClass { ... }
  │
  ├─ Lexer: TOKEN_CLASS, TOKEN_IDENT("MyClass"), ...
  │
  ├─ Parser: NODE_CLASS_DEF { name="MyClass", ... }
  │
  └─ Interpreter: VAL_CLASS { name="MyClass", methods=... }


new MyClass()
  │
  ├─ Lexer: TOKEN_NEW, TOKEN_IDENT("MyClass"), ...
  │
  ├─ Parser: NODE_NEW { class_name="MyClass", args=[] }
  │
  └─ Interpreter: VAL_OBJECT { class_ref=..., fields=... }


obj.field
  │
  ├─ Lexer: TOKEN_IDENT("obj"), TOKEN_DOT, TOKEN_IDENT("field")
  │
  ├─ Parser: NODE_MEMBER_ACCESS { object=..., member_name="field" }
  │
  └─ Interpreter: Value (field value from object)


this
  │
  ├─ Lexer: TOKEN_THIS
  │
  ├─ Parser: NODE_THIS
  │
  └─ Interpreter: Value (current instance from environment)
```

---

## Final Checklist Before Starting

Before you begin implementation, ensure:

- [x] Plan reviewed and understood
- [x] Development environment set up
- [x] Can compile and run KLang
- [ ] Git branch created for OOP work
- [ ] Backup of current working code
- [ ] Tests passing on current codebase
- [ ] Team aligned on plan (if applicable)
- [ ] Time allocated (2-3 weeks)
- [ ] Reference materials ready (Crafting Interpreters, etc.)

---

## Resources

### Documentation
- `docs/OOP_IMPLEMENTATION_PLAN.md` - Detailed step-by-step guide
- `docs/OOP_QUICK_START.md` - Quick reference
- `docs/architecture.md` - Overall architecture

### Code References
- `src/parser.c` lines 295-362 - Function parsing (similar to class parsing)
- `src/interpreter.c` NODE_FUNC_DEF case - Function evaluation pattern
- `tests/test_parser.c` - Testing patterns to follow

### External Resources
- **Crafting Interpreters**: https://craftinginterpreters.com/ (Chapters 12-13)
- **Python Language Reference**: Class definitions
- **JavaScript MDN**: Object-oriented programming

---

**Start Date**: ___________  
**Target Completion**: ___________  
**Actual Completion**: ___________

Good luck! 🚀
