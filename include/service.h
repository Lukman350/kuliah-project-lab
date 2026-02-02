#ifndef SERVICE_H
#define SERVICE_H

#include "model.h"

// Book Service
void create_book(const char *name, const char *type, float price);
void view_books();
void delete_book(const char *code);
void save_all_books();

// SalesRecord Service
void create_sales_record(const char *bookCode, int soldCount);
void view_sales_records();
void delete_sales_record(const char *bookCode);
void save_all_sales_records();

// Utility Service
void initialize_data();

#endif /* SERVICE_H */