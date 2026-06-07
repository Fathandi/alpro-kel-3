#!/usr/bin/env bash
set -euo pipefail

# Reusable compile script
# Pakai dari root project: ./compile.sh
# Akan compile ke: computer-store-inventory/output/main.exe

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJ_DIR="$ROOT_DIR/computer-store-inventory"

cd "$PROJ_DIR"
mkdir -p output

OUT="output/main.exe"

echo "[1/1] Compiling C project (multi-file)..."

gcc \
  src/main.c \
  src/domain/product.c \
  src/adapters/repository_cjson.c \
  src/lib/cJSON.c \
  -o "$OUT"

echo "[OK] $OUT"

