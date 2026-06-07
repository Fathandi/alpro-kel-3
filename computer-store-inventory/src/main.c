#include <stdio.h>
#include <stdlib.h>
#include "domain/product.h"
#include "ports/repository.h"
#include "ports/presentation.h"

/*
================================================================================
APPLICATION ENTRY POINT - MAIN
================================================================================
File: src/main.c

Deskripsi:
  Composition root aplikasi yang bertanggung jawab pada:
  - Dependency injection untuk repository dan presentation adapter
  - Main event loop untuk menu interaktif
  - Routing user input ke fungsi yang sesuai

Pola Hexagonal Architecture:
  - main.c = Composition Root (orchestrator/router)
  - domain/ = Business logic (pure, no I/O)
  - ports/ = Interface contracts (repository, presentation)
  - adapters/ = Concrete implementations (cJSON, console)

Dependencies:
  - createCJsonRepository() dari repository_cjson.c
  - glb_presentation dari presentation_console.c
  - Data globals dari domain/product.c

Flow Aplikasi:
  1. Injection repository adapter (cJSON)
  2. Load data dari file JSON
  3. Loop menu utama:
     - Tampilkan menu via presentation adapter
     - Input pilihan user
     - Route ke fungsi sesuai pilihan
  4. Pada exit: save data & return 0

Design Benefits:
  - Domain tidak terikat pada repository/presentation implementation
  - Easy to switch adapter (JSON → SQLite, console → GUI)
  - Testable: bisa mock adapter untuk testing
  - Maintainable: clear separation of concerns

================================================================================
*/

/* Pre-declare the constructor for the cJSON repository from repository_cjson.c */
extern ProductRepository createCJsonRepository();

/* Pre-declare the presentation adapter from presentation_console.c */
extern PresentationAdapter glb_presentation;

/* =========================================
   MAIN APPLICATION LOOP
   ========================================= */

int main() {
    /* ===== INITIALIZATION PHASE =====
       Dependency Injection: inject concrete adapters
    */
    
    /* Inject repository adapter (cJSON) dan load data dari file
       
       Proses:
         1. createCJsonRepository() membuat instance adapter
         2. Assign ke global glb_repository
         3. Panggil load() untuk membaca file products.json
         4. Data produk diisi ke glb_arr_products[]
         5. Count diupdate ke glb_int_product_count
    */
    glb_repository = createCJsonRepository();
    glb_repository.load(glb_arr_products, &glb_int_product_count);

    int lcl_int_choice;
    
    /* ===== MAIN APPLICATION LOOP =====
       Menu interaktif sampai user memilih exit (0)
    */
    while (1) {
        /* Display menu menggunakan presentation adapter
           Menu menampilkan statistik (total produk, stok, nilai aset) */
        glb_presentation.displayMenu();
        printf("Pilihan Anda: ");
        
        /* Input pilihan user dengan error handling
           
           Scenario:
             - User input angka: lcl_int_choice = angka, scanf() return 1
             - User input invalid (string): scanf() return 0, set = -1
             - clearBuffer() membersihkan sisa input
        */
        if(scanf("%d", &lcl_int_choice) != 1) {
            clearBuffer();
            lcl_int_choice = -1;
        }

        /* Route user choice ke fungsi yang sesuai */
        switch (lcl_int_choice) {
            case 1:
                glb_presentation.addProduct();
                break;
            case 2:
                glb_presentation.displayAllProducts();
                break;
            case 3:
                glb_presentation.updateProduct();
                break;
            case 4:
                glb_presentation.deleteProduct();
                break;
            case 5:
                glb_presentation.searchProduct();
                break;
            case 6:
                glb_presentation.sortProducts();
                break;
            case 7:
                glb_presentation.stockIn();
                break;
            case 8:
                glb_presentation.stockOut();
                break;
            case 9:
                glb_presentation.lowStockReport();
                break;
            case 10:
                glb_presentation.inventoryStats();
                break;
            case 0:
                /* Exit handler: save data & terminate
                   
                   Proses:
                     1. Tampilkan pesan konfirmasi exit
                     2. Save semua data ke file JSON via repository adapter
                     3. Return 0 (sukses exit)
                   
                   Note:
                     - glb_repository.save() memanggil cjsonSave()
                     - cjsonSave() convert array → JSON → write file
                     - Semua perubahan data selama sesi disimpan
                */
                printf("Keluar dari program. Menyimpan data...\n");
                glb_repository.save(glb_arr_products, glb_int_product_count);
                return 0;
            default:
                /* Handle invalid input
                   
                   User menginput pilihan di luar range 0-10
                   Tampilkan error dan loop kembali ke menu
                */
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    }
    return 0;
}
