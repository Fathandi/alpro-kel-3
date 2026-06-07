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
      presentation.h
    adapters/
      repository_cjson.c
      presentation_console.c
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
  - **Composition Root**: tempat dependency injection & routing.
  - Memanggil `createCJsonRepository()` untuk inject repository adapter (cJSON).
  - Memanggil `createConsolePresentation()` atau instantiasi `glb_presentation` untuk inject presentation adapter (console).
  - Memanggil port lewat function pointer:
    - `glb_repository.load(...)` & `glb_repository.save(...)` untuk data persistence.
    - `glb_presentation.displayMenu()`, `glb_presentation.addProduct()`, dll untuk UI.
  - Mengimplementasikan main event loop: menu → input user → routing ke fungsi → loop.

- **`computer-store-inventory/src/domain/product.h`**
  - Deklarasi tipe data `Product` (`typedef struct ...`).
  - Deklarasi variabel global dengan `extern`.

- **`computer-store-inventory/src/domain/product.c`**
  - Implementasi logika bisnis inventaris: CRUD, searching (linear), sorting (bubble sort), operasi stok.
  - **PURE domain layer**: TIDAK ada operasi I/O (printf, scanf). Semua I/O dipindahkan ke presentation adapter.

- **`computer-store-inventory/src/ports/repository.h`**
  - **Port**: kontrak akses data.
  - Berisi `typedef struct ProductRepository` yang menggunakan **function pointer** (`load` dan `save`).

- **`computer-store-inventory/src/ports/presentation.h`**
  - **Port**: kontrak user interface.
  - Berisi `typedef struct PresentationAdapter` dengan function pointer untuk semua operasi UI (menu, input form, display).
  - Memisahkan business logic dari presentation detail.

- **`computer-store-inventory/src/adapters/repository_cjson.c`**
  - **Adapter** implementasi untuk menyimpan/memuat data JSON.
  - `cjsonLoad`: baca `data/products.json` → parse → isi array `Product`.
  - `cjsonSave`: ubah array `Product` → buat JSON → tulis ke file.
  - `createCJsonRepository()`: factory function untuk inject adapter ke main.

- **`computer-store-inventory/src/adapters/presentation_console.c`**
  - **Adapter** implementasi untuk user interface berbasis console/terminal.
  - Berisi semua fungsi UI: `displayMenu()`, `addProduct()`, `updateProduct()`, `deleteProduct()`, dll.
  - `clearBuffer()`: utility untuk membersihkan input buffer setelah `scanf()`.
  - Instantiasi global `glb_presentation` berisi function pointer ke semua fungsi UI.

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

 - **`computer-store-inventory/output/` dan executable hasil build (`main.exe`)**
  - Ini adalah **artefak kompilasi** (bukan source code). Jika di repo terdapat `inventory_app`, itu kemungkinan besar duplikasi dari hasil build yang sama.
  - Sebaiknya artefak build tidak disimpan dalam repository; tambahkan nama-nama artefak (mis. `main.exe`, `inventory_app`) ke `.gitignore`. Jika Anda memilih menyimpan satu executable untuk dokumentasi, gunakan satu nama yang konsisten (mis. `main.exe`).

---

## Refactoring: Separation of Concerns (Presentation dari Domain)

Pada versi terbaru, aplikasi telah di-refactor mengikuti **Hexagonal Architecture** dengan pemisahan yang lebih ketat:

### Masalah Sebelumnya
- Semua fungsi UI (`addProduct()`, `displayMenu()`, `searchProduct()`, dll) berada di `product.c` (domain layer).
- Domain layer tercampur dengan presentation logic (printf, scanf).
- Sulit untuk testing, refactoring, atau switch UI tanpa ubah domain.

### Solusi: Presentation Port & Adapter

**Sebelum:**
```
product.c (domain + presentation tercampur)
├── void addProduct()    // berisi printf & scanf
├── void displayMenu()   // UI logic
└── void searchProduct() // UI logic
```

**Sesudah:**
```
product.c (PURE domain)
├── Global data: glb_arr_products[], glb_int_product_count
├── Global repository: glb_repository
└── [Tidak ada I/O, hanya data & business logic]

presentation.h (PORT = kontrak)
└── PresentationAdapter struct dengan function pointer

presentation_console.c (ADAPTER = implementasi konkret)
├── void addProduct()    // implementasi console UI
├── void displayMenu()   // implementasi console UI
└── glb_presentation = { .addProduct, .displayMenu, ... }

main.c (ROUTER/ORCHESTRATOR)
├── Dependency injection: glb_presentation = createConsoleAdapter()
├── Memanggil UI via: glb_presentation.addProduct()
└── Routing user choice → fungsi yang sesuai
```

### Keuntungan Refactoring
1. **Domain = Pure Logic**: Business logic terpisah dari UI detail.
2. **Testable**: Bisa unit test domain tanpa mock I/O.
3. **Flexible**: Ganti console → GUI → Web hanya dengan adapter baru.
4. **Maintainable**: Clear separation = mudah dibaca & dimodifikasi.
5. **Scalable**: Mudah add feature baru tanpa kacaukan existing code.

### Mapping Responsibility

| Komponen | Tanggung Jawab |
|----------|---|
| **domain/product.c** | Data & business logic (CRUD, search, sort, stock) |
| **ports/presentation.h** | Kontrak interface UI (function pointer spec) |
| **adapters/presentation_console.c** | Implementasi UI (printf, scanf, form input) |
| **main.c** | Dependency injection & routing |

---
Bagian ini fokus pada “sintaks/konsep C yang sering terasa rumit” namun justru penting untuk memahami project.

### Ringkasan singkat: Sintaks kunci untuk pemula
Berikut adalah daftar singkat sintaks/pola yang sering muncul di proyek ini, dengan penjelasan singkat dan contoh minimal — ditujukan agar programmer junior cepat paham apa yang harus dicari di kode.

- Header guard: mencegah multiple-include pada header.
  - Contoh: `#ifndef PRODUCT_H` / `#define PRODUCT_H` ... `#endif`.
- `typedef struct`: membuat nama tipe yang mudah dipakai.
  - Contoh: `typedef struct { int id; char name[100]; } Product;` → gunakan `Product` langsung.
- `extern` vs definisi: deklarasi di header, definisi satu-satunya di `.c`.
  - Contoh: di header `extern int count;` di `.c` `int count = 0;`.
- Function pointer untuk "port/adaptor": memanggil implementasi melalui pointer fungsi.
  - Contoh tipe: `int (*load)(Product[], int*);` lalu panggil `repo.load(arr, &n);`.
- `static` pada fungsi: membuat fungsi hanya terlihat di file yang sama (private helper).
  - Contoh: `static char* readFile(const char* filename) { ... }`.
- Parameter array/pointer: `Product products[]` setara `Product* products`.
  - Contoh akses: `products[i].id`.
- Passing by reference: gunakan pointer untuk mengubah variabel pemanggil.
  - Contoh: `void set(int *p) { *p = 5; }` panggil `set(&x);`.
- Perbedaan `scanf` dan `fgets`: `scanf` untuk angka; `fgets` lebih aman untuk string.
  - Ingat: bersihkan input buffer setelah `scanf` sebelum `fgets`.
- Penggunaan cJSON: parse string JSON ke struktur `cJSON*`, iterasi array, ambil field, lalu isi struct C.

Jika ingin, saya bisa mengubah bagian berikutnya menjadi penjelasan bertahap (contoh file + baris) untuk lebih jelas bagi pemula.

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

