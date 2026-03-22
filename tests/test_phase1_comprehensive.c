/* 
 * Comprehensive Phase 1 JavaScript Features Test Suite
 * Tests: var/let/const, arrow functions, template literals, enhanced object literals
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/ast.h"
#include "../src/interpreter.h"

/* Global test counters */
static int g_test_total = 0;
static int g_test_passed = 0;
static int g_test_failed = 0;

#define TEST_START(name) do { \
    printf("\n🔵 Testing: %s\n", name); \
    g_test_total++; \
} while(0)

#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "  ❌ FAIL: %s at %s:%d\n", msg, __FILE__, __LINE__); \
        g_test_failed++; \
        return 0; \
    } \
} while (0)

#define ASSERT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        fprintf(stderr, "  ❌ FAIL: %s (%d != %d) at %s:%d\n", \
                msg, (int)(a), (int)(b), __FILE__, __LINE__); \
        g_test_failed++; \
        return 0; \
    } \
} while (0)

#define ASSERT_STR_EQ(a, b, msg) do { \
    if (strcmp((a), (b)) != 0) { \
        fprintf(stderr, "  ❌ FAIL: %s (\"%s\" != \"%s\") at %s:%d\n", \
                msg, (a), (b), __FILE__, __LINE__); \
        g_test_failed++; \
        return 0; \
    } \
} while (0)

#define TEST_END() do { \
    printf("  ✅ PASS\n"); \
    g_test_passed++; \
    return 1; \
} while(0)

static Value run_code(const char *source) {
    Lexer lexer;
    Parser parser;
    ASTNode **nodes;
    int count;
    Interpreter *interp;
    Value result;
    int i;

    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);
    
    interp = interpreter_new();
    result = make_null();
    
    for (i = 0; i < count; i++) {
        value_free(&result);
        result = eval_node(interp, nodes[i]);
    }
    
    for (i = 0; i < count; i++) ast_free(nodes[i]);
    free(nodes);
    interpreter_free(interp);
    lexer_free(&lexer);
    return result;
}

/* ===== VAR/LET/CONST TESTS ===== */

int test_var_basic() {
    Value v = run_code("var x = 42\nx");
    ASSERT_EQ(v.type, VAL_INT, "var should create integer variable");
    ASSERT_EQ(v.as.int_val, 42, "var should store correct value");
    value_free(&v);
    TEST_END();
}

int test_let_basic() {
    Value v = run_code("let y = 100\ny");
    ASSERT_EQ(v.type, VAL_INT, "let should create integer variable");
    ASSERT_EQ(v.as.int_val, 100, "let should store correct value");
    value_free(&v);
    TEST_END();
}

int test_const_basic() {
    Value v = run_code("const PI = 3.14\nPI");
    ASSERT_EQ(v.type, VAL_FLOAT, "const should create float variable");
    value_free(&v);
    TEST_END();
}

int test_var_reassignment() {
    Value v = run_code("var x = 1\nx = 2\nx");
    ASSERT_EQ(v.type, VAL_INT, "var should allow reassignment");
    ASSERT_EQ(v.as.int_val, 2, "var should have new value");
    value_free(&v);
    TEST_END();
}

int test_let_reassignment() {
    Value v = run_code("let x = 1\nx = 2\nx");
    ASSERT_EQ(v.type, VAL_INT, "let should allow reassignment");
    ASSERT_EQ(v.as.int_val, 2, "let should have new value");
    value_free(&v);
    TEST_END();
}

int test_var_redeclaration() {
    Value v = run_code("var x = 1\nvar x = 2\nx");
    ASSERT_EQ(v.type, VAL_INT, "var should allow redeclaration");
    ASSERT_EQ(v.as.int_val, 2, "var should have new value after redeclaration");
    value_free(&v);
    TEST_END();
}

int test_let_block_scope() {
    Value v = run_code("let x = 10\n{ let x = 20 }\nx");
    ASSERT_EQ(v.type, VAL_INT, "let should respect block scope");
    ASSERT_EQ(v.as.int_val, 10, "outer let should not be affected by inner scope");
    value_free(&v);
    TEST_END();
}

int test_var_without_init() {
    Value v = run_code("var x\nx");
    ASSERT_EQ(v.type, VAL_NULL, "var without init should be null");
    value_free(&v);
    TEST_END();
}

int test_let_without_init() {
    Value v = run_code("let x\nx");
    ASSERT_EQ(v.type, VAL_NULL, "let without init should be null");
    value_free(&v);
    TEST_END();
}

/* ===== ARROW FUNCTION TESTS ===== */

int test_arrow_single_param() {
    Value v = run_code("const square = x => x * x\nsquare(5)");
    ASSERT_EQ(v.type, VAL_INT, "arrow function should return integer");
    ASSERT_EQ(v.as.int_val, 25, "arrow function should calculate correctly");
    value_free(&v);
    TEST_END();
}

int test_arrow_multiple_params() {
    Value v = run_code("const add = (a, b) => a + b\nadd(3, 7)");
    ASSERT_EQ(v.type, VAL_INT, "arrow function with multiple params should work");
    ASSERT_EQ(v.as.int_val, 10, "arrow function should add correctly");
    value_free(&v);
    TEST_END();
}

int test_arrow_no_params() {
    Value v = run_code("const greet = () => \"Hello!\"\ngreet()");
    ASSERT_EQ(v.type, VAL_STRING, "arrow function with no params should return string");
    ASSERT_STR_EQ(v.as.str_val, "Hello!", "arrow function should return correct string");
    value_free(&v);
    TEST_END();
}

int test_arrow_block_body() {
    Value v = run_code("const complex = x => { let y = x * 2\n return y + 1 }\ncomplex(10)");
    ASSERT_EQ(v.type, VAL_INT, "arrow function with block body should work");
    ASSERT_EQ(v.as.int_val, 21, "arrow function should calculate correctly");
    value_free(&v);
    TEST_END();
}

int test_arrow_with_var() {
    Value v = run_code("var double = x => x * 2\ndouble(6)");
    ASSERT_EQ(v.type, VAL_INT, "arrow function with var should work");
    ASSERT_EQ(v.as.int_val, 12, "arrow function should calculate correctly");
    value_free(&v);
    TEST_END();
}

int test_arrow_with_let() {
    Value v = run_code("let triple = x => x * 3\ntriple(7)");
    ASSERT_EQ(v.type, VAL_INT, "arrow function with let should work");
    ASSERT_EQ(v.as.int_val, 21, "arrow function should calculate correctly");
    value_free(&v);
    TEST_END();
}

/* ===== TEMPLATE LITERAL TESTS ===== */

int test_template_basic() {
    Value v = run_code("`Hello, World!`");
    ASSERT_EQ(v.type, VAL_STRING, "template literal should return string");
    ASSERT_STR_EQ(v.as.str_val, "Hello, World!", "template literal should contain correct text");
    value_free(&v);
    TEST_END();
}

int test_template_empty() {
    Value v = run_code("``");
    ASSERT_EQ(v.type, VAL_STRING, "empty template literal should return string");
    ASSERT_STR_EQ(v.as.str_val, "", "empty template literal should be empty");
    value_free(&v);
    TEST_END();
}

int test_template_interpolation() {
    Value v = run_code("let name = \"Alice\"\n`Hello, ${name}!`");
    ASSERT_EQ(v.type, VAL_STRING, "template with interpolation should return string");
    ASSERT_STR_EQ(v.as.str_val, "Hello, Alice!", "template should interpolate variable");
    value_free(&v);
    TEST_END();
}

int test_template_multiple_interpolations() {
    Value v = run_code("let x = 10\nlet y = 20\n`${x} + ${y} = ${x + y}`");
    ASSERT_EQ(v.type, VAL_STRING, "template with multiple interpolations should work");
    ASSERT_STR_EQ(v.as.str_val, "10 + 20 = 30", "template should calculate expressions");
    value_free(&v);
    TEST_END();
}

int test_template_with_number() {
    Value v = run_code("let age = 25\n`Age: ${age}`");
    ASSERT_EQ(v.type, VAL_STRING, "template should convert number to string");
    ASSERT_STR_EQ(v.as.str_val, "Age: 25", "template should show number correctly");
    value_free(&v);
    TEST_END();
}

int test_template_multiline() {
    Value v = run_code("`Line 1\nLine 2\nLine 3`");
    ASSERT_EQ(v.type, VAL_STRING, "multiline template should work");
    ASSERT_STR_EQ(v.as.str_val, "Line 1\nLine 2\nLine 3", "multiline template should preserve newlines");
    value_free(&v);
    TEST_END();
}

int test_template_with_expression() {
    Value v = run_code("let price = 100\n`Total: ${price * 1.08}`");
    ASSERT_EQ(v.type, VAL_STRING, "template should evaluate expressions");
    value_free(&v);
    TEST_END();
}

/* ===== ENHANCED OBJECT LITERALS TESTS ===== */

int test_object_shorthand() {
    Value v = run_code("let x = 10\nlet y = 20\nlet obj = {x, y}\nobj.x");
    ASSERT_EQ(v.type, VAL_INT, "object shorthand should work");
    ASSERT_EQ(v.as.int_val, 10, "object shorthand should use variable value");
    value_free(&v);
    TEST_END();
}

int test_object_method_shorthand() {
    Value v = run_code("let obj = { greet() { return \"Hi!\" } }\nobj.greet()");
    ASSERT_EQ(v.type, VAL_STRING, "object method shorthand should work");
    ASSERT_STR_EQ(v.as.str_val, "Hi!", "object method should return correct value");
    value_free(&v);
    TEST_END();
}

int test_object_computed_property() {
    Value v = run_code("let key = \"test\"\nlet obj = { [key]: 42 }\nobj.test");
    ASSERT_EQ(v.type, VAL_INT, "computed property should work");
    ASSERT_EQ(v.as.int_val, 42, "computed property should have correct value");
    value_free(&v);
    TEST_END();
}

int test_object_mixed() {
    Value v = run_code("let name = \"Bob\"\nlet obj = { name, greet() { return \"Hello\" } }\nobj.greet()");
    ASSERT_EQ(v.type, VAL_STRING, "mixed object features should work");
    ASSERT_STR_EQ(v.as.str_val, "Hello", "mixed object should work correctly");
    value_free(&v);
    TEST_END();
}

/* ===== COMBINED FEATURES TESTS ===== */

int test_combined_arrow_template() {
    Value v = run_code("const greet = name => `Hello, ${name}!`\ngreet(\"World\")");
    ASSERT_EQ(v.type, VAL_STRING, "arrow function with template should work");
    ASSERT_STR_EQ(v.as.str_val, "Hello, World!", "combined features should work correctly");
    value_free(&v);
    TEST_END();
}

int test_combined_const_arrow() {
    Value v = run_code("const add = (a, b) => a + b\nconst x = 5\nconst y = 10\nadd(x, y)");
    ASSERT_EQ(v.type, VAL_INT, "const with arrow function should work");
    ASSERT_EQ(v.as.int_val, 15, "const variables should work with arrow functions");
    value_free(&v);
    TEST_END();
}

int test_combined_let_template() {
    Value v = run_code("let user = \"Alice\"\nlet age = 30\n`${user} is ${age} years old`");
    ASSERT_EQ(v.type, VAL_STRING, "let with template should work");
    ASSERT_STR_EQ(v.as.str_val, "Alice is 30 years old", "let variables should work in templates");
    value_free(&v);
    TEST_END();
}

int test_combined_var_object() {
    Value v = run_code("var x = 10\nvar y = 20\nvar point = {x, y}\npoint.x + point.y");
    ASSERT_EQ(v.type, VAL_INT, "var with object shorthand should work");
    ASSERT_EQ(v.as.int_val, 30, "object shorthand should work with var");
    value_free(&v);
    TEST_END();
}

int test_combined_all_features() {
    const char *code = 
        "const format = (name, age) => `Name: ${name}, Age: ${age}`\n"
        "let name = \"Charlie\"\n"
        "let age = 25\n"
        "let person = {name, age}\n"
        "format(person.name, person.age)";
    
    Value v = run_code(code);
    ASSERT_EQ(v.type, VAL_STRING, "all features combined should work");
    ASSERT_STR_EQ(v.as.str_val, "Name: Charlie, Age: 25", "all features should work together");
    value_free(&v);
    TEST_END();
}

/* ===== MAIN TEST RUNNER ===== */

int main(void) {
    printf("\n");
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║                                           ║\n");
    printf("║  Phase 1 JavaScript Features Test Suite  ║\n");
    printf("║                                           ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    
    printf("\n━━━ VAR/LET/CONST Tests ━━━\n");
    TEST_START("var basic declaration");
    g_test_passed += test_var_basic();
    
    TEST_START("let basic declaration");
    g_test_passed += test_let_basic();
    
    TEST_START("const basic declaration");
    g_test_passed += test_const_basic();
    
    TEST_START("var reassignment");
    g_test_passed += test_var_reassignment();
    
    TEST_START("let reassignment");
    g_test_passed += test_let_reassignment();
    
    TEST_START("var redeclaration");
    g_test_passed += test_var_redeclaration();
    
    TEST_START("let block scope");
    g_test_passed += test_let_block_scope();
    
    TEST_START("var without initialization");
    g_test_passed += test_var_without_init();
    
    TEST_START("let without initialization");
    g_test_passed += test_let_without_init();
    
    printf("\n━━━ Arrow Function Tests ━━━\n");
    TEST_START("arrow function - single parameter");
    g_test_passed += test_arrow_single_param();
    
    TEST_START("arrow function - multiple parameters");
    g_test_passed += test_arrow_multiple_params();
    
    TEST_START("arrow function - no parameters");
    g_test_passed += test_arrow_no_params();
    
    TEST_START("arrow function - block body");
    g_test_passed += test_arrow_block_body();
    
    TEST_START("arrow function with var");
    g_test_passed += test_arrow_with_var();
    
    TEST_START("arrow function with let");
    g_test_passed += test_arrow_with_let();
    
    printf("\n━━━ Template Literal Tests ━━━\n");
    TEST_START("template literal - basic");
    g_test_passed += test_template_basic();
    
    TEST_START("template literal - empty");
    g_test_passed += test_template_empty();
    
    TEST_START("template literal - interpolation");
    g_test_passed += test_template_interpolation();
    
    TEST_START("template literal - multiple interpolations");
    g_test_passed += test_template_multiple_interpolations();
    
    TEST_START("template literal - with number");
    g_test_passed += test_template_with_number();
    
    TEST_START("template literal - multiline");
    g_test_passed += test_template_multiline();
    
    TEST_START("template literal - with expression");
    g_test_passed += test_template_with_expression();
    
    printf("\n━━━ Enhanced Object Literal Tests ━━━\n");
    TEST_START("object - property shorthand");
    g_test_passed += test_object_shorthand();
    
    TEST_START("object - method shorthand");
    g_test_passed += test_object_method_shorthand();
    
    TEST_START("object - computed property");
    g_test_passed += test_object_computed_property();
    
    TEST_START("object - mixed features");
    g_test_passed += test_object_mixed();
    
    printf("\n━━━ Combined Features Tests ━━━\n");
    TEST_START("arrow function + template literal");
    g_test_passed += test_combined_arrow_template();
    
    TEST_START("const + arrow function");
    g_test_passed += test_combined_const_arrow();
    
    TEST_START("let + template literal");
    g_test_passed += test_combined_let_template();
    
    TEST_START("var + object shorthand");
    g_test_passed += test_combined_var_object();
    
    TEST_START("all features combined");
    g_test_passed += test_combined_all_features();
    
    /* Report results */
    printf("\n");
    printf("═══════════════════════════════════════════\n");
    printf("  Test Results\n");
    printf("═══════════════════════════════════════════\n");
    printf("  Total:  %d\n", g_test_total);
    printf("  Passed: %d ✅\n", g_test_passed);
    printf("  Failed: %d ❌\n", g_test_failed);
    printf("═══════════════════════════════════════════\n");
    if (g_test_failed == 0) {
        printf("\n🎉 ALL TESTS PASSED! 🎉\n\n");
    } else {
        printf("\n⚠️  SOME TESTS FAILED ⚠️\n\n");
    }
    
    return g_test_failed;
}
