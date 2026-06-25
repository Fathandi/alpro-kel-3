MANUAL COMPILE INSTRUCTIONS (GCC)

mkdir -p output && \
gcc \
  src/main.c \
  src/domain/product.c \
  src/adapters/repository_cjson.c \
  src/adapters/presentation_console.c \
  src/lib/cJSON.c \
  -o output/main.exe

3) Hasil build:
   computer-store-inventory/output/main.exe

4) Jalankan program:
   ./output/main.exe

Catatan:
- Command ini tidak mengubah logika program.
- Hanya cara kompilasi yang diganti dari script (.sh) menjadi instruksi manual.

