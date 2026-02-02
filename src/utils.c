#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "model.h"

void get_file_contents(const char *filename, void **output, size_t *count, size_t record_size, int (*parse_line)(const char *, void *))
{
  FILE *file = fopen(filename, "a+");
  if (!file)
  {
    *count = 0;
    *output = NULL;
    return;
  }

  void *records = NULL;
  size_t capacity = 0;
  *count = 0;
  char line[256];
  while (fgets(line, sizeof(line), file))
  {
    if (*count >= capacity)
    {
      capacity = (capacity == 0) ? 1 : capacity * 2;
      records = realloc(records, capacity * record_size);
    }

    void *record_ptr = (char *)records + (*count * record_size);
    if (parse_line(line, record_ptr))
    {
      (*count)++;
    }
  }
  fclose(file);
  *output = records;
}

int parse_book_line(const char *line, void *output)
{
  Book *book = (Book *)output;
  return sscanf(line, "%15[^|]|%63[^|]|%31[^|]|%f", book->code, book->name, book->type, &book->price) == 4;
}

int parse_sales_record_line(const char *line, void *output)
{
  SalesRecord *record = (SalesRecord *)output;
  return sscanf(line, "%15[^|]|%d|%f", record->bookCode, &record->soldCount, &record->totalRevenue) == 3;
}

char *generate_book_code()
{
  static char code[16];
  char *lastCode = get_last_book_code();
  int number = 1;

  if (lastCode != NULL)
  {
    number = atoi(lastCode + 3) + 1;
  }

  sprintf(code, "BK%03d", number);
  return code;
}

char *get_last_book_code()
{
  if (BOOK_COUNT == 0)
  {
    return NULL;
  }
  return BOOKS[BOOK_COUNT - 1].code;
}