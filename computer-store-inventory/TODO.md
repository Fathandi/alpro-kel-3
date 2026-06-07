# TODO - Dokumentasi Sintaks C pada README

- [x] Tambahkan section baru: “Kunci Pemahaman Sintaks C & Konsep yang Dipakai” di `computer-store-inventory/README.md`.
- [x] Jelaskan `#include`, header guard (`#ifndef/#define/#endif`), dan `typedef struct`.
- [x] Jelaskan `extern`, global state (`glb_arr_products`, `glb_int_product_count`, `glb_repository`).
- [x] Jelaskan fungsi pointer dan port/adapters: `ProductRepository` (function pointer `load/save`).
- [x] Jelaskan `static` helper `readFile()` di adapter JSON.
- [x] Jelaskan “pointer & array” pada parameter `Product products[]` dan `Product products[], int* count`.
- [x] Jelaskan kenapa memakai `scanf` + `clearBuffer()` untuk menu, dan `fgets` + `strcspn` untuk string.
- [x] Tambahkan section parsing/serialization JSON dengan cJSON: alur `readFile -> cJSON_Parse -> cJSON_ArrayForEach -> cJSON_GetObjectItemCaseSensitive -> save via cJSON_Print`.
- [x] Pastikan penulisan konsisten, tidak mengubah kode program, dan README tetap ringkas tapi informatif.

