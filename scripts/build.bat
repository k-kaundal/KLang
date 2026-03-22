@echo off
REM KLang Build Script for Windows
REM Builds KLang using MinGW or MSVC

setlocal enabledelayedexpansion

echo ========================================
echo      KLang Build for Windows
echo ========================================
echo.

REM Check for gcc (MinGW)
where gcc >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    set COMPILER=gcc
    set CC=gcc
    echo Using MinGW GCC
    goto :build
)

REM Check for cl (MSVC)
where cl >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    set COMPILER=msvc
    set CC=cl
    echo Using Microsoft Visual C++
    goto :build
)

echo Error: No C compiler found!
echo Please install one of the following:
echo   - MinGW-w64: https://www.mingw-w64.org/
echo   - MSYS2: https://www.msys2.org/
echo   - Visual Studio Build Tools
echo.
exit /b 1

:build
echo Building KLang...
echo.

REM Clean previous builds
if exist klang.exe del klang.exe
if exist src\*.o del src\*.o

REM Compile with GCC (MinGW)
if "%COMPILER%"=="gcc" (
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/lexer.c -o src/lexer.o
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/ast.c -o src/ast.o
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/parser.c -o src/parser.o
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/interpreter.c -o src/interpreter.o
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/vm.c -o src/vm.o
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/compiler.c -o src/compiler.o
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/gc.c -o src/gc.o
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/runtime.c -o src/runtime.o
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/repl.c -o src/repl.o
    gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -c src/cli.c -o src/cli.o
    
    gcc -o klang.exe src/lexer.o src/ast.o src/parser.o src/interpreter.o src/vm.o src/compiler.o src/gc.o src/runtime.o src/repl.o src/cli.o -lm
    
    if !ERRORLEVEL! NEQ 0 (
        echo Build failed!
        exit /b 1
    )
)

REM Check if build succeeded
if exist klang.exe (
    echo.
    echo ========================================
    echo        Build Successful!
    echo ========================================
    echo.
    echo KLang binary: klang.exe
    echo.
    echo Try running:
    echo   klang.exe repl
    echo   klang.exe run examples\hello.kl
    echo.
) else (
    echo Build failed - klang.exe not created
    exit /b 1
)

endlocal
