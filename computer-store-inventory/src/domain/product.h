#ifndef PRODUCT_H
#define PRODUCT_H

typedef struct {
    int id;
    char name[100];
    char category[50];
    float price;
    int stock;
    char supplier[100];
} Product;

#define PRODUCT_MAX 100

extern Product glb_arr_products[PRODUCT_MAX];
extern int glb_int_product_count;

#endif // PRODUCT_H

