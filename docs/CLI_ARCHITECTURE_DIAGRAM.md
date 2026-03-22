# KLang CLI Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────────────┐
│                          KLang CLI Architecture                          │
│                              (Enhanced)                                  │
└─────────────────────────────────────────────────────────────────────────┘

                            User Input
                                │
                                ▼
┌───────────────────────────────────────────────────────────────────────┐
│                         main() Entry Point                            │
│                         (src/cli.c)                                   │
│                                                                       │
│  • Parse global flags (--help, --version, --no-color, etc.)         │
│  • Initialize color system                                           │
│  • Load configuration (klang.toml, env vars)                        │
│  • Dispatch to command handler                                       │
└───────────────────────────────────────────────────────────────────────┘
                                │
                    ┌───────────┴───────────┐
                    ▼                       ▼
┌─────────────────────────────┐   ┌─────────────────────────────┐
│   Global Flags Handler      │   │   Command Dispatcher        │
│   (src/cli/help.c)          │   │   (src/cli/commands.c)      │
│                             │   │                             │
│  • --help                   │   │  • Find command by name     │
│  • --version                │   │  • Fuzzy matching           │
│  • help <command>           │   │  • Suggestions for typos    │
└─────────────────────────────┘   │  • Route to handler         │
                                  └─────────────────────────────┘
                                              │
                      ┌───────────────────────┼───────────────────────┐
                      ▼                       ▼                       ▼
        ┌─────────────────────┐  ┌─────────────────────┐  ┌─────────────────────┐
        │   Existing Commands │  │   New Commands      │  │   Utility Commands  │
        └─────────────────────┘  └─────────────────────┘  └─────────────────────┘
                │                          │                          │
    ┌───────────┼───────────┐  ┌───────────┼───────────┐  ┌───────────┼──────────┐
    ▼           ▼           ▼  ▼           ▼           ▼  ▼           ▼          ▼
┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐
│  run   │ │ build  │ │  repl  │ │ check  │ │  fmt   │ │  test  │ │  info  │ │ config │
│        │ │        │ │        │ │        │ │        │ │        │ │        │ │        │
│ Run a  │ │Compile │ │Interactive│ Syntax │ │Format  │ │  Run   │ │ System │ │ Manage │
│ file   │ │  file  │ │  REPL  │ │ check  │ │  code  │ │ tests  │ │  info  │ │ config │
└────────┘ └────────┘ └────────┘ └────────┘ └────────┘ └────────┘ └────────┘ └────────┘
    │           │           │          │          │          │          │          │
    └───────────┴───────────┴──────────┴──────────┴──────────┴──────────┴──────────┘
                                        │
                    ┌───────────────────┼───────────────────┐
                    ▼                   ▼                   ▼
        ┌─────────────────────┐  ┌─────────────────────┐  ┌─────────────────────┐
        │   Core Modules      │  │   Language Core     │  │   Support Systems   │
        └─────────────────────┘  └─────────────────────┘  └─────────────────────┘
                    │                   │                          │
    ┌───────────────┼───────────┐      │              ┌───────────┼───────────┐
    ▼               ▼           ▼      │              ▼           ▼           ▼
┌─────────┐  ┌──────────┐  ┌──────────┐ │      ┌──────────┐  ┌──────────┐  ┌──────────┐
│  Args   │  │  Colors  │  │ Commands │ │      │  Config  │  │  Watch   │  │  Utils   │
│ Parser  │  │  System  │  │  System  │ │      │  System  │  │  System  │  │          │
│         │  │          │  │          │ │      │          │  │          │  │          │
│ Parse   │  │ ANSI     │  │ Register │ │      │  TOML    │  │ inotify/ │  │ File     │
│ flags   │  │ codes    │  │ Dispatch │ │      │  parser  │  │  kqueue  │  │ helpers  │
│ and     │  │ TTY      │  │ Suggest  │ │      │  Load    │  │  Watch   │  │ String   │
│ args    │  │ detect   │  │ commands │ │      │  Save    │  │  files   │  │ funcs    │
└─────────┘  └──────────┘  └──────────┘ │      └──────────┘  └──────────┘  └──────────┘
                                         │
                        ┌────────────────┼────────────────┐
                        ▼                ▼                ▼
                  ┌──────────┐    ┌──────────┐    ┌──────────┐
                  │  Lexer   │    │  Parser  │    │Interpreter│
                  │          │───▶│          │───▶│          │
                  │ Tokenize │    │ Build    │    │ Execute  │
                  │  source  │    │   AST    │    │   AST    │
                  └──────────┘    └──────────┘    └──────────┘


═══════════════════════════════════════════════════════════════════════════

                        Module Dependency Graph

┌─────────────┐
│   main()    │
└──────┬──────┘
       │
       ├──────────────────┐
       ▼                  ▼
┌─────────────┐    ┌─────────────┐
│    Help     │    │  Commands   │
│   System    │    │   System    │
└─────────────┘    └──────┬──────┘
                          │
       ┌──────────────────┼──────────────────┐
       ▼                  ▼                  ▼
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│    Args     │    │   Colors    │    │   Config    │
│   Parser    │    │   System    │    │   System    │
└─────────────┘    └─────────────┘    └─────────────┘
       │                  │                  │
       └──────────────────┴──────────────────┘
                          │
                          ▼
                   ┌─────────────┐
                   │  Commands   │
                   │  (run, etc) │
                   └──────┬──────┘
                          │
                          ▼
                   ┌─────────────┐
                   │   Lexer,    │
                   │   Parser,   │
                   │ Interpreter │
                   └─────────────┘


═══════════════════════════════════════════════════════════════════════════

                        REPL Architecture

┌─────────────────────────────────────────────────────────────────────────┐
│                        Enhanced REPL System                              │
└─────────────────────────────────────────────────────────────────────────┘
                                │
                    ┌───────────┴───────────┐
                    ▼                       ▼
        ┌─────────────────────┐   ┌─────────────────────┐
        │   REPL Core         │   │   Input Handler     │
        │   (repl.c)          │   │   (readline.c)      │
        │                     │   │                     │
        │  • Main loop        │◀──│  • linenoise        │
        │  • Command dispatch │   │  • History          │
        │  • State management │   │  • Line editing     │
        └─────────────────────┘   └─────────────────────┘
                    │                       │
        ┌───────────┴───────────┐          │
        ▼                       ▼          │
┌─────────────────┐   ┌─────────────────┐ │
│  Special Cmds   │   │   Completion    │ │
│  (.help, etc)   │   │   (Tab)         │◀┘
│                 │   │                 │
│  • .help        │   │  • Keywords     │
│  • .load        │   │  • Variables    │
│  • .save        │   │  • Functions    │
│  • .type        │   │  • Commands     │
│  • .ast         │   └─────────────────┘
│  • .clear       │
│  • .env         │
│  • .reset       │
└─────────────────┘
        │
        ▼
┌─────────────────────────────────────────┐
│         Interpreter                     │
│         (eval expressions)              │
└─────────────────────────────────────────┘


═══════════════════════════════════════════════════════════════════════════

                        Command Flow Diagram

User types: "klang run --watch app.kl"
│
├─▶ main() parses arguments
│   │
│   ├─▶ args_parse() extracts:
│   │   • Command: "run"
│   │   • Flags: ["--watch"]
│   │   • Positional: ["app.kl"]
│   │
│   ├─▶ command_find("run") locates handler
│   │
│   └─▶ cmd_run_handler() executes
│       │
│       ├─▶ Read file (read_file())
│       ├─▶ Lex source (lexer_init())
│       ├─▶ Parse AST (parser_init())
│       └─▶ Execute (eval_node())
│           │
│           └─▶ Print results with colors
│
▼
Success! (exit 0)


═══════════════════════════════════════════════════════════════════════════

                        Configuration Hierarchy

┌─────────────────────────────────────────────────────────────────────────┐
│                        Configuration Loading Order                       │
│                        (Later sources override earlier)                  │
└─────────────────────────────────────────────────────────────────────────┘

Priority: 1 (Lowest)
┌─────────────────────────────────────┐
│   System Config                     │
│   /etc/klang/config.toml            │
│                                     │
│   Default settings for all users    │
└─────────────────────────────────────┘
            │
            ▼ (Override)
Priority: 2
┌─────────────────────────────────────┐
│   User Config                       │
│   ~/.klang/config.toml              │
│                                     │
│   Personal preferences              │
└─────────────────────────────────────┘
            │
            ▼ (Override)
Priority: 3
┌─────────────────────────────────────┐
│   Project Config                    │
│   ./klang.toml                      │
│                                     │
│   Project-specific settings         │
└─────────────────────────────────────┘
            │
            ▼ (Override)
Priority: 4
┌─────────────────────────────────────┐
│   Environment Variables             │
│   KLANG_*, NO_COLOR, etc.           │
│                                     │
│   Runtime overrides                 │
└─────────────────────────────────────┘
            │
            ▼ (Override)
Priority: 5 (Highest)
┌─────────────────────────────────────┐
│   Command-line Flags                │
│   --no-color, --verbose, etc.       │
│                                     │
│   Explicit user intent              │
└─────────────────────────────────────┘
            │
            ▼
    ┌───────────────┐
    │ Final Config  │
    │   (merged)    │
    └───────────────┘


═══════════════════════════════════════════════════════════════════════════

                        Error Handling Flow

┌─────────────────────────────────────────────────────────────────────────┐
│                          Error Types & Handlers                          │
└─────────────────────────────────────────────────────────────────────────┘

┌─────────────────────┐
│  User Input Error   │
│  (Unknown command)  │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────────────────────┐
│  1. Print error (red)               │
│  2. Suggest alternatives (cyan)     │
│  3. Show available commands         │
│  4. Exit with code 2                │
└─────────────────────────────────────┘

┌─────────────────────┐
│  Argument Error     │
│  (Invalid flags)    │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────────────────────┐
│  1. Print error (red)               │
│  2. Show correct usage              │
│  3. Hint: "Run --help for more"    │
│  4. Exit with code 3                │
└─────────────────────────────────────┘

┌─────────────────────┐
│  File Not Found     │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────────────────────┐
│  1. Print error with path (red)     │
│  2. Check file extension            │
│  3. Suggest similar files           │
│  4. Exit with code 6                │
└─────────────────────────────────────┘

┌─────────────────────┐
│  Syntax Error       │
│  (Parse failure)    │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────────────────────┐
│  1. Print error with line number    │
│  2. Show code context               │
│  3. Highlight error location        │
│  4. Exit with code 4                │
└─────────────────────────────────────┘

┌─────────────────────┐
│  Runtime Error      │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────────────────────┐
│  1. Print error with stack trace    │
│  2. Show line number and file       │
│  3. Suggest debugging tips          │
│  4. Exit with code 5                │
└─────────────────────────────────────┘


═══════════════════════════════════════════════════════════════════════════

                        File Structure (Physical)

src/
├── cli/                        # CLI Infrastructure
│   ├── args.c                  # Argument parser (300 LOC)
│   ├── args.h
│   ├── colors.c                # Color output (150 LOC)
│   ├── colors.h
│   ├── commands.c              # Command system (200 LOC)
│   ├── commands.h
│   ├── help.c                  # Help system (100 LOC)
│   ├── help.h
│   ├── config.c                # Config files (200 LOC)
│   ├── config.h
│   ├── utils.c                 # Utilities (100 LOC)
│   └── utils.h
│
├── cli_commands/               # Command Implementations
│   ├── cmd_run.c              # klang run (150 LOC)
│   ├── cmd_build.c            # klang build (150 LOC)
│   ├── cmd_repl.c             # klang repl (200 LOC)
│   ├── cmd_check.c            # klang check (250 LOC)
│   ├── cmd_fmt.c              # klang fmt (300 LOC)
│   ├── cmd_test.c             # klang test (400 LOC)
│   ├── cmd_init.c             # klang init (200 LOC)
│   ├── cmd_info.c             # klang info (150 LOC)
│   ├── cmd_watch.c            # klang watch (250 LOC)
│   ├── cmd_bench.c            # klang bench (200 LOC)
│   ├── cmd_doc.c              # klang doc (200 LOC)
│   └── cmd_config.c           # klang config (100 LOC)
│
├── repl/                       # Enhanced REPL
│   ├── repl.c                 # Main REPL (300 LOC)
│   ├── repl.h
│   ├── readline.c             # Readline wrapper (200 LOC)
│   ├── readline.h
│   ├── completion.c           # Autocomplete (250 LOC)
│   ├── completion.h
│   ├── highlight.c            # Syntax highlight (300 LOC)
│   └── highlight.h
│
├── cli.c                       # Main entry (150 LOC)
├── lexer.c                     # (Existing)
├── parser.c                    # (Existing)
├── interpreter.c               # (Existing)
└── ...                         # (Other existing files)

Total New Code: ~6,300 LOC
Total New Files: ~35 files


═══════════════════════════════════════════════════════════════════════════

                        Build System Integration

Makefile Changes:

# New variables
CLI_SRC = $(wildcard src/cli/*.c)
CLI_CMD_SRC = $(wildcard src/cli_commands/*.c)
REPL_SRC = $(wildcard src/repl/*.c)

CLI_OBJ = $(CLI_SRC:.c=.o)
CLI_CMD_OBJ = $(CLI_CMD_SRC:.c=.o)
REPL_OBJ = $(REPL_SRC:.c=.o)

ALL_OBJ = $(OBJECTS) $(CLI_OBJ) $(CLI_CMD_OBJ) $(REPL_OBJ)

# Dependencies
LIBS = -lm

# Optional: linenoise (for REPL)
# LIBS += -llinenoise

# Optional: toml parser (for config)
# LIBS += -ltoml

# Main target
klang: $(ALL_OBJ)
	$(CC) $(CFLAGS) -o klang $(ALL_OBJ) $(LIBS)

# Clean
clean:
	rm -f src/*.o src/cli/*.o src/cli_commands/*.o src/repl/*.o klang


═══════════════════════════════════════════════════════════════════════════

                        Testing Structure

tests/
├── unit/                       # Unit tests
│   ├── test_args.c            # Test argument parser
│   ├── test_colors.c          # Test color system
│   ├── test_commands.c        # Test command system
│   ├── test_config.c          # Test config loading
│   └── ...
│
├── integration/                # Integration tests
│   ├── test_cli_help.sh       # Test help output
│   ├── test_cli_run.sh        # Test run command
│   ├── test_cli_check.sh      # Test check command
│   └── ...
│
├── fixtures/                   # Test files
│   ├── valid.kl               # Valid KLang file
│   ├── syntax_error.kl        # File with errors
│   └── configs/               # Test configs
│       ├── test1.toml
│       └── test2.toml
│
└── run_tests.sh                # Test runner


═══════════════════════════════════════════════════════════════════════════

Legend:
  ┌─┐  Box (component)
  │    Vertical connection
  ─    Horizontal connection
  ▼    Data/control flow direction
  ═    Section separator
  ◀──  Bidirectional flow
  LOC  Lines of Code

