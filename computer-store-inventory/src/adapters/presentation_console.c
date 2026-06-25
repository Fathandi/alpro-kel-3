#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ports/presentation.h"
#include "../ports/repository.h"
#include "../domain/product.h"

/*
================================================================================
PRESENTATION ADAPTER - CONSOLE IMPLEMENTATION
================================================================================
File: src/adapters/presentation_console.c

Peran:
  Adapter ini mengimplementasikan semua operasi pada PresentationAdapter
  (port UI) menggunakan antarmuka console/terminal.

Prinsip desain:
  - Adapter adalah lokasi I/O: printf/scanf/fgets.
  - Domain (src/domain/product.c) berisi state & aturan bisnis.
  - main.c hanya melakukan routing/menu; detail interaksi user ada di sini.

Konsep Hexagonal (Ports & Adapters):
  - presentation.h: kontrak (function pointer) yang dipanggil oleh main
  - presentation_console.c: implementasi konkret kontrak

Catatan pembaca (untuk pemula):
  - struct berisi pointer fungsi = cara polimorfisme di C.
  - main memanggil glb_presentation.<fungsi>() tanpa tahu implementasinya.

================================================================================
*/

/* =========================================
   UTILITY FUNCTION - INPUT BUFFER MANAGEMENT
   ========================================= */

/*
  Fungsi: clearBuffer()

  Kenapa ada?
  - Banyak input angka memakai scanf(). scanf menyisakan karakter delimiter
    (umumnya newline '\n') di input buffer.
  - Jika setelah itu kita memanggil fgets(), fgets bisa membaca newline sisa
    sehingga hasil string menjadi kosong.

  Kontrak:
    - Pre: setelah scanf, buffer mungkin masih berisi newline sisa
    - Post: buffer dibersihkan sampai '\n' atau EOF

  Dampak:
    - Tidak mengubah logika bisnis.
    - Hanya memastikan urutan input user berjalan benar.
*/
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* =========================================
   PRESENTATION FUNCTIONS - MENU & DISPLAY
   ========================================= */

/*
  displayMenu()

  Output:
    - Menampilkan header program.
    - Menghitung statistik dari state domain (glb_arr_products &
      glb_int_product_count):
        * Total produk jenis
        * Total stok keseluruhan
        * Nilai aset = sum(price * stock)
    - Menampilkan daftar pilihan aksi 0..10.

  Catatan:
    - Fungsi ini hanya menampilkan (tidak mengubah data).
*/
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

/*
  addProduct()

  Tanggung jawab:
    - Validasi kapasitas array domain (PRODUCT_MAX).
    - Mengumpulkan input user untuk membuat satu Product.
    - Menambahkan Product ke glb_arr_products.
    - Memperbarui glb_int_product_count.
    - Memanggil glb_repository.save(...) untuk persistensi JSON.

  I/O:
    - scanf untuk angka, fgets untuk string.
    - clearBuffer dipakai untuk menghindari newline sisa setelah scanf.

  Catatan desain:
    - Pemeriksaan keunikan ID belum dilakukan di sini.
*/
void addProduct() {
    /* Tambah Produk Baru
       
       Flow Proses:
         1. Validasi kapasitas array (max 100)
         2. Input semua field produk dari user via console
         3. Tambah ke array glb_arr_products
         4. Increment counter glb_int_product_count
         5. Simpan ke storage via glb_repository.save()
       
       Input Fields:
         - ID: integer unik (user input)
         - Nama: string max 100 karakter
         - Kategori: string max 50 karakter
         - Harga: float (Rupiah)
         - Stok: integer jumlah barang
         - Supplier: string max 100 karakter
       
       Note:
         - Validasi keunikan ID TIDAK dilakukan di sini
         - Responsibility: domain layer atau presentation layer?
         - Saat ini: hanya collect input & append ke array
    */
    if (glb_int_product_count >= PRODUCT_MAX) {
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

/*
  displayAllProducts()

  Output:
    - Mencetak tabel seluruh produk yang tersimpan dalam state domain.
*/
void displayAllProducts() {
    printf("\n--- Semua Produk ---\n");
    printf("%-5s %-30s %-15s %-15s %-10s %-15s\n", "ID", "Nama", "Kategori", "Harga", "Stok", "Supplier");
    printf("------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < glb_int_product_count; i++) {
        printf("%-5d %-30s %-15s Rp%-13.0f %-10d %-15s\n",
            glb_arr_products[i].id,
            glb_arr_products[i].name,
            glb_arr_products[i].category,
            glb_arr_products[i].price,
            glb_arr_products[i].stock,
            glb_arr_products[i].supplier);
    }
}

/*
  updateProduct()

  Flow:
    1) Input ID target.
    2) Cari indeks produk di array.
    3) Jika ditemukan: minta nilai baru (nama, kategori, harga, stok, supplier).
    4) Simpan ulang via repository.save(...).
*/
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

/*
  deleteProduct()

  Flow:
    1) Input ID.
    2) Cari indeks.
    3) Jika ditemukan: geser elemen setelahnya satu posisi ke kiri.
    4) Decrement glb_int_product_count.
    5) Simpan via repository.save(...).
*/
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

/*
  searchProduct()

  Mode pencarian:
    - 1: berdasarkan ID (exact match)
    - 2: berdasarkan Nama (substring menggunakan strstr)

  Output:
    - Menampilkan produk yang cocok; jika tidak ada, mencetak "Produk tidak ditemukan".
*/
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
                printf("\nDitemukan: %s (Kategori: %s, Harga: Rp%.0f, Stok: %d, Supplier: %s)\n",
                    glb_arr_products[i].name,
                    glb_arr_products[i].category,
                    glb_arr_products[i].price,
                    glb_arr_products[i].stock,
                    glb_arr_products[i].supplier);
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
                printf("\nDitemukan: %s (ID: %d, Kategori: %s, Harga: Rp%.0f, Stok: %d, Supplier: %s)\n",
                    glb_arr_products[i].name,
                    glb_arr_products[i].id,
                    glb_arr_products[i].category,
                    glb_arr_products[i].price,
                    glb_arr_products[i].stock,
                    glb_arr_products[i].supplier);
                lcl_int_found = 1;
            }
        }
    }

    if (!lcl_int_found) {
        printf("Produk tidak ditemukan.\n");
    }
}

/*
  sortProducts()

  Metode sorting: bubble sort (berdasarkan pilihan user)
    - 1: name (strcmp)
    - 2: price (float)
    - 3: stock (int)

  Setelah sorting: data disimpan ulang ke JSON.
*/
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

/*
  stockIn()

  Flow:
    - Cari produk berdasarkan ID.
    - Tambah stok (stock += qty).
    - Simpan ulang ke JSON.
*/
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

/*
  stockOut()

  Flow:
    - Cari produk berdasarkan ID.
    - Validasi stok cukup sebelum mengurangi.
    - Jika stok tidak cukup: gagal dan stok tidak berubah.
    - Jika cukup: stock -= qty.
    - Simpan ulang ke JSON.
*/
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

/*
  lowStockReport()

  Laporan:
    - Menampilkan produk dengan stock < 5.
    - Jika tidak ada: menampilkan pesan "Tidak ada produk dengan stok menipis".
*/
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

/*
  inventoryStats()

  Statistik:
    - total jenis produk = glb_int_product_count
    - total stok keseluruhan
    - total nilai aset
*/
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

/* =========================================
   PRESENTATION ADAPTER INSTANTIATION
   ========================================= */

/*
  Instansiasi PresentationAdapter

  Penting:
  - Struktur ini berisi pointer ke fungsi-fungsi UI di file ini.
  - main.c memanggil melalui global glb_presentation.

  Dampak desain:
  - main tidak terikat pada console.
  - Di masa depan, tinggal buat adapter baru (mis. GUI) dan ikat ke glb_presentation.
*/
PresentationAdapter glb_presentation = {
    .displayMenu = displayMenu,
    .addProduct = addProduct,
    .displayAllProducts = displayAllProducts,
    .updateProduct = updateProduct,
    .deleteProduct = deleteProduct,
    .searchProduct = searchProduct,
    .sortProducts = sortProducts,
    .stockIn = stockIn,
    .stockOut = stockOut,
    .lowStockReport = lowStockReport,
    .inventoryStats = inventoryStats,
};

