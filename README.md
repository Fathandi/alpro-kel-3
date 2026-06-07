# Computer Store Inventory Management System (C99)

## Deskripsi Singkat
Aplikasi console berbasis **bahasa C (C99)** untuk mengelola inventaris toko komputer.
- Tidak memakai framework.
- Tidak memakai database server.
- Persistensi data menggunakan **file JSON** (file handling).

## Arsitektur: Hexagonal (Ports & Adapters)
Aplikasi dibagi menjadi komponen:
- **Domain**: aturan bisnis dan struktur data (`src/domain`).
- **Ports**: kontrak akses data (`src/ports`).
- **Adapters**: implementasi akses data, pada project ini adapter JSON (`src/adapters`).
- **Composition root/UI**: `src/main.c` yang melakukan *dependency injection* (memilih adapter konkret).

Dengan pola ini, `main.c` tidak “terikat” ke format penyimpanan tertentu.

## Struktur Folder
```text
computer-store-inventory/
  data/
    products.json
  src/
    main.c
    domain/
      product.h
      product.c
    ports/
      repository.h
    adapters/
      repository_cjson.c
    lib/
      cJSON.c
      cJSON.h
  README.md
  pseudocode.md
  flowchart.mmd
```

---

## Ringkasan Fungsi File (yang ada di repository)
Agar reviewer cepat paham, berikut peran file-file utama (sesuai struktur project):

- **`computer-store-inventory/src/main.c`**
  - UI menu (pilihan 1..10).
  - Tempat *dependency injection* pada Hexagonal Architecture.
  - Memanggil port lewat function pointer: `glb_repository.load(...)` dan `glb_repository.save(...)`.

- **`computer-store-inventory/src/domain/product.h`**
  - Deklarasi tipe data `Product` (`typedef struct ...`).
  - Deklarasi variabel global dengan `extern`.
  - Deklarasi fungsi-fungsi yang dipakai UI/logic.

- **`computer-store-inventory/src/domain/product.c`**
  - Implementasi logika bisnis inventaris:
    CRUD, searching (linear), sorting (bubble sort), dan operasi stok.

- **`computer-store-inventory/src/ports/repository.h`**
  - **Port**: kontrak akses data.
  - Berisi `typedef struct ProductRepository` yang menggunakan **function pointer** (`load` dan `save`).

- **`computer-store-inventory/src/adapters/repository_cjson.c`**
  - **Adapter** implementasi untuk menyimpan/memuat data JSON.
  - `cjsonLoad`: baca `data/products.json` → parse → isi array `Product`.
  - `cjsonSave`: ubah array `Product` → buat JSON → tulis ke file.

- **`computer-store-inventory/src/lib/cJSON.h` dan `cJSON.c`**
  - Source vendor library **cJSON**.
  - `cJSON.h` berisi definisi tipe `cJSON` dan deklarasi fungsi API.
  - `cJSON.c` adalah implementasi API-nya.

- **`computer-store-inventory/data/products.json`**
  - “Database lokal” berbentuk JSON (persistensi data).

- **`computer-store-inventory/output/` dan executable hasil build (`main.exe`, `inventory_app`)**
  - Ini adalah **artefak kompilasi** (bukan source code).
  - Karena bukan bagian dari logika, file ini sebaiknya diabaikan oleh git melalui `.gitignore`.

---

## Kenapa ada `main.exe` / `inventory_app` walau ada `src/main.c`?
`src/main.c` adalah **kode sumber (teks C)** yang nantinya dikompilasi.
Sedangkan `main.exe` / `inventory_app` adalah **hasil kompilasi** (binary/eksekusi).
Jadi keberadaan executable diperlukan agar program bisa dijalankan, tetapi tidak diperlukan untuk kompilasi ulang di repo (source code-nya yang penting). 


---

## A. Penjelasan sintaks C yang tidak umum (tetapi dipakai di kode)
Bagian ini fokus pada “sintaks/konsep C yang sering terasa rumit” namun justru penting untuk memahami project.

### 1) Header guard `#ifndef/#define/#endif`
Di file header `.h` terdapat pola:
```c
#ifndef PRODUCT_H
#define PRODUCT_H
...
#endif
```
**Kenapa penting?**
Saat beberapa file `#include` header yang sama, tanpa guard header isinya bisa dideklarasikan ulang dan memicu error saat kompilasi.

Di project, header guard juga digunakan pada library `cJSON.h` (mis. `#ifndef cJSON__h`).

### 2) `typedef struct { ... } Product;`
Contoh dari `src/domain/product.h`:
```c
typedef struct {
    int id;
    char name[100];
    char category[50];
    float price;
    int stock;
    char supplier[100];
} Product;
```
**Kenapa pakai `typedef`?**
Supaya pemakaian tipe jadi lebih ringkas (`Product`), bukan `struct { ... }` berulang.

### 3) `extern` (deklarasi variabel global, bukan definisi)
Di `product.h` terdapat deklarasi:
```c
extern Product glb_arr_products[100];
extern int glb_int_product_count;
```
Lalu definisinya berada di `product.c`:
```c
Product glb_arr_products[100];
int glb_int_product_count = 0;
```
**Aturan penting C:**
- `extern` di header = *deklarasi*.
- variabel harus *didefinisikan* di satu `.c` supaya linker punya alamatnya.

### 4) Function pointer untuk “polimorfisme” di C
Port (kontrak) di `src/ports/repository.h` menggunakan function pointer:
```c
typedef struct {
    int (*load)(Product products[], int* count);
    int (*save)(Product products[], int count);
} ProductRepository;
```
Bacanya:
- `load` adalah pointer fungsi.
- `save` adalah pointer fungsi.

Di `src/main.c`, adapter disuntikkan:
```c
glb_repository = createCJsonRepository();
glb_repository.load(glb_arr_products, &glb_int_product_count);
```
Terlihat “tidak umum”, tapi ini pola umum untuk hexagonal architecture di C.

### 5) `static` helper: fungsi hanya untuk file itu
Di adapter JSON ada:
```c
static char* readFile(const char* filename) { ... }
```
`static` membatasi *linkage* fungsi agar tidak terlihat dari file lain (mencegah bentrok simbol).

### 6) Pointer & array di parameter: `Product products[]`
Signature:
```c
int cjsonLoad(Product products[], int* count)
```
Di C, `Product products[]` pada parameter sebenarnya setara dengan:
- `Product* products`

Elemen diakses memakai indeks:
```c
products[i].id
products[i].stock
```

### 7) `int* count` (pass by reference “versi C”)
C selalu pass-by-value.
Untuk mengubah nilai variabel milik pemanggil, dipakai pointer:
```c
*count = lcl_int_index;
```

### 8) `scanf` vs `fgets`, plus `clearBuffer()`
Pada menu:
- `scanf("%d", &lcl_int_choice)` membaca angka.
- setelah `scanf`, newline bisa tersisa.
- `clearBuffer()` menghapus sisa input sampai newline/EOF.

Untuk string (nama/kategori/supplier):
- dipakai `fgets(...)` karena lebih aman untuk input string.
- newline dihapus dengan `strcspn(str, "\n")`.

### 9) cJSON parsing & serialisasi (flow yang benar-benar dipakai)
Adapter `src/adapters/repository_cjson.c` melakukan:
1) `readFile()` → menghasilkan `char* json_string`.
2) `cJSON_Parse(json_string)` → menghasilkan `cJSON* root`.
3) Validasi tipe:
   - `cJSON_IsArray(root)`
4) Iterasi array dengan macro:
   - `cJSON_ArrayForEach(item, json)`
5) Ambil field per item menggunakan:
   - `cJSON_GetObjectItemCaseSensitive(item, "id")`
6) Validasi tipe data:
   - `cJSON_IsNumber(...)` / `cJSON_IsString(...)`
7) Isi struct `Product`.
8) Saat simpan:
   - buat array dan object baru (`cJSON_CreateArray`, `cJSON_CreateObject`)
   - tambah field (`cJSON_AddNumberToObject`, `cJSON_AddStringToObject`)
   - `cJSON_Print(root)` lalu tulis ke file.

**Macro cJSON yang terlihat rumit**
Contoh `cJSON_ArrayForEach` (di `cJSON.h`):
```c
#define cJSON_ArrayForEach(element, array) \
    for(element = (array != NULL) ? (array)->child : NULL; \
        element != NULL; \
        element = element->next)
```
Artinya iterasi dilakukan lewat linked structure `child/next` internal cJSON.

---

## B. Kesesuaian proses penulisan kode dengan aturan (yang relevan ke project)
Checklist yang sesuai praktik C untuk project ini:
1) Deklarasi di `.h`, implementasi di `.c`.
2) Header guard agar `#include` aman.
3) `extern` hanya untuk deklarasi global di header.
4) `static` dipakai untuk helper internal.
5) Port dan adapter dipisahkan lewat function pointer.
6) Integrasi library cJSON memakai API dari `cJSON.h`.
7) Kontrak input-output jelas:
   - `load(..., int* count)` mengubah `count` via pointer.
   - `save(..., int count)` menggunakan `count` untuk iterasi.

---

## C. Dari mana sumber library cJSON didapat? Siapa provider?
Di project ini, cJSON **tidak dipasang via package manager**, tetapi disertakan langsung sebagai vendor code.

Sumber file library dalam repo:
- `computer-store-inventory/src/lib/cJSON.c`
- `computer-store-inventory/src/lib/cJSON.h`

Provider / asal library:
- pada file `cJSON.h` terdapat copyright & notice yang menyebut **Dave Gamble and cJSON contributors**.

Cara integrasi di project:
- build memasukkan `lib/cJSON.c`.
- adapter `repository_cjson.c` meng-*include* `../lib/cJSON.h`.

---

## Kesimpulan
Project memenuhi kebutuhan tugas: pengelolaan inventaris, pencarian linear, sorting bubble sort, serta persistensi data JSON.
Selain itu, README ini memberikan penjelasan sintaks C yang “tidak umum” dan menunjukkan keterkaitannya langsung dengan kode.

