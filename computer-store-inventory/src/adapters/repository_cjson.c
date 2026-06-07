#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ports/repository.h"
#include "../domain/product.h"
#include "../lib/cJSON.h"

/*
================================================================================
REPOSITORY ADAPTER - cJSON IMPLEMENTATION
================================================================================
File: src/adapters/repository_cjson.c

Deskripsi:
  Adapter konkret untuk repository port. Menyimpan dan memuat data produk
  menggunakan format JSON via library cJSON.

Pola Hexagonal Architecture:
  - Adapter = implementasi konkret dari repository port
  - Bertanggung jawab pada persistence logic (read/write file JSON)
  - Domain layer tidak perlu tahu detail implementasi ini
  
Fitur:
  - Load: Baca file JSON → Parse → Konversi ke array Product
  - Save: Konversi array Product → Buat JSON → Tulis ke file
  
File Storage:
  - Path: computer-store-inventory/data/products.json
  - Format: Array JSON berisi object dengan field: id, name, category, price, stock, supplier
  
Dependency:
  - Library cJSON untuk parsing & serialisasi JSON
  - Standard C library (stdio.h, stdlib.h, string.h)

================================================================================
*/

/* =========================================
   HELPER FUNCTION - FILE I/O
   ========================================= */

/* Membaca seluruh isi file ke dalam memory sebagai string
   
   Proses:
     1. Buka file dengan mode "rb" (read binary)
     2. Cari posisi akhir file menggunakan fseek() & ftell()
     3. Alokasi memory sesuai ukuran file (+ 1 untuk null terminator)
     4. Baca seluruh file ke dalam buffer
     5. Tambahkan null terminator (\0) untuk string yang valid
     6. Tutup file dan return pointer ke buffer
   
   Return Value:
     - Pointer ke string berisi isi file (harus free() oleh caller)
     - NULL jika error (file tidak ada, memory allocation gagal, dll)
   
   Catatan:
     - Caller bertanggung jawab free() memory setelah digunakan
     - Safe untuk file besar: menggunakan dynamic allocation
*/
static char* readFile(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char* data = (char*)malloc(length + 1);
    if (!data) {
        fclose(f);
        return NULL;
    }
    
    fread(data, 1, length, f);
    data[length] = '\0';
    fclose(f);
    return data;
}

/* =========================================
   REPOSITORY ADAPTER - LOAD IMPLEMENTATION
   ========================================= */

/* Memuat data produk dari file JSON ke dalam memory (array Product)
   
   Flow Proses:
     1. Baca file JSON menggunakan helper readFile()
     2. Parse JSON string → struktur cJSON menggunakan cJSON_Parse()
     3. Validasi:
        - Apakah JSON parsing berhasil?
        - Apakah root adalah array?
     4. Iterasi setiap element dalam array JSON
     5. Extract field per element: id, name, category, price, stock, supplier
     6. Validasi tipe data field menggunakan cJSON_IsNumber(), cJSON_IsString()
     7. Assign ke struct Product di dalam array
     8. Update parameter count dengan jumlah produk yang berhasil dimuat
     9. Cleanup memory (free JSON tree & file string)
   
   Parameter:
     - products[]: array produk yang akan diisi data dari file
     - count: pointer ke integer, akan diupdate dengan jumlah produk dimuat
   
   Return: 0 = sukses, non-zero = error code (tidak dipakai, reserved)
   
   Catatan Error Handling:
     - Jika file tidak ada: return 0, count = 0 (fresh start)
     - Jika JSON invalid: return 0, count = 0 (fresh start)
     - Jika bukan array: return 0, count = 0 (fresh start)
*/
int cjsonLoad(Product products[], int* count) {
    char* json_string = readFile("computer-store-inventory/data/products.json");
    if (!json_string) {
        *count = 0;
        return 0; // File not found or error, start fresh
    }
    
    cJSON* json = cJSON_Parse(json_string);
    free(json_string);
    
    if (!json) {
        *count = 0;
        return 0; // Invalid JSON
    }
    
    if (!cJSON_IsArray(json)) {
        cJSON_Delete(json);
        *count = 0;
        return 0;
    }
    
    int lcl_int_index = 0;
    cJSON* item = NULL;
    
    cJSON_ArrayForEach(item, json) {
        if (lcl_int_index >= 100) break; // Array limit
        
        cJSON* id = cJSON_GetObjectItemCaseSensitive(item, "id");
        cJSON* name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON* category = cJSON_GetObjectItemCaseSensitive(item, "category");
        cJSON* price = cJSON_GetObjectItemCaseSensitive(item, "price");
        cJSON* stock = cJSON_GetObjectItemCaseSensitive(item, "stock");
        cJSON* supplier = cJSON_GetObjectItemCaseSensitive(item, "supplier");
        
        if (cJSON_IsNumber(id)) products[lcl_int_index].id = id->valueint;
        if (cJSON_IsString(name)) strcpy(products[lcl_int_index].name, name->valuestring);
        if (cJSON_IsString(category)) strcpy(products[lcl_int_index].category, category->valuestring);
        if (cJSON_IsNumber(price)) products[lcl_int_index].price = (float)price->valuedouble;
        if (cJSON_IsNumber(stock)) products[lcl_int_index].stock = stock->valueint;
        if (cJSON_IsString(supplier)) strcpy(products[lcl_int_index].supplier, supplier->valuestring);
        
        lcl_int_index++;
    }
    
    *count = lcl_int_index;
    cJSON_Delete(json);
    return 1;
}

/* =========================================
   REPOSITORY ADAPTER - SAVE IMPLEMENTATION
   ========================================= */

/* Menyimpan data produk dari memory ke file JSON
   
   Flow Proses:
     1. Buat root array JSON menggunakan cJSON_CreateArray()
     2. Loop setiap produk dalam array:
        a. Buat object JSON baru untuk produk
        b. Tambahkan field: id, name, category, price, stock, supplier
        c. Append object ke root array
     3. Konversi JSON tree ke string menggunakan cJSON_Print()
     4. Tulis string JSON ke file dengan mode "w" (write text)
     5. Cleanup memory (free string & JSON tree)
   
   Parameter:
     - products[]: array berisi 'count' produk yang akan disimpan
     - count: jumlah produk dalam array
   
   Return: 0 = error, 1 = sukses
   
   Data Format JSON:
     [
       {
         "id": 1,
         "name": "Laptop Dell",
         "category": "Komputer",
         "price": 15000000,
         "stock": 5,
         "supplier": "PT Distributor"
       },
       ...
     ]
   
   Catatan:
     - Setiap save akan overwrite file lama (tidak append)
     - Urutan field dalam JSON sesuai urutan di kode
*/
int cjsonSave(Product products[], int count) {
    cJSON* root = cJSON_CreateArray();
    
    for (int i = 0; i < count; i++) {
        cJSON* obj = cJSON_CreateObject();
        cJSON_AddNumberToObject(obj, "id", products[i].id);
        cJSON_AddStringToObject(obj, "name", products[i].name);
        cJSON_AddStringToObject(obj, "category", products[i].category);
        cJSON_AddNumberToObject(obj, "price", products[i].price);
        cJSON_AddNumberToObject(obj, "stock", products[i].stock);
        cJSON_AddStringToObject(obj, "supplier", products[i].supplier);
        
        cJSON_AddItemToArray(root, obj);
    }
    
    char* json_string = cJSON_Print(root);
    cJSON_Delete(root);
    
    if (json_string) {
        FILE* f = fopen("computer-store-inventory/data/products.json", "w");
        if (f) {
            fprintf(f, "%s\n", json_string);
            fclose(f);
        } else {
            printf("Error saving file!\n");
        }
        free(json_string);
        return 1;
    }
    return 0;
}

/* =========================================
   REPOSITORY ADAPTER FACTORY
   ========================================= */

/* Factory function untuk membuat instance ProductRepository dengan adapter cJSON
   
   Pola: Factory Pattern + Dependency Injection
   
   Tanggung jawab:
     - Membuat struct ProductRepository
     - Assign function pointer ke implementasi cJSON (cjsonLoad & cjsonSave)
     - Return struct yang berisi function pointer
   
   Return Value:
     - ProductRepository struct berisi:
       - load = pointer ke fungsi cjsonLoad()
       - save = pointer ke fungsi cjsonSave()
   
   Penggunaan di main.c:
     glb_repository = createCJsonRepository();
     glb_repository.load(products, &count);  // Panggil cjsonLoad()
     glb_repository.save(products, count);   // Panggil cjsonSave()
   
   Keuntungan:
     - main.c tidak perlu include file adapter
     - Mudah switch ke adapter lain dengan factory baru
     - Hexagonal architecture: dependency injection di composition root
*/
ProductRepository createCJsonRepository() {
    ProductRepository repo;
    repo.load = cjsonLoad;
    repo.save = cjsonSave;
    return repo;
}
