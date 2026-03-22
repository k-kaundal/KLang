#include "../src/interpreter.h"
#include "../src/runtime.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Test helper
#define TEST(name) printf("\n[TEST] %s\n", name)
#define PASS() printf("✓ PASS\n")
#define FAIL(msg) { printf("✗ FAIL: %s\n", msg); return 0; }

// Run a K file and check if it succeeded
int run_test_file(const char *filename) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "./klang run %s > /tmp/klang_test_output.txt 2>&1", filename);
    int result = system(cmd);
    return (result == 0);
}

int main() {
    printf("===========================================\n");
    printf("  KLang Promise Tests\n");
    printf("===========================================\n");
    
    // Test 1: Promise.resolve
    TEST("Promise.resolve()");
    FILE *f = fopen("/tmp/test_promise_resolve.k", "w");
    fprintf(f, "const p = Promise.resolve(42)\n");
    fprintf(f, "println(p)\n");
    fclose(f);
    
    if (run_test_file("/tmp/test_promise_resolve.k")) {
        PASS();
    } else {
        FAIL("Promise.resolve failed");
    }
    
    // Test 2: Promise.reject
    TEST("Promise.reject()");
    f = fopen("/tmp/test_promise_reject.k", "w");
    fprintf(f, "const p = Promise.reject('error')\n");
    fprintf(f, "println(p)\n");
    fclose(f);
    
    if (run_test_file("/tmp/test_promise_reject.k")) {
        PASS();
    } else {
        FAIL("Promise.reject failed");
    }
    
    // Test 3: Promise.then()
    TEST("Promise.then() chaining");
    f = fopen("/tmp/test_promise_then.k", "w");
    fprintf(f, "Promise.resolve(10).then(v => {\n");
    fprintf(f, "    println('Got:', v)\n");
    fprintf(f, "    return v * 2\n");
    fprintf(f, "}).then(v => {\n");
    fprintf(f, "    println('Doubled:', v)\n");
    fprintf(f, "})\n");
    fclose(f);
    
    if (run_test_file("/tmp/test_promise_then.k")) {
        PASS();
    } else {
        FAIL("Promise.then() failed");
    }
    
    // Test 4: Promise.catch()
    TEST("Promise.catch() error handling");
    f = fopen("/tmp/test_promise_catch.k", "w");
    fprintf(f, "Promise.reject('error').catch(e => {\n");
    fprintf(f, "    println('Caught:', e)\n");
    fprintf(f, "})\n");
    fclose(f);
    
    if (run_test_file("/tmp/test_promise_catch.k")) {
        PASS();
    } else {
        FAIL("Promise.catch() failed");
    }
    
    // Test 5: Promise constructor
    TEST("new Promise((resolve, reject) => {...})");
    f = fopen("/tmp/test_promise_constructor.k", "w");
    fprintf(f, "const p = new Promise((resolve, reject) => {\n");
    fprintf(f, "    resolve(123)\n");
    fprintf(f, "})\n");
    fprintf(f, "p.then(v => println('Value:', v))\n");
    fclose(f);
    
    if (run_test_file("/tmp/test_promise_constructor.k")) {
        PASS();
    } else {
        FAIL("Promise constructor failed");
    }
    
    printf("\n===========================================\n");
    printf("  All Promise Tests Complete!\n");
    printf("===========================================\n");
    
    return 0;
}
