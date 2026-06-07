/*
================================================================================
PRODUCT DOMAIN LAYER - HEADER
================================================================================
File: src/domain/product.h

Deskripsi:
  Mendefinisikan struktur data Product dan deklarasi variabel global untuk
  penyimpanan data inventaris. File ini adalah interface antara layer domain
  dan layer lainnya.

Konten Utama:
  - Definisi typedef struct Product
  - Deklarasi variabel global untuk array Products dan counter
  - Inklusif dengan library eksternal (presentation port)

Perhatian:
  - Semua definisi struktur data harus berada di file ini saja
  - Variabel global dideklarasikan dengan 'extern' untuk dipakai di file lain
  - Header guard mencegah multiple-include

================================================================================
*/

#ifndef PRODUCT_H
#define PRODUCT_H

/* =========================================
   PRODUCT STRUCTURE DEFINITION
   Tipe data untuk menyimpan informasi produk
   ========================================= */

typedef struct {
    int id;              /* Unique identifier untuk setiap produk */
    char name[100];      /* Nama produk (max 100 karakter) */
    char category[50];   /* Kategori produk (max 50 karakter) */
    float price;         /* Harga produk dalam Rupiah */
    int stock;           /* Jumlah stok barang yang tersedia */
    char supplier[100];  /* Nama supplier/pemasok (max 100 karakter) */
} Product;

/* =========================================
   GLOBAL VARIABLES DECLARATION
   Array penyimpanan produk dan counter
   ========================================= */

/* Array penyimpanan data produk - kapasitas max 100 produk */
extern Product glb_arr_products[100];

/* Tracking jumlah produk yang saat ini tersimpan dalam array */
extern int glb_int_product_count;

#endif // PRODUCT_H
