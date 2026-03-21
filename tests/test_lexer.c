#include "test_runner.h"
#include "lexer.h"
#include <stdio.h>

void run_lexer_tests(void) {
    printf("--- Lexer Tests ---\n");

    {
        Lexer lexer;
        Token t;
        lexer_init(&lexer, "let x = 42");

        t = lexer_next_token(&lexer);
        ASSERT_EQ(t.type, TOKEN_LET);
        token_free(&t);

        t = lexer_next_token(&lexer);
        ASSERT_EQ(t.type, TOKEN_IDENT);
        ASSERT_STR_EQ(t.value, "x");
        token_free(&t);

        t = lexer_next_token(&lexer);
        ASSERT_EQ(t.type, TOKEN_ASSIGN);
        token_free(&t);

        t = lexer_next_token(&lexer);
        ASSERT_EQ(t.type, TOKEN_NUMBER);
        ASSERT_STR_EQ(t.value, "42");
        token_free(&t);

        t = lexer_next_token(&lexer);
        ASSERT_EQ(t.type, TOKEN_EOF);
        token_free(&t);

        lexer_free(&lexer);
    }

    {
        Lexer lexer;
        Token t;
        lexer_init(&lexer, "fn if else while for return break continue in true false");

        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_FN); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_IF); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_ELSE); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_WHILE); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_FOR); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_RETURN); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_BREAK); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_CONTINUE); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_IN); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_TRUE); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_FALSE); token_free(&t);
        lexer_free(&lexer);
    }

    {
        Lexer lexer;
        Token t;
        lexer_init(&lexer, "+ - * / % == != < > <= >= = ! -> ..");

        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_PLUS); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_MINUS); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_STAR); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_SLASH); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_PERCENT); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_EQ); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_NEQ); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_LT); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_GT); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_LEQ); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_GEQ); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_ASSIGN); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_BANG); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_ARROW); token_free(&t);
        t = lexer_next_token(&lexer); ASSERT_EQ(t.type, TOKEN_DOTDOT); token_free(&t);
        lexer_free(&lexer);
    }

    {
        Lexer lexer;
        Token t;
        lexer_init(&lexer, "\"hello world\"");
        t = lexer_next_token(&lexer);
        ASSERT_EQ(t.type, TOKEN_STRING);
        ASSERT_STR_EQ(t.value, "hello world");
        token_free(&t);
        lexer_free(&lexer);
    }

    {
        Lexer lexer;
        Token peek;
        Token next;
        lexer_init(&lexer, "fn main");
        peek = lexer_peek_token(&lexer);
        ASSERT_EQ(peek.type, TOKEN_FN);
        next = lexer_next_token(&lexer);
        ASSERT_EQ(next.type, TOKEN_FN);
        token_free(&next);
        lexer_free(&lexer);
        (void)peek;
    }

    printf("Lexer tests done.\n");
}
