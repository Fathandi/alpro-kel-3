#ifndef PRESENTATION_H
#define PRESENTATION_H

#include "../domain/product.h"

/* =========================================
   PRESENTATION PORT
   Abstraksi untuk UI/presentation layer
   ========================================= */

typedef struct {
    void (*displayMenu)();
    void (*addProduct)();
    void (*displayAllProducts)();
    void (*updateProduct)();
    void (*deleteProduct)();
    void (*searchProduct)();
    void (*sortProducts)();
    void (*stockIn)();
    void (*stockOut)();
    void (*lowStockReport)();
    void (*inventoryStats)();
} PresentationAdapter;

extern PresentationAdapter glb_presentation;

/* Utility function for clearing input buffer */
void clearBuffer();

#endif // PRESENTATION_H
