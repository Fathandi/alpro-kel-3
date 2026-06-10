#include <stdio.h>
#include <stdlib.h>
#include "domain/product.h"
#include "ports/repository.h"
#include "ports/presentation.h"

extern ProductRepository createCJsonRepository();
extern PresentationAdapter glb_presentation;

int main() {
    glb_repository = createCJsonRepository();
    glb_repository.load(glb_arr_products, &glb_int_product_count);

    int lcl_int_choice;

    while (1) {
        glb_presentation.displayMenu();
        printf("Pilihan Anda: ");

        if (scanf("%d", &lcl_int_choice) != 1) {
            clearBuffer();
            lcl_int_choice = -1;
        }

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
                printf("Keluar dari program. Menyimpan data...\n");
                glb_repository.save(glb_arr_products, glb_int_product_count);
                return 0;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    }
}

