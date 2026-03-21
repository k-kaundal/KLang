#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdio.h>
#include <string.h>

extern int test_pass_count;
extern int test_fail_count;

#define ASSERT_TRUE(expr) do { \
    if (expr) { test_pass_count++; } \
    else { test_fail_count++; printf("FAIL: %s:%d: %s\n", __FILE__, __LINE__, #expr); } \
} while(0)

#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))
#define ASSERT_STR_EQ(a, b) ASSERT_TRUE(strcmp((a), (b)) == 0)

void run_lexer_tests(void);
void run_parser_tests(void);
void run_interpreter_tests(void);

#endif
