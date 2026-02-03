#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "service.h"

void print_menu();

int main()
{
  int choice;

  initialize_data();

  while (1)
  {
    print_menu();
    scanf("%d", &choice);
    getchar(); // Consume newline character

    switch (choice)
    {
    case 1: // Add Book
    {
      char name[64] = "", type[32] = "";
      float price = 0.0f;
      printf("Masukkan nama buku: ");
      fgets(name, sizeof(name), stdin);
      name[strcspn(name, "\n")] = 0; // Remove newline character
      printf("Masukkan tipe buku: ");
      fgets(type, sizeof(type), stdin);
      type[strcspn(type, "\n")] = 0;
      printf("Masukkan harga buku: ");
      scanf("%f", &price);

      create_book(name, type, price);

      getchar();
      break;
    }
    case 2: // View Sales Records
    {
      view_sales_records();

      getchar();
      break;
    }
    case 3: // View All Books
    {
      view_books();

      getchar();
      break;
    }
    case 4: // Delete Sales Record
    {
      char bookCode[16] = "";
      printf("Masukkan kode buku untuk menghapus catatan penjualan: ");
      fgets(bookCode, sizeof(bookCode), stdin);
      bookCode[strcspn(bookCode, "\n")] = 0; // Remove newline character
      delete_sales_record(bookCode);

      getchar();
      break;
    }
    case 5: // Delete Book
    {
      int index;
      printf("Masukkan indeks buku yang akan dihapus: ");
      scanf("%d", &index);

      delete_book(index);

      getchar();
      break;
    }
    case 6: // Exit
    {
      save_all_books();
      save_all_sales_records();
      printf("Data berhasil disimpan. Keluar dari program.\n");
      return 0;
    }
    case 7: // Input Sales Transaction
    {
      char bookCode[16] = "";
      int soldCount = 0;

      printf("Masukkan kode buku yang terjual: ");
      fgets(bookCode, sizeof(bookCode), stdin);
      bookCode[strcspn(bookCode, "\n")] = 0; // Remove newline character
      printf("Masukkan jumlah buku yang terjual: ");
      scanf("%d", &soldCount);

      create_sales_record(bookCode, soldCount);

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
