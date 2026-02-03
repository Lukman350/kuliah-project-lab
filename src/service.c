#include <stdio.h>
#include <string.h>
#include "model.h"
#include "repository.h"
#include "service.h"
#include "utils.h"

void initialize_data()
{
  get_all_books(&BOOKS, &BOOK_COUNT);
  get_all_sales_records(&SALES_RECORDS, &SALES_RECORD_COUNT);
}

// Book Service
void create_book(const char *name, const char *type, float price)
{
  Book newBook;
  strcpy(newBook.code, generate_book_code());
  strcpy(newBook.name, name);
  strcpy(newBook.type, type);
  newBook.price = price;

  add_book(newBook);

  printf("Buku berhasil ditambahkan.\n\n");
}

void view_books()
{
  if (BOOK_COUNT == 0)
  {
    printf("\nTidak ada data buku tersedia.\n\n");
    return;
  }

  printf("\nDaftar Buku Literasi Nusantara Jaya:\n");
  printf("=================================\n");
  for (size_t i = 0; i < BOOK_COUNT; i++)
  {
    printf("Kode Buku   : %s\n", BOOKS[i].code);
    printf("Nama Buku   : %s\n", BOOKS[i].name);
    printf("Tipe Buku   : %s\n", BOOKS[i].type);
    printf("Harga Buku  : %.0f\n", BOOKS[i].price);
    printf("--------------------------------\n");
  }

  printf("\n");
}

void delete_book(const int index)
{
  if (index < 0 || (size_t)index >= BOOK_COUNT)
  {
    printf("Index buku tidak valid.\n\n");
    return;
  }

  int deleted = delete_book_by_index(index);
  if (deleted)
    printf("Data Successfully delete..\n\n");
  else
    printf("Buku dengan indeks %d tidak ditemukan\n\n", index);
}

void save_all_books()
{
  save_books();
}

// SalesRecord Service
void create_sales_record(const char *bookCode, int soldCount)
{
  Book *book = get_book_by_code(bookCode);

  if (book == NULL)
  {
    printf("Buku dengan kode %s tidak ditemukan.\n\n", bookCode);
    return;
  }

  float totalRevenue = book->price * soldCount;

  // SalesRecord *existingRecord = get_sales_record_by_book_code(bookCode);
  // if (existingRecord != NULL)
  // {
  //   existingRecord->soldCount += soldCount;
  //   existingRecord->totalRevenue += totalRevenue;
  //   printf("Transaksi penjualan berhasil diperbarui.\n\n");
  //   return;
  // }

  SalesRecord newRecord;
  strcpy(newRecord.bookCode, bookCode);
  newRecord.soldCount = soldCount;
  newRecord.totalRevenue = totalRevenue;
  add_sales_record(newRecord);

  printf("Transaksi penjualan berhasil dicatat.\n\n");
}

void view_sales_records()
{
  if (SALES_RECORD_COUNT == 0)
  {
    printf("\nTidak ada data penjualan tersedia.\n\n");
    return;
  }

  printf("\nDaftar Transaksi Penjualan:\n");
  printf("=================================\n");
  for (size_t i = 0; i < SALES_RECORD_COUNT; i++)
  {
    printf("Kode Buku       : %s\n", SALES_RECORDS[i].bookCode);
    printf("Jumlah Terjual  : %d\n", SALES_RECORDS[i].soldCount);
    printf("Total Pendapatan: %.0f\n", SALES_RECORDS[i].totalRevenue);
    printf("--------------------------------\n");
  }

  printf("\n");
}

void delete_sales_record(const char *bookCode)
{
  SalesRecord *record = get_sales_record_by_book_code(bookCode);
  if (record == NULL)
  {
    printf("Transaksi penjualan untuk kode buku %s tidak ditemukan.\n\n", bookCode);
    return;
  }

  delete_sales_record_by_book_code(bookCode);
  printf("Transaksi penjualan untuk kode buku %s berhasil dihapus.\n\n", bookCode);
}

void save_all_sales_records()
{
  save_sales_records();
}