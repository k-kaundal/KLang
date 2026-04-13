/**
 * test_capability_implementation.c
 * 
 * Test suite for capability.c and security.c implementations
 * Verifies all functions work correctly
 */

#include "../../include/capability.h"
#include "../../include/security.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Test counters */
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, message) do { \
    if (condition) { \
        printf("  ✓ %s\n", message); \
        tests_passed++; \
    } else { \
        printf("  ✗ %s\n", message); \
        tests_failed++; \
    } \
} while (0)

/* ===================================================================
 * Pattern Matching Tests
 * =================================================================== */

void test_pattern_matching(void) {
    printf("\n=== Pattern Matching Tests ===\n");
    
    /* Basic wildcard tests */
    TEST_ASSERT(resource_matches_pattern("/tmp/file.txt", "/tmp/*"),
                "Basic wildcard: /tmp/* matches /tmp/file.txt");
    TEST_ASSERT(!resource_matches_pattern("/tmp/subdir/file.txt", "/tmp/*"),
                "Basic wildcard: /tmp/* doesn't match /tmp/subdir/file.txt");
    
    /* Recursive wildcard tests */
    TEST_ASSERT(resource_matches_pattern("/tmp/subdir/file.txt", "/tmp/**"),
                "Recursive wildcard: /tmp/** matches /tmp/subdir/file.txt");
    TEST_ASSERT(resource_matches_pattern("/tmp/a/b/c/file.txt", "/tmp/**"),
                "Recursive wildcard: /tmp/** matches deep paths");
    
    /* Extension matching */
    TEST_ASSERT(resource_matches_pattern("file.json", "*.json"),
                "Extension match: *.json matches file.json");
    TEST_ASSERT(!resource_matches_pattern("file.txt", "*.json"),
                "Extension mismatch: *.json doesn't match file.txt");
    
    /* Exact match */
    TEST_ASSERT(resource_matches_pattern("/etc/config", "/etc/config"),
                "Exact match: paths match exactly");
    
    /* NULL and wildcard patterns */
    TEST_ASSERT(resource_matches_pattern("/any/path", NULL),
                "NULL pattern matches anything");
    TEST_ASSERT(resource_matches_pattern("/any/path", "*"),
                "Single * matches anything");
    
    /* Question mark wildcard */
    TEST_ASSERT(resource_matches_pattern("test", "t?st"),
                "Question mark: t?st matches test");
    TEST_ASSERT(!resource_matches_pattern("toast", "t?st"),
                "Question mark: t?st doesn't match toast");
}

/* ===================================================================
 * CapabilitySet Tests
 * =================================================================== */

void test_capability_set(void) {
    printf("\n=== CapabilitySet Tests ===\n");
    
    CapabilitySet *set = capability_set_new();
    TEST_ASSERT(set != NULL, "capability_set_new() creates set");
    TEST_ASSERT(set->count == 0, "New set has count 0");
    TEST_ASSERT(set->capacity > 0, "New set has positive capacity");
    
    /* Add capabilities */
    capability_set_add(set, CAP_FILE_READ, "/tmp/*");
    TEST_ASSERT(set->count == 1, "Adding capability increments count");
    
    capability_set_add(set, CAP_NETWORK_CONNECT, "api.example.com");
    TEST_ASSERT(set->count == 2, "Adding second capability works");
    
    /* Find capabilities */
    Capability *cap = capability_set_find(set, CAP_FILE_READ, "/tmp/test.txt");
    TEST_ASSERT(cap != NULL, "Finding matching capability succeeds");
    TEST_ASSERT(cap->type == CAP_FILE_READ, "Found capability has correct type");
    
    cap = capability_set_find(set, CAP_FILE_WRITE, "/tmp/test.txt");
    TEST_ASSERT(cap == NULL, "Finding non-existent capability returns NULL");
    
    /* Remove capability */
    capability_set_remove(set, 0);
    TEST_ASSERT(set->count == 1, "Removing capability decrements count");
    
    capability_set_free(set);
    printf("  ✓ capability_set_free() completes\n");
    tests_passed++;
}

/* ===================================================================
 * CapabilityContext Tests
 * =================================================================== */

void test_capability_context(void) {
    printf("\n=== CapabilityContext Tests ===\n");
    
    CapabilityContext *ctx = capability_context_new();
    TEST_ASSERT(ctx != NULL, "capability_context_new() creates context");
    TEST_ASSERT(ctx->granted != NULL, "Context has granted set");
    TEST_ASSERT(ctx->requested != NULL, "Context has requested set");
    
    /* Grant capabilities */
    capability_grant(ctx, CAP_FILE_READ, "/tmp/*");
    TEST_ASSERT(ctx->granted->count == 1, "Granting capability adds to set");
    
    /* Check capabilities */
    int has = capability_has(ctx, CAP_FILE_READ, "/tmp/test.txt");
    TEST_ASSERT(has == 1, "capability_has() finds granted capability");
    
    has = capability_has(ctx, CAP_FILE_WRITE, "/tmp/test.txt");
    TEST_ASSERT(has == 0, "capability_has() returns false for missing capability");
    
    /* CAP_ALL grants everything */
    capability_grant(ctx, CAP_ALL, NULL);
    has = capability_has(ctx, CAP_NETWORK_CONNECT, "anything");
    TEST_ASSERT(has == 1, "CAP_ALL grants any capability");
    
    capability_context_free(ctx);
    printf("  ✓ capability_context_free() completes\n");
    tests_passed++;
}

/* ===================================================================
 * Capability Type Utilities Tests
 * =================================================================== */

void test_capability_utilities(void) {
    printf("\n=== Capability Utilities Tests ===\n");
    
    /* Type names */
    const char *name = capability_type_name(CAP_FILE_READ);
    TEST_ASSERT(strcmp(name, "file-read") == 0, 
                "capability_type_name(CAP_FILE_READ) returns 'file-read'");
    
    /* Flag names */
    const char *flag = capability_flag_name(CAP_FILE_READ);
    TEST_ASSERT(strcmp(flag, "--allow-read") == 0,
                "capability_flag_name(CAP_FILE_READ) returns '--allow-read'");
    
    /* Parse from name */
    CapabilityType type = capability_type_from_name("file-read");
    TEST_ASSERT(type == CAP_FILE_READ,
                "capability_type_from_name('file-read') returns CAP_FILE_READ");
    
    type = capability_type_from_name("net");
    TEST_ASSERT(type == CAP_NETWORK_CONNECT,
                "capability_type_from_name('net') returns CAP_NETWORK_CONNECT");
    
    type = capability_type_from_name("unknown");
    TEST_ASSERT(type == CAP_NONE,
                "capability_type_from_name('unknown') returns CAP_NONE");
}

/* ===================================================================
 * CapabilityManager Tests
 * =================================================================== */

void test_capability_manager(void) {
    printf("\n=== CapabilityManager Tests ===\n");
    
    capability_manager_init();
    CapabilityManager *mgr = capability_manager_get();
    TEST_ASSERT(mgr != NULL, "capability_manager_get() returns manager");
    TEST_ASSERT(mgr->context_count == 0, "New manager has no contexts");
    
    /* Register context */
    CapabilityContext *ctx = capability_context_new();
    capability_manager_register(ctx);
    TEST_ASSERT(mgr->context_count == 1, "Registering context increments count");
    
    /* Unregister context */
    capability_manager_unregister(ctx);
    TEST_ASSERT(mgr->context_count == 0, "Unregistering context decrements count");
    
    capability_context_free(ctx);
    capability_manager_shutdown();
    printf("  ✓ capability_manager_shutdown() completes\n");
    tests_passed++;
}

/* ===================================================================
 * UnsafeContext Tests
 * =================================================================== */

void test_unsafe_context(void) {
    printf("\n=== UnsafeContext Tests ===\n");
    
    UnsafeContext *ctx = unsafe_context_new();
    TEST_ASSERT(ctx != NULL, "unsafe_context_new() creates context");
    TEST_ASSERT(ctx->in_unsafe_block == 0, "New context is not in unsafe block");
    TEST_ASSERT(ctx->unsafe_depth == 0, "New context has depth 0");
    
    /* Enter unsafe block */
    unsafe_context_enter(ctx, 42, "test.kl");
    TEST_ASSERT(ctx->in_unsafe_block == 1, "Entering sets in_unsafe_block");
    TEST_ASSERT(ctx->unsafe_depth == 1, "Entering sets depth to 1");
    TEST_ASSERT(ctx->line_number == 42, "Entering records line number");
    
    /* Nested unsafe blocks */
    unsafe_context_enter(ctx, 43, "test.kl");
    TEST_ASSERT(ctx->unsafe_depth == 2, "Nested block increases depth");
    
    /* Exit unsafe blocks */
    unsafe_context_exit(ctx);
    TEST_ASSERT(ctx->unsafe_depth == 1, "Exiting decreases depth");
    TEST_ASSERT(ctx->in_unsafe_block == 1, "Still in unsafe after one exit");
    
    unsafe_context_exit(ctx);
    TEST_ASSERT(ctx->unsafe_depth == 0, "Exiting all blocks sets depth to 0");
    TEST_ASSERT(ctx->in_unsafe_block == 0, "No longer in unsafe after all exits");
    
    unsafe_context_free(ctx);
    printf("  ✓ unsafe_context_free() completes\n");
    tests_passed++;
}

/* ===================================================================
 * SecurityContext Tests
 * =================================================================== */

void test_security_context(void) {
    printf("\n=== SecurityContext Tests ===\n");
    
    SecurityContext *ctx = security_context_new();
    TEST_ASSERT(ctx != NULL, "security_context_new() creates context");
    TEST_ASSERT(ctx->unsafe_ctx != NULL, "Context has unsafe context");
    TEST_ASSERT(ctx->capability_ctx != NULL, "Context has capability context");
    TEST_ASSERT(ctx->security_level == SECURITY_LEVEL_PERMISSIVE,
                "Default security level is PERMISSIVE");
    
    /* Set security levels */
    security_context_set_level(ctx, SECURITY_LEVEL_STRICT);
    TEST_ASSERT(ctx->security_level == SECURITY_LEVEL_STRICT,
                "Setting level to STRICT works");
    TEST_ASSERT(ctx->strict_mode == 1, "strict_mode flag is set");
    
    security_context_enable_warn_mode(ctx);
    TEST_ASSERT(ctx->security_level == SECURITY_LEVEL_WARN,
                "Enabling warn mode works");
    TEST_ASSERT(ctx->warn_only == 1, "warn_only flag is set");
    
    /* Statistics */
    unsigned long unsafe_ops, cap_checks, cap_denials;
    security_context_get_stats(ctx, &unsafe_ops, &cap_checks, &cap_denials);
    TEST_ASSERT(unsafe_ops == 0, "Initial unsafe ops count is 0");
    TEST_ASSERT(cap_checks == 0, "Initial cap checks count is 0");
    TEST_ASSERT(cap_denials == 0, "Initial cap denials count is 0");
    
    security_context_free(ctx);
    printf("  ✓ security_context_free() completes\n");
    tests_passed++;
}

/* ===================================================================
 * Security Utilities Tests
 * =================================================================== */

void test_security_utilities(void) {
    printf("\n=== Security Utilities Tests ===\n");
    
    /* Security level names */
    const char *name = security_level_name(SECURITY_LEVEL_STRICT);
    TEST_ASSERT(strcmp(name, "strict") == 0,
                "security_level_name(STRICT) returns 'strict'");
    
    /* Parse security level */
    SecurityLevel level = security_level_from_name("strict");
    TEST_ASSERT(level == SECURITY_LEVEL_STRICT,
                "security_level_from_name('strict') returns STRICT");
    
    /* Unsafe operation type names */
    name = unsafe_operation_type_name(UNSAFE_OP_FILE_IO);
    TEST_ASSERT(strcmp(name, "file-io") == 0,
                "unsafe_operation_type_name(FILE_IO) returns 'file-io'");
    
    /* Check if operation is unsafe */
    int op_type = is_unsafe_operation("readFile");
    TEST_ASSERT(op_type == UNSAFE_OP_FILE_IO,
                "is_unsafe_operation('readFile') returns FILE_IO");
    
    op_type = is_unsafe_operation("malloc");
    TEST_ASSERT(op_type == UNSAFE_OP_MEMORY,
                "is_unsafe_operation('malloc') returns MEMORY");
    
    op_type = is_unsafe_operation("safe_function");
    TEST_ASSERT(op_type == -1,
                "is_unsafe_operation('safe_function') returns -1");
}

/* ===================================================================
 * Legacy SafetyContext Tests
 * =================================================================== */

void test_legacy_safety_context(void) {
    printf("\n=== Legacy SafetyContext Tests ===\n");
    
    SafetyContext *ctx = safety_context_new();
    TEST_ASSERT(ctx != NULL, "safety_context_new() creates context");
    TEST_ASSERT(ctx->in_unsafe_context == 0, "New context is safe");
    
    safety_context_enter_unsafe(ctx);
    TEST_ASSERT(ctx->in_unsafe_context == 1, "Entering unsafe works");
    TEST_ASSERT(safety_context_is_unsafe(ctx) == 1, "safety_context_is_unsafe() returns true");
    
    safety_context_exit_unsafe(ctx);
    TEST_ASSERT(ctx->in_unsafe_context == 0, "Exiting unsafe works");
    
    safety_context_free(ctx);
    printf("  ✓ safety_context_free() completes\n");
    tests_passed++;
}

/* ===================================================================
 * Main Test Runner
 * =================================================================== */

int main(void) {
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  KLang Security Implementation Test Suite                     ║\n");
    printf("║  Testing: capability.c and security.c                         ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    test_pattern_matching();
    test_capability_set();
    test_capability_context();
    test_capability_utilities();
    test_capability_manager();
    test_unsafe_context();
    test_security_context();
    test_security_utilities();
    test_legacy_safety_context();
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Test Results                                                  ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║  Passed: %-4d                                                  ║\n", tests_passed);
    printf("║  Failed: %-4d                                                  ║\n", tests_failed);
    printf("║  Total:  %-4d                                                  ║\n", tests_passed + tests_failed);
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    
    if (tests_failed == 0) {
        printf("║  ✓ ALL TESTS PASSED!                                          ║\n");
        printf("╚════════════════════════════════════════════════════════════════╝\n");
        return 0;
    } else {
        printf("║  ✗ SOME TESTS FAILED                                           ║\n");
        printf("╚════════════════════════════════════════════════════════════════╝\n");
        return 1;
    }
}
