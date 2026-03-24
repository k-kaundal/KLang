CC = gcc

# Try to find llvm-config in order of preference
# Check standard PATH first, then Homebrew paths (macOS), then fallback
LLVM_CONFIG := $(shell which llvm-config-16 2>/dev/null || which llvm-config-18 2>/dev/null || which llvm-config-17 2>/dev/null || which llvm-config 2>/dev/null || (test -x /opt/homebrew/opt/llvm@16/bin/llvm-config && echo "/opt/homebrew/opt/llvm@16/bin/llvm-config") || (test -x /opt/homebrew/opt/llvm@17/bin/llvm-config && echo "/opt/homebrew/opt/llvm@17/bin/llvm-config") || (test -x /opt/homebrew/opt/llvm@18/bin/llvm-config && echo "/opt/homebrew/opt/llvm@18/bin/llvm-config") || (test -x /opt/homebrew/opt/llvm/bin/llvm-config && echo "/opt/homebrew/opt/llvm/bin/llvm-config") || (test -x /usr/local/opt/llvm@16/bin/llvm-config && echo "/usr/local/opt/llvm@16/bin/llvm-config") || (test -x /usr/local/opt/llvm@17/bin/llvm-config && echo "/usr/local/opt/llvm@17/bin/llvm-config") || (test -x /usr/local/opt/llvm@18/bin/llvm-config && echo "/usr/local/opt/llvm@18/bin/llvm-config") || (test -x /usr/local/opt/llvm/bin/llvm-config && echo "/usr/local/opt/llvm/bin/llvm-config") || echo "llvm-config-16")

LLVM_CFLAGS = $(shell $(LLVM_CONFIG) --cflags 2>/dev/null)
LLVM_LDFLAGS = $(shell $(LLVM_CONFIG) --ldflags --libs core executionengine mcjit native passes 2>/dev/null)

CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -Isrc -Iinclude -g $(LLVM_CFLAGS)
LDFLAGS = $(LLVM_LDFLAGS) -lm -lreadline -lpthread -ldl

SRC = src/lexer.c src/ast.c src/parser.c src/interpreter.c src/vm_stack.c src/vm_register.c src/ssa_ir.c src/compiler.c src/gc.c src/runtime.c src/repl.c src/cli.c src/cli_colors.c src/cli_help.c src/cli_commands.c src/formatter.c src/error_reporter.c src/config.c src/test_runner.c src/project_init.c src/llvm_backend.c src/type_checker.c src/package_manager.c src/lsp_server.c src/debugger.c src/type_system.c src/parallel.c src/wasm_backend.c src/plugin_system.c src/cloud_native.c
OBJ = $(SRC:.c=.o)
TARGET = klang

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

TEST_SRC = tests/test_runner.c tests/test_lexer.c tests/test_parser.c tests/test_interpreter.c tests/test_arrow_functions.c tests/test_ternary.c tests/test_async_await.c src/lexer.c src/ast.c src/parser.c src/interpreter.c src/runtime.c src/gc.c src/vm_stack.c src/vm_register.c src/compiler.c
test: $(TEST_SRC)
	$(CC) $(CFLAGS) -Itests -o test_runner $(TEST_SRC) -lm
	./test_runner

run: $(TARGET)
	./klang repl

# Installation directories
PREFIX ?= /usr/local
INSTALL_BIN = $(PREFIX)/bin
INSTALL_SHARE = $(PREFIX)/share/klang

# Install to system (requires sudo)
install: $(TARGET)
	@echo "Installing KLang to $(PREFIX)..."
	@mkdir -p $(INSTALL_BIN)
	@mkdir -p $(INSTALL_SHARE)
	@cp $(TARGET) $(INSTALL_BIN)/
	@chmod +x $(INSTALL_BIN)/$(TARGET)
	@[ -d examples ] && cp -r examples $(INSTALL_SHARE)/ || true
	@[ -d docs ] && cp -r docs $(INSTALL_SHARE)/ || true
	@[ -d stdlib ] && cp -r stdlib $(INSTALL_SHARE)/ || true
	@echo "✓ KLang installed to $(INSTALL_BIN)/$(TARGET)"
	@echo "Run 'klang --version' to verify installation"

# Install to user directory (no sudo required)
install-user: $(TARGET)
	@echo "Installing KLang to $(HOME)/.local..."
	@mkdir -p $(HOME)/.local/bin
	@mkdir -p $(HOME)/.local/share/klang
	@cp $(TARGET) $(HOME)/.local/bin/
	@chmod +x $(HOME)/.local/bin/$(TARGET)
	@[ -d examples ] && cp -r examples $(HOME)/.local/share/klang/ || true
	@[ -d docs ] && cp -r docs $(HOME)/.local/share/klang/ || true
	@[ -d stdlib ] && cp -r stdlib $(HOME)/.local/share/klang/ || true
	@echo "✓ KLang installed to $(HOME)/.local/bin/$(TARGET)"
	@echo ""
	@echo "Add to PATH if not already there:"
	@echo "  export PATH=\"\$$PATH:$(HOME)/.local/bin\""
	@echo ""
	@echo "For bash: echo 'export PATH=\"\$$PATH:$(HOME)/.local/bin\"' >> ~/.bashrc"
	@echo "For zsh:  echo 'export PATH=\"\$$PATH:$(HOME)/.local/bin\"' >> ~/.zshrc"

# Uninstall from system
uninstall:
	@echo "Uninstalling KLang from $(PREFIX)..."
	@rm -f $(INSTALL_BIN)/$(TARGET)
	@rm -rf $(INSTALL_SHARE)
	@echo "✓ KLang uninstalled"

# Uninstall from user directory
uninstall-user:
	@echo "Uninstalling KLang from $(HOME)/.local..."
	@rm -f $(HOME)/.local/bin/$(TARGET)
	@rm -rf $(HOME)/.local/share/klang
	@echo "✓ KLang uninstalled"

clean:
	rm -f src/*.o $(TARGET) test_runner

.PHONY: all test run clean install install-user uninstall uninstall-user
