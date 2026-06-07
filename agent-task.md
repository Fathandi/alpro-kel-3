Anda adalah seorang Senior C Developer dan Software Architect yang bertugas membuat project tugas akhir mata kuliah Dasar Pemrograman / Algoritma dan Pemrograman.

Buat project secara lengkap, runnable, dan production-quality untuk skala tugas kuliah.

# TUJUAN PROJECT

Membuat aplikasi console bernama:

**Computer Store Inventory Management System**

Sistem digunakan untuk mengelola inventaris toko komputer.

Bahasa yang digunakan:

* C (ANSI C / C99)
* Tanpa framework
* Tanpa database
* Penyimpanan menggunakan file JSON
* Berjalan di terminal / console

---

# ATURAN PENTING

Project WAJIB memenuhi seluruh requirement mata kuliah berikut:

## 1. Tipe Data

Minimal menggunakan:

```c
int
float
char[]
```

Contoh:

```c
int stock;
float price;
char name[100];
```

Harus terlihat jelas dalam source code.

---

## 2. Operasi Aritmatika

Harus digunakan pada:

* Barang masuk
* Barang keluar
* Perhitungan total aset inventaris

Contoh:

```c
stock += qty;
stock -= qty;

totalAsset += product.price * product.stock;
```

---

## 3. Percabangan

Gunakan:

```c
if
else
switch
```

Untuk:

* Menu utama
* Validasi stok
* Validasi input

---

## 4. Perulangan

Gunakan:

```c
for
while
```

Untuk:

* Menu utama
* Menampilkan data
* Searching
* Sorting
* Laporan

---

## 5. Fungsi / Prosedur

Seluruh fitur harus dipisahkan menjadi fungsi.

Contoh:

```c
void addProduct();
void updateProduct();
void deleteProduct();
void searchProduct();
void sortProducts();
```

---

## 6. Sorting

WAJIB menggunakan:

```text
Bubble Sort
```

Jangan menggunakan qsort.

Sorting harus tersedia untuk:

* Nama
* Harga
* Stok

---

## 7. Searching

WAJIB menggunakan:

```text
Linear Search
```

Jangan menggunakan Binary Search.

Searching harus tersedia untuk:

* ID
* Nama

---

## 8. Array

WAJIB menggunakan array.

Contoh:

```c
Product products[100];
```

Array digunakan sebagai penyimpanan data utama saat runtime.

---

## 9. File Handling

WAJIB menggunakan:

```c
fopen
fclose
fprintf
fscanf
fgets
```

Data harus dibaca dari file saat startup.

Data harus disimpan ke file saat terjadi perubahan.

---

## 10. Minimal 50 Baris

Project final diperkirakan 800+ baris.

---

# STUDI KASUS

Sebuah toko komputer menjual:

* Processor
* RAM
* SSD
* HDD
* VGA
* Motherboard
* Power Supply
* Monitor

Permasalahan:

* Stok dicatat manual
* Sulit mencari barang
* Sulit mengetahui stok menipis
* Data hilang ketika aplikasi ditutup

Aplikasi dibuat untuk mengatasi masalah tersebut.

---

# ENTITY PRODUCT

Gunakan struktur berikut:

```c
typedef struct
{
    int id;
    char name[100];
    char category[50];
    float price;
    int stock;
    char supplier[100];
} Product;
```

---

# FITUR WAJIB

## Dashboard

Saat startup tampil:

```text
========================
COMPUTER STORE INVENTORY
========================

Total Produk : 42
Total Stok   : 320
Nilai Barang : Rp245000000

1. Kelola Produk
2. Cari Produk
3. Laporan
0. Keluar
```

---

## Tambah Produk

User dapat menambahkan produk baru.

Field:

* ID
* Nama
* Kategori
* Harga
* Stok
* Supplier

---

## Lihat Semua Produk

Tampilkan tabel:

```text
ID    Nama            Kategori    Harga      Stok
```

---

## Update Produk

Update berdasarkan ID.

Field yang bisa diubah:

* Nama
* Kategori
* Harga
* Stok
* Supplier

---

## Hapus Produk

Hapus berdasarkan ID.

---

## Cari Produk

Cari berdasarkan:

### ID

Implementasi:

```text
Linear Search
```

### Nama

Implementasi:

```text
Linear Search
```

---

## Sorting Produk

Urutkan berdasarkan:

### Nama

Bubble Sort

### Harga

Bubble Sort

### Stok

Bubble Sort

---

## Barang Masuk

Tambah stok.

Contoh:

```text
SSD Samsung +10
```

---

## Barang Keluar

Kurangi stok.

Contoh:

```text
SSD Samsung -5
```

Harus ada validasi:

```c
if(stock < qty)
```

Agar stok tidak negatif.

---

## Laporan Stok Menipis

Tampilkan produk dengan:

```c
stock < 5
```

---

## Statistik Inventaris

Hitung:

```c
totalProduct
totalStock
totalAsset
```

---

# PENYIMPANAN DATA

WAJIB menggunakan file:

```text
products.json
```

Gunakan JSON ASLI.

Contoh:

```json
[
  {
    "id": 1,
    "name": "Kingston Fury 16GB",
    "category": "RAM",
    "price": 850000,
    "stock": 12,
    "supplier": "PT Memory Indonesia"
  }
]
```

JANGAN menggunakan format pipe (`|`) atau CSV.

---

# PARSER JSON

Buat desain adapter yang dapat ditukar.

Repository interface:

```c
typedef struct
{
    int (*load)(Product products[], int* count);
    int (*save)(Product products[], int count);
} ProductRepository;
```

Implementasikan:

```c
createManualJsonRepository()
```

Gunakan parser JSON manual berbasis:

```c
fgets
sscanf
strstr
```

Struktur kode harus memungkinkan penambahan:

```c
createCJsonRepository()
```

di masa depan tanpa mengubah business logic.

Konsepnya seperti Lego / Plug and Play.

---

# STRUKTUR FOLDER

Gunakan struktur berikut:

```text
computer-store-inventory/
│
├── src/
│   ├── main.c
│
│   ├── product.h
│   ├── product.c
│
│   ├── repository.h
│   ├── repository.c
│
│   └── products.json
│
├── README.md
├── pseudocode.md
└── flowchart.mmd
```

---

# DOKUMENTASI REQUIREMENT

SANGAT PENTING.

Setiap bagian source code yang memenuhi requirement tugas HARUS diberi komentar.

Contoh:

```c
/* =========================================
   REQUIREMENT:
   ARRAY
   ========================================= */
Product products[100];
```

Contoh:

```c
/* =========================================
   REQUIREMENT:
   SEARCHING (LINEAR SEARCH)
   ========================================= */
for(int i=0;i<productCount;i++)
{
    ...
}
```

Contoh:

```c
/* =========================================
   REQUIREMENT:
   SORTING (BUBBLE SORT)
   ========================================= */
```

Contoh:

```c
/* =========================================
   REQUIREMENT:
   FILE HANDLING
   ========================================= */
```

Contoh:

```c
/* =========================================
   REQUIREMENT:
   ARITHMETIC OPERATION
   ========================================= */
```

Dosen harus dapat menemukan seluruh requirement hanya dengan membaca komentar.

---

# README.md

Buat README lengkap yang berisi:

1. Judul Project
2. Deskripsi
3. Studi Kasus
4. Tujuan
5. Struktur Folder
6. Cara Compile
7. Cara Menjalankan
8. Penjelasan Fitur
9. Mapping Requirement Mata Kuliah ke Source Code
10. Screenshot Placeholder
11. Kesimpulan

Tambahkan tabel:

| Requirement | File | Fungsi |
| ----------- | ---- | ------ |

---

# pseudocode.md

Buat pseudocode lengkap:

* Main Menu
* Tambah Produk
* Update Produk
* Delete Produk
* Search Produk
* Sort Produk
* Stock In
* Stock Out
* Load JSON
* Save JSON

---

# flowchart.mmd

Buat flowchart menggunakan Mermaid.

Gunakan syntax Mermaid yang valid.

Harus mencakup:

* Start
* Load JSON
* Dashboard
* Menu
* Semua fitur
* Save JSON
* Exit

---

# KUALITAS KODE

Kode harus:

* Bisa di-compile
* Tidak pseudo code
* Tidak ada TODO
* Tidak ada placeholder
* Tidak ada fungsi kosong
* Tidak ada kode yang hilang
* Tidak ada dependency eksternal
* Tidak menggunakan library JSON pihak ketiga

Semua fitur harus benar-benar berfungsi.

Output akhir harus berupa seluruh isi file yang diperlukan untuk project secara lengkap.
