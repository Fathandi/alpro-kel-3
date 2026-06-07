#ifndef PRODUCT_H
#define PRODUCT_H

/* =========================================
   REQUIREMENT:
   TIPE DATA (int, float, char[])
   ========================================= */
typedef struct {
    int id;
    char name[100];
    char category[50];
    float price;
    int stock;
    char supplier[100];
} Product;

/* =========================================
   REQUIREMENT:
   ARRAY
   ========================================= */
extern Product glb_arr_products[100];
extern int glb_int_product_count;

/* Functions for UI & logic */
void displayMenu();
void addProduct();
void displayAllProducts();
void updateProduct();
void deleteProduct();
void searchProduct();
void sortProducts();
void stockIn();
void stockOut();
void lowStockReport();
void inventoryStats();
void clearBuffer();

#endif // PRODUCT_H
