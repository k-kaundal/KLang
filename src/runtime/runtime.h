#ifndef RUNTIME_H
#define RUNTIME_H

#include "runtime/interpreter/interpreter.h"

void runtime_init(Interpreter *interp);
void runtime_free(Interpreter *interp);
void klang_error(const char *msg, int line);
void klang_panic(const char *msg);

#endif
