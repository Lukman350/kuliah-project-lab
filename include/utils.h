#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void get_file_contents(const char *filename, void **output, size_t *count, size_t record_size, int (*parse_line)(const char *, void *));
int parse_book_line(const char *line, void *output);
int parse_sales_record_line(const char *line, void *output);
char *generate_book_code();
char *get_last_book_code();

#endif /* UTILS_H */