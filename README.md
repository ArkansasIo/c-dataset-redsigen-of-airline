# RecordBase

C++ records database system (console based) with a Lua OO scripting layer,
a Python REST backend and a web frontend. Author: apocalypse coder0.

Redesigned from the airline reservation program: airline → catalog,
flight → dataset, passenger → entry, reservation → transaction,
seat assignment → index key.

## Repository layout

```
database navigation control.cpp   main console program (menu-driven linked lists)
src/csv_loader.h                  CSV import library for the data/ files
src/csv_import.c                  standalone CSV import verification tool
src/reports.h                     reports: datasets by start time,
                                  entries by last name, statistics
src/types.h                       C++23 data model (catalog/dataset/entry/...)
data/catalogs.csv                 catalog master data
data/datasets.csv                 dataset registry (unique dataset ID)
data/entries.csv                  entry master data (unique entry ID)
data/transactions.csv             transactions (unique transaction ID)
data/indexes.csv                  index keys per dataset
database/schema.sql               MySQL schema (catalog, dataset, entry,
                                  transaction tree: node/branch/sub-branch/module,
                                  app_setting)
database/dataset.sql              sample dataset for MySQL
lib/dbtree.lua                    Lua OO database tree library
scripts/demo.lua                  Lua demo script
backend/server.py                 REST backend (Python stdlib + SQLite)
web/index.html                    web frontend
docs/UML.md                       UML diagrams
config/config.json                JSON configuration (database, API, rows, logging)
config/settings.env               environment-variable settings overrides
config/api.md                     REST API specification (v1)
config/reservation_edit.h         C structs mirroring the transaction tree
```

## Build

```powershell
# main console program
clang++ "database navigation control.cpp" -o recordbase.exe

# CSV import verification tool
clang++ src/csv_import.c -o csv_import.exe
```

## Run

```
recordbase.exe     # loads database.dat (if present) and shows the main menu
csv_import.exe     # verifies the CSV dataset in data/
lua scripts/demo.lua        # Lua OO library demo
python backend/server.py    # REST backend + frontend at http://localhost:8080
```

Main menu: add/delete/list catalogs, enter the dataset menu per catalog
(add/delete/list datasets, entry menu, transaction menu, index key
menu), then option 5 saves everything back to `database.dat`.

## Data model

- **Dataset**: Dataset ID (unique key), source/target table, date,
  start/end time, total rows, rows indexed.
- **Transaction**: Transaction ID (unique key), Entry ID, Dataset ID, Amount.
- **Entry**: Entry ID (unique key), name, address, city.

Business rules:
- Entries and datasets must already exist before they can appear on a transaction.
- Unique IDs are never duplicated across the files.
- Report outputs: all datasets sorted by start time; all entries on a
  selected dataset sorted by last name; save option stores the data file.

## Database

Apply `database/schema.sql` then `database/dataset.sql` to a MySQL server:

```
mysql -u root -p < database/schema.sql
mysql -u root -p < database/dataset.sql
```

Settings live in the `app_setting` table and can be overridden by
`config/settings.env`.

## Backend + Frontend

```
python backend/server.py
# open http://localhost:8080 in a browser
```

The backend seeds SQLite from `data/*.csv` and serves the JSON API under
`/api/*` (see `config/api.md`). The frontend shows catalogs, datasets,
entries and transactions with add/delete on transactions.
