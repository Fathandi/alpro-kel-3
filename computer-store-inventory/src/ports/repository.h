#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "../domain/product.h"

/* Hexagonal Architecture: Repository Interface (Port) */
typedef struct {
    int (*load)(Product products[], int* count);
    int (*save)(Product products[], int count);
} ProductRepository;

/* Global Repository Instance injected at runtime */
extern ProductRepository glb_repository;

#endif // REPOSITORY_H
