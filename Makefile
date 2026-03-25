CC = gcc

# Try to find llvm-config with version-agnostic approach
# Priority: Homebrew paths first (any version), then PATH, then specific versions
LLVM_CONFIG := $(shell \
	(test -x /opt/homebrew/opt/llvm/bin/llvm-config && echo "/opt/homebrew/opt/llvm/bin/llvm-config") || \
	(test -x /usr/local/opt/llvm/bin/llvm-config && echo "/usr/local/opt/llvm/bin/llvm-config") || \
	which llvm-config 2>/dev/null || \
	which llvm-config-22 2>/dev/null || \
	which llvm-config-21 2>/dev/null || \
	which llvm-config-20 2>/dev/null || \
	which llvm-config-19 2>/dev/null || \
	which llvm-config-18 2>/dev/null || \
	which llvm-config-17 2>/dev/null || \
	which llvm-config-16 2>/dev/null || \
	which llvm-config-15 2>/dev/null || \
	which llvm-config-14 2>/dev/null || \
	(test -x /opt/homebrew/opt/llvm@22/bin/llvm-config && echo "/opt/homebrew/opt/llvm@22/bin/llvm-config") || \
	(test -x /opt/homebrew/opt/llvm@21/bin/llvm-config && echo "/opt/homebrew/opt/llvm@21/bin/llvm-config") || \
	(test -x /opt/homebrew/opt/llvm@20/bin/llvm-config && echo "/opt/homebrew/opt/llvm@20/bin/llvm-config") || \
	(test -x /opt/homebrew/opt/llvm@19/bin/llvm-config && echo "/opt/homebrew/opt/llvm@19/bin/llvm-config") || \
	(test -x /opt/homebrew/opt/llvm@18/bin/llvm-config && echo "/opt/homebrew/opt/llvm@18/bin/llvm-config") || \
	(test -x /usr/local/opt/llvm@22/bin/llvm-config && echo "/usr/local/opt/llvm@22/bin/llvm-config") || \
	(test -x /usr/local/opt/llvm@21/bin/llvm-config && echo "/usr/local/opt/llvm@21/bin/llvm-config") || \
	echo "llvm-config")

LLVM_CFLAGS = $(shell $(LLVM_CONFIG) --cflags 2>/dev/null)
LLVM_LDFLAGS = $(shell $(LLVM_CONFIG) --ldflags --libs core executionengine mcjit native passes 2>/dev/null)

# Platform detection for conditional linking
UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)
ifeq ($(OS),Windows_NT)
    # Windows (native or MinGW)
    PLATFORM_CFLAGS = -D_GNU_SOURCE
    PLATFORM_LDFLAGS = -lm -lreadline
else ifeq ($(UNAME_S),Linux)
    # Linux
    PLATFORM_CFLAGS = -D_POSIX_C_SOURCE=200809L
    PLATFORM_LDFLAGS = -lm -lreadline -lpthread -ldl
else ifeq ($(UNAME_S),Darwin)
    # macOS
    PLATFORM_CFLAGS = -D_POSIX_C_SOURCE=200809L
    PLATFORM_LDFLAGS = -lm -lreadline -lpthread
else
    # Other Unix-like (BSD, etc.)
    PLATFORM_CFLAGS = -D_POSIX_C_SOURCE=200809L
    PLATFORM_LDFLAGS = -lm -lreadline -lpthread -ldl
endif

# KLP Protocol Support (Phase 2-4)
ENABLE_KLP ?= 1
ifeq ($(ENABLE_KLP),1)
    PLATFORM_CFLAGS += -DENABLE_KLP
    KLP_LDFLAGS = -lz
else
    KLP_LDFLAGS =
endif

# Quantum-Resistant Crypto (Phase 3)
ENABLE_LIBOQS ?= 0
ifeq ($(ENABLE_LIBOQS),1)
    PLATFORM_CFLAGS += -DENABLE_LIBOQS
    CRYPTO_LDFLAGS = -loqs -lcrypto
else
    CRYPTO_LDFLAGS =
endif

# RDMA Support (Phase 4)
ENABLE_RDMA ?= 0
ifeq ($(ENABLE_RDMA),1)
    PLATFORM_CFLAGS += -DENABLE_RDMA
    RDMA_LDFLAGS = -libverbs -lrdmacm
else
    RDMA_LDFLAGS =
endif

# Hardware Acceleration (Phase 4)
ENABLE_CUDA ?= 0
ifeq ($(ENABLE_CUDA),1)
    PLATFORM_CFLAGS += -DENABLE_CUDA
    CUDA_LDFLAGS = -lcuda -lcudart
else
    CUDA_LDFLAGS =
endif

# Dynamic version from git or VERSION file
GIT_VERSION := $(shell git describe --tags --always --dirty 2>/dev/null)
FILE_VERSION := $(shell cat VERSION 2>/dev/null || echo "dev")
VERSION := $(or $(GIT_VERSION),$(FILE_VERSION))

CFLAGS = -Wall -Wextra -std=c99 $(PLATFORM_CFLAGS) -Isrc -Iinclude -g $(LLVM_CFLAGS) -DKLANG_VERSION=\"$(VERSION)\"
LDFLAGS = $(LLVM_LDFLAGS) $(PLATFORM_LDFLAGS) $(KLP_LDFLAGS) $(CRYPTO_LDFLAGS) $(RDMA_LDFLAGS) $(CUDA_LDFLAGS)

SRC = src/lexer.c src/ast.c src/parser.c src/interpreter.c src/vm_stack.c src/vm_register.c src/ssa_ir.c src/compiler.c src/gc.c src/runtime.c src/repl.c src/cli.c src/cli_colors.c src/cli_help.c src/cli_commands.c src/formatter.c src/error_reporter.c src/config.c src/test_runner.c src/project_init.c src/llvm_backend.c src/type_checker.c src/package_manager.c src/lsp_server.c src/debugger.c src/type_system.c src/parallel.c src/wasm_backend.c src/plugin_system.c src/cloud_native.c src/http_server.c src/klp_protocol.c src/klp_server.c src/klp_client.c src/klp_runtime.c src/klp_crypto.c
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

# Phase 2 Unit Tests
PHASE2_SRC = src/lexer.c src/ast.c src/parser.c src/interpreter.c src/runtime.c src/gc.c src/vm_stack.c src/vm_register.c src/compiler.c src/ssa_ir.c src/llvm_backend.c src/type_checker.c src/config.c src/error_reporter.c src/cli_colors.c

test-pointers: $(PHASE2_SRC)
	$(CC) $(CFLAGS) -o test_pointers_unit tests/test_pointers_unit.c $(PHASE2_SRC) $(LDFLAGS)
	./test_pointers_unit

test-structs: $(PHASE2_SRC)
	$(CC) $(CFLAGS) -o test_structs_unit tests/test_structs_unit.c $(PHASE2_SRC) $(LDFLAGS)
	./test_structs_unit

test-memory: $(PHASE2_SRC)
	$(CC) $(CFLAGS) -o test_memory_unit tests/test_memory_unit.c $(PHASE2_SRC) $(LDFLAGS)
	./test_memory_unit

test-phase2: test-pointers test-structs test-memory
	@echo ""
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "✓ All Phase 2 unit tests completed"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

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

# Benchmarking
BENCH_CFLAGS = -O3 -Ibenchmarks/framework
BENCH_LDFLAGS = -lm

benchmarks: $(TARGET)
	@echo "Building benchmarks..."
	@mkdir -p benchmarks/reports
	@$(CC) $(BENCH_CFLAGS) -o benchmarks/language/arithmetic_bench benchmarks/language/arithmetic_bench.c $(BENCH_LDFLAGS)
	@$(CC) $(BENCH_CFLAGS) -o benchmarks/language/loop_bench benchmarks/language/loop_bench.c $(BENCH_LDFLAGS)
	@$(CC) $(BENCH_CFLAGS) -o benchmarks/language/function_bench benchmarks/language/function_bench.c $(BENCH_LDFLAGS)
	@$(CC) $(BENCH_CFLAGS) -o benchmarks/language/string_bench benchmarks/language/string_bench.c $(BENCH_LDFLAGS)
	@$(CC) $(BENCH_CFLAGS) -o benchmarks/memory/memory_bench benchmarks/memory/memory_bench.c $(BENCH_LDFLAGS)
	@echo "✓ Benchmarks built"

bench: benchmarks
	@./benchmarks/run_benchmarks.sh

bench-quick: benchmarks
	@echo "Running quick benchmarks..."
	@./benchmarks/language/arithmetic_bench
	@echo "✓ Quick benchmark complete"

# KLP Protocol Benchmarks
klp-benchmark: $(TARGET)
	@echo "Building KLP benchmark..."
	@$(CC) $(CFLAGS) -o benchmarks/klp_benchmark benchmarks/klp_benchmark.c \
		src/klp_protocol.o src/klp_server.o src/klp_client.o \
		-lz -lpthread -lm
	@echo "✓ KLP benchmark built"
	@echo "Running KLP benchmark..."
	@./benchmarks/klp_benchmark

# KLP Examples
klp-examples:
	@echo "Building KLP examples..."
	@$(CC) $(CFLAGS) -o examples/klp_echo_server examples/klp_echo_server.c \
		src/klp_protocol.o src/klp_server.o -lz -lpthread
	@$(CC) $(CFLAGS) -o examples/klp_test_client examples/klp_test_client.c \
		src/klp_protocol.o src/klp_client.o -lz
	@echo "✓ KLP examples built"
	@echo ""
	@echo "Run server: ./examples/klp_echo_server 9000"
	@echo "Run client: ./examples/klp_test_client 127.0.0.1 9000"

clean:
	rm -f src/*.o $(TARGET) test_runner
	rm -f benchmarks/language/*_bench
	rm -f benchmarks/reports/*.json
	rm -f benchmarks/klp_benchmark
	rm -f examples/klp_echo_server examples/klp_test_client
	rm -f test_pointers_unit test_structs_unit test_memory_unit

clean-bench:
	rm -f benchmarks/language/*_bench
	rm -f benchmarks/reports/*.json
	rm -f benchmarks/reports/*.md

.PHONY: all test run clean install install-user uninstall uninstall-user benchmarks bench bench-quick clean-bench test-pointers test-structs test-memory test-phase2 klp-benchmark klp-examples
