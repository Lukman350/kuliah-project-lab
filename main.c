#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *BOOK_FILE = "databuku.txt";
const char *SALES_RECORD_FILE = "history.txt";

FILE *bookFile = NULL;
FILE *salesRecordFile = NULL;

typedef struct
{
  char code[16];
  char name[64];
  char type[32];
  float price;
} Book;

typedef struct
{
  char bookCode[16];
  int soldCount;
  float totalRevenue;
} SalesRecord;

void get_all_books(Book **output, size_t *count);
void get_all_sales_records(SalesRecord **output, size_t *count);

void get_book_by_code(const char *code, Book *output);
void get_sales_record_by_book_code(const char *bookCode, SalesRecord *output);
void add_book(const char *name, const char *type, float price);
void add_sales_record(const char *bookCode, int soldCount);
void delete_book_by_code(const char *code);
void delete_sales_record_by_book_code(const char *bookCode);

int main()
{
  printf("Hello, World!\n");
  return 0;
}