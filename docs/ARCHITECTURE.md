# KLang Architecture

## Overview
KLang is a dynamically-typed scripting language implemented in C.

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
