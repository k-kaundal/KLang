# Changelog

All notable changes to KLang will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added - AI-Native Core Language (v0.8.0) 🤖🎉

**KLang is now the world's first AI-Native Core Language!**

#### AI Standard Library (100% KLang Implementation)
- **ai.kl** - Core AI utilities (7,948 chars)
  - `AIConfig` class for AI service configuration
  - `Message` class for chat messages
  - `ChatSession` class for conversation management
  - `Embedding` class with vector operations
  - `BatchProcessor` for batch processing
  - `countTokens()` and `estimateCost()` utilities

- **llm.kl** - LLM integration (11,893 chars)
  - `LLMConfig` class for provider configuration
  - `LLMClient` class for API interactions
  - Support for OpenAI (GPT-4, GPT-3.5-turbo)
  - Support for Anthropic (Claude 3)
  - Support for Ollama (local models)
  - `llm()` function for quick queries
  - `llmStream()` for streaming (planned)

- **embedding.kl** - Vector operations (7,024 chars)
  - `EmbeddingStore` class for semantic search
  - `TextChunker` class for document chunking
  - Vector math: add, subtract, scale, average
  - `semanticSearch()` helper function

- **prompt.kl** - Prompt engineering (9,395 chars)
  - `PromptTemplate` - Variable substitution
  - `FewShotPrompt` - Few-shot learning
  - `ChainOfThoughtPrompt` - Step-by-step reasoning
  - `RAGPrompt` - Context + question format
  - `ReasoningPrompt` - Multi-step reasoning
  - `SystemPrompts` - Pre-built system prompts
  - Prompt optimization and token management

- **rag.kl** - RAG systems (9,694 chars)
  - `Document` class with embeddings
  - `VectorDB` - In-memory vector database
  - `RAGSystem` - Complete RAG pipeline
  - `SemanticCache` - Query result caching
  - `buildRAGFromFiles()` helper

- **agents.kl** - Autonomous agents (12,258 chars)
  - `AgentMemory` - Short/long-term memory + facts
  - `AgentAction` - Action definitions
  - `Agent` - Base autonomous agent
  - `ReActAgent` - Reasoning + Acting pattern
  - `TaskAgent` - Task queue execution

**Total:** 58,212 characters of pure KLang AI code

#### Core Infrastructure (C Bindings)
- **Environment Variables** - `env.get()`, `env.set()`, `env.has()`
  - Secure API key loading
  - Configuration management
  - Cross-platform support (Linux/macOS/Windows)

- **HTTP Client** - `http.get()`, `http.post()`, `http.request()`
  - REST API calls
  - Custom headers support
  - Request body for POST
  - Dictionary-based configuration
  - Built on curl for reliability

- **JSON Streaming** - `__json_parse_stream()` (API placeholder)
  - Token-by-token streaming support
  - Callback-based processing
  - (Full implementation planned)

**Total:** 390 lines of C bindings (~2,847 characters)

#### Example Programs
- **ai_chatbot_simple.kl** (2,249 chars) - Basic chatbot with API calls
- **ai_rag_demo.kl** (4,865 chars) - RAG and semantic search
- **ai_agent_demo.kl** (4,469 chars) - Autonomous agents
- **ai_prompt_engineering.kl** (4,615 chars) - Prompt patterns

**Total:** 16,198 characters of example code

#### Documentation
- **AI_NATIVE_GUIDE.md** (18,658 chars) - Complete user guide
- **AI_NATIVE_IMPLEMENTATION.md** (16,185 chars) - Technical documentation
- **AI_QUICK_REFERENCE.md** (7,135 chars) - Quick reference
- **AI_NATIVE_ANNOUNCEMENT.md** (7,236 chars) - Feature announcement
- Updated **README.md** with AI-native highlights
- Updated **stdlib/README.md** with AI module docs

**Total:** 49,214 characters of documentation

### Added - CLI Enhancements (v0.2.0) 🎉

#### Professional CLI Interface
- **Color Support** - Beautiful ANSI color output throughout the CLI
  - Cyan for headers and info
  - Green for success and commands
  - Red for errors
  - Yellow for warnings and tips
  - `--no-color` flag to disable colors
- **Comprehensive Help System**
  - `klang --help` or `klang -h` - Full help with examples
  - `klang help <command>` - Command-specific help
  - Beautiful ASCII art headers and formatting
  - Usage examples for every command
- **Version Information**
  - `klang --version` or `klang -v` - Show version, build date, and description

#### New Commands
- **Syntax Checker** - `klang check <file>`
  - Validates syntax without executing code
  - Reports errors with colored output
  - Shows statement count on success
  - Useful for CI/CD pipelines
- **File Information** - `klang info <file>`
  - Displays comprehensive file statistics
  - Line counts (total, code, comments, empty)
  - File size and path information
  - Comment ratio percentage
  - Extension validation

#### Enhanced REPL
- Professional welcome banner with ASCII art
- Colored prompt (`>`) in green
- Result indicator (`=>`) in cyan
- Helpful tips on startup
- Goodbye message on exit
- Improved user experience

#### Better Error Messages
- Colored error labels (red "Error:")
- Clear, descriptive error messages
- Helpful suggestions for next steps
- Examples: "Try 'klang --help' for available commands"
- Invalid extension errors with file name
- Missing argument errors with usage hints

#### Implementation Details
- New module: `cli_colors.c/h` - Color support system
- New module: `cli_help.c/h` - Help system and documentation
- New module: `cli_commands.c/h` - Check and info commands
- Enhanced: `cli.c` - Main CLI with new command handling
- Enhanced: `repl.c` - REPL with colors and improved UX
- Updated: `Makefile` - Include new source files

### Added - Previous Features
- Single quote support for strings - both `'...'` and `"..."` are now supported
- Enhanced escape sequence support including `\r` (carriage return)
- Comprehensive string manipulation functions (Dart-like API):
  - `uppercase(str)` - Convert string to uppercase
  - `lowercase(str)` - Convert string to lowercase  
  - `trim(str)` - Remove leading/trailing whitespace
  - `split(str, delimiter)` - Split string into list
  - `join(list, delimiter)` - Join list elements into string
  - `substring(str, start, end)` - Extract substring
  - `startswith(str, prefix)` - Check if string starts with prefix
  - `endswith(str, suffix)` - Check if string ends with suffix
  - `contains(str, substring)` - Check if string contains substring
  - `indexof(str, substring)` - Find index of substring
  - `replace(str, old, new)` - Replace all occurrences
  - `repeat(str, count)` - Repeat string n times
- Enhanced comment support:
  - `//` single-line comments
  - `///` documentation comments
  - `/* ... */` multi-line comments
  - `#` single-line comments (existing)
- REPL improvements:
  - `exit()` function for exiting REPL (in addition to `exit` keyword)
  - `quit()` function as alias for `exit()`
  - Optional exit code support: `exit(0)`, `exit(1)`, etc.

### Fixed
- Fixed double-free memory issue when passing lists to functions
- Added deep copy for list variables to prevent memory corruption
- Improved memory management for string operations in functions

### Changed
- Updated REPL welcome message to mention both `exit` and `exit()` commands
- Enhanced lexer to support multiple comment styles

## [0.1.0] - 2024-XX-XX

### Added
- Initial release of KLang
- Basic interpreter with REPL
- Core language features:
  - Variables (`let`, `const`)
  - Data types (int, float, string, bool, null)
  - Operators (arithmetic, comparison, logical)
  - Control flow (if/else, while, for)
  - Functions with parameters and return values
  - Lists and list operations
- Object-Oriented Programming:
  - Class definitions
  - Object instantiation with `new`
  - Methods and fields
  - Inheritance
  - `this` keyword for member access
- Built-in functions:
  - `print()`, `println()` - Output functions
  - `input()` - Input function
  - `len()` - Get length of strings and lists
  - `str()`, `int()`, `float()` - Type conversion
  - `type()` - Get type of value
- Command-line interface:
  - `klang repl` - Interactive mode
  - `klang run <file>` - Execute scripts
- Cross-platform support (Linux, macOS, Windows)
- Build system with Makefile
- Comprehensive test suite
- Documentation and examples

[Unreleased]: https://github.com/k-kaundal/KLang/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/k-kaundal/KLang/releases/tag/v0.1.0
