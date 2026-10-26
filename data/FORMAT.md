# RecordBase - Data File Formats

All files live in `data/` and use UTF-8 CSV with a header row on line 1.
Comma is the field separator; fields may be double-quoted.

## catalogs.csv
| Column | Type | Notes |
|--------|------|-------|
| catalogname | string | unique catalog name |

## datasets.csv
| Column | Type | Notes |
|--------|------|-------|
| catalogname | string | references catalogs.csv |
| dataset_id | string | **unique key** |
| date_of_dataset | YYYY-MM-DD | |
| source_table | string | source table name |
| target_table | string | target table name |
| start_time | HH:MM | job start time |
| end_time | HH:MM | job end time |
| total_rows | int | capacity |
| rows_indexed | int | must be <= total_rows |

## entries.csv
| Column | Type | Notes |
|--------|------|-------|
| name_suffix | string | Mr., Mrs., Dr., ... |
| name | string | full name (First Last) |
| entry_id | string | **unique key** (E0001...) |
| address | string | |
| city | string | |
| gender | M/F/O | |
| data_of_birth | YYYY-MM-DD | |
| weight | string | e.g. 78kg |

## transactions.csv
| Column | Type | Notes |
|--------|------|-------|
| transaction_id | string | **unique key** (T0001...) |
| entry_id | string | must exist in entries.csv |
| dataset_id | string | must exist in datasets.csv |
| amount | decimal | transaction amount |

## indexes.csv
| Column | Type | Notes |
|--------|------|-------|
| dataset_id | string | must exist in datasets.csv |
| index_key | string | key id, e.g. idx_12A |
| entry_id | string | empty if the key is free |
| available | 0/1 | 1 = key free, 0 = key taken |
| amount | decimal | amount for the key |

## Integrity rules
1. Transaction rows must reference an existing `entry_id` and `dataset_id`.
2. Unique keys (`dataset_id`, `entry_id`, `transaction_id`) must not repeat.
3. `rows_indexed` on a dataset should equal the number of rows for that dataset
   with `available = 0` in indexes.csv.

## Loading order
1. `catalogs.csv`
2. `datasets.csv`
3. `entries.csv`
4. `transactions.csv`
5. `indexes.csv`

The loader in `src/csv_loader.h` follows this order automatically.
