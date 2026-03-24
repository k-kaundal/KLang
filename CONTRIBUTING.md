# Contributing to KLang 🤝

Thank you for your interest in contributing to KLang! We welcome contributions from the community.

## Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/YOUR_USERNAME/KLang.git
   cd KLang
   ```
3. **Create a branch** for your changes:
   ```bash
   git checkout -b feature/your-feature-name
   ```

## Development Setup

### Prerequisites
- GCC or Clang compiler
- LLVM 16+ (for JIT compilation)
- libreadline-dev
- Make

### Build
```bash
make
```

### Run Tests
```bash
make test                    # C unit tests
./tests/run_all_tests.sh     # Feature tests
```

## Contribution Guidelines

### Code Style
- Use 4 spaces for indentation (C code)
- Follow existing code patterns
- Add comments for complex logic
- Keep functions focused and small

### Commit Messages
- Use clear, descriptive commit messages
- Start with a verb (Add, Fix, Update, Remove, etc.)
- Reference issues when applicable: `Fixes #123`

Example:
```
Add support for async generators

- Implement async generator syntax
- Add tests for async iteration
- Update documentation

Fixes #123
```

### Pull Request Process

1. **Update tests** - Add or update tests for your changes
2. **Run all tests** - Ensure nothing breaks
3. **Update documentation** - Keep docs in sync with code
4. **Create PR** - Use a clear title and description
5. **Address feedback** - Respond to review comments

### What to Contribute

We welcome:
- 🐛 Bug fixes
- ✨ New features
- 📝 Documentation improvements
- 🎨 Code refactoring
- 🧪 Additional tests
- 🌍 Translations
- 💡 Ideas and suggestions (open an issue first!)

### Areas Needing Help

- Standard library expansion
- Performance optimizations
- More AI capabilities
- IDE integrations
- Package manager
- More examples

## Code Review Process

1. All submissions require review
2. Maintainers will review your PR
3. Address any requested changes
4. Once approved, your PR will be merged

## Testing

### Running Tests
```bash
# C unit tests
make test

# Feature tests
./tests/run_all_tests.sh

# Specific test
./klang run tests/features/test_name.k
```

### Writing Tests
- Add test files to `tests/features/`
- Follow existing test patterns
- Test both success and error cases
- Include edge cases

## Documentation

- User-facing docs go in `docs/`
- API changes need `docs/API_REFERENCE.md` updates
- Add examples to `examples/` when appropriate

## Questions?

- 💬 [Open a discussion](https://github.com/k-kaundal/KLang/discussions)
- 🐛 [Report a bug](https://github.com/k-kaundal/KLang/issues/new?template=bug_report.md)
- ✨ [Request a feature](https://github.com/k-kaundal/KLang/issues/new?template=feature_request.md)

## Code of Conduct

Please note that this project is released with a [Code of Conduct](CODE_OF_CONDUCT.md). By participating in this project you agree to abide by its terms.

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for making KLang better!** 🚀
