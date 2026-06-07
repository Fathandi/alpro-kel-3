#ifndef PRESENTATION_H
#define PRESENTATION_H

#include "../domain/product.h"

/*
================================================================================
PRESENTATION PORT - INTERFACE DEFINITION
================================================================================
File: src/ports/presentation.h

Deskripsi:
  Port mendefinisikan kontrak untuk user interface tanpa mengikat domain
  ke implementasi konkret UI. Menggunakan struct dengan function pointer
  untuk polimorfisme di C.

Pola Hexagonal Architecture:
  - Port = Interface/Kontrak yang domain/main butuhkan untuk UI
  - Adapter = Implementasi konkret (mis: console adapter)
  - Mudah ganti adapter: console → GUI → Web tanpa ubah domain

Keuntungan:
  - Domain tetap pure logic, tidak perlu tahu tentang UI
  - Presentation logic terisolasi di adapter
  - Testable tanpa perlu interaksi I/O

Function Pointer Catalog:
  Setiap fungsi di struct mewakili satu operasi UI:
  - displayMenu: tampilkan menu utama
  - addProduct: tampilkan form & terima input untuk tambah produk
  - displayAllProducts: tampilkan tabel semua produk
  - updateProduct: tampilkan form & terima input untuk update
  - deleteProduct: tampilkan konfirmasi & terima input untuk delete
  - searchProduct: tampilkan form pencarian & hasil
  - sortProducts: tampilkan pilihan sort & hasil
  - stockIn: tampilkan form penambahan stok
  - stockOut: tampilkan form pengurangan stok
  - lowStockReport: tampilkan laporan produk stok menipis
  - inventoryStats: tampilkan statistik inventaris

================================================================================
*/

/* =========================================
   PRESENTATION ADAPTER - FUNCTION POINTERS
   Abstraksi untuk UI/presentation layer
   ========================================= */

typedef struct {
    void (*displayMenu)();           /* Tampilkan menu utama dengan statistik */
    void (*addProduct)();            /* Form input untuk produk baru */
    void (*displayAllProducts)();    /* Tabel daftar semua produk */
    void (*updateProduct)();         /* Form input untuk update produk */
    void (*deleteProduct)();         /* Form konfirmasi dan input delete */
    void (*searchProduct)();         /* Form pencarian produk */
    void (*sortProducts)();          /* Form pilihan sorting */
    void (*stockIn)();               /* Form penambahan stok barang */
    void (*stockOut)();              /* Form pengurangan stok barang */
    void (*lowStockReport)();        /* Laporan produk stok < 5 */
    void (*inventoryStats)();        /* Statistik lengkap inventaris */
} PresentationAdapter;

/* Global Presentation Adapter Instance yang akan diinjeksi oleh adapter konkret
   Mengikuti Dependency Injection pattern di Hexagonal Architecture */
extern PresentationAdapter glb_presentation;

/* =========================================
   UTILITY FUNCTIONS
   ========================================= */

/* Membersihkan input buffer setelah scanf()
   
   Fungsi ini penting untuk menghindari bug saat scanf diikuti fgets:
   - scanf hanya membaca input angka, meninggalkan newline di buffer
   - fgets sebelumnya akan langsung membaca newline sisa
   - clearBuffer() menghapus sisa newline/input sampai EOF
*/
void clearBuffer();

#endif // PRESENTATION_H
