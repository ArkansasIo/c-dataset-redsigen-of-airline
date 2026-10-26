# RecordBase - Transaction Tree API (v1)

Base URL: `http://localhost:8080/api` (see `backend/server.py` and `config/config.json`)

## Tree structure

```
transaction (tree root)
└── tree_node        (sub-node / subtree)
    └── tree_branch  (branch)
        └── tree_sub_branch (sub-branch)
            └── tree_module (modular leaf)
```

## Endpoints (implemented in backend/server.py)

| Method | Path | Description |
|--------|------|-------------|
| GET    | `/catalogs` | List all catalogs |
| GET    | `/datasets` | All datasets, sorted by start time |
| GET    | `/entries` | All entries, sorted by name |
| GET    | `/transactions` | List all transactions |
| GET    | `/transactions/{id}/tree` | Transaction with its index keys |
| POST   | `/transactions` | Create transaction. Body: `{ "transaction_id", "entry_id", "dataset_id", "amount" }` |
| DELETE | `/transactions/{id}` | Delete transaction |
| GET    | `/stats` | Row counts + total amount |

## Full API spec (MySQL-backed, reference)

### Transactions (tree)
| Method | Path | Description |
|--------|------|-------------|
| GET    | `/transactions` | List all transactions |
| GET    | `/transactions/{id}` | Get one transaction |
| POST   | `/transactions` | Create transaction. Body: `{ "entry_id", "datasetkey", "amount" }` |
| DELETE | `/transactions/{id}` | Delete transaction and its whole tree |

### Sub-nodes / subtrees
| Method | Path | Description |
|--------|------|-------------|
| GET    | `/transactions/{id}/nodes` | List sub-nodes of a transaction |
| POST   | `/transactions/{id}/nodes` | Add sub-node. Body: `{ "node_name", "node_type", "parent_node_id", "index_key" }` |
| PATCH  | `/nodes/{node_id}` | Update sub-node (e.g. toggle `available`) |
| DELETE | `/nodes/{node_id}` | Delete sub-node and everything under it |

### Branches / Sub-branches / Modules
| Method | Path | Description |
|--------|------|-------------|
| GET    | `/nodes/{node_id}/branches` | List branches under a sub-node |
| POST   | `/nodes/{node_id}/branches` | Add branch. Body: `{ "branch_name", "status" }` |
| DELETE | `/branches/{branch_id}` | Delete branch |
| POST   | `/branches/{branch_id}/sub-branches` | Add sub-branch. Body: `{ "sub_branch_name" }` |
| DELETE | `/sub-branches/{id}` | Delete sub-branch |
| POST   | `/sub-branches/{id}/modules` | Add module. Body: `{ "module_name", "module_value" }` |
| DELETE | `/modules/{module_id}` | Delete module |

### Datasets, entries & settings
| Method | Path | Description |
|--------|------|-------------|
| GET    | `/datasets` | All datasets, sorted by start time |
| GET    | `/datasets/{id}/entries` | Entries on a dataset, sorted by name |
| GET    | `/settings` | Current settings (from `app_setting` table) |
| PUT    | `/settings` | Update settings. Body: `{ "key": "value", ... }` |

## Business rules
- A transaction requires an **existing** entry ID and dataset ID.
- Entry and dataset unique IDs are never duplicated.
- Deleting a parent node cascades to its branches/sub-branches/modules.

## Example: full tree response

```json
GET /api/transactions/T0001/tree
{
  "transaction": {
    "transaction_id": "T0001",
    "entry_id": "E0001",
    "dataset_id": "PR101",
    "amount": 245.00
  },
  "index_keys": [
    { "index_key": "idx_12A", "entry_id": "E0001", "available": 0, "amount": 245.00 },
    { "index_key": "idx_12B", "entry_id": "E0002", "available": 0, "amount": 245.00 }
  ]
}
```
