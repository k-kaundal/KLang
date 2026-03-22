#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

/* Run tests from a file or directory
 * If path is a directory, recursively finds all *_test.kl files
 * If path is a file, runs that single test file
 */
void run_tests(const char *path);

#endif /* TEST_RUNNER_H */
