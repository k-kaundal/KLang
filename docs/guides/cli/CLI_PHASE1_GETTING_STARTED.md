# KLang CLI Phase 1 - Getting Started Guide

## 🎯 Goal
Implement the core CLI infrastructure (help, version, colors, argument parsing) in **5 days**.

---

## 📋 Prerequisites

### Skills Required
- ✅ C programming (pointers, structs, malloc/free)
- ✅ Command-line experience (bash, flags, arguments)
- ✅ Git basics (branch, commit, push)
- ✅ Make/Makefile knowledge

### Tools Needed
```bash
# Development tools
gcc or clang
make
git
valgrind (for memory leak checking)

# Optional but recommended
gdb (debugger)
clang-format (code formatting)
```

---

## 🚀 Quick Start (30 Minutes)

### Step 1: Set Up Development Environment (5 min)

```bash
# Clone the repository (if not already)
cd /home/runner/work/KLang/KLang

# Create feature branch
git checkout -b feature/cli-phase1-infrastructure

# Create directory structure
mkdir -p src/cli src/cli_commands src/repl tests/unit

# Verify current CLI works
make clean && make
./klang run examples/stdlib_demo.kl
```

### Step 2: Create Header Files (10 min)

```bash
# Create all header files first
touch src/cli/args.h
touch src/cli/colors.h
touch src/cli/commands.h
touch src/cli/help.h

# Create corresponding C files
touch src/cli/args.c
touch src/cli/colors.c
touch src/cli/commands.c
touch src/cli/help.c
```

### Step 3: Copy Starter Code (10 min)

Copy the complete implementations from `docs/CLI_IMPLEMENTATION_GUIDE.md`:
- Section 1.1 → `src/cli/args.c` and `args.h`
- Section 1.2 → `src/cli/colors.c` and `colors.h`
- Section 1.3 → `src/cli/commands.c` and `commands.h`
- Section 1.4 → `src/cli/help.c` and `help.h`

### Step 4: Update Makefile (5 min)

Add to your `Makefile`:

```makefile
# Add CLI sources
CLI_SOURCES = src/cli/args.c src/cli/colors.c src/cli/commands.c src/cli/help.c
CLI_OBJECTS = $(CLI_SOURCES:.c=.o)

# Update main target
klang: $(OBJECTS) $(CLI_OBJECTS) src/cli.o
	$(CC) $(CFLAGS) -o klang $(OBJECTS) $(CLI_OBJECTS) src/cli.o $(LDFLAGS)

# Update clean target
clean:
	rm -f src/*.o src/cli/*.o src/cli_commands/*.o klang test_runner
```

---

## 📅 Day-by-Day Implementation Plan

### Day 1: Argument Parser (8 hours)

**Morning (4 hours):**
1. ✅ Implement `args.h` interface
2. ✅ Implement `args_parse()` function
3. ✅ Implement `args_get_*()` functions
4. ✅ Test basic parsing

```c
// Test code (save as test_args.c)
#include "src/cli/args.h"
#include <assert.h>
#include <string.h>

int main() {
    // Test 1: Long flags
    char *argv1[] = {"prog", "--verbose", "--output", "file.txt"};
    ArgSpec specs1[] = {
        {"verbose", 'v', "Verbose", ARG_TYPE_NONE, NULL, false, NULL},
        {"output", 'o', "Output", ARG_TYPE_STRING, "<file>", false, NULL},
    };
    ParsedArgs *args1 = args_parse(4, argv1, specs1, 2);
    assert(args1 != NULL);
    assert(args_is_set(args1, "verbose"));
    assert(strcmp(args_get_string(args1, "output"), "file.txt") == 0);
    args_free(args1);
    
    // Test 2: Short flags
    char *argv2[] = {"prog", "-v", "-o", "output.txt"};
    ParsedArgs *args2 = args_parse(4, argv2, specs1, 2);
    assert(args2 != NULL);
    assert(args_is_set(args2, "verbose"));
    assert(strcmp(args_get_string(args2, "output"), "output.txt") == 0);
    args_free(args2);
    
    // Test 3: Combined flags
    char *argv3[] = {"prog", "-vo", "result.txt"};
    ParsedArgs *args3 = args_parse(3, argv3, specs1, 2);
    assert(args3 != NULL);
    assert(args_is_set(args3, "verbose"));
    assert(strcmp(args_get_string(args3, "output"), "result.txt") == 0);
    args_free(args3);
    
    printf("✓ All argument parser tests passed!\n");
    return 0;
}
```

```bash
# Compile and test
gcc -o test_args test_args.c src/cli/args.c
./test_args
```

**Afternoon (4 hours):**
1. ✅ Handle edge cases (missing values, unknown flags)
2. ✅ Add error messages
3. ✅ Test with valgrind (no memory leaks)
4. ✅ Write more comprehensive tests

```bash
# Check for memory leaks
valgrind --leak-check=full ./test_args
```

**End of Day 1 Checklist:**
- [ ] `args_parse()` handles all flag types
- [ ] Error messages are clear
- [ ] No memory leaks
- [ ] Tests pass

---

### Day 2: Color System (8 hours)

**Morning (4 hours):**
1. ✅ Implement `colors.h` interface
2. ✅ Implement `colors_init()` with TTY detection
3. ✅ Implement `color_printf()`
4. ✅ Test colors in terminal

```c
// Test code (save as test_colors.c)
#include "src/cli/colors.h"

int main() {
    // Test with colors enabled
    colors_init(false);
    
    print_error("This is an error message");
    print_warning("This is a warning message");
    print_success("This is a success message");
    print_info("This is an info message");
    
    color_printf(COLOR_BOLD, "Bold text\n");
    color_printf(COLOR_GREEN, "Green text\n");
    
    // Test with colors disabled
    colors_init(true);
    print_error("This should be plain text");
    
    printf("✓ Color tests passed!\n");
    return 0;
}
```

```bash
# Compile and test
gcc -o test_colors test_colors.c src/cli/colors.c
./test_colors
./test_colors | cat  # Test with pipe (should disable colors)
NO_COLOR=1 ./test_colors  # Test env var
```

**Afternoon (4 hours):**
1. ✅ Implement convenience functions (`print_error()`, etc.)
2. ✅ Test TTY detection (terminal vs pipe)
3. ✅ Test `NO_COLOR` environment variable
4. ✅ Document color codes

**End of Day 2 Checklist:**
- [ ] Colors work in terminal
- [ ] Colors disabled when piped
- [ ] `NO_COLOR` env var works
- [ ] All convenience functions work

---

### Day 3: Command System (8 hours)

**Morning (4 hours):**
1. ✅ Implement `commands.h` interface
2. ✅ Implement `command_register()`
3. ✅ Implement `command_find()`
4. ✅ Test registration and lookup

```c
// Test code (save as test_commands.c)
#include "src/cli/commands.h"
#include "src/cli/colors.h"
#include <assert.h>

int test_handler(int argc, char **argv) {
    printf("Test command executed!\n");
    return 0;
}

int main() {
    colors_init(false);
    
    // Register a test command
    Command test_cmd = {
        .name = "test",
        .description = "Test command",
        .handler = test_handler,
    };
    command_register(&test_cmd);
    
    // Find the command
    Command *found = command_find("test");
    assert(found != NULL);
    assert(strcmp(found->name, "test") == 0);
    
    // Test not found
    Command *not_found = command_find("nonexistent");
    assert(not_found == NULL);
    
    // Test suggestion
    const char *suggestion = command_suggest("tset");  // Typo: test
    assert(suggestion != NULL);
    printf("Suggestion for 'tset': %s\n", suggestion);
    
    printf("✓ All command system tests passed!\n");
    return 0;
}
```

```bash
# Compile and test
gcc -o test_commands test_commands.c src/cli/commands.c src/cli/colors.c
./test_commands
```

**Afternoon (4 hours):**
1. ✅ Implement Levenshtein distance for suggestions
2. ✅ Implement `command_suggest()`
3. ✅ Implement `command_execute()`
4. ✅ Test fuzzy matching

**End of Day 3 Checklist:**
- [ ] Commands can be registered
- [ ] Commands can be found
- [ ] Fuzzy matching works
- [ ] Suggestions are helpful

---

### Day 4: Help System & Integration (8 hours)

**Morning (4 hours):**
1. ✅ Implement `help.h` interface
2. ✅ Implement `help_print_general()`
3. ✅ Implement `help_print_version()`
4. ✅ Test help output

```c
// Test code (save as test_help.c)
#include "src/cli/help.h"
#include "src/cli/commands.h"
#include "src/cli/colors.h"

int dummy_handler(int argc, char **argv) { return 0; }

int main() {
    colors_init(false);
    
    // Register some dummy commands
    Command run_cmd = {"run", "Run a file", NULL, NULL, NULL, 0, dummy_handler};
    Command build_cmd = {"build", "Build a file", NULL, NULL, NULL, 0, dummy_handler};
    command_register(&run_cmd);
    command_register(&build_cmd);
    
    // Test general help
    printf("=== General Help ===\n");
    help_print_general("klang");
    
    printf("\n=== Version Info ===\n");
    help_print_version();
    
    printf("\n✓ Help system tests passed!\n");
    return 0;
}
```

```bash
# Compile and test
gcc -o test_help test_help.c src/cli/help.c src/cli/commands.c src/cli/colors.c
./test_help
```

**Afternoon (4 hours):**
1. ✅ Refactor `src/cli.c` to use new system
2. ✅ Add `--help` and `--version` flags
3. ✅ Register existing commands (run, build, repl)
4. ✅ Test integration

**End of Day 4 Checklist:**
- [ ] `klang --help` works
- [ ] `klang --version` works
- [ ] General help looks professional
- [ ] All existing commands still work

---

### Day 5: Polish & Testing (8 hours)

**Morning (4 hours):**
1. ✅ Add command-specific help
2. ✅ Improve error messages
3. ✅ Add examples to help text
4. ✅ Test all commands

```bash
# Integration tests
./klang --help
./klang --version
./klang help
./klang help run
./klang run --help
./klang build --help
./klang repl --help

# Test unknown command
./klang foobar  # Should show suggestion

# Test existing functionality
./klang run examples/stdlib_demo.kl
./klang repl
```

**Afternoon (4 hours):**
1. ✅ Run valgrind on all commands
2. ✅ Fix any memory leaks
3. ✅ Update documentation
4. ✅ Prepare PR

```bash
# Memory leak check
valgrind --leak-check=full ./klang --help
valgrind --leak-check=full ./klang --version
valgrind --leak-check=full ./klang run examples/stdlib_demo.kl

# Clean up and commit
git add .
git commit -m "Phase 1: Core CLI infrastructure

- Implemented argument parser
- Added color output system
- Created command registration system
- Added help and version flags
- Refactored CLI to use new infrastructure

Tests pass, no memory leaks."

git push origin feature/cli-phase1-infrastructure
```

**End of Day 5 Checklist:**
- [ ] All tests pass
- [ ] No memory leaks (valgrind clean)
- [ ] Documentation updated
- [ ] PR ready for review

---

## 🧪 Testing Checklist

### Unit Tests
- [ ] Argument parser handles all flag types
- [ ] Color system works with/without TTY
- [ ] Command registration and lookup work
- [ ] Help system formats output correctly

### Integration Tests
```bash
# Test help
./klang --help | grep "Usage:"
./klang -h | grep "Usage:"
./klang help | grep "Usage:"

# Test version
./klang --version | grep "KLang"
./klang -v | grep "KLang"

# Test unknown command
./klang foobar 2>&1 | grep "Unknown command"
./klang foobar 2>&1 | grep "Did you mean"

# Test existing commands still work
./klang run examples/stdlib_demo.kl
echo "print(1+1)" | ./klang repl

# Test colors
./klang --help  # Should be colored
./klang --help | cat  # Should be plain
NO_COLOR=1 ./klang --help  # Should be plain
```

### Memory Tests
```bash
# Check for leaks
valgrind --leak-check=full --show-leak-kinds=all ./klang --help
valgrind --leak-check=full --show-leak-kinds=all ./klang --version
valgrind --leak-check=full --show-leak-kinds=all ./klang run examples/stdlib_demo.kl
```

---

## 🐛 Common Issues & Solutions

### Issue 1: Compilation Errors
```
error: undefined reference to `args_parse'
```
**Solution:** Make sure all C files are included in Makefile:
```makefile
CLI_SOURCES = src/cli/args.c src/cli/colors.c src/cli/commands.c src/cli/help.c
```

### Issue 2: Colors Not Working
```
# Colors show as plain text
```
**Solution:** Check TTY detection:
```c
#include <unistd.h>
bool is_tty = isatty(fileno(stdout));
printf("TTY: %d\n", is_tty);  // Should be 1 in terminal, 0 when piped
```

### Issue 3: Memory Leaks
```
valgrind: definitely lost: 100 bytes
```
**Solution:** Ensure all `malloc()` has matching `free()`:
```c
// Check args_parse() frees all allocations
// Check command registration doesn't leak
// Use valgrind to find exact location
```

### Issue 4: Segmentation Fault
```
Segmentation fault (core dumped)
```
**Solution:** Use gdb to debug:
```bash
gdb ./klang
(gdb) run --help
(gdb) backtrace  # Show stack trace
```

---

## 📝 Code Review Checklist

Before submitting PR, check:

### Code Quality
- [ ] No compiler warnings (`-Wall -Wextra`)
- [ ] Consistent indentation (4 spaces)
- [ ] Functions are documented
- [ ] Error handling is thorough
- [ ] Memory is properly managed

### Functionality
- [ ] `--help` shows all commands
- [ ] `--version` shows correct info
- [ ] Unknown commands suggest alternatives
- [ ] Colors work correctly
- [ ] All existing commands still work

### Testing
- [ ] Unit tests pass
- [ ] Integration tests pass
- [ ] No memory leaks (valgrind)
- [ ] Works in different terminals
- [ ] Works when piped

### Documentation
- [ ] README updated with new flags
- [ ] Code is commented
- [ ] Commit message is descriptive

---

## 🎉 Success Criteria

### Must Have
✅ `klang --help` displays beautiful, colored help  
✅ `klang --version` shows version information  
✅ Unknown commands show suggestions  
✅ All existing commands still work  
✅ No memory leaks  
✅ All tests pass

### Nice to Have
✅ Help output is formatted nicely  
✅ Examples in help text  
✅ Consistent error messages  
✅ Fast startup time (<100ms)

---

## 📊 Progress Tracking

### Daily Progress
- [ ] Day 1: Argument parser complete
- [ ] Day 2: Color system complete
- [ ] Day 3: Command system complete
- [ ] Day 4: Help system & integration complete
- [ ] Day 5: Testing & polish complete

### Overall Phase 1 Progress
```
[████████░░] 80% - Day 4 complete, polish remaining
```

---

## 🔗 Quick Links

- **Implementation Code:** [CLI_IMPLEMENTATION_GUIDE.md](./CLI_IMPLEMENTATION_GUIDE.md)
- **Full Plan:** [CLI_ENHANCEMENT_PLAN.md](./CLI_ENHANCEMENT_PLAN.md)
- **Architecture:** [CLI_ARCHITECTURE_DIAGRAM.md](./CLI_ARCHITECTURE_DIAGRAM.md)
- **Roadmap:** [CLI_ROADMAP.md](./CLI_ROADMAP.md)

---

## 💬 Getting Help

### Stuck on Day 1-2?
- Review argument parsing examples in Implementation Guide
- Check test cases for expected behavior
- Use valgrind to find memory issues

### Stuck on Day 3-4?
- Review command system architecture
- Test each function independently
- Check that command registration works

### Stuck on Day 5?
- Run all tests systematically
- Fix issues one at a time
- Use gdb for debugging

---

## 🚢 Shipping

### Before Creating PR:
1. Run all tests: `make test` (create if needed)
2. Check memory: `valgrind ./klang --help`
3. Test manually: Try all commands
4. Update docs: README, CHANGELOG
5. Write good commit message

### PR Template:
```markdown
## Phase 1: Core CLI Infrastructure

### Changes
- Implemented argument parser (args.c)
- Added color output system (colors.c)
- Created command registration (commands.c)
- Added help system (help.c)
- Refactored main CLI to use new infrastructure

### Testing
- All unit tests pass
- Integration tests pass
- No memory leaks (valgrind clean)
- Tested on [terminal type]

### Documentation
- Updated README with new flags
- Added code comments
- Created test files

### Checklist
- [x] Code compiles without warnings
- [x] Tests pass
- [x] No memory leaks
- [x] Documentation updated
- [x] Ready for review
```

---

## 🎯 Next Steps After Phase 1

Once Phase 1 is complete and merged:
1. Take a break! You did it! 🎉
2. Review Phase 2 plan (check, fmt, info commands)
3. Create Phase 2 branch
4. Start implementing new commands

**Good luck! You've got this! 💪**

---

**Created:** 2024-03-22  
**Version:** 1.0  
**Status:** ✅ Ready to Use
