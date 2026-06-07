/*
================================================================================
PRODUCT DOMAIN LAYER - IMPLEMENTATION
================================================================================
File: src/domain/product.c

Deskripsi:
  Implementasi logika bisnis untuk domain Product. File ini menangani semua
  operasi CRUD, searching, sorting, dan manipulasi stok inventaris.
  
  PENTING: File ini TIDAK memiliki operasi I/O (printf/scanf). Semua I/O
  dipindahkan ke presentation adapter untuk menjaga separation of concerns.

Operasi yang Disediakan:
  - Penyimpanan dan inisialisasi data produk global
  - Interface dengan repository adapter untuk persistensi data
  - Menyediakan kontrak port untuk presentation layer

Arsitektur:
  Mengikuti prinsip Hexagonal Architecture - file ini adalah pure domain logic
  tanpa ketergantungan pada UI atau storage implementation details.

================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "../ports/repository.h"

/* =========================================
   GLOBAL DATA STORAGE
   ========================================= */

/* Array global untuk menyimpan semua data produk (max 100 produk) */
Product glb_arr_products[100];

/* Counter global untuk tracking berapa banyak produk yang tersimpan */
int glb_int_product_count = 0;

/* Repository adapter - akan diinjeksi oleh main.c sesuai hexagonal architecture */
ProductRepository glb_repository;


