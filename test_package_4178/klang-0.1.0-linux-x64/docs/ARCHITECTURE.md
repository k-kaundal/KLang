# KLang Architecture

## Overview
KLang is a lightweight, expression-first language implemented in C with a tree-walking interpreter today and a bytecode VM pipeline under construction. Core language basics (variables, functions, loops, comments, built-in printing) are exercised by unit tests in `tests/`.

## Components

### Lexer (src/lexer.c)
Tokenizes source code into a stream of tokens.

### Parser (src/parser.c)
Recursive descent parser producing an AST.

### Interpreter (src/interpreter.c)
Tree-walking interpreter with lexical scoping.

### VM (src/vm.c)
Stack-based bytecode virtual machine (Phase 2).

### Compiler (src/compiler.c)
Compiles AST to bytecode for the VM.

### GC (src/gc.c)
Mark-and-sweep garbage collector.

### Runtime (src/runtime.c)
Built-in functions and runtime initialization.

### CLI (src/cli.c)
Command-line entry point that powers `klang repl`, `klang run <file>`, and `klang build <file>` for desktop usage across POSIX/macOS/Windows (MinGW/Clang).
