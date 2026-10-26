# Build the modular C++23 version and the CSV import tool on Windows (clang++).
# Usage:  powershell -File build.ps1
$ErrorActionPreference = "Stop"
$clang = "C:\Program Files\LLVM\bin\clang++.exe"

if (-not (Test-Path $clang)) {
    $clang = "clang++"
}

$mods = @(
    "src/catalog.cpp",
    "src/dataset.cpp",
    "src/entry.cpp",
    "src/transaction.cpp",
    "src/index_key.cpp",
    "src/storage.cpp",
    "src/menus.cpp",
    "src/main.cpp"
)

Write-Host "== Building recordbase.exe (modular C++23) =="
& $clang -std=c++2b @mods -o recordbase.exe
if ($LASTEXITCODE -ne 0) { throw "recordbase.exe build failed" }
Write-Host "OK: recordbase.exe"

Write-Host "== Building csv_import.exe =="
& $clang "src/csv_import.c" -o csv_import.exe
if ($LASTEXITCODE -ne 0) { throw "csv_import.exe build failed" }
Write-Host "OK: csv_import.exe"

Write-Host "== Build complete =="
