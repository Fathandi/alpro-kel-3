#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ports/repository.h"
#include "../domain/product.h"
#include "../lib/cJSON.h"

static char* readFile(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char* data = (char*)malloc(length + 1);
    if (!data) {
        fclose(f);
        return NULL;
    }
    
    fread(data, 1, length, f);
    data[length] = '\0';
    fclose(f);
    return data;
}

int cjsonLoad(Product products[], int* count) {
    char* json_string = readFile("computer-store-inventory/data/products.json");
    if (!json_string) {
        *count = 0;
        return 0; // File not found or error, start fresh
    }
    
    cJSON* json = cJSON_Parse(json_string);
    free(json_string);
    
    if (!json) {
        *count = 0;
        return 0; // Invalid JSON
    }
    
    if (!cJSON_IsArray(json)) {
        cJSON_Delete(json);
        *count = 0;
        return 0;
    }
    
    int lcl_int_index = 0;
    cJSON* item = NULL;
    
    cJSON_ArrayForEach(item, json) {
        if (lcl_int_index >= PRODUCT_MAX) break; // Array limit
        
        cJSON* id = cJSON_GetObjectItemCaseSensitive(item, "id");
        cJSON* name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON* category = cJSON_GetObjectItemCaseSensitive(item, "category");
        cJSON* price = cJSON_GetObjectItemCaseSensitive(item, "price");
        cJSON* stock = cJSON_GetObjectItemCaseSensitive(item, "stock");
        cJSON* supplier = cJSON_GetObjectItemCaseSensitive(item, "supplier");
        
        if (cJSON_IsNumber(id)) products[lcl_int_index].id = id->valueint;
        if (cJSON_IsString(name)) strcpy(products[lcl_int_index].name, name->valuestring);
        if (cJSON_IsString(category)) strcpy(products[lcl_int_index].category, category->valuestring);
        if (cJSON_IsNumber(price)) products[lcl_int_index].price = (float)price->valuedouble;
        if (cJSON_IsNumber(stock)) products[lcl_int_index].stock = stock->valueint;
        if (cJSON_IsString(supplier)) strcpy(products[lcl_int_index].supplier, supplier->valuestring);
        
        lcl_int_index++;
    }
    
    *count = lcl_int_index;
    cJSON_Delete(json);
    return 1;
}


int cjsonSave(Product products[], int count) {
    cJSON* root = cJSON_CreateArray();
    
    for (int i = 0; i < count; i++) {
        cJSON* obj = cJSON_CreateObject();
        cJSON_AddNumberToObject(obj, "id", products[i].id);
        cJSON_AddStringToObject(obj, "name", products[i].name);
        cJSON_AddStringToObject(obj, "category", products[i].category);
        cJSON_AddNumberToObject(obj, "price", products[i].price);
        cJSON_AddNumberToObject(obj, "stock", products[i].stock);
        cJSON_AddStringToObject(obj, "supplier", products[i].supplier);
        
        cJSON_AddItemToArray(root, obj);
    }
    
    char* json_string = cJSON_Print(root);
    cJSON_Delete(root);
    
    if (json_string) {
        FILE* f = fopen("computer-store-inventory/data/products.json", "w");
        if (f) {
            fprintf(f, "%s\n", json_string);
            fclose(f);
        } else {
            printf("Error saving file!\n");
        }
        free(json_string);
        return 1;
    }
    return 0;
}

ProductRepository createCJsonRepository() {
    ProductRepository repo;
    repo.load = cjsonLoad;
    repo.save = cjsonSave;
    return repo;
}
