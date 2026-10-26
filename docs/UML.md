# RecordBase - UML Diagrams

## 1. Class diagram (C++ core, `src/types.h`)

```
+--------------+       +----------------+       +--------------+
|   catalog    | 1   * |    dataset     | 1   * |    entry     |
+--------------+-------+----------------+-------+--------------+
| catalogname  |       | dataset_id (UK)|       | entry_id(UK) |
| next         |       | date_of_dataset|       | name_suffix  |
| node1        |       | source_table   |       | name         |
+--------------+       | target_table   |       | address      |
                       | start_time     |       | city         |
                       | end_time       |       | gender       |
                       | total_rows     |       | data_of_birth|
                       | rows_indexed   |       | weight       |
                       | next, node2,   |       | next         |
                       | node3          |       +--------------+
                       +----------------+
                                | 1
                                v *
                       +----------------+
                       |  transaction   |
                       +----------------+
                       | transaction_id |  (UK)
                       | entry_id       |
                       | dataset_id     |
                       | amount (cost)  |
                       | next           |
                       +----------------+

   dataset 1 --- * index_key { key_name, amount, available }
```

## 2. Database tree hierarchy (MySQL / Lua OO)

```
transaction_tbl (root)
  └── tree_node            (sub-node / subtree, self-referencing parent_node_id)
        └── tree_branch    (branch)
              └── tree_sub_branch (sub-branch)
                    └── tree_module (modular leaf)
```

## 3. Domain mapping (old airline -> new database)

| Old (airline program) | New (RecordBase) |
|-----------------------|------------------|
| airline               | catalog          |
| flight                | dataset          |
| passenger             | entry            |
| reservation           | transaction      |
| seat_assignment       | index_key        |
| airline.txt           | database.dat     |
| airport_control (DB)  | recordbase (DB)  |

## 4. System components

```
+-------------------+     +----------------+     +----------------+
| database          |     | backend/       |     | web/           |
| navigation        | --> | server.py      | <-- | index.html     |
| control.cpp (C++) |     | REST + SQLite  |     | browser UI     |
+-------------------+     +----------------+     +----------------+
        |                        |
        v                        v
+-------------------+     +----------------+
| src/* (C++23)     |     | database/*.sql |
| lib/dbtree.lua    |     | MySQL schema + |
| scripts/demo.lua  |     | dataset        |
+-------------------+     +----------------+
```

## 5. Use cases

```
            +--------------------+
            |    RecordBase      |
            +--------------------+
   Operator --> (Manage catalogs)     |
            --> (Manage datasets)     |-- Backend API
   Clerk    --> (Add transactions)    |
            --> (Browse entries)      |
   Admin    --> (Save/load database.dat)
            --> (Run Lua scripts)
```
