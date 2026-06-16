#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "../ports/repository.h"

Product glb_arr_products[PRODUCT_MAX];

int glb_int_product_count = 0;

ProductRepository glb_repository;


