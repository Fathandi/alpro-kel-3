#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "../domain/product.h"

typedef struct {
    int (*load)(Product products[], int* count);
    int (*save)(Product products[], int count);
} ProductRepository;

extern ProductRepository glb_repository;

#endif // REPOSITORY_H

