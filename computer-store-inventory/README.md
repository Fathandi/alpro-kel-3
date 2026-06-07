# Computer Store Inventory Management System

## Deskripsi
Aplikasi console berbasis bahasa C untuk mengelola inventaris toko komputer. Aplikasi ini dibuat tanpa menggunakan framework maupun database, melainkan menggunakan file JSON secara manual sebagai media penyimpanan data (file handling).

## Studi Kasus
Sebuah toko komputer yang menjual berbagai macam perangkat keras (Processor, RAM, SSD, HDD, VGA, dll). Permasalahan yang dihadapi adalah pencatatan stok yang manual sehingga rentan hilang, sulit untuk mencari barang, dan sulit untuk mengetahui stok yang menipis. Aplikasi ini hadir untuk digitalisasi pendataan inventaris dengan fitur persistensi data (JSON).

## Tujuan
Project ini merupakan tugas akhir mata kuliah Dasar Pemrograman / Algoritma dan Pemrograman yang ditujukan untuk mengimplementasikan berbagai materi dari perkuliahan seperti tipe data, operasi aritmatika, percabangan, perulangan, fungsi, sorting, searching, array, dan file handling.

## Tech Stack & Arsitektur
- **Bahasa Pemrograman**: C (C99)
- **Arsitektur**: Hexagonal Architecture (Ports & Adapters)
- **Format Data**: JSON
- **Library Eksternal**: cJSON (digunakan sebagai Adapter penyedia JSON Parsing)

## Struktur Folder & Penjelasan File
```text
computer-store-inventory/
│
├── data/
│   └── products.json        (File database lokal berbentuk JSON tempat menyimpan data)
├── src/
│   ├── main.c               (Source code utama, UI menu dan Injeksi Dependensi)
│   ├── domain/
│   │   ├── product.h        (Header file: Deklarasi struktur `Product` dan fungsi logika bisnis)
│   │   └── product.c        (Source file: Implementasi logika bisnis seperti CRUD, Sort, Search)
│   ├── ports/
│   │   └── repository.h     (Header file: Interface / Port pola Hexagonal untuk akses data)
│   ├── adapters/
│   │   └── repository_cjson.c (Source file: Implementasi Adapter JSON menggunakan library cJSON)
│   └── lib/
│       ├── cJSON.h          (Header file: Deklarasi fungsi library open-source cJSON)
│       └── cJSON.c          (Source file: Implementasi library cJSON)
│
├── .gitignore               (Daftar file yang tidak dimasukkan ke version control git)
├── README.md                (Dokumentasi ini)
├── pseudocode.md            (Algoritma / Pseudocode program)
└── flowchart.mmd            (Flowchart eksekusi program)
```

### Kenapa Menggunakan Ekstensi `.c` dan `.h`?
Dalam bahasa C, praktik terbaik adalah memecah kode menjadi dua jenis file untuk memisahkan **Deklarasi** dan **Implementasi**:
- **`.h` (Header File)**: Berfungsi sebagai *interface* (antarmuka) publik. File ini hanya berisi deklarasi (judul fungsi, *struct*, konstanta, variabel global). Tujuannya agar file atau modul lain bisa mengetahui apa saja fitur yang tersedia tanpa perlu peduli dengan detail cara kerjanya.
- **`.c` (Source File)**: Berisi kode implementasi atau logika pemrograman sesungguhnya dari fungsi-fungsi yang telah dideklarasikan di dalam file `.h`.
Pemecahan ini membuat kode jauh lebih modular, terstruktur, mudah dipelihara (maintainability), dan juga dapat mencegah masalah konflik definisi (*multiple definition error*) saat proses kompilasi (*linking*).

## Cara Compile
Gunakan compiler C standard (misalnya GCC). Karena kita memecah file ke dalam beberapa struktur folder (Domain, Adapter, Lib), maka semua file `.c` harus ikut disertakan saat kompilasi:
```bash
cd computer-store-inventory/src
gcc main.c domain/product.c adapters/repository_cjson.c lib/cJSON.c -o inventory_app
```

## Cara Menjalankan
Setelah di-compile, jalankan executable-nya:
```bash
# Windows
inventory_app.exe

# Linux / Mac
./inventory_app
```

## Penjelasan Fitur
1. **Tambah Produk**: Menambahkan barang baru beserta data ID, Nama, Kategori, Harga, Stok, dan Supplier.
2. **Lihat Semua Produk**: Menampilkan daftar seluruh barang dalam format tabel.
3. **Update Produk**: Mengubah informasi detail barang berdasarkan ID.
4. **Hapus Produk**: Menghapus data barang dari memori dan file berdasarkan ID.
5. **Cari Produk**: Pencarian barang secara linear (Linear Search) dengan ID atau potongan string pada nama barang.
6. **Sort Produk**: Mengurutkan barang menggunakan Bubble Sort, dapat dipilih berdasarkan Nama, Harga, atau Stok.
7. **Barang Masuk**: Menambahkan jumlah stok yang ada.
8. **Barang Keluar**: Mengurangi jumlah stok yang ada, dilengkapi validasi jika stok kurang.
9. **Laporan Stok Menipis**: Menampilkan barang-barang yang jumlah stoknya di bawah 5.
10. **Statistik Inventaris**: Mengkalkulasi nilai aset dari rumus total harga dikali stok.

## Mapping Requirement Mata Kuliah ke Source Code

| Requirement | File | Keterangan / Fungsi Implementasi |
| ----------- | ---- | -------------------------------- |
| Minimal 3 Tipe Data | `domain/product.h` | Terdapat `int`, `float`, dan array `char` pada struct `Product`. |
| Operasi Aritmatika | `domain/product.c` | Penambahan/Pengurangan stok di `stockIn()` / `stockOut()`, hitung nilai aset. |
| Percabangan | `main.c` & `product.c` | Terdapat `if/else` dan `switch` untuk memilih menu dan validasi. |
| Perulangan | `main.c` & `product.c` | Penggunaan `while(1)` untuk menu utama dan `for` untuk iterasi data. |
| Fungsi / Prosedur | `domain/product.c` | Modularisasi kode ke berbagai prosedur seperti `addProduct()`, `searchProduct()`, dll. |
| Metode Pengurutan | `domain/product.c` | Menggunakan *Bubble Sort* di fungsi `sortProducts()`. |
| Metode Pencarian | `domain/product.c` | Menggunakan *Linear Search* di fungsi `searchProduct()`. |
| Array | `domain/product.h` | Deklarasi `Product glb_arr_products[100]` sebagai penyimpanan runtime. |
| File Handling | `adapters/repository_cjson.c`| Menggunakan `fopen()`, `fprintf()`, `fread()`, `fclose()` untuk file JSON. |
| 50 Baris Kode | Keseluruhan file | Total kode keseluruhan dari domain, main, dan adapter berjumlah lebih dari 500 baris. |

## Desain Pola Arsitektur (Hexagonal)
Sesuai arahan tugas, program menerapkan desain seperti *Hexagonal Architecture* / *Ports and Adapters* untuk *JSON Parser*. Hal ini dibuktikan dari adanya `struct ProductRepository` (terletak di `ports/repository.h`) yang berisi *function pointer* `load` dan `save`. Hal ini membuat sistem *plug and play*; karena Anda menggunakan cJSON, maka dibuat adapter baru (`createCJsonRepository()` di `adapters/repository_cjson.c`) yang disambungkan ke aplikasi utama lewat `main.c`.

## Kesimpulan
Aplikasi telah memenuhi seluruh kriteria dan rubrik penilaian mata kuliah dengan menyatukan implementasi Array, Searching, Sorting, File Handling, serta konsep modular programming dalam bahasa C.
