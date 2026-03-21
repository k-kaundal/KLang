#include "test_runner.h"
#include <stdio.h>

int test_pass_count = 0;
int test_fail_count = 0;

int main(void) {
    printf("Running KLang tests...\n\n");
    run_lexer_tests();
    run_parser_tests();
    run_interpreter_tests();
    printf("\n=== Results: %d passed, %d failed ===\n", test_pass_count, test_fail_count);
    return test_fail_count > 0 ? 1 : 0;
}
