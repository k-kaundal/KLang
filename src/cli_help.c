#include <stdio.h>
#include <string.h>
#include "cli_help.h"
#include "cli_colors.h"

#define KLANG_VERSION "0.2.0"
#define KLANG_BUILD_DATE __DATE__

void print_version(void) {
    printf("%sKLang%s version %s%s%s\n", 
           get_color(COLOR_BOLD_CYAN),
           get_color(COLOR_RESET),
           get_color(COLOR_BOLD_GREEN),
           KLANG_VERSION,
           get_color(COLOR_RESET));
    printf("Build date: %s\n", KLANG_BUILD_DATE);
    printf("A high-performance, statically-typed programming language\n");
}

void print_help(void) {
    printf("\n");
    printf("%s╔═══════════════════════════════════════════════════════════╗%s\n",
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
    printf("%s║                   %sKLang CLI v%s%s                     ║%s\n",
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_BOLD_WHITE), KLANG_VERSION,
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
    printf("%s║        High-Performance Programming Language            ║%s\n",
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
    printf("%s╚═══════════════════════════════════════════════════════════╝%s\n\n",
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));

    printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
    printf("    klang [OPTIONS] <COMMAND> [ARGS]\n\n");

    printf("%sCOMMANDS:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
    
    printf("  %srun%s <file>       Execute a KLang source file (.kl, .k, .klang)\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %sbuild%s <file>     Compile and run via bytecode VM\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %scompile%s <file>   Compile to native executable (LLVM)\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %srepl%s             Start interactive REPL mode\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %scheck%s <file>     Check syntax without executing\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %sfmt%s <file>       Format KLang source code\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %sinfo%s <file>      Display file information and statistics\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %stest%s <path>      Run tests from file or directory\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %sinit%s <name>      Initialize a new KLang project\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %spkg%s <command>    Package manager (install, list, update, etc.)\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %slsp%s              Start Language Server Protocol server\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %sconfig%s           Show current configuration\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %sversion%s          Show version information\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %shelp%s [command]   Show help for a command\n\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));

    printf("%sOPTIONS:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
    printf("  %s-h, --help%s       Show this help message\n",
           get_color(COLOR_CYAN), get_color(COLOR_RESET));
    printf("  %s-v, --version%s    Show version information\n",
           get_color(COLOR_CYAN), get_color(COLOR_RESET));
    printf("  %s--no-color%s       Disable colored output\n\n",
           get_color(COLOR_CYAN), get_color(COLOR_RESET));

    printf("%sEXAMPLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
    printf("  klang run program.kl           # Run a program\n");
    printf("  klang compile program.kl       # Compile to native code\n");
    printf("  klang repl                     # Start REPL\n");
    printf("  klang check script.k           # Check syntax\n");
    printf("  klang fmt script.k             # Format code\n");
    printf("  klang test tests/              # Run all tests\n");
    printf("  klang init my-project --git    # Create new project\n");
    printf("  klang pkg install lodash@1.0   # Install a package\n");
    printf("  klang pkg list                 # List installed packages\n");
    printf("  klang info myfile.klang        # Show file info\n");
    printf("  klang help run                 # Help for 'run' command\n\n");

    printf("%sFILE EXTENSIONS:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
    printf("  Supported: %s.kl%s, %s.k%s, %s.klang%s\n\n",
           get_color(COLOR_YELLOW), get_color(COLOR_RESET),
           get_color(COLOR_YELLOW), get_color(COLOR_RESET),
           get_color(COLOR_YELLOW), get_color(COLOR_RESET));

    printf("For more information, visit: %shttps://github.com/k-kaundal/KLang%s\n",
           get_color(COLOR_BLUE), get_color(COLOR_RESET));
}

void print_command_help(const char* command) {
    if (strcmp(command, "run") == 0) {
        printf("\n%sKLang Run Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang run <file>\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Execute a KLang source file using the interpreter.\n");
        printf("    The file must have a valid extension (.kl, .k, .klang).\n\n");
        printf("%sEXAMPLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang run script.kl\n");
        printf("    klang run examples/hello.k\n");
        printf("    klang run /path/to/program.klang\n\n");
    }
    else if (strcmp(command, "build") == 0) {
        printf("\n%sKLang Build Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang build <file>\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Compile the source file to bytecode and execute via VM.\n");
        printf("    Provides better performance than interpretation.\n\n");
        printf("%sEXAMPLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang build script.kl\n");
        printf("    klang build app.k\n\n");
    }
    else if (strcmp(command, "compile") == 0) {
        printf("\n%sKLang Compile Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang compile <file>\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Compile KLang source to native machine code using LLVM.\n");
        printf("    Generates a standalone executable that runs without the KLang runtime.\n");
        printf("    Provides maximum performance with full optimization.\n\n");
        printf("%sFEATURES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    - Native code generation via LLVM\n");
        printf("    - Optimization passes (mem2reg, constant folding, DCE)\n");
        printf("    - Standalone executables (no runtime dependency)\n");
        printf("    - Full performance, comparable to C/C++\n\n");
        printf("%sEXAMPLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang compile script.kl    # Generates ./script executable\n");
        printf("    klang compile app.k        # Generates ./app executable\n");
        printf("    ./script                   # Run the compiled binary\n\n");
    }
    else if (strcmp(command, "repl") == 0) {
        printf("\n%sKLang REPL Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang repl\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Start an interactive Read-Eval-Print Loop for KLang.\n");
        printf("    Type 'exit' or 'quit' to leave the REPL.\n\n");
        printf("%sREPL COMMANDS:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    exit / quit    Exit the REPL\n");
        printf("    exit(n)        Exit with code n\n\n");
    }
    else if (strcmp(command, "check") == 0) {
        printf("\n%sKLang Check Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang check <file>\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Check the syntax of a KLang file without executing it.\n");
        printf("    Useful for catching errors before runtime.\n\n");
        printf("%sEXAMPLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang check script.kl\n");
        printf("    klang check *.k\n\n");
    }
    else if (strcmp(command, "info") == 0) {
        printf("\n%sKLang Info Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang info <file>\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Display information and statistics about a KLang file.\n");
        printf("    Shows line count, size, and basic analysis.\n\n");
        printf("%sEXAMPLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang info script.kl\n\n");
    }
    else if (strcmp(command, "fmt") == 0) {
        printf("\n%sKLang Format Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang fmt <file> [OPTIONS]\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Format KLang source code with consistent style.\n");
        printf("    Formats indentation, spacing, and code structure.\n\n");
        printf("%sOPTIONS:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    --check           Check if file needs formatting (don't modify)\n");
        printf("    --indent=N        Set indent size (1-8, default: 4)\n");
        printf("    --tabs            Use tabs instead of spaces\n\n");
        printf("%sEXAMPLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang fmt script.kl              # Format with defaults\n");
        printf("    klang fmt script.kl --check      # Check only\n");
        printf("    klang fmt script.kl --indent=2   # Use 2-space indent\n");
        printf("    klang fmt script.kl --tabs       # Use tabs\n\n");
    }
    else if (strcmp(command, "test") == 0) {
        printf("\n%sKLang Test Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang test <file|directory>\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Run KLang test files.\n");
        printf("    Discovers and runs all *_test.kl files in directories.\n\n");
        printf("%sEXAMPLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang test tests/              # Run all tests in directory\n");
        printf("    klang test tests/foo_test.kl   # Run specific test\n\n");
    }
    else if (strcmp(command, "init") == 0) {
        printf("\n%sKLang Init Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang init <project-name> [OPTIONS]\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Initialize a new KLang project with standard structure.\n");
        printf("    Creates directories, template files, and configuration.\n\n");
        printf("%sOPTIONS:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    --git             Initialize git repository\n\n");
        printf("%sEXAMPLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang init my-project           # Create project\n");
        printf("    klang init my-project --git     # Create with git\n\n");
    }
    else if (strcmp(command, "config") == 0) {
        printf("\n%sKLang Config Command%s\n\n", 
               get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
        printf("%sUSAGE:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    klang config\n\n");
        printf("%sDESCRIPTION:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    Display current KLang configuration.\n");
        printf("    Shows settings loaded from environment variables.\n\n");
        printf("%sENVIRONMENT VARIABLES:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
        printf("    KLANG_NO_COLOR        Disable colored output\n");
        printf("    KLANG_DEBUG           Enable debug mode\n");
        printf("    KLANG_INDENT_SIZE     Set default indent size (1-8)\n");
        printf("    KLANG_USE_TABS        Use tabs instead of spaces\n");
        printf("    KLANG_HISTORY_SIZE    REPL history size (0-10000)\n");
        printf("    KLANG_PATH            Module search paths (colon-separated)\n");
        printf("    KLANG_MAX_LINE_LENGTH Max line length for formatter\n\n");
    }
    else {
        print_error("Unknown command");
        fprintf(stderr, "Use 'klang help' for available commands.\n");
    }
}
