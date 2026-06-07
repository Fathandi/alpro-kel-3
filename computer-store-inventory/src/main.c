#include <stdio.h>
#include <stdlib.h>
#include "domain/product.h"
#include "ports/repository.h"

/* Pre-declare the constructor for the cJSON repository from repository_cjson.c */
extern ProductRepository createCJsonRepository();

int main() {
    /* Inject the cJSON Repository adapter */
    glb_repository = createCJsonRepository();
    glb_repository.load(glb_arr_products, &glb_int_product_count);

    int lcl_int_choice;
    
    while (1) {
        displayMenu();
        printf("Pilihan Anda: ");
        if(scanf("%d", &lcl_int_choice) != 1) {
            clearBuffer();
            lcl_int_choice = -1;
        }

        switch (lcl_int_choice) {
            case 1:
                addProduct();
                break;
            case 2:
                displayAllProducts();
                break;
            case 3:
                updateProduct();
                break;
            case 4:
                deleteProduct();
                break;
            case 5:
                searchProduct();
                break;
            case 6:
                sortProducts();
                break;
            case 7:
                stockIn();
                break;
            case 8:
                stockOut();
                break;
            case 9:
                lowStockReport();
                break;
            case 10:
                inventoryStats();
                break;
            case 0:
                printf("Keluar dari program. Menyimpan data...\n");
                glb_repository.save(glb_arr_products, glb_int_product_count);
                return 0;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    }
    return 0;
}
