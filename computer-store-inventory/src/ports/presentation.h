#ifndef PRESENTATION_H
#define PRESENTATION_H

#include "../domain/product.h"

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

void clearBuffer();

#endif // PRESENTATION_H

