#include "../core/lexer/lexer.h"
#include "../core/parser/parser.h"
#include "../core/ast/ast.h"
#include "../runtime/interpreter/interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "formatter.h"

typedef struct {
    FILE *output;
    int indent_level;
    int indent_size;
    int use_tabs;
    int line_pos;
    int max_line_length;
} Formatter;

static void formatter_init(Formatter *fmt, FILE *output, int indent_size, int use_tabs) {
    fmt->output = output;
    fmt->indent_level = 0;
    fmt->indent_size = indent_size;
    fmt->use_tabs = use_tabs;
    fmt->line_pos = 0;
    fmt->max_line_length = 100;
}

static void write_indent(Formatter *fmt) {
    int i, j;
    for (i = 0; i < fmt->indent_level; i++) {
        if (fmt->use_tabs) {
            fprintf(fmt->output, "\t");
            fmt->line_pos += 4; /* Assume tab = 4 spaces for line length */
        } else {
            for (j = 0; j < fmt->indent_size; j++) {
                fprintf(fmt->output, " ");
                fmt->line_pos++;
            }
        }
    }
}

static void write_str(Formatter *fmt, const char *str) {
    fprintf(fmt->output, "%s", str);
    fmt->line_pos += strlen(str);
}

static void write_newline(Formatter *fmt) {
    fprintf(fmt->output, "\n");
    fmt->line_pos = 0;
}

static void format_node(Formatter *fmt, ASTNode *node);

static void format_binary_op(Formatter *fmt, ASTNode *node) {
    const char *op_str = NULL;
    
    format_node(fmt, node->data.binop.left);
    write_str(fmt, " ");
    
    op_str = node->data.binop.op;
    
    write_str(fmt, op_str);
    write_str(fmt, " ");
    format_node(fmt, node->data.binop.right);
}

static void format_node(Formatter *fmt, ASTNode *node) {
    int i;
    
    if (!node) return;

    switch (node->type) {
        case NODE_NUMBER:
            fprintf(fmt->output, "%g", node->data.num_val);
            break;

        case NODE_STRING:
            fprintf(fmt->output, "\"%s\"", node->data.str_val);
            break;

        case NODE_IDENT:
            fprintf(fmt->output, "%s", node->data.ident.name);
            break;

        case NODE_BINOP:
            format_binary_op(fmt, node);
            break;

        case NODE_ASSIGN:
            format_node(fmt, node->data.assign_stmt.value);
            break;

        case NODE_LET:
            write_indent(fmt);
            if (node->data.let_stmt.decl_type == DECL_CONST) {
                write_str(fmt, "const ");
            } else if (node->data.let_stmt.decl_type == DECL_VAR) {
                write_str(fmt, "var ");
            } else {
                write_str(fmt, "let ");
            }
            write_str(fmt, node->data.let_stmt.name);
            if (node->data.let_stmt.type_annot) {
                write_str(fmt, ": ");
                write_str(fmt, node->data.let_stmt.type_annot);
            }
            if (node->data.let_stmt.value) {
                write_str(fmt, " = ");
                format_node(fmt, node->data.let_stmt.value);
            }
            write_newline(fmt);
            break;

        case NODE_IF:
            write_indent(fmt);
            write_str(fmt, "if ");
            format_node(fmt, node->data.if_stmt.cond);
            write_str(fmt, " {");
            write_newline(fmt);
            fmt->indent_level++;
            if (node->data.if_stmt.then_block) {
                format_node(fmt, node->data.if_stmt.then_block);
            }
            fmt->indent_level--;
            write_indent(fmt);
            write_str(fmt, "}");
            
            if (node->data.if_stmt.else_block) {
                write_str(fmt, " else {");
                write_newline(fmt);
                fmt->indent_level++;
                format_node(fmt, node->data.if_stmt.else_block);
                fmt->indent_level--;
                write_indent(fmt);
                write_str(fmt, "}");
            }
            write_newline(fmt);
            break;

        case NODE_WHILE:
            write_indent(fmt);
            write_str(fmt, "while ");
            format_node(fmt, node->data.while_stmt.cond);
            write_str(fmt, " {");
            write_newline(fmt);
            fmt->indent_level++;
            if (node->data.while_stmt.body) {
                format_node(fmt, node->data.while_stmt.body);
            }
            fmt->indent_level--;
            write_indent(fmt);
            write_str(fmt, "}");
            write_newline(fmt);
            break;

        case NODE_FOR:
            write_indent(fmt);
            write_str(fmt, "for ");
            write_str(fmt, node->data.for_stmt.var);
            write_str(fmt, " in ");
            format_node(fmt, node->data.for_stmt.start);
            write_str(fmt, " .. ");
            format_node(fmt, node->data.for_stmt.end);
            write_str(fmt, " {");
            write_newline(fmt);
            fmt->indent_level++;
            if (node->data.for_stmt.body) {
                format_node(fmt, node->data.for_stmt.body);
            }
            fmt->indent_level--;
            write_indent(fmt);
            write_str(fmt, "}");
            write_newline(fmt);
            break;

        case NODE_CALL:
            format_node(fmt, node->data.call.callee);
            write_str(fmt, "(");
            for (i = 0; i < node->data.call.args.count; i++) {
                format_node(fmt, node->data.call.args.items[i]);
                if (i < node->data.call.args.count - 1) {
                    write_str(fmt, ", ");
                }
            }
            write_str(fmt, ")");
            break;

        case NODE_FUNC_DEF:
            write_indent(fmt);
            write_str(fmt, "fn ");
            write_str(fmt, node->data.func_def.name);
            write_str(fmt, "(");
            for (i = 0; i < node->data.func_def.params.count; i++) {
                if (node->data.func_def.params.items[i]->type == NODE_IDENT) {
                    write_str(fmt, node->data.func_def.params.items[i]->data.ident.name);
                    if (node->data.func_def.param_types && node->data.func_def.param_types[i]) {
                        write_str(fmt, ": ");
                        write_str(fmt, node->data.func_def.param_types[i]);
                    }
                }
                if (i < node->data.func_def.params.count - 1) {
                    write_str(fmt, ", ");
                }
            }
            write_str(fmt, ")");
            if (node->data.func_def.return_type) {
                write_str(fmt, " -> ");
                write_str(fmt, node->data.func_def.return_type);
            }
            write_str(fmt, " {");
            write_newline(fmt);
            fmt->indent_level++;
            if (node->data.func_def.body) {
                format_node(fmt, node->data.func_def.body);
            }
            fmt->indent_level--;
            write_indent(fmt);
            write_str(fmt, "}");
            write_newline(fmt);
            break;

        case NODE_RETURN:
            write_indent(fmt);
            write_str(fmt, "return");
            if (node->data.return_stmt.value) {
                write_str(fmt, " ");
                format_node(fmt, node->data.return_stmt.value);
            }
            write_newline(fmt);
            break;

        case NODE_LIST:
            write_str(fmt, "[");
            for (i = 0; i < node->data.list.elements.count; i++) {
                format_node(fmt, node->data.list.elements.items[i]);
                if (i < node->data.list.elements.count - 1) {
                    write_str(fmt, ", ");
                }
            }
            write_str(fmt, "]");
            break;

        case NODE_INDEX:
            format_node(fmt, node->data.index_expr.obj);
            write_str(fmt, "[");
            format_node(fmt, node->data.index_expr.index);
            write_str(fmt, "]");
            break;

        case NODE_BLOCK:
            for (i = 0; i < node->data.block.stmts.count; i++) {
                format_node(fmt, node->data.block.stmts.items[i]);
            }
            break;

        default:
            /* For unhandled node types, just skip silently */
            break;
    }
}

int format_file(const char *input_path, const char *output_path, int indent_size, int use_tabs) {
    FILE *input_file;
    FILE *output_file;
    char *source;
    long size;
    size_t read_bytes;
    Lexer lexer;
    Parser parser;
    ASTNode **nodes;
    int count = 0;
    int i;
    Formatter fmt;

    /* Read input file */
    input_file = fopen(input_path, "r");
    if (!input_file) {
        fprintf(stderr, "Cannot open input file: %s\n", input_path);
        return 1;
    }

    fseek(input_file, 0, SEEK_END);
    size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    source = malloc(size + 1);
    read_bytes = fread(source, 1, size, input_file);
    source[read_bytes] = '\0';
    fclose(input_file);

    /* Parse source */
    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);

    if (parser.had_error) {
        fprintf(stderr, "Parse error: cannot format file with syntax errors\n");
        free(source);
        for (i = 0; i < count; i++) {
            if (nodes[i]) ast_free(nodes[i]);
        }
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
        return 1;
    }

    /* Open output file */
    output_file = fopen(output_path, "w");
    if (!output_file) {
        fprintf(stderr, "Cannot open output file: %s\n", output_path);
        free(source);
        for (i = 0; i < count; i++) {
            if (nodes[i]) ast_free(nodes[i]);
        }
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
        return 1;
    }

    /* Format and write */
    formatter_init(&fmt, output_file, indent_size, use_tabs);
    for (i = 0; i < count; i++) {
        if (nodes[i]) {
            format_node(&fmt, nodes[i]);
        }
    }

    /* Cleanup */
    fclose(output_file);
    for (i = 0; i < count; i++) {
        if (nodes[i]) ast_free(nodes[i]);
    }
    free(nodes);
    parser_free(&parser);
    lexer_free(&lexer);
    free(source);

    return 0;
}
