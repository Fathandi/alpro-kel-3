# Pseudocode Computer Store Inventory Management System

## Variabel Global
```
GLOBAL arr_products AS ARRAY OF Product[100]
GLOBAL int_product_count AS INTEGER = 0
```

## Main Program / Dashboard
```
BEGIN
    CALL createManualJsonRepository() RETURNING repository
    CALL repository.load(arr_products, int_product_count)

    WHILE True DO
        CALL calculateTotalAssets()
        DISPLAY "========================"
        DISPLAY "COMPUTER STORE INVENTORY"
        DISPLAY "========================"
        DISPLAY "Total Produk", int_product_count
        DISPLAY "Total Aset", total_assets

        DISPLAY "Menu:"
        DISPLAY "1. Tambah Produk"
        DISPLAY "2. Lihat Produk"
        DISPLAY "3. Update Produk"
        DISPLAY "4. Hapus Produk"
        DISPLAY "5. Cari Produk"
        DISPLAY "6. Sort Produk"
        DISPLAY "7. Barang Masuk"
        DISPLAY "8. Barang Keluar"
        DISPLAY "9. Laporan Stok Menipis"
        DISPLAY "10. Statistik Inventaris"
        DISPLAY "0. Keluar"
        
        INPUT choice
        
        SWITCH choice
            CASE 1: CALL addProduct()
            CASE 2: CALL displayAllProducts()
            CASE 3: CALL updateProduct()
            CASE 4: CALL deleteProduct()
            CASE 5: CALL searchProduct()
            CASE 6: CALL sortProducts()
            CASE 7: CALL stockIn()
            CASE 8: CALL stockOut()
            CASE 9: CALL lowStockReport()
            CASE 10: CALL inventoryStats()
            CASE 0:
                CALL repository.save(arr_products, int_product_count)
                EXIT PROGRAM
            DEFAULT:
                DISPLAY "Pilihan tidak valid"
        END SWITCH
    END WHILE
END
```

## Load JSON (File Handling)
```
BEGIN manualLoadJson(products, count)
    OPEN FILE "products.json" FOR READING
    IF FILE NOT FOUND THEN
        count = 0
        RETURN 0
    END IF
    
    index = 0
    WHILE NOT END_OF_FILE DO
        READ LINE from FILE
        IF LINE CONTAINS "{" THEN
            in_object = True
            new_product = EMPTY Product
        ELSE IF LINE CONTAINS "}" THEN
            IF in_object THEN
                products[index] = new_product
                index = index + 1
                in_object = False
            END IF
        ELSE IF in_object THEN
            PARSE key and value FROM LINE
            ASSIGN value TO new_product.key
        END IF
    END WHILE
    
    count = index
    CLOSE FILE
    RETURN 1
END
```

## Save JSON (File Handling)
```
BEGIN manualSaveJson(products, count)
    OPEN FILE "products.json" FOR WRITING
    WRITE "[\n" TO FILE
    FOR i = 0 TO count - 1 DO
        WRITE "  {\n" TO FILE
        WRITE "    \"id\": " + products[i].id + ",\n" TO FILE
        WRITE "    \"name\": \"" + products[i].name + "\",\n" TO FILE
        WRITE "    \"category\": \"" + products[i].category + "\",\n" TO FILE
        WRITE "    \"price\": " + products[i].price + ",\n" TO FILE
        WRITE "    \"stock\": " + products[i].stock + ",\n" TO FILE
        WRITE "    \"supplier\": \"" + products[i].supplier + "\"\n" TO FILE
        IF i < count - 1 THEN
            WRITE "  },\n" TO FILE
        ELSE
            WRITE "  }\n" TO FILE
        END IF
    END FOR
    WRITE "]\n" TO FILE
    CLOSE FILE
END
```

## Tambah Produk
```
BEGIN addProduct
    IF int_product_count >= 100 THEN
        DISPLAY "Kapasitas penuh"
        RETURN
    END IF

    INPUT new_product.id
    INPUT new_product.name
    INPUT new_product.category
    INPUT new_product.price
    INPUT new_product.stock
    INPUT new_product.supplier
    
    arr_products[int_product_count] = new_product
    int_product_count = int_product_count + 1
    
    CALL repository.save()
END
```

## Search Produk (Linear Search)
```
BEGIN searchProduct
    INPUT search_type (1: ID, 2: Nama)
    found = False
    
    IF search_type == 1 THEN
        INPUT search_id
        FOR i = 0 TO int_product_count - 1 DO
            IF arr_products[i].id == search_id THEN
                DISPLAY arr_products[i]
                found = True
                BREAK
            END IF
        END FOR
    ELSE IF search_type == 2 THEN
        INPUT search_name
        FOR i = 0 TO int_product_count - 1 DO
            IF arr_products[i].name CONTAINS search_name THEN
                DISPLAY arr_products[i]
                found = True
            END IF
        END FOR
    END IF
    
    IF NOT found THEN
        DISPLAY "Tidak ditemukan"
    END IF
END
```

## Sort Produk (Bubble Sort)
```
BEGIN sortProducts
    INPUT sort_type (1: Nama, 2: Harga, 3: Stok)
    
    FOR i = 0 TO int_product_count - 2 DO
        FOR j = 0 TO int_product_count - i - 2 DO
            swap = False
            
            IF sort_type == 1 THEN
                IF arr_products[j].name > arr_products[j+1].name THEN
                    swap = True
                END IF
            ELSE IF sort_type == 2 THEN
                IF arr_products[j].price > arr_products[j+1].price THEN
                    swap = True
                END IF
            ELSE IF sort_type == 3 THEN
                IF arr_products[j].stock > arr_products[j+1].stock THEN
                    swap = True
                END IF
            END IF
            
            IF swap THEN
                TEMP = arr_products[j]
                arr_products[j] = arr_products[j+1]
                arr_products[j+1] = TEMP
            END IF
        END FOR
    END FOR
    
    CALL repository.save()
END
```

## Stock In (Operasi Aritmatika)
```
BEGIN stockIn
    INPUT target_id
    index = findIndexById(target_id)
    
    IF index != -1 THEN
        INPUT qty_in
        arr_products[index].stock = arr_products[index].stock + qty_in
        CALL repository.save()
    END IF
END
```

## Stock Out (Operasi Aritmatika & Validasi)
```
BEGIN stockOut
    INPUT target_id
    index = findIndexById(target_id)
    
    IF index != -1 THEN
        INPUT qty_out
        IF arr_products[index].stock >= qty_out THEN
            arr_products[index].stock = arr_products[index].stock - qty_out
            CALL repository.save()
        ELSE
            DISPLAY "Stok tidak mencukupi"
        END IF
    END IF
END
```
