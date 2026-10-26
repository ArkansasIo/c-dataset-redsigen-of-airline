# RecordBase Hierarchical Data Model

The engine now models data recursively instead of limiting relationships to catalog -> dataset -> entry -> transaction.

```text
TREE: recordbase
└── SUBTREE
    └── BRANCH
        └── SUBBRANCH
            └── NODE
                └── SUBNODE
                    └── MODULE
                        └── SUBMODULE
                            └── DATA -> records -> fields
```

## Semantics

- **Tree**: global database root.
- **Subtree**: logical domain or partition.
- **Branch**: major catalog/module grouping.
- **Subbranch**: dataset or collection grouping.
- **Node**: an individual record entity such as an entry.
- **Subnode**: a dependent entity such as a transaction.
- **Module/Submodule**: extension points for application subsystems.
- **Data**: leaf storage for typed records.

The C++ `HierarchyTree` enforces unique node IDs, valid parent references, recursive traversal, recursive deletion, typed records, and structural validation. `DatasetEngine` maps catalogs, datasets, entries, transactions and indexes into this hierarchy.

## Invariants

1. Every non-root node has exactly one parent.
2. Parent child lists must agree with `parent_id`.
3. IDs are unique across the hierarchy.
4. The root is unique.
5. Deleting a parent recursively deletes descendants.
6. Transactions require an existing dataset and entry.
7. Data records require a valid owning node.
8. `validate()` must pass before a dataset is considered structurally valid.

## Database mapping

`database/hierarchy.sql` mirrors the in-memory tree with `tree_node`, `data_record`, and `data_value`, plus a dataset registry and application configuration table.
