CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -Isrc -Iinclude -g
SRC = src/lexer.c src/ast.c src/parser.c src/interpreter.c src/vm.c src/compiler.c src/gc.c src/runtime.c src/repl.c src/cli.c
OBJ = $(SRC:.c=.o)
TARGET = klang

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

TEST_SRC = tests/test_runner.c tests/test_lexer.c tests/test_parser.c tests/test_interpreter.c src/lexer.c src/ast.c src/parser.c src/interpreter.c src/runtime.c src/gc.c src/vm.c src/compiler.c
test: $(TEST_SRC)
	$(CC) $(CFLAGS) -Itests -o test_runner $(TEST_SRC) -lm
	./test_runner

run: $(TARGET)
	./klang repl

clean:
	rm -f src/*.o $(TARGET) test_runner

.PHONY: all test run clean
