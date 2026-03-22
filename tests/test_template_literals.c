#include "../src/interpreter.h"
#include "../src/parser.h"
#include "../src/lexer.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_STR_EQ(a, b) assert(strcmp((a), (b)) == 0)

static Value run_expr(const char *code) {
    Lexer lexer;
    Parser parser;
    Interpreter interp;
    ASTNode *node;
    Value result;
    
    lexer_init(&lexer, code);
    parser_init(&parser, &lexer);
    node = parse_expression(&parser);
    
    interpreter_init(&interp);
    result = eval_node(&interp, node);
    
    ast_free(node);
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(&interp);
    
    return result;
}

static Value run_program(const char *code) {
    Lexer lexer;
    Parser parser;
    Interpreter interp;
    ASTNode *node;
    Value result;
    
    lexer_init(&lexer, code);
    parser_init(&parser, &lexer);
    node = parse(&parser);
    
    interpreter_init(&interp);
    result = eval_node(&interp, node);
    
    ast_free(node);
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(&interp);
    
    return result;
}

void test_basic_template_literal(void) {
    printf("Testing basic template literal...\n");
    Value v = run_expr("`Hello, World!`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Hello, World!");
    value_free(&v);
}

void test_empty_template_literal(void) {
    printf("Testing empty template literal...\n");
    Value v = run_expr("``");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "");
    value_free(&v);
}

void test_template_literal_with_single_interpolation(void) {
    printf("Testing template literal with single interpolation...\n");
    Value v = run_program("let name = \"Alice\"\n`Hello, ${name}!`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Hello, Alice!");
    value_free(&v);
}

void test_template_literal_with_multiple_interpolations(void) {
    printf("Testing template literal with multiple interpolations...\n");
    Value v = run_program("let x = 10\nlet y = 20\n`${x} + ${y} = ${x + y}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "10 + 20 = 30");
    value_free(&v);
}

void test_template_literal_with_number(void) {
    printf("Testing template literal with number...\n");
    Value v = run_program("let age = 25\n`Age: ${age}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Age: 25");
    value_free(&v);
}

void test_template_literal_with_expression(void) {
    printf("Testing template literal with expression...\n");
    Value v = run_program("let price = 100\nlet tax = 0.08\n`Total: $${price * (1 + tax)}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Total: $108");
    value_free(&v);
}

void test_template_literal_multiline(void) {
    printf("Testing multiline template literal...\n");
    Value v = run_expr("`Line 1\nLine 2\nLine 3`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Line 1\nLine 2\nLine 3");
    value_free(&v);
}

void test_template_literal_with_nested_function_call(void) {
    printf("Testing template literal with nested function call...\n");
    const char *code = 
        "fn double(x) { return x * 2 }\n"
        "let n = 5\n"
        "`Result: ${double(n)}`";
    Value v = run_program(code);
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Result: 10");
    value_free(&v);
}

void test_template_literal_with_boolean(void) {
    printf("Testing template literal with boolean...\n");
    Value v = run_program("let flag = true\n`Flag is ${flag}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Flag is true");
    value_free(&v);
}

void test_template_literal_at_start(void) {
    printf("Testing template literal with interpolation at start...\n");
    Value v = run_program("let name = \"Bob\"\n`${name} is here`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Bob is here");
    value_free(&v);
}

void test_template_literal_at_end(void) {
    printf("Testing template literal with interpolation at end...\n");
    Value v = run_program("let name = \"Charlie\"\n`Name: ${name}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Name: Charlie");
    value_free(&v);
}

void test_template_literal_only_interpolation(void) {
    printf("Testing template literal with only interpolation...\n");
    Value v = run_program("let val = 42\n`${val}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "42");
    value_free(&v);
}

void test_template_literal_with_comparison(void) {
    printf("Testing template literal with comparison...\n");
    Value v = run_program("let x = 10\nlet y = 5\n`${x} > ${y}: ${x > y}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "10 > 5: true");
    value_free(&v);
}

void test_template_literal_with_list_length(void) {
    printf("Testing template literal with list length...\n");
    Value v = run_program("let items = [1, 2, 3]\n`Count: ${items.length}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Count: 3");
    value_free(&v);
}

void test_template_literal_concatenation(void) {
    printf("Testing template literal concatenation...\n");
    Value v = run_program("let a = `Hello`\nlet b = ` World`\na + b");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Hello World");
    value_free(&v);
}

void test_template_literal_in_function_return(void) {
    printf("Testing template literal in function return...\n");
    const char *code = 
        "fn greet(name) { return `Hello, ${name}!` }\n"
        "greet(\"Dave\")";
    Value v = run_program(code);
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Hello, Dave!");
    value_free(&v);
}

void test_template_literal_with_string_concat(void) {
    printf("Testing template literal with string concatenation in interpolation...\n");
    Value v = run_program("let first = \"John\"\nlet last = \"Doe\"\n`Name: ${first + \" \" + last}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Name: John Doe");
    value_free(&v);
}

void test_template_literal_with_float(void) {
    printf("Testing template literal with float...\n");
    Value v = run_program("let pi = 3.14159\n`Pi is approximately ${pi}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Pi is approximately 3.14159");
    value_free(&v);
}

void test_template_literal_poem(void) {
    printf("Testing multiline template literal (poem)...\n");
    Value v = run_expr("`Roses are red\nViolets are blue\nKLang is awesome\nAnd so are you!`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "Roses are red\nViolets are blue\nKLang is awesome\nAnd so are you!");
    value_free(&v);
}

void test_template_literal_with_escaped_backtick(void) {
    printf("Testing template literal with escaped backtick...\n");
    Value v = run_expr("`This is a backtick: \\``");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "This is a backtick: `");
    value_free(&v);
}

void test_template_literal_adjacent_interpolations(void) {
    printf("Testing template literal with adjacent interpolations...\n");
    Value v = run_program("let a = \"Hello\"\nlet b = \"World\"\n`${a}${b}`");
    ASSERT_EQ(v.type, VAL_STRING);
    ASSERT_STR_EQ(v.as.str_val, "HelloWorld");
    value_free(&v);
}

int main(void) {
    printf("Running template literal tests...\n\n");
    
    test_basic_template_literal();
    test_empty_template_literal();
    test_template_literal_with_single_interpolation();
    test_template_literal_with_multiple_interpolations();
    test_template_literal_with_number();
    test_template_literal_with_expression();
    test_template_literal_multiline();
    test_template_literal_with_nested_function_call();
    test_template_literal_with_boolean();
    test_template_literal_at_start();
    test_template_literal_at_end();
    test_template_literal_only_interpolation();
    test_template_literal_with_comparison();
    test_template_literal_with_list_length();
    test_template_literal_concatenation();
    test_template_literal_in_function_return();
    test_template_literal_with_string_concat();
    test_template_literal_with_float();
    test_template_literal_poem();
    test_template_literal_with_escaped_backtick();
    test_template_literal_adjacent_interpolations();
    
    printf("\n✓ All template literal tests passed!\n");
    return 0;
}
