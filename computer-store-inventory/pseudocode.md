# Pseudocode Computer Store Inventory Management System (sesuai implementasi C)

> Catatan: pseudocode ini mengikuti struktur menu dan logika fungsi di `src/main.c` dan `src/domain/product.c`, serta persistensi data memakai adapter `src/adapters/repository_cjson.c` (cJSON).

---

## Variabel Global
```
GLOBAL arr_products : ARRAY[100] OF Product
GLOBAL int_product_count : INTEGER = 0
GLOBAL glb_repository : ProductRepository
```

---

## Main Program / Dashboard
```
BEGIN main()
    // inject adapter (Hexagonal Architecture)
    glb_repository = createCJsonRepository()

    // load persisted data into memory
    glb_repository.load(arr_products, &int_product_count)

    WHILE True DO
        displayMenu()  // menampilkan total produk, total stok, nilai aset

        INPUT choice
        IF choice tidak terbaca (scanf gagal) THEN
            choice = -1
        END IF

        SWITCH choice
            CASE 1:
                addProduct()
            CASE 2:
                displayAllProducts()
            CASE 3:
                updateProduct()
            CASE 4:
                deleteProduct()
            CASE 5:
                searchProduct()
            CASE 6:
                sortProducts()
            CASE 7:
                stockIn()
            CASE 8:
                stockOut()
            CASE 9:
                lowStockReport()
            CASE 10:
                inventoryStats()
            CASE 0:
                print "Keluar dari program. Menyimpan data..."
                glb_repository.save(arr_products, int_product_count)
                EXIT PROGRAM
            DEFAULT:
                print "Pilihan tidak valid. Silakan coba lagi."
        END SWITCH
    END WHILE
END
```

---

## Port & Adapter: Repository (cJSON)
### Contract (port)
```
typedef struct ProductRepository
    load(products[], *count) RETURNS int
    save(products[], count) RETURNS int
END
```

### Adapter Load JSON (`cjsonLoad`)
```
BEGIN cjsonLoad(products, *count)
    json_string = read entire file "computer-store-inventory/data/products.json" as string

    IF json_string == NULL THEN
        *count = 0
        RETURN 0
    END IF

    root = cJSON_Parse(json_string)
    free(json_string)

    IF root == NULL THEN
        *count = 0
        RETURN 0
    END IF

    IF root bukan array THEN
        cJSON_Delete(root)
        *count = 0
        RETURN 0
    END IF

    index = 0
    FOR each item IN root array DO
        IF index >= 100 THEN
            BREAK
        END IF

        // baca field object dengan validasi tipe
        id       = item["id"]
        name     = item["name"]
        category = item["category"]
        price    = item["price"]
        stock    = item["stock"]
        supplier = item["supplier"]

        IF id is number THEN products[index].id = id
        IF name is string THEN products[index].name = name
        IF category is string THEN products[index].category = category
        IF price is number THEN products[index].price = price
        IF stock is number THEN products[index].stock = stock
        IF supplier is string THEN products[index].supplier = supplier

        index = index + 1
    END FOR

    *count = index
    cJSON_Delete(root)
    RETURN 1
END
```

### Adapter Save JSON (`cjsonSave`)
```
BEGIN cjsonSave(products, count)
    root = cJSON_CreateArray()

    FOR i = 0 TO count - 1 DO
        obj = cJSON_CreateObject()

        cJSON_AddNumberToObject(obj, "id", products[i].id)
        cJSON_AddStringToObject(obj, "name", products[i].name)
        cJSON_AddStringToObject(obj, "category", products[i].category)
        cJSON_AddNumberToObject(obj, "price", products[i].price)
        cJSON_AddNumberToObject(obj, "stock", products[i].stock)
        cJSON_AddStringToObject(obj, "supplier", products[i].supplier)

        cJSON_AddItemToArray(root, obj)
    END FOR

    json_string = cJSON_Print(root)
    cJSON_Delete(root)

    buka file "computer-store-inventory/data/products.json" untuk mode write
    tulis json_string ke file
    close file

    free(json_string)
    RETURN 1
END
```

---

## displayMenu
```
BEGIN displayMenu()
    // hitung statistik untuk ditampilkan
    total_asset = 0
    total_stock = 0
    FOR i = 0 TO int_product_count - 1 DO
        total_asset += products[i].price * products[i].stock
        total_stock += products[i].stock
    END FOR

    DISPLAY menu 1..10 dan 0
END
```

---

## addProduct
```
BEGIN addProduct()
    IF int_product_count >= 100 THEN
        print "Kapasitas penyimpanan penuh!"
        RETURN
    END IF

    INPUT id
    INPUT name (pakai fgets di C)
    INPUT category
    INPUT price
    INPUT stock
    INPUT supplier

    arr_products[int_product_count] = new_product
    int_product_count = int_product_count + 1

    glb_repository.save(arr_products, int_product_count)
    print "Produk berhasil ditambahkan!"
END
```

---

## displayAllProducts
```
BEGIN displayAllProducts()
    print header tabel
    FOR i = 0 TO int_product_count - 1 DO
        print arr_products[i] (ID, name, category, price, stock)
    END FOR
END
```

---

## updateProduct
```
BEGIN updateProduct()
    INPUT target_id

    found_index = -1
    FOR i = 0 TO int_product_count - 1 DO
        IF arr_products[i].id == target_id THEN
            found_index = i
            BREAK
        END IF
    END FOR

    IF found_index == -1 THEN
        print "Produk tidak ditemukan"
        RETURN
    END IF

    // update field (name, category, price, stock, supplier)
    INPUT new name
    INPUT new category
    INPUT new price
    INPUT new stock
    INPUT new supplier

    glb_repository.save(arr_products, int_product_count)
    print "Produk berhasil diupdate!"
END
```

---

## deleteProduct
```
BEGIN deleteProduct()
    INPUT target_id

    found_index = -1
    FOR i = 0 TO int_product_count - 1 DO
        IF arr_products[i].id == target_id THEN
            found_index = i
            BREAK
        END IF
    END FOR

    IF found_index == -1 THEN
        print "Produk tidak ditemukan"
        RETURN
    END IF

    // shift left untuk menutup gap
    FOR i = found_index TO int_product_count - 2 DO
        arr_products[i] = arr_products[i + 1]
    END FOR

    int_product_count = int_product_count - 1
    glb_repository.save(arr_products, int_product_count)
    print "Produk berhasil dihapus!"
END
```

---

## searchProduct (linear)
```
BEGIN searchProduct()
    INPUT search_choice (1: ID, 2: Nama)
    found = False

    IF search_choice == 1 THEN
        INPUT search_id
        FOR i = 0 TO int_product_count - 1 DO
            IF arr_products[i].id == search_id THEN
                display arr_products[i]
                found = True
                BREAK
            END IF
        END FOR

    ELSE IF search_choice == 2 THEN
        INPUT query_name
        FOR i = 0 TO int_product_count - 1 DO
            IF arr_products[i].name contains query_name THEN
                display arr_products[i]
                found = True
            END IF
        END FOR
    END IF

    IF found == False THEN
        print "Produk tidak ditemukan."
    END IF
END
```

---

## sortProducts (bubble sort)
```
BEGIN sortProducts()
    INPUT sort_choice (1: Nama, 2: Harga, 3: Stok)

    FOR i = 0 TO int_product_count - 2 DO
        FOR j = 0 TO int_product_count - i - 2 DO
            swap = False

            IF sort_choice == 1 THEN
                IF arr_products[j].name > arr_products[j+1].name THEN swap = True
            ELSE IF sort_choice == 2 THEN
                IF arr_products[j].price > arr_products[j+1].price THEN swap = True
            ELSE IF sort_choice == 3 THEN
                IF arr_products[j].stock > arr_products[j+1].stock THEN swap = True
            END IF

            IF swap THEN
                TEMP = arr_products[j]
                arr_products[j] = arr_products[j+1]
                arr_products[j+1] = TEMP
            END IF
        END FOR
    END FOR

    glb_repository.save(arr_products, int_product_count)
    print "Pengurutan berhasil..."
END
```

---

## stockIn
```
BEGIN stockIn()
    INPUT target_id

    found_index = -1
    FOR i = 0 TO int_product_count - 1 DO
        IF arr_products[i].id == target_id THEN
            found_index = i
            BREAK
        END IF
    END FOR

    IF found_index == -1 THEN
        print "Produk tidak ditemukan."
        RETURN
    END IF

    INPUT qty_in
    arr_products[found_index].stock = arr_products[found_index].stock + qty_in

    glb_repository.save(arr_products, int_product_count)
    print "Stok berhasil ditambahkan..."
END
```

---

## stockOut
```
BEGIN stockOut()
    INPUT target_id

    found_index = -1
    FOR i = 0 TO int_product_count - 1 DO
        IF arr_products[i].id == target_id THEN
            found_index = i
            BREAK
        END IF
    END FOR

    IF found_index == -1 THEN
        print "Produk tidak ditemukan."
        RETURN
    END IF

    INPUT qty_out

    IF arr_products[found_index].stock < qty_out THEN
        print "Gagal: Stok tidak mencukupi!"
        RETURN
    END IF

    arr_products[found_index].stock = arr_products[found_index].stock - qty_out
    glb_repository.save(arr_products, int_product_count)
    print "Stok berhasil dikurangi..."
END
```

---

## lowStockReport
```
BEGIN lowStockReport()
    print "Produk dengan stok < 5"

    any = False
    FOR i = 0 TO int_product_count - 1 DO
        IF arr_products[i].stock < 5 THEN
            display arr_products[i]
            any = True
        END IF
    END FOR

    IF any == False THEN
        print "Tidak ada produk dengan stok menipis."
    END IF
END
```

---

## inventoryStats
```
BEGIN inventoryStats()
    total_asset = 0
    total_stock = 0

    FOR i = 0 TO int_product_count - 1 DO
        total_asset += arr_products[i].price * arr_products[i].stock
        total_stock += arr_products[i].stock
    END FOR

    print total jenis produk, total stok keseluruhan, dan total nilai aset
END
```

