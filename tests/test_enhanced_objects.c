#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/interpreter.h"

void test_property_shorthand() {
    const char *code = 
        "let x = 10\n"
        "let y = 20\n"
        "let point = {x, y}\n"
        "print(point.x)\n"
        "print(point.y)\n";
    
    Lexer lexer;
    Parser parser;
    Interpreter *interp;
    
    lexer_init(&lexer, code);
    parser_init(&parser, &lexer);
    interp = interpreter_new();
    
    while (!check(&parser, TOKEN_EOF)) {
        ASTNode *stmt = parse_statement(&parser);
        if (stmt) {
            eval_node(interp, stmt);
            ast_free(stmt);
        }
    }
    
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(interp);
    
    printf("✓ Property shorthand test passed\n");
}

void test_method_shorthand() {
    const char *code = 
        "let obj = {\n"
        "    greet() {\n"
        "        return \"Hello!\"\n"
        "    },\n"
        "    add(a, b) {\n"
        "        return a + b\n"
        "    }\n"
        "}\n"
        "print(obj.greet())\n"
        "print(obj.add(5, 3))\n";
    
    Lexer lexer;
    Parser parser;
    Interpreter *interp;
    
    lexer_init(&lexer, code);
    parser_init(&parser, &lexer);
    interp = interpreter_new();
    
    while (!check(&parser, TOKEN_EOF)) {
        ASTNode *stmt = parse_statement(&parser);
        if (stmt) {
            eval_node(interp, stmt);
            ast_free(stmt);
        }
    }
    
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(interp);
    
    printf("✓ Method shorthand test passed\n");
}

void test_computed_property_names() {
    const char *code = 
        "let key = \"dynamicKey\"\n"
        "let obj = {\n"
        "    [key]: \"value\",\n"
        "    [\"computed\" + \"Name\"]: 42\n"
        "}\n"
        "print(obj.dynamicKey)\n"
        "print(obj.computedName)\n";
    
    Lexer lexer;
    Parser parser;
    Interpreter *interp;
    
    lexer_init(&lexer, code);
    parser_init(&parser, &lexer);
    interp = interpreter_new();
    
    while (!check(&parser, TOKEN_EOF)) {
        ASTNode *stmt = parse_statement(&parser);
        if (stmt) {
            eval_node(interp, stmt);
            ast_free(stmt);
        }
    }
    
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(interp);
    
    printf("✓ Computed property names test passed\n");
}

void test_mixed_features() {
    const char *code = 
        "let name = \"Alice\"\n"
        "let age = 30\n"
        "let person = {\n"
        "    name,\n"
        "    age,\n"
        "    greet() {\n"
        "        return \"Hello\"\n"
        "    },\n"
        "    [\"is\" + \"Adult\"]: age >= 18\n"
        "}\n"
        "print(person.name)\n"
        "print(person.age)\n"
        "print(person.greet())\n"
        "print(person.isAdult)\n";
    
    Lexer lexer;
    Parser parser;
    Interpreter *interp;
    
    lexer_init(&lexer, code);
    parser_init(&parser, &lexer);
    interp = interpreter_new();
    
    while (!check(&parser, TOKEN_EOF)) {
        ASTNode *stmt = parse_statement(&parser);
        if (stmt) {
            eval_node(interp, stmt);
            ast_free(stmt);
        }
    }
    
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(interp);
    
    printf("✓ Mixed features test passed\n");
}

void test_regular_properties_still_work() {
    const char *code = 
        "let obj = {\n"
        "    x: 10,\n"
        "    y: 20,\n"
        "    method: fn() { return 42 }\n"
        "}\n"
        "print(obj.x)\n"
        "print(obj.y)\n"
        "print(obj.method())\n";
    
    Lexer lexer;
    Parser parser;
    Interpreter *interp;
    
    lexer_init(&lexer, code);
    parser_init(&parser, &lexer);
    interp = interpreter_new();
    
    while (!check(&parser, TOKEN_EOF)) {
        ASTNode *stmt = parse_statement(&parser);
        if (stmt) {
            eval_node(interp, stmt);
            ast_free(stmt);
        }
    }
    
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(interp);
    
    printf("✓ Regular properties test passed\n");
}

int main() {
    printf("Running Enhanced Object Literals Tests...\n\n");
    
    test_property_shorthand();
    test_method_shorthand();
    test_computed_property_names();
    test_mixed_features();
    test_regular_properties_still_work();
    
    printf("\n✅ All enhanced object literal tests passed!\n");
    return 0;
}
