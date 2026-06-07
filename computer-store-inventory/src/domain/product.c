#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "../ports/repository.h"

/* =========================================
   DOMAIN LAYER - PRODUCT
   Pure business logic, NO I/O operations
   ========================================= */

Product glb_arr_products[100];
int glb_int_product_count = 0;
ProductRepository glb_repository;


