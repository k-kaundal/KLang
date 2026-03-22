#ifndef CLI_COMMANDS_H
#define CLI_COMMANDS_H

/* Command implementations */
void cmd_check_file(const char *path);
void cmd_info_file(const char *path);
void cmd_fmt_file(const char *path, int check_only, int indent_size, int use_tabs);

#endif /* CLI_COMMANDS_H */
