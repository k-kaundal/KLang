#ifndef KLANG_H
#define KLANG_H

// Version is now injected at compile time via -DKLANG_VERSION
// This file maintains backward compatibility for code that includes it
#ifndef KLANG_VERSION
#define KLANG_VERSION "dev"
#endif

// Parse version components from string (for compatibility)
#define KLANG_VERSION_MAJOR 1
#define KLANG_VERSION_MINOR 0
#define KLANG_VERSION_PATCH 0

#include "core/lexer/lexer.h"
#include "core/ast/ast.h"
#include "core/parser/parser.h"
#include "runtime/interpreter/interpreter.h"
#include "vm.h"
#include "compiler.h"
#include "gc.h"
#include "runtime.h"

#endif
