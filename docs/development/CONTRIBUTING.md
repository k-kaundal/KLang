# Contributing to KLang

Thank you for your interest in contributing to KLang! This document provides guidelines and instructions for contributing.

---

## Table of Contents
- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Pull Request Process](#pull-request-process)
- [Issue Reporting](#issue-reporting)

---

## Code of Conduct

### Our Pledge
We pledge to make participation in KLang a harassment-free experience for everyone, regardless of age, body size, disability, ethnicity, gender identity, experience level, nationality, personal appearance, race, religion, or sexual identity and orientation.

### Our Standards
- Use welcoming and inclusive language
- Be respectful of differing viewpoints and experiences
- Gracefully accept constructive criticism
- Focus on what is best for the community
- Show empathy towards other community members

---

## Getting Started

### Prerequisites
- Basic knowledge of C programming
- Familiarity with compilers and interpreters
- Understanding of programming language design (helpful but not required)
- Git and GitHub knowledge

### Areas to Contribute
1. **Core Language Features**
   - Lexer improvements
   - Parser enhancements
   - Interpreter optimization
   - VM development

2. **Standard Library**
   - Built-in functions
   - Data structures
   - Utility functions

3. **Documentation**
   - Tutorials
   - Examples
   - API documentation
   - Language specification

4. **Testing**
   - Unit tests
   - Integration tests
   - Performance tests

5. **Tools**
   - IDE integrations
   - Build tools
   - Package manager

---

## Development Setup

### 1. Fork and Clone
```bash
# Fork the repository on GitHub
# Then clone your fork
git clone https://github.com/YOUR_USERNAME/KLang.git
cd KLang

# Add upstream remote
git remote add upstream https://github.com/k-kaundal/KLang.git
```

### 2. Build
```bash
make clean && make
```

### 3. Test
```bash
make test
```

### 4. Create a Branch
```bash
git checkout -b feature/my-new-feature
# or
git checkout -b fix/bug-description
```

---

## How to Contribute

### Finding Issues to Work On

1. **Good First Issues**: Look for issues labeled `good-first-issue`
2. **Help Wanted**: Issues labeled `help-wanted`
3. **Documentation**: Issues labeled `documentation`
4. **Bugs**: Issues labeled `bug`

### Types of Contributions

#### 1. Bug Fixes
```bash
# Create a branch
git checkout -b fix/issue-123-description

# Make your changes
# ...

# Test thoroughly
make test
./klang run examples/*.kl

# Commit
git commit -m "fix: description of fix

Closes #123"
```

#### 2. New Features
```bash
# Discuss the feature first (create an issue)
# Get approval from maintainers

# Create a branch
git checkout -b feature/my-feature

# Implement
# ...

# Add tests
# Add documentation
# Commit
```

#### 3. Documentation
```bash
git checkout -b docs/improve-readme

# Update documentation
# Proofread

git commit -m "docs: improve README with examples"
```

#### 4. Tests
```bash
git checkout -b test/add-parser-tests

# Add tests
# Ensure they pass

git commit -m "test: add comprehensive parser tests"
```

---

## Coding Standards

### C Code Style

#### General Rules
- Use C99 standard
- Indent with 4 spaces (no tabs)
- Maximum line length: 100 characters
- Use meaningful variable names

#### Naming Conventions
```c
// Functions: snake_case
void parse_expression(Parser *parser);
Value eval_node(Interpreter *interp, ASTNode *node);

// Types: PascalCase
typedef struct Token Token;
typedef struct ASTNode ASTNode;

// Constants: UPPER_SNAKE_CASE
#define MAX_TOKENS 1000
#define DEFAULT_BUFFER_SIZE 256

// Variables: snake_case
int line_number = 0;
char *token_value = NULL;
```

#### Function Documentation
```c
/**
 * Parse a primary expression
 * 
 * @param parser - Parser instance
 * @return ASTNode* - Parsed expression node
 */
static ASTNode *parse_primary(Parser *parser) {
    // Implementation
}
```

#### Error Handling
```c
// Always check for NULL
if (!node) {
    fprintf(stderr, "Error: null node\n");
    return NULL;
}

// Provide meaningful error messages
fprintf(stderr, "Parse error at line %d: expected %s, got %s\n",
        parser->current.line,
        token_type_name(expected),
        token_type_name(parser->current.type));
```

#### Memory Management
```c
// Always free allocated memory
char *str = strdup("text");
// Use str
free(str);
str = NULL;

// Use helper functions
void ast_free(ASTNode *node) {
    if (!node) return;
    // Free all fields
    free(node);
}
```

### KLang Code Style

#### For Examples and Tests
```klang
# Use clear variable names
let user_count = 10
let total_price = 199.99

# Functions: snake_case or camelCase (consistent)
fn calculate_total() { }
fn getUserName() { }

# Classes: PascalCase
class UserAccount { }
class PaymentProcessor { }

# Add comments for clarity
# Calculate the factorial of n
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1
    }
    return n * factorial(n - 1)
}
```

---

## Testing Guidelines

### Writing Tests

#### Test File Structure
```c
// tests/test_myfeature.c
#include "test_runner.h"
#include "../src/myfeature.h"

void test_feature_basic() {
    // Arrange
    MyFeature *feature = create_feature();
    
    // Act
    int result = feature_do_something(feature);
    
    // Assert
    assert(result == expected_value);
    
    // Cleanup
    feature_free(feature);
}

void test_feature_edge_case() {
    // Test edge cases
}

void run_myfeature_tests() {
    test_feature_basic();
    test_feature_edge_case();
    printf("My feature tests done.\n");
}
```

#### Test Coverage
- Test normal cases
- Test edge cases
- Test error conditions
- Test null/invalid input

#### Running Tests
```bash
# Run all tests
make test

# Add your tests to test_runner.c
```

---

## Pull Request Process

### Before Submitting

1. **Update Your Branch**
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Run Tests**
   ```bash
   make test
   ```

3. **Check Code Style**
   ```bash
   # Ensure code follows style guide
   ```

4. **Update Documentation**
   ```bash
   # Update relevant docs
   ```

### PR Checklist

- [ ] Code builds successfully
- [ ] All tests pass
- [ ] New tests added for new features
- [ ] Documentation updated
- [ ] Commit messages are clear
- [ ] Branch is up to date with main
- [ ] No merge conflicts

### Commit Message Format

```
type: brief description

Longer description if needed.
Explain the what and why, not the how.

Closes #123
```

**Types:**
- `feat:` - New feature
- `fix:` - Bug fix
- `docs:` - Documentation changes
- `test:` - Adding tests
- `refactor:` - Code refactoring
- `perf:` - Performance improvement
- `style:` - Code style changes
- `chore:` - Build process, tools, etc.

**Examples:**
```
feat: add support for abstract classes

Implements abstract class syntax and validation.
Abstract classes cannot be instantiated.

Closes #45

---

fix: resolve segfault in parser

The parser was not checking for null before
dereferencing. Added null check.

Fixes #78

---

docs: add examples for inheritance

Added three comprehensive examples showing
class inheritance patterns.
```

### Creating the PR

1. **Push to Your Fork**
   ```bash
   git push origin feature/my-feature
   ```

2. **Open PR on GitHub**
   - Go to the main repository
   - Click "New Pull Request"
   - Select your branch
   - Fill in the template

3. **PR Title and Description**
   ```markdown
   ## Description
   Brief description of changes
   
   ## Type of Change
   - [ ] Bug fix
   - [x] New feature
   - [ ] Documentation update
   
   ## Testing
   Describe testing done
   
   ## Checklist
   - [x] Tests pass
   - [x] Documentation updated
   - [x] Code follows style guide
   ```

### Review Process

1. **Automated Checks**
   - CI/CD builds
   - Tests run
   - Code analysis

2. **Code Review**
   - Maintainers review
   - May request changes
   - Discussion in comments

3. **Updates**
   ```bash
   # Make requested changes
   git add .
   git commit -m "address review comments"
   git push origin feature/my-feature
   ```

4. **Merge**
   - Once approved, maintainers merge
   - Your contribution is live!

---

## Issue Reporting

### Bug Reports

Use this template:
```markdown
## Bug Description
Clear description of the bug

## Steps to Reproduce
1. Step 1
2. Step 2
3. Step 3

## Expected Behavior
What should happen

## Actual Behavior
What actually happens

## Environment
- OS: Ubuntu 22.04
- GCC Version: 11.2
- KLang Version: 0.1.0

## Additional Context
Any other relevant information
```

### Feature Requests

Use this template:
```markdown
## Feature Description
What feature would you like?

## Use Case
Why is this feature needed?

## Proposed Solution
How should it work?

## Alternatives
Other approaches considered

## Additional Context
Any other relevant information
```

---

## Development Tips

### Debugging

#### Using GDB
```bash
make clean && make DEBUG=1
gdb ./klang
(gdb) break parser.c:123
(gdb) run run test.kl
(gdb) print *node
```

#### Using Print Debugging
```c
fprintf(stderr, "DEBUG: value=%d, line=%d\n", value, line);
```

### Performance

#### Profiling
```bash
# Using gprof
make CFLAGS="-pg"
./klang run test.kl
gprof ./klang gmon.out > analysis.txt
```

#### Benchmarking
```bash
time ./klang run benchmark.kl
```

### Documentation

#### Writing Docs
- Use Markdown
- Include code examples
- Keep explanations clear and concise
- Update table of contents

#### API Documentation
```c
/**
 * Brief description
 *
 * Detailed description
 *
 * @param param1 Description of param1
 * @param param2 Description of param2
 * @return Description of return value
 *
 * @example
 * Value result = my_function(arg1, arg2);
 */
```

---

## Community

### Communication Channels

- **GitHub Issues**: Bug reports, feature requests
- **GitHub Discussions**: Questions, ideas, general discussion
- **Pull Requests**: Code contributions

### Getting Help

- Read the [documentation](docs/)
- Check [existing issues](https://github.com/k-kaundal/KLang/issues)
- Ask in [GitHub Discussions](https://github.com/k-kaundal/KLang/discussions)

---

## Recognition

Contributors are recognized in:
- CONTRIBUTORS.md file
- Release notes
- GitHub contributors page

---

## License

By contributing to KLang, you agree that your contributions will be licensed under the MIT License.

---

## Questions?

If you have questions about contributing, please:
1. Check the [documentation](docs/)
2. Search [existing issues](https://github.com/k-kaundal/KLang/issues)
3. Ask in [GitHub Discussions](https://github.com/k-kaundal/KLang/discussions)
4. Email the maintainers

---

**Thank you for contributing to KLang!** 🚀
