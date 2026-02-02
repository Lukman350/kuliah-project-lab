#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <stdio.h>
#include "model.h"

extern FILE *bookFile;
extern FILE *salesRecordFile;

// Book Model
void add_book(Book newBook);
void get_all_books(Book **output, size_t *count);
Book *get_book_by_code(const char *code);
void delete_book_by_code(const char *code);
void save_books();

// SalesRecord Model
void add_sales_record(SalesRecord newRecord);
void get_all_sales_records(SalesRecord **output, size_t *count);
SalesRecord *get_sales_record_by_book_code(const char *bookCode);
void delete_sales_record_by_book_code(const char *bookCode);
void save_sales_records();

#endif /* REPOSITORY_H */