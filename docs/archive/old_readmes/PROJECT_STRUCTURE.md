# KLang Project Structure

```
KLang/
│
├── README.md                    # Project overview and quick start
├── LICENSE                      # MIT License
├── CHANGELOG.md                 # Version history and changes
├── Makefile                     # Build system
├── .gitignore                   # Git ignore patterns
│
├── src/                         # Source code (C)
│   ├── lexer.c / .h            # Tokenization
│   ├── parser.c / .h           # AST generation
│   ├── ast.c / .h              # Abstract Syntax Tree
│   ├── interpreter.c / .h      # Execution engine
│   ├── runtime.c / .h          # Runtime functions & standard library
│   ├── vm.c / .h               # Bytecode virtual machine
│   ├── compiler.c / .h         # Bytecode compiler
│   ├── gc.c / .h               # Garbage collector
│   ├── repl.c                  # Interactive REPL
│   └── cli.c                   # Command-line interface
│
├── include/                     # Public headers
│   └── (header files)
│
├── stdlib/                      # Standard library (KLang)
│   ├── math.kl                 # Mathematical functions
│   ├── string.kl               # String utilities
│   └── io.kl                   # I/O operations
│
├── docs/                        # Documentation (organized)
│   ├── README.md               # Documentation index
│   ├── features/               # Feature documentation
│   │   ├── JavaScript features
│   │   ├── Python features
│   │   ├── OOP features
│   │   ├── Async/await
│   │   ├── Generators
│   │   └── Modules
│   ├── guides/                 # User guides
│   │   ├── GETTING_STARTED.md
│   │   ├── LANGUAGE_REFERENCE.md
│   │   ├── OOP_GUIDE.md
│   │   └── Quick references
│   ├── api/                    # API documentation
│   │   ├── MATH_MODULE_DOCS.md
│   │   ├── Array methods
│   │   └── String operations
│   ├── architecture/           # System architecture
│   ├── roadmaps/              # Project roadmaps
│   ├── implementation/         # Technical reports
│   └── development/            # Contributor guides
│
├── tests/                       # Test suite (organized)
│   ├── README.md               # Test documentation
│   ├── unit/                   # Unit tests (C)
│   │   ├── test_lexer.c
│   │   ├── test_parser.c
│   │   ├── test_interpreter.c
│   │   └── (feature tests)
│   ├── features/               # Feature tests (KLang)
│   │   ├── test_math.k
│   │   ├── test_range.k
│   │   └── (language features)
│   ├── integration/            # Integration tests
│   ├── examples/               # Example programs as tests
│   └── fixtures/               # Test data and fixtures
│
├── examples/                    # Example programs
│   ├── hello.kl                # Hello World
│   ├── algorithms.kl           # Algorithm examples
│   ├── oop_demo.kl            # OOP demonstrations
│   ├── async_demo.k           # Async/await examples
│   └── (more examples)
│
├── scripts/                     # Build and utility scripts
│   ├── build.sh                # Unix build script
│   ├── build.bat               # Windows build script
│   ├── install.sh              # Unix installer
│   ├── install-windows.ps1     # Windows installer
│   ├── setup.sh                # Development setup
│   └── quick-install.sh        # Quick install
│
├── build/                       # Build artifacts (gitignored)
│   ├── *.o                     # Object files
│   ├── klang                   # Compiled executable
│   └── (build outputs)
│
└── tools/                       # Development tools
    └── (future: debugger, profiler, etc.)
```

## Directory Purpose

### `/src` - Source Code
Core implementation in C:
- Lexer, Parser, Interpreter
- Runtime and Standard Library
- VM and Compiler (in progress)
- Garbage Collector (planned)

### `/include` - Public Headers
Header files for public API

### `/stdlib` - Standard Library
KLang standard library modules:
- Math, String, I/O
- Future: File system, OS operations, JSON, Regex

### `/docs` - Documentation
Complete documentation suite:
- **features/** - Feature-specific docs
- **guides/** - User guides and tutorials
- **api/** - API references
- **architecture/** - System design
- **roadmaps/** - Planning documents
- **implementation/** - Technical reports
- **development/** - Contributor guides

### `/tests` - Test Suite
Comprehensive testing:
- **unit/** - Component tests (C)
- **features/** - Language feature tests (KLang)
- **integration/** - End-to-end tests
- **examples/** - Example programs as tests
- **fixtures/** - Test data

### `/examples` - Example Programs
Working example programs demonstrating features

### `/scripts` - Build Scripts
Cross-platform build and installation scripts

### `/build` - Build Output
Compiled artifacts (not in version control)

### `/tools` - Development Tools
Future home for debugging, profiling, and analysis tools

## File Naming Conventions

### Source Files (C)
- `component.c` / `component.h` - Implementation and header
- Snake case: `my_module.c`
- Test files: `test_component.c`

### KLang Files
- `.k` or `.kl` extension
- Snake case: `my_program.kl`
- Test files: `test_feature.k`

### Documentation
- ALL_CAPS for major docs: `README.md`, `CONTRIBUTING.md`
- Title case for features: `Feature_Name.md`
- Descriptive names: `Getting_Started.md`

### Scripts
- `.sh` for Unix/Linux
- `.bat` for Windows CMD
- `.ps1` for PowerShell
- Descriptive names: `build.sh`, `install.sh`

## Build Output Locations

```
build/
├── src/                    # Object files
│   └── *.o
├── tests/                  # Test binaries
│   └── test_*
├── klang                   # Main executable
└── *.log                   # Build logs
```

## Git Workflow

### Tracked
- Source code (`src/`, `include/`)
- Standard library (`stdlib/`)
- Documentation (`docs/`)
- Tests (`tests/`)
- Examples (`examples/`)
- Build scripts (`scripts/`)
- Configuration files

### Ignored
- Build artifacts (`build/`, `*.o`)
- Executables (`klang`, `test_*`)
- Logs (`*.log`, `*.txt`)
- IDE files (`.vscode/`, `.idea/`)
- OS files (`.DS_Store`, `Thumbs.db`)

## Adding New Files

### New Feature
1. Implement in `src/feature.c`
2. Add tests in `tests/unit/test_feature.c`
3. Add examples in `examples/feature_demo.kl`
4. Document in `docs/features/Feature.md`

### New Documentation
1. Choose appropriate directory in `docs/`
2. Follow naming conventions
3. Update `docs/README.md` index

### New Test
1. Unit test → `tests/unit/`
2. Feature test → `tests/features/`
3. Integration test → `tests/integration/`
4. Update `tests/README.md`

## Navigation Tips

- **New users**: Start with `README.md` then `docs/GETTING_STARTED.md`
- **Developers**: See `docs/development/` and `docs/architecture/`
- **Contributors**: Read `docs/development/CONTRIBUTING.md`
- **Feature lookup**: Browse `docs/features/` or `docs/api/`
- **Examples**: Check `examples/` directory

## Maintenance

This structure follows industry best practices:
- Clear separation of concerns
- Easy navigation
- Scalable organization
- Professional appearance
- CI/CD friendly

Last updated: March 2026
