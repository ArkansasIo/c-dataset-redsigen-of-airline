#!/usr/bin/env bash
# Build the modular C++23 version and the CSV import tool (Linux/macOS/MSYS).
set -e
MODS="src/catalog.cpp src/dataset.cpp src/entry.cpp src/transaction.cpp \
src/index_key.cpp src/storage.cpp src/menus.cpp src/main.cpp"

echo "== Building recordbase (modular C++23) =="
clang++ -std=c++2b $MODS -o recordbase
echo "OK: recordbase"

echo "== Building csv_import =="
clang++ src/csv_import.c -o csv_import
echo "OK: csv_import"

echo "== Build complete =="
