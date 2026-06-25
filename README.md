# COMPUTER STORE INVENTORY — PRESENTATION MODERATOR (EXPERT BRIEF)

Anda sedang melihat sistem manajemen inventaris toko komputer berbasis **C (C99)**.
Bukan sekadar program yang jalan—ini dirancang dengan pola **Hexagonal Architecture (Ports & Adapters)** agar logika bisnis tetap bersih, storage bisa diganti, dan UI bisa bertukar tanpa merusak inti sistem.

---

## 1) Tujuan Sistem (Apa yang dipimpin oleh program ini?)
Sistem ini mengelola data **produk** yang tersimpan dalam memori, lalu dipersistensikan ke file **JSON**.
Operasi yang tersedia via menu:
- Tambah produk (Add)
- Lihat semua produk
- Update produk
- Hapus produk
- Cari produk (by ID / by Nama)
- Sort produk (by Name / Price / Stock)
- Stock In (tambah stok)
- Stock Out (kurangi stok dengan validasi stok cukup)
- Laporan stok menipis (< 5)
- Statistik inventaris

Setiap perubahan data akan disimpan ulang ke `data/products.json` sebelum program keluar (dan pada beberapa aksi juga disimpan segera).

---

## 2) “Medan perang” Arsitektur: Hexagonal (Ports & Adapters)
Pola ini membagi sistem menjadi 4 peran utama:

### A. Domain Layer — `src/domain/product.c` & `src/domain/product.h`
**Ini jantungnya.** Domain bertanggung jawab pada struktur data & logika bisnis.
Pada implementasi Anda saat ini, domain memuat:
- tipe `Product`
- penyimpanan global:
  - `glb_arr_products[PRODUCT_MAX]`
  - `glb_int_product_count`
- global repository instance `glb_repository`

Intinya: domain tidak boleh bergantung pada detail UI atau cara storage.

### B. Ports Layer — Kontrak
Ports adalah “kontrak komando”. Domain/main memanggil lewat kontrak, bukan detail implementasinya.

1) `src/ports/repository.h`
- Mendefinisikan `ProductRepository` dengan **function pointer**:
  - `load(Product products[], int* count)`
  - `save(Product products[], int count)`

2) `src/ports/presentation.h`
- Mendefinisikan `PresentationAdapter` dengan function pointer untuk semua operasi UI:
  - `displayMenu`, `addProduct`, `displayAllProducts`, `updateProduct`, `deleteProduct`,
  - `searchProduct`, `sortProducts`, `stockIn`, `stockOut`, `lowStockReport`, `inventoryStats`
- Juga menyediakan `clearBuffer()` untuk manajemen input console.

### C. Adapters Layer — Implementasi nyata
Adapter adalah “pasukan eksekutor” yang benar-benar melakukan I/O.

1) Repository Adapter: `src/adapters/repository_cjson.c`
- Mengimplementasikan `load` dan `save` menggunakan library **cJSON**.
- Storage-nya adalah file:
  - `computer-store-inventory/data/products.json`

2) Presentation Adapter: `src/adapters/presentation_console.c`
- Mengimplementasikan semua operasi UI via `printf/scanf/fgets`.
- Memegang semua detail input/output yang berhubungan dengan terminal.

### D. Composition Root / Router — `src/main.c`
`main.c` adalah komandannya yang mengikat semuanya:
- Dependency Injection:
  - `glb_repository = createCJsonRepository();`
  - presentation adapter diikat lewat global `glb_presentation`.
- Event loop:
  - tampilkan menu (`glb_presentation.displayMenu()`)
  - baca input pilihan
  - routing ke handler menu dengan `switch`
  - saat exit:
    - `glb_repository.save(...)`
    - program terminasi

Jika di medan perang ada perubahan storage atau UI, **yang diganti cukup adapter/factory**, bukan memaksa domain berubah.

---

## 3) Alur Eksekusi Program (Narasi moderator)
Saat program mulai:
1) `main()` membuat repository adapter cJSON.
2) Program memanggil `glb_repository.load(...)`:
   - membaca `data/products.json`
   - parse JSON menjadi array `Product`
   - update `glb_int_product_count`
3) Masuk ke loop menu:
   - menu tampil beserta statistik (total produk, total stok, total nilai aset)
   - user memilih aksi dari 0–10
   - `switch` menjalankan function pointer di `glb_presentation`
4) Saat user pilih `0`:
   - program menyimpan ulang seluruh array `Product` ke JSON
   - exit

---

## 4) Semua aspek implementasi (tanpa terkecuali)

### 4.1 Struktur data `Product`
`Product` berisi:
- `id` (int) : unik per produk (di UI Anda belum ada check keunikan, tapi field memang dipakai sebagai kunci pencarian)
- `name[100]`
- `category[50]`
- `price` (float)
- `stock` (int)
- `supplier[100]`

### 4.2 Batas kapasitas — `PRODUCT_MAX`
- Array produk maksimal `PRODUCT_MAX` (100).
- Saat add: bila count sudah penuh, program menolak penambahan.
- Saat load: jika JSON lebih panjang dari kapasitas, proses berhenti di batas array.

### 4.3 Input Handling Console (yang harus Anda sebut saat presentasi)
Di console adapter:
- Angka dibaca menggunakan `scanf`.
- String dibaca menggunakan `fgets`.
- Ada problem klasik: `scanf` sering meninggalkan newline sisa.
- Solusinya: `clearBuffer()` yang membersihkan buffer sampai `\n` atau EOF.
- Setelah `fgets`, newline dibuang dengan `strcspn(str, "\n")`.

Untuk seorang yang awam sekalipun: ini penting karena kalau tidak, input string berikutnya bisa terbaca kosong.

### 4.4 Sorting (bubble sort)
Sort dilakukan menggunakan nested loop ala bubble sort.
Pilihan user menentukan kriteria:
- 1: berdasarkan `name` (menggunakan `strcmp`)
- 2: berdasarkan `price`
- 3: berdasarkan `stock`

Setelah sorting, program menyimpan ulang data.

### 4.5 Search (ID atau substring nama)
- Mode ID: compare langsung `product[i].id == input_id`
- Mode Nama: gunakan `strstr(product[i].name, query)` untuk pencarian substring.

### 4.6 Stock In / Stock Out
- Stock In:
  - cari produk berdasarkan ID
  - tambahkan `qty` ke `product.stock`
- Stock Out:
  - validasi stok mencukupi:
    - jika `stock < qty` → gagal
    - jika cukup → kurangi

### 4.7 Repository JSON menggunakan cJSON
Di `repository_cjson.c`:
- `readFile()` membaca file JSON menjadi string dinamis.
- `cJSON_Parse(json_string)` membuat tree JSON.
- `cJSON_IsArray(root)` memastikan struktur adalah array.
- Iterasi array menggunakan `cJSON_ArrayForEach` (yang berjalan lewat linked structure `child/next`).
- Field dipetakan ke `Product`:
  - `id`, `name`, `category`, `price`, `stock`, `supplier`
- Saat save:
  - dibuat `cJSON_CreateArray`
  - setiap produk jadi `cJSON_CreateObject`
  - field ditambahkan ke object
  - `cJSON_Print(root)` menghasilkan string JSON
  - ditulis dengan mode `w` (overwrite)

---

## 5) Cara menjalankan (manual compile)
Build menggunakan instruksi GCC manual yang disediakan di:
- `compile_instructions.txt`

Output build:
- `computer-store-inventory/output/main.exe`

---
