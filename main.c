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

Book *BOOKS = NULL;
size_t bookCount = 0;
SalesRecord *SALES_RECORDS = NULL;
size_t salesRecordCount = 0;

void print_menu();

void get_all_books(Book **output, size_t *count);
void get_all_sales_records(SalesRecord **output, size_t *count);
Book *get_book_by_code(const char *code);
SalesRecord *get_sales_record_by_book_code(const char *bookCode);

void add_book(Book newBook);
void add_sales_record(SalesRecord newRecord);
void delete_book_by_code(const char *code);
void delete_sales_record_by_book_code(const char *bookCode);

void save_books();
void save_sales_records();
char *generate_book_code();
char *get_last_book_code();

int main()
{
  int choice;

  // Init all structs from files
  get_all_books(&BOOKS, &bookCount);
  get_all_sales_records(&SALES_RECORDS, &salesRecordCount);

  while (1)
  {
    print_menu();
    scanf("%d", &choice);
    getchar(); // Consume newline character

    switch (choice)
    {
    case 1: // Add Book
    {
      char code[16] = "", name[64] = "", type[32] = "";
      float price = 0.0f;
      printf("Masukkan nama buku: ");
      fgets(name, sizeof(name), stdin);
      name[strcspn(name, "\n")] = 0; // Remove newline character
      printf("Masukkan tipe buku: ");
      fgets(type, sizeof(type), stdin);
      type[strcspn(type, "\n")] = 0;
      printf("Masukkan harga buku: ");
      scanf("%f", &price);
      strcpy(code, generate_book_code());

      Book newBook;
      strcpy(newBook.code, code);
      strcpy(newBook.name, name);
      strcpy(newBook.type, type);
      newBook.price = price;

      add_book(newBook);

      printf("Buku berhasil ditambahkan.\n\n");
      getchar();
      break;
    }
    case 2: // View Sales Records
    {

      getchar();
      break;
    }
    case 3: // View All Books
    {
      if (bookCount == 0)
      {
        printf("\nTidak ada data buku tersedia.\n\n");
        getchar();
        break;
      }

      printf("\nDaftar Buku Literasi Nusantara Jaya:\n");
      printf("=================================\n");
      for (size_t i = 0; i < bookCount; i++)
      {
        printf("Kode Buku   : %s\n", BOOKS[i].code);
        printf("Nama Buku   : %s\n", BOOKS[i].name);
        printf("Tipe Buku   : %s\n", BOOKS[i].type);
        printf("Harga Buku  : %.0f\n", BOOKS[i].price);
        printf("--------------------------------\n");
      }

      printf("\n");

      getchar();
      break;
    }
    case 4: // Delete Sales Record
    {

      getchar();
      break;
    }
    case 5: // Delete Book
    {

      getchar();
      break;
    }
    case 6: // Exit
    {
      save_books();
      save_sales_records();
      printf("Data berhasil disimpan. Keluar dari program.\n");
      return 0;
    }
    case 7: // Input Sales Transaction
    {
      char bookCode[16] = "";
      int soldCount = 0;
      float totalRevenue = 0.0f;

      printf("Masukkan kode buku yang terjual: ");
      fgets(bookCode, sizeof(bookCode), stdin);
      bookCode[strcspn(bookCode, "\n")] = 0; // Remove newline character
      Book *book = get_book_by_code(bookCode);

      if (book == NULL)
      {
        printf("Buku dengan kode %s tidak ditemukan.\n\n", bookCode);
        break;
      }

      printf("Masukkan jumlah buku yang terjual: ");
      scanf("%d", &soldCount);

      totalRevenue = book->price * soldCount;

      SalesRecord *existingRecord = get_sales_record_by_book_code(bookCode);
      if (existingRecord != NULL)
      {
        existingRecord->soldCount += soldCount;
        existingRecord->totalRevenue += totalRevenue;
        printf("Transaksi penjualan berhasil diperbarui.\n\n");
        getchar();
        break;
      }

      SalesRecord newRecord;
      strcpy(newRecord.bookCode, bookCode);
      newRecord.soldCount = soldCount;
      newRecord.totalRevenue = totalRevenue;
      add_sales_record(newRecord);

      printf("Transaksi penjualan berhasil dicatat.\n\n");
      getchar();
      break;
    }
    default:
      printf("Pilihan tidak valid. Silakan coba lagi.\n\n");
      break;
    }
  }

  if (BOOKS)
    free(BOOKS);
  if (SALES_RECORDS)
    free(SALES_RECORDS);

  return 0;
}

void print_menu()
{
  printf("Selamat datang di Toko Buku Literasi Nusantara Jaya\n");
  printf("Menu yang tersedia:\n");
  printf("1. Tambah Buku\n");
  printf("2. Lihat Catatan Penjualan\n");
  printf("3. Lihat Semua Buku\n");
  printf("4. Hapus Catatan Penjualan\n");
  printf("5. Hapus Buku\n");
  printf("6. Exit\n");
  printf("7. Input Transaksi Penjualan\n");
  printf("=============================================\n");
  printf("Pilih menu (1-7): ");
}

void save_books()
{
  bookFile = fopen(BOOK_FILE, "w");
  if (bookFile == NULL)
  {
    printf("Gagal membuka file %s\n", BOOK_FILE);
    return;
  }
  for (size_t i = 0; i < bookCount; i++)
  {
    fprintf(bookFile, "%s|%s|%s|%.0f\n", BOOKS[i].code, BOOKS[i].name, BOOKS[i].type, BOOKS[i].price);
  }
  fclose(bookFile);
}

void save_sales_records()
{
  salesRecordFile = fopen(SALES_RECORD_FILE, "w");
  if (salesRecordFile == NULL)
  {
    printf("Gagal membuka file %s\n", SALES_RECORD_FILE);
    return;
  }
  for (size_t i = 0; i < salesRecordCount; i++)
  {
    fprintf(salesRecordFile, "%s|%d|%.0f\n", SALES_RECORDS[i].bookCode, SALES_RECORDS[i].soldCount, SALES_RECORDS[i].totalRevenue);
  }
  fclose(salesRecordFile);
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
  if (bookCount == 0)
  {
    return NULL;
  }
  return BOOKS[bookCount - 1].code;
}

Book *get_book_by_code(const char *code)
{
  for (size_t i = 0; i < bookCount; i++)
  {
    if (strcmp(BOOKS[i].code, code) == 0)
    {
      return &BOOKS[i];
    }
  }
  return NULL;
}

SalesRecord *get_sales_record_by_book_code(const char *bookCode)
{
  for (size_t i = 0; i < salesRecordCount; i++)
  {
    if (strcmp(SALES_RECORDS[i].bookCode, bookCode) == 0)
    {
      return &SALES_RECORDS[i];
    }
  }
  return NULL;
}

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

void get_all_sales_records(SalesRecord **output, size_t *count)
{
  get_file_contents(SALES_RECORD_FILE, (void **)output, count, sizeof(SalesRecord), parse_sales_record_line);
}

void get_all_books(Book **output, size_t *count)
{
  get_file_contents(BOOK_FILE, (void **)output, count, sizeof(Book), parse_book_line);
}

void add_book(Book newBook)
{
  BOOKS = realloc(BOOKS, (bookCount + 1) * sizeof(Book)); // Resize array
  BOOKS[bookCount] = newBook;
  bookCount++;
}

void add_sales_record(SalesRecord newRecord)
{
  SALES_RECORDS = realloc(SALES_RECORDS, (salesRecordCount + 1) * sizeof(SalesRecord)); // Resize array
  SALES_RECORDS[salesRecordCount] = newRecord;
  salesRecordCount++;
}
