#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h> // getch()
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

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

// MENU
void print_menu();
void print_sort_menu();

// UI
void clear_screen();
void wait_and_back_to_menu();
void press_any_key();

// ERROR
int read_int(const char *prompt, int *outValue);
char read_char_choice(const char *prompt);

// CURD
void get_all_books(Book **output, size_t *count);
void get_all_sales_records(SalesRecord **output, size_t *count);
Book *get_book_by_code(const char *code);
SalesRecord *get_sales_record_by_book_code(const char *bookCode);

void add_book(Book newBook);
void add_sales_record(SalesRecord newRecord);

int delete_sales_record_by_index(size_t index);
int delete_book_by_index(size_t index);

void save_books();
void save_sales_records();
char *generate_book_code();
char *get_last_book_code();

// SORT
Book *clone_books(const Book *source, size_t count);
void show_books(const Book *arr, size_t count);

void sort_books_by_name_bubble_asc(Book *arr, size_t count);
void sort_books_by_price_selection_desc(Book *arr, size_t count);

int main()
{
  int choice;

  clear_screen();

  // Init all structs from files
  get_all_books(&BOOKS, &bookCount);
  get_all_sales_records(&SALES_RECORDS, &salesRecordCount);

  while (1)
  {
    print_menu();

    if (!read_int("Pilih menu (1-8): ", &choice))
    {
      printf("Input menu tidak valid.\n");
      wait_and_back_to_menu();
      continue;
    }

    clear_screen();

    switch (choice)
    {

    // Add Book
    case 1:
    {
      char code[16] = "", name[64] = "", type[32] = "";
      float price = 0.0f;

      printf("Masukkan nama buku: ");
      fgets(name, sizeof(name), stdin);
      name[strcspn(name, "\n")] = 0;

      printf("Masukkan tipe buku: ");
      fgets(type, sizeof(type), stdin);
      type[strcspn(type, "\n")] = 0;

      char line[64];
      printf("Masukkan harga buku: ");
      if (!fgets(line, sizeof(line), stdin) || sscanf(line, "%f", &price) != 1)
      {
        printf("Harga tidak valid.\n");
        wait_and_back_to_menu();
        break;
      }

      strcpy(code, generate_book_code());

      Book newBook;
      strcpy(newBook.code, code);
      strcpy(newBook.name, name);
      strcpy(newBook.type, type);
      newBook.price = price;

      add_book(newBook);

      printf("Buku berhasil ditambahkan.\n\n");
      wait_and_back_to_menu();
      break;
    }

    // View Sales Records
    case 2:
    {

      if (salesRecordCount == 0)
      {
        printf("\nTidak ada data Catatan Penjualan tersedia.\n\n");
        wait_and_back_to_menu();
        break;
      }

      printf("\nDaftar Penjualan Buku Literasi Nusantara Jaya:\n");
      printf("=================================\n");
      for (size_t i = 0; i < salesRecordCount; i++)
      {
        printf("Index            : %zu\n", i + 1);
        printf("Kode Buku        : %s\n", SALES_RECORDS[i].bookCode);
        printf("Jumlah Terjual   : %d Pcs\n", SALES_RECORDS[i].soldCount);
        printf("Total Harga      : %.0f\n", SALES_RECORDS[i].totalRevenue);
        printf("--------------------------------\n");
      }

      printf("\n");

      wait_and_back_to_menu();
      break;
    }

    // View All Books
    case 3:
    {
      if (bookCount == 0)
      {
        printf("\nTidak ada data buku tersedia.\n\n");
        wait_and_back_to_menu();
        break;
      }

      printf("\nDaftar Buku Literasi Nusantara Jaya:\n");
      printf("=================================\n");
      for (size_t i = 0; i < bookCount; i++)
      {
        printf("Index       : %zu\n", i + 1);
        printf("Kode Buku   : %s\n", BOOKS[i].code);
        printf("Nama Buku   : %s\n", BOOKS[i].name);
        printf("Tipe Buku   : %s\n", BOOKS[i].type);
        printf("Harga Buku  : %.0f\n", BOOKS[i].price);
        printf("--------------------------------\n");
      }

      printf("\n");

      wait_and_back_to_menu();
      break;
    }

    // Delete Sales Record
    case 4:
    {
      if (salesRecordCount == 0)
      {
        printf("\nTidak ada data Catatan Penjualan tersedia.\n\n");
        wait_and_back_to_menu();
        break;
      }

      printf("\nDaftar Penjualan Buku Literasi Nusantara Jaya:\n");
      printf("=================================\n");
      for (size_t i = 0; i < salesRecordCount; i++)
      {
        printf("Index            : %zu\n", i + 1);
        printf("Kode Buku        : %s\n", SALES_RECORDS[i].bookCode);
        printf("Jumlah Terjual   : %d Pcs\n", SALES_RECORDS[i].soldCount);
        printf("Total Harga      : %.0f\n", SALES_RECORDS[i].totalRevenue);
        printf("--------------------------------\n");
      }

      int idx = 0;
      if (!read_int("Masukkan index catatan penjualan yang ingin dihapus: ", &idx))
      {
        printf("Input index tidak valid.\n");
        wait_and_back_to_menu();
        break;
      }

      if (delete_sales_record_by_index((size_t)idx))
      {
        printf("Catatan Penjualan berhasil di hapus..\n\n");
      }
      else
      {
        printf("Index tidak valid. Harus antara 1 sampai %zu.\n\n", salesRecordCount);
      }

      wait_and_back_to_menu();
      break;
    }

    // Delete Book
    case 5:
    {

      if (bookCount == 0)
      {
        printf("\nTidak ada data buku tersedia.\n\n");
        wait_and_back_to_menu();
        break;
      }

      printf("\nDaftar Buku Literasi Nusantara Jaya:\n");
      printf("=================================\n");
      for (size_t i = 0; i < bookCount; i++)
      {
        printf("Index       : %zu\n", i + 1);
        printf("Kode Buku   : %s\n", BOOKS[i].code);
        printf("Nama Buku   : %s\n", BOOKS[i].name);
        printf("Tipe Buku   : %s\n", BOOKS[i].type);
        printf("Harga Buku  : %.0f\n", BOOKS[i].price);
        printf("--------------------------------\n");
      }

      int idx = 0;
      if (!read_int("Masukkan index buku yang ingin dihapus: ", &idx))
      {
        printf("Input index tidak valid.\n");
        wait_and_back_to_menu();
        break;
      }

      if (delete_book_by_index((size_t)idx))
      {
        printf("Data Buku berhasil dihapus\n\n");
      }
      else
      {
        printf("Index tidak valid. Harus antara 1 sampai %zu.\n\n", bookCount);
      }

      wait_and_back_to_menu();
      break;
    }

    // Exit
    case 6:
    {

      save_books();
      save_sales_records();

      if (BOOKS)
        free(BOOKS);
      if (SALES_RECORDS)
        free(SALES_RECORDS);

      printf("Data berhasil disimpan. Keluar dari program.\n");
      return 0;
    }

    // Input Sales Transaction
    case 7:
    {

      char bookCode[16] = "";
      int soldCount = 0;
      float totalRevenue = 0.0f;

      printf("\nDaftar Buku yang Tersedia:\n");
      printf("=================================\n");
      for (size_t i = 0; i < bookCount; i++)
      {
        printf("Kode Buku : %s | Nama Buku : %s | Tipe Buku : %s | Harga Buku: %.0f\n",
               BOOKS[i].code,
               BOOKS[i].name,
               BOOKS[i].type,
               BOOKS[i].price);
      }
      printf("--------------------------------\n");

      printf("Masukkan kode buku yang terjual: ");
      fgets(bookCode, sizeof(bookCode), stdin);
      bookCode[strcspn(bookCode, "\n")] = 0; // Remove newline character
      Book *book = get_book_by_code(bookCode);

      if (book == NULL)
      {
        printf("Buku dengan kode %s tidak ditemukan.\n\n", bookCode);
        wait_and_back_to_menu();
        break;
      }

      if (!read_int("Masukkan jumlah buku yang terjual: ", &soldCount) || soldCount <= 0)
      {
        printf("Jumlah buku tidak valid.\n");
        wait_and_back_to_menu();
        break;
      }

      totalRevenue = book->price * soldCount;

      SalesRecord newRecord;

      strcpy(newRecord.bookCode, bookCode);
      newRecord.soldCount = soldCount;
      newRecord.totalRevenue = totalRevenue;
      add_sales_record(newRecord);

      printf("Transaksi penjualan berhasil dicatat.\n\n");
      wait_and_back_to_menu();
      break;
    }

    // Sort
    case 8:
    {
      if (bookCount == 0)
      {
        printf("Tidak ada data buku untuk diurutkan.\n");
        wait_and_back_to_menu();
        break;
      }

      while (1)
      {
        clear_screen();
        print_sort_menu();
        char sortChoice = read_char_choice("Pilih menu sort (a/b/q): ");

        if (sortChoice == 'q' || sortChoice == 'Q')
        {
          clear_screen();
          break; // back to main menu
        }

        Book *tempBooks = clone_books(BOOKS, bookCount);
        if (!tempBooks)
        {
          printf("Gagal alokasi memori untuk proses sorting.\n");
          wait_and_back_to_menu();
          continue;
        }

        if (sortChoice == 'a' || sortChoice == 'A')
        {
          clear_screen();

          sort_books_by_name_bubble_asc(tempBooks, bookCount);
          printf("\nHasil urut Nama Buku (Ascending, Bubble Sort) [Preview Only]\n");
          show_books(tempBooks, bookCount);
          free(tempBooks);
          wait_and_back_to_menu();
        }
        else if (sortChoice == 'b' || sortChoice == 'B')
        {
          clear_screen();

          sort_books_by_price_selection_desc(tempBooks, bookCount);
          printf("\nHasil urut Harga Buku (Descending, Selection Sort) [Preview Only]\n");
          show_books(tempBooks, bookCount);
          free(tempBooks);
          wait_and_back_to_menu();
        }
        else
        {
          free(tempBooks);
          printf("\nPilihan sub menu tidak tersedia.\n");
          printf("Tekan tombol apapun untuk mencoba lagi...");
          press_any_key();
          // stay in submenu
        }
      }

      break;
    }

    default:
      printf("Pilihan tidak valid. Silakan coba lagi.\n\n");
      wait_and_back_to_menu();
      break;
    }
  }

  if (BOOKS)
    free(BOOKS);
  if (SALES_RECORDS)
    free(SALES_RECORDS);

  return 0;
}

// MENU
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
  printf("8. Sort Buku (berdasarkan nama dan harga)\n");
  printf("=============================================\n");
}

void print_sort_menu()
{
  printf("Sort Buku\n");
  printf("Menu yang tersedia:\n");
  printf("a. Urutkan berdasarkan Nama Buku (Ascending, Bubble Sort)\n");
  printf("b. Urutkan berdasarkan Harga Buku (Descending, Selection Sort\n");
  printf("q. Kembali ke Menu Utama\n");
  printf("=============================================\n");
}

// UI
void clear_screen()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void press_any_key()
{
#ifdef _WIN32
  getch();
#else
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO); // non-canonical, no echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}

void wait_and_back_to_menu()
{
  printf("\nTekan tombol apapun untuk kembali ke menu...");
  press_any_key();
  clear_screen();
}

// ERROR
int read_int(const char *prompt, int *outValue)
{
  char line[64];
  char extra;
  int value;

  printf("%s", prompt);
  if (!fgets(line, sizeof(line), stdin))
    return 0;
  if (sscanf(line, "%d %c", &value, &extra) != 1)
    return 0;

  *outValue = value;
  return 1;
}

char read_char_choice(const char *prompt)
{
  char line[32];
  printf("%s", prompt);
  if (!fgets(line, sizeof(line), stdin))
    return '\0';
  return (char)line[0];
}

// CURD
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
    number = atoi(lastCode + 2) + 1;
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

int delete_sales_record_by_index(size_t index)
{

  if (index < 1 || index > salesRecordCount)
  {
    return 0;
  }

  size_t deletePos = index - 1;

  for (size_t i = deletePos; i < salesRecordCount - 1; i++)
  {
    SALES_RECORDS[i] = SALES_RECORDS[i + 1];
  }

  salesRecordCount--;

  if (salesRecordCount == 0)
  {
    free(SALES_RECORDS);
    SALES_RECORDS = NULL;
  }

  else
  {
    SalesRecord *tmp = realloc(SALES_RECORDS, salesRecordCount * sizeof(SalesRecord));
    if (tmp != NULL)
    {
      SALES_RECORDS = tmp;
    }
  }

  return 1;
}

int delete_book_by_index(size_t index)
{

  if (index < 1 || index > bookCount)
  {
    return 0;
  }

  size_t deletePos = index - 1;

  for (size_t i = deletePos; i < bookCount - 1; i++)
  {
    BOOKS[i] = BOOKS[i + 1];
  }

  bookCount--;

  if (bookCount == 0)
  {
    free(BOOKS);
    BOOKS = NULL;
  }

  else
  {
    Book *tmp = realloc(BOOKS, bookCount * sizeof(Book));
    if (tmp != NULL)
    {
      BOOKS = tmp;
    }
  }

  return 1;
}

// SORT
Book *clone_books(const Book *source, size_t count)
{
  if (count == 0 || source == NULL)
    return NULL;

  Book *copy = (Book *)malloc(count * sizeof(Book));
  if (!copy)
    return NULL;

  memcpy(copy, source, count * sizeof(Book));
  return copy;
}

void show_books(const Book *arr, size_t count)
{
  if (count == 0)
  {
    printf("Tidak ada data buku.\n");
    return;
  }

  printf("\nDaftar Buku:\n");
  printf("=================================\n");
  for (size_t i = 0; i < count; i++)
  {
    printf("Index       : %zu\n", i + 1);
    printf("Kode Buku   : %s\n", arr[i].code);
    printf("Nama Buku   : %s\n", arr[i].name);
    printf("Tipe Buku   : %s\n", arr[i].type);
    printf("Harga Buku  : %.0f\n", arr[i].price);
    printf("--------------------------------\n");
  }
}

// Bubble Sort by name ASC (on provided array only)
void sort_books_by_name_bubble_asc(Book *arr, size_t count)
{
  if (!arr || count < 2)
    return;

  for (size_t i = 0; i < count - 1; i++)
  {
    int swapped = 0;
    for (size_t j = 0; j < count - i - 1; j++)
    {
      if (strcmp(arr[j].name, arr[j + 1].name) > 0)
      {
        Book temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
        swapped = 1;
      }
    }
    if (!swapped)
      break;
  }
}

// Selection Sort by price DESC (on provided array only)
void sort_books_by_price_selection_desc(Book *arr, size_t count)
{
  if (!arr || count < 2)
    return;

  for (size_t i = 0; i < count - 1; i++)
  {
    size_t maxIdx = i;
    for (size_t j = i + 1; j < count; j++)
    {
      if (arr[j].price > arr[maxIdx].price)
      {
        maxIdx = j;
      }
    }
    if (maxIdx != i)
    {
      Book temp = arr[i];
      arr[i] = arr[maxIdx];
      arr[maxIdx] = temp;
    }
  }
}
