#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "../domain/product.h"

/* =========================================
   REPOSITORY PORT - INTERFACE DEFINITION
   Kontrak untuk akses data persistence
   ========================================= 
   
   Deskripsi:
     Port mendefinisikan interface (kontrak) untuk operasi persistence tanpa
     mengikat domain ke implementasi konkret. Menggunakan function pointer
     untuk mencapai polimorfisme di C.
   
   Pola Hexagonal Architecture:
     - Port = Interface/Kontrak yang domain butuhkan
     - Adapter = Implementasi konkret (mis: cJSON adapter)
     - Domain tidak perlu tahu detail (file vs database vs REST)
   
   Keuntungan:
     - Mudah switch adapter tanpa ubah domain logic
     - Testable: bisa mock adapter untuk unit testing
     - Fleksibel: JSON hari ini, SQLite besok
*/

typedef struct {
    /* Function pointer: load(Product[], int* count)
       
       Tanggung jawab:
         - Membaca data dari storage (file, database, dll)
         - Parsing data ke dalam array Product[]
         - Update parameter 'count' dengan jumlah produk yang dimuat
       
       Return: 0 = sukses, non-zero = error code
    */
    int (*load)(Product products[], int* count);
    
    /* Function pointer: save(Product[], int count)
       
       Tanggung jawab:
         - Mengambil array Product[] berisi 'count' elemen
         - Konversi ke format storage (JSON, CSV, binary, dll)
         - Simpan ke storage
       
       Return: 0 = sukses, non-zero = error code
    */
    int (*save)(Product products[], int count);
} ProductRepository;

/* Global Repository Instance yang akan diinjeksi oleh main.c
   Mengikuti Dependency Injection pattern di Hexagonal Architecture */
extern ProductRepository glb_repository;

#endif // REPOSITORY_H
