#ifndef FORMATTER_H
#define FORMATTER_H

/* Format a KLang source file 
 * Returns 0 on success, 1 on error
 */
int format_file(const char *input_path, const char *output_path, int indent_size, int use_tabs);

#endif /* FORMATTER_H */
