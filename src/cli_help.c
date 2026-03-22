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
    printf("  %srepl%s             Start interactive REPL mode\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %scheck%s <file>     Check syntax without executing\n",
           get_color(COLOR_GREEN), get_color(COLOR_RESET));
    printf("  %sinfo%s <file>      Display file information and statistics\n",
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
    printf("  klang repl                     # Start REPL\n");
    printf("  klang check script.k           # Check syntax\n");
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
    else {
        print_error("Unknown command");
        fprintf(stderr, "Use 'klang help' for available commands.\n");
    }
}
