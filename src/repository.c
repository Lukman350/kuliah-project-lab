#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "model.h"
#include "repository.h"
#include "utils.h"
#include "constants.h"

// Define global variables
Book *BOOKS = NULL;
size_t BOOK_COUNT = 0;
SalesRecord *SALES_RECORDS = NULL;
size_t SALES_RECORD_COUNT = 0;
FILE *bookFile = NULL;
FILE *salesRecordFile = NULL;

// Book Model
void add_book(Book newBook)
{
  BOOKS = realloc(BOOKS, (BOOK_COUNT + 1) * sizeof(Book)); // Resize array
  BOOKS[BOOK_COUNT] = newBook;
  BOOK_COUNT++;
}

void get_all_books(Book **output, size_t *count)
{
  get_file_contents(BOOK_FILE, (void **)output, count, sizeof(Book), parse_book_line);
}

Book *get_book_by_code(const char *code)
{
  for (size_t i = 0; i < BOOK_COUNT; i++)
  {
    if (strcmp(BOOKS[i].code, code) == 0)
    {
      return &BOOKS[i];
    }
  }
  return NULL;
}

void delete_book_by_code(const char *code)
{
  for (size_t i = 0; i < BOOK_COUNT; i++)
  {
    if (strcmp(BOOKS[i].code, code) == 0)
    {
      // Shift remaining books
      for (size_t j = i; j < BOOK_COUNT - 1; j++)
      {
        BOOKS[j] = BOOKS[j + 1];
      }
      BOOK_COUNT--;
      BOOKS = realloc(BOOKS, BOOK_COUNT * sizeof(Book)); // Resize array
      return;
    }
  }
}

void save_books()
{
  bookFile = fopen(BOOK_FILE, "w");
  if (bookFile == NULL)
  {
    printf("Gagal membuka file %s\n", BOOK_FILE);
    return;
  }
  for (size_t i = 0; i < BOOK_COUNT; i++)
  {
    fprintf(bookFile, "%s|%s|%s|%.0f\n", BOOKS[i].code, BOOKS[i].name, BOOKS[i].type, BOOKS[i].price);
  }
  fclose(bookFile);
}

// SalesRecord Model
void add_sales_record(SalesRecord newRecord)
{
  SALES_RECORDS = realloc(SALES_RECORDS, (SALES_RECORD_COUNT + 1) * sizeof(SalesRecord)); // Resize array
  SALES_RECORDS[SALES_RECORD_COUNT] = newRecord;
  SALES_RECORD_COUNT++;
}

void get_all_sales_records(SalesRecord **output, size_t *count)
{
  get_file_contents(SALES_RECORD_FILE, (void **)output, count, sizeof(SalesRecord), parse_sales_record_line);
}

SalesRecord *get_sales_record_by_book_code(const char *bookCode)
{
  for (size_t i = 0; i < SALES_RECORD_COUNT; i++)
  {
    if (strcmp(SALES_RECORDS[i].bookCode, bookCode) == 0)
    {
      return &SALES_RECORDS[i];
    }
  }
  return NULL;
}

void delete_sales_record_by_book_code(const char *bookCode)
{
  for (size_t i = 0; i < SALES_RECORD_COUNT; i++)
  {
    if (strcmp(SALES_RECORDS[i].bookCode, bookCode) == 0)
    {
      // Shift remaining sales records
      for (size_t j = i; j < SALES_RECORD_COUNT - 1; j++)
      {
        SALES_RECORDS[j] = SALES_RECORDS[j + 1];
      }
      SALES_RECORD_COUNT--;
      SALES_RECORDS = realloc(SALES_RECORDS, SALES_RECORD_COUNT * sizeof(SalesRecord)); // Resize array
      return;
    }
  }
}

void save_sales_records()
{
  salesRecordFile = fopen(SALES_RECORD_FILE, "w");
  if (salesRecordFile == NULL)
  {
    printf("Gagal membuka file %s\n", SALES_RECORD_FILE);
    return;
  }
  for (size_t i = 0; i < SALES_RECORD_COUNT; i++)
  {
    fprintf(salesRecordFile, "%s|%d|%.0f\n", SALES_RECORDS[i].bookCode, SALES_RECORDS[i].soldCount, SALES_RECORDS[i].totalRevenue);
  }
  fclose(salesRecordFile);
}