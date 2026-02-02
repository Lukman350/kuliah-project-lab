#include <stdio.h>
#include <stdlib.h>

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

int main()
{
  printf("Hello, World!\n");
  return 0;
}