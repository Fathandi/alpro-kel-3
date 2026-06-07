#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "../ports/repository.h"

Product glb_arr_products[100];
int glb_int_product_count = 0;
ProductRepository glb_repository;

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void displayMenu() {
    printf("\n========================\n");
    printf("COMPUTER STORE INVENTORY\n");
    printf("========================\n");
    
    float lcl_float_total_asset = 0;
    int lcl_int_total_stock = 0;
    for(int i = 0; i < glb_int_product_count; i++) {
        lcl_float_total_asset += glb_arr_products[i].price * glb_arr_products[i].stock;
        lcl_int_total_stock += glb_arr_products[i].stock;
    }

    printf("\nTotal Produk : %d\n", glb_int_product_count);
    printf("Total Stok   : %d\n", lcl_int_total_stock);
    printf("Nilai Barang : Rp%.0f\n\n", lcl_float_total_asset);
    
    printf("1. Tambah Produk\n");
    printf("2. Lihat Semua Produk\n");
    printf("3. Update Produk\n");
    printf("4. Hapus Produk\n");
    printf("5. Cari Produk\n");
    printf("6. Sort Produk\n");
    printf("7. Barang Masuk\n");
    printf("8. Barang Keluar\n");
    printf("9. Laporan Stok Menipis\n");
    printf("10. Statistik Inventaris\n");
    printf("0. Keluar\n");
}

void addProduct() {
    if (glb_int_product_count >= 100) {
        printf("Kapasitas penyimpanan penuh!\n");
        return;
    }

    Product lcl_new_product;
    printf("\n--- Tambah Produk ---\n");
    printf("ID: ");
    scanf("%d", &lcl_new_product.id);
    clearBuffer();
    
    printf("Nama: ");
    fgets(lcl_new_product.name, 100, stdin);
    lcl_new_product.name[strcspn(lcl_new_product.name, "\n")] = 0;
    
    printf("Kategori: ");
    fgets(lcl_new_product.category, 50, stdin);
    lcl_new_product.category[strcspn(lcl_new_product.category, "\n")] = 0;
    
    printf("Harga: ");
    scanf("%f", &lcl_new_product.price);
    
    printf("Stok: ");
    scanf("%d", &lcl_new_product.stock);
    clearBuffer();
    
    printf("Supplier: ");
    fgets(lcl_new_product.supplier, 100, stdin);
    lcl_new_product.supplier[strcspn(lcl_new_product.supplier, "\n")] = 0;

    glb_arr_products[glb_int_product_count] = lcl_new_product;
    glb_int_product_count++;
    
    glb_repository.save(glb_arr_products, glb_int_product_count);
    printf("Produk berhasil ditambahkan!\n");
}

void displayAllProducts() {
    printf("\n--- Semua Produk ---\n");
    printf("%-5s %-30s %-15s %-15s %-10s\n", "ID", "Nama", "Kategori", "Harga", "Stok");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < glb_int_product_count; i++) {
        printf("%-5d %-30s %-15s Rp%-13.0f %-10d\n", 
            glb_arr_products[i].id, 
            glb_arr_products[i].name, 
            glb_arr_products[i].category, 
            glb_arr_products[i].price, 
            glb_arr_products[i].stock);
    }
}

void updateProduct() {
    int lcl_int_id;
    printf("\nMasukkan ID Produk yang akan diupdate: ");
    scanf("%d", &lcl_int_id);
    clearBuffer();

    int lcl_int_found = -1;
    for (int i = 0; i < glb_int_product_count; i++) {
        if (glb_arr_products[i].id == lcl_int_id) {
            lcl_int_found = i;
            break;
        }
    }

    if (lcl_int_found != -1) {
        printf("Mengupdate data %s\n", glb_arr_products[lcl_int_found].name);
        
        printf("Nama Baru: ");
        fgets(glb_arr_products[lcl_int_found].name, 100, stdin);
        glb_arr_products[lcl_int_found].name[strcspn(glb_arr_products[lcl_int_found].name, "\n")] = 0;
        
        printf("Kategori Baru: ");
        fgets(glb_arr_products[lcl_int_found].category, 50, stdin);
        glb_arr_products[lcl_int_found].category[strcspn(glb_arr_products[lcl_int_found].category, "\n")] = 0;
        
        printf("Harga Baru: ");
        scanf("%f", &glb_arr_products[lcl_int_found].price);
        
        printf("Stok Baru: ");
        scanf("%d", &glb_arr_products[lcl_int_found].stock);
        clearBuffer();
        
        printf("Supplier Baru: ");
        fgets(glb_arr_products[lcl_int_found].supplier, 100, stdin);
        glb_arr_products[lcl_int_found].supplier[strcspn(glb_arr_products[lcl_int_found].supplier, "\n")] = 0;
        
        glb_repository.save(glb_arr_products, glb_int_product_count);
        printf("Produk berhasil diupdate!\n");
    } else {
        printf("Produk dengan ID %d tidak ditemukan.\n", lcl_int_id);
    }
}

void deleteProduct() {
    int lcl_int_id;
    printf("\nMasukkan ID Produk yang akan dihapus: ");
    scanf("%d", &lcl_int_id);
    
    int lcl_int_found = -1;
    for (int i = 0; i < glb_int_product_count; i++) {
        if (glb_arr_products[i].id == lcl_int_id) {
            lcl_int_found = i;
            break;
        }
    }

    if (lcl_int_found != -1) {
        for (int i = lcl_int_found; i < glb_int_product_count - 1; i++) {
            glb_arr_products[i] = glb_arr_products[i + 1];
        }
        glb_int_product_count--;
        glb_repository.save(glb_arr_products, glb_int_product_count);
        printf("Produk berhasil dihapus!\n");
    } else {
        printf("Produk dengan ID %d tidak ditemukan.\n", lcl_int_id);
    }
}

void searchProduct() {
    int lcl_int_choice;
    printf("\n--- Cari Produk ---\n");
    printf("1. Berdasarkan ID\n");
    printf("2. Berdasarkan Nama\n");
    printf("Pilihan: ");
    scanf("%d", &lcl_int_choice);
    clearBuffer();

    int lcl_int_found = 0;
    
    if (lcl_int_choice == 1) {
        int lcl_int_id;
        printf("Masukkan ID: ");
        scanf("%d", &lcl_int_id);
        
        for (int i = 0; i < glb_int_product_count; i++) {
            if (glb_arr_products[i].id == lcl_int_id) {
                printf("\nDitemukan: %s (Kategori: %s, Harga: Rp%.0f, Stok: %d)\n", 
                    glb_arr_products[i].name, glb_arr_products[i].category, 
                    glb_arr_products[i].price, glb_arr_products[i].stock);
                lcl_int_found = 1;
                break;
            }
        }
    } else if (lcl_int_choice == 2) {
        char lcl_str_query[100];
        printf("Masukkan Nama: ");
        fgets(lcl_str_query, 100, stdin);
        lcl_str_query[strcspn(lcl_str_query, "\n")] = 0;
        
        for (int i = 0; i < glb_int_product_count; i++) {
            if (strstr(glb_arr_products[i].name, lcl_str_query) != NULL) {
                printf("\nDitemukan: %s (ID: %d, Kategori: %s, Harga: Rp%.0f, Stok: %d)\n", 
                    glb_arr_products[i].name, glb_arr_products[i].id, glb_arr_products[i].category, 
                    glb_arr_products[i].price, glb_arr_products[i].stock);
                lcl_int_found = 1;
            }
        }
    }

    if (!lcl_int_found) {
        printf("Produk tidak ditemukan.\n");
    }
}

void sortProducts() {
    int lcl_int_choice;
    printf("\n--- Sort Produk ---\n");
    printf("1. Berdasarkan Nama\n");
    printf("2. Berdasarkan Harga\n");
    printf("3. Berdasarkan Stok\n");
    printf("Pilihan: ");
    scanf("%d", &lcl_int_choice);

    for (int i = 0; i < glb_int_product_count - 1; i++) {
        for (int j = 0; j < glb_int_product_count - i - 1; j++) {
            int lcl_int_swap = 0;
            
            if (lcl_int_choice == 1) {
                if (strcmp(glb_arr_products[j].name, glb_arr_products[j+1].name) > 0) {
                    lcl_int_swap = 1;
                }
            } else if (lcl_int_choice == 2) {
                if (glb_arr_products[j].price > glb_arr_products[j+1].price) {
                    lcl_int_swap = 1;
                }
            } else if (lcl_int_choice == 3) {
                if (glb_arr_products[j].stock > glb_arr_products[j+1].stock) {
                    lcl_int_swap = 1;
                }
            }

            if (lcl_int_swap) {
                Product lcl_temp = glb_arr_products[j];
                glb_arr_products[j] = glb_arr_products[j+1];
                glb_arr_products[j+1] = lcl_temp;
            }
        }
    }
    
    printf("Pengurutan berhasil. Silakan cek di menu 'Lihat Semua Produk'.\n");
    glb_repository.save(glb_arr_products, glb_int_product_count);
}

void stockIn() {
    int lcl_int_id;
    int lcl_int_qty;
    printf("\n--- Barang Masuk ---\n");
    printf("ID Produk: ");
    scanf("%d", &lcl_int_id);
    
    int lcl_int_found = -1;
    for (int i = 0; i < glb_int_product_count; i++) {
        if (glb_arr_products[i].id == lcl_int_id) {
            lcl_int_found = i;
            break;
        }
    }

    if (lcl_int_found != -1) {
        printf("Nama Produk: %s\n", glb_arr_products[lcl_int_found].name);
        printf("Jumlah Masuk (+): ");
        scanf("%d", &lcl_int_qty);
        
        glb_arr_products[lcl_int_found].stock += lcl_int_qty;
        
        glb_repository.save(glb_arr_products, glb_int_product_count);
        printf("Stok berhasil ditambahkan. Stok sekarang: %d\n", glb_arr_products[lcl_int_found].stock);
    } else {
        printf("Produk tidak ditemukan.\n");
    }
}

void stockOut() {
    int lcl_int_id;
    int lcl_int_qty;
    printf("\n--- Barang Keluar ---\n");
    printf("ID Produk: ");
    scanf("%d", &lcl_int_id);
    
    int lcl_int_found = -1;
    for (int i = 0; i < glb_int_product_count; i++) {
        if (glb_arr_products[i].id == lcl_int_id) {
            lcl_int_found = i;
            break;
        }
    }

    if (lcl_int_found != -1) {
        printf("Nama Produk: %s\n", glb_arr_products[lcl_int_found].name);
        printf("Stok Tersedia: %d\n", glb_arr_products[lcl_int_found].stock);
        printf("Jumlah Keluar (-): ");
        scanf("%d", &lcl_int_qty);
        
        if (glb_arr_products[lcl_int_found].stock < lcl_int_qty) {
            printf("Gagal: Stok tidak mencukupi!\n");
        } else {
            glb_arr_products[lcl_int_found].stock -= lcl_int_qty;
            glb_repository.save(glb_arr_products, glb_int_product_count);
            printf("Stok berhasil dikurangi. Stok sekarang: %d\n", glb_arr_products[lcl_int_found].stock);
        }
    } else {
        printf("Produk tidak ditemukan.\n");
    }
}

void lowStockReport() {
    printf("\n--- Laporan Stok Menipis (< 5) ---\n");
    printf("%-5s %-30s %-10s\n", "ID", "Nama", "Stok");
    printf("--------------------------------------------------\n");
    
    int lcl_int_found = 0;
    for (int i = 0; i < glb_int_product_count; i++) {
        if (glb_arr_products[i].stock < 5) {
            printf("%-5d %-30s %-10d\n", glb_arr_products[i].id, glb_arr_products[i].name, glb_arr_products[i].stock);
            lcl_int_found = 1;
        }
    }
    
    if (!lcl_int_found) {
        printf("Tidak ada produk dengan stok menipis.\n");
    }
}

void inventoryStats() {
    printf("\n--- Statistik Inventaris ---\n");
    float lcl_float_total_asset = 0;
    int lcl_int_total_stock = 0;
    
    for(int i = 0; i < glb_int_product_count; i++) {
        lcl_float_total_asset += glb_arr_products[i].price * glb_arr_products[i].stock;
        lcl_int_total_stock += glb_arr_products[i].stock;
    }
    
    printf("Total Jenis Produk : %d\n", glb_int_product_count);
    printf("Total Keseluruhan Stok : %d\n", lcl_int_total_stock);
    printf("Total Nilai Aset Barang: Rp%.0f\n", lcl_float_total_asset);
}
