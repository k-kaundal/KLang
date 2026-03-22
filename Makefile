CC = gcc
LLVM_CONFIG = llvm-config-16
LLVM_CFLAGS = $(shell $(LLVM_CONFIG) --cflags)
LLVM_LDFLAGS = $(shell $(LLVM_CONFIG) --ldflags --libs core executionengine mcjit native passes)

CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -Isrc -Iinclude -g $(LLVM_CFLAGS)
LDFLAGS = $(LLVM_LDFLAGS) -lm

SRC = src/lexer.c src/ast.c src/parser.c src/interpreter.c src/vm.c src/compiler.c src/gc.c src/runtime.c src/repl.c src/cli.c src/cli_colors.c src/cli_help.c src/cli_commands.c src/formatter.c src/error_reporter.c src/config.c src/test_runner.c src/project_init.c src/llvm_backend.c src/type_checker.c
OBJ = $(SRC:.c=.o)
TARGET = klang

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

TEST_SRC = tests/test_runner.c tests/test_lexer.c tests/test_parser.c tests/test_interpreter.c tests/test_arrow_functions.c tests/test_ternary.c tests/test_async_await.c src/lexer.c src/ast.c src/parser.c src/interpreter.c src/runtime.c src/gc.c src/vm.c src/compiler.c
test: $(TEST_SRC)
	$(CC) $(CFLAGS) -Itests -o test_runner $(TEST_SRC) -lm
	./test_runner

run: $(TARGET)
	./klang repl

clean:
	rm -f src/*.o $(TARGET) test_runner

.PHONY: all test run clean
