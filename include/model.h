#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>

typedef struct
{
  char code[16];
  char name[64];
  char type[32];
  float price;
} Book;

extern Book *BOOKS;
extern size_t BOOK_COUNT;

typedef struct
{
  char bookCode[16];
  int soldCount;
  float totalRevenue;
} SalesRecord;

extern SalesRecord *SALES_RECORDS;
extern size_t SALES_RECORD_COUNT;

#endif /* MODEL_H */