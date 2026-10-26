-- =====================================================================
-- RecordBase - Full Sample Dataset
-- Loads demo data into every table:
--   catalog, dataset, entry, transaction_tbl,
--   tree_node -> tree_branch -> tree_sub_branch -> tree_module,
--   index_key, app_setting
-- =====================================================================
USE recordbase;

START TRANSACTION;

-- Catalogs (5) ----------------------------------------------------------
INSERT INTO catalog (catalogname, description) VALUES
  ('Personnel Records',  'Employee and contractor master data'),
  ('Financial Records',  'Invoices, ledger and payments'),
  ('Medical Records',    'Patient and visit records'),
  ('Inventory Records',  'Warehouse stock and parts'),
  ('Legal Records',      'Contracts and case files');

-- Datasets (10) -----------------------------------------------------------
INSERT INTO dataset (datasetkey, date_of_dataset, source_table, target_table,
                     start_time, end_time, total_rows, rows_indexed, status, catalog_id) VALUES
  ('PR101', '2026-11-01', 'employees_raw',    'employees',    '08:30', '10:15', 150, 2,  'DONE',    1),
  ('PR205', '2026-11-02', 'contractors_raw',  'contractors',  '13:00', '15:45', 150, 1,  'DONE',    1),
  ('PR310', '2026-11-05', 'payroll_raw',      'payroll',      '07:45', '11:20', 180, 0,  'PENDING', 1),
  ('FR120', '2026-11-01', 'invoices_raw',     'invoices',     '09:10', '11:05', 180, 3,  'DONE',    2),
  ('FR330', '2026-11-03', 'ledger_raw',       'ledger',       '16:40', '19:55', 180, 1,  'RUNNING', 2),
  ('FR410', '2026-11-08', 'payments_raw',     'payments',     '11:00', '12:30', 120, 0,  'PENDING', 2),
  ('MR777', '2026-11-04', 'patients_raw',     'patients',     '10:00', '13:35', 220, 5,  'DONE',    3),
  ('MR450', '2026-11-06', 'visits_raw',       'visits',       '06:15', '09:50', 200, 4,  'DONE',    3),
  ('IV610', '2026-11-09', 'stock_raw',        'stock',        '08:00', '10:00', 300, 2,  'RUNNING', 4),
  ('LG520', '2026-11-10', 'contracts_raw',    'contracts',    '14:00', '17:00', 250, 0,  'PENDING', 5);

-- Entries (16) -------------------------------------------------------------
INSERT INTO entry (entry_id, name_suffix, name, address, city, gender, date_of_birth, weight_kg) VALUES
  ('E0001', 'Mr.',   'John Smith',    '12 Oak Street',     'Little Rock',   'M', '1990-04-12', 78.00),
  ('E0002', 'Ms.',   'Emma Johnson',  '88 Maple Ave',      'Dallas',        'F', '1985-09-30', 62.00),
  ('E0003', 'Dr.',   'Ravi Patel',    '410 Cedar Rd',      'Denver',        'M', '1978-01-22', 80.00),
  ('E0004', 'Mrs.',  'Liu Chen',      '75 Birch Blvd',     'Chicago',       'F', '1992-07-08', 55.00),
  ('E0005', 'Mr.',   'Carlos Garcia', '9 Elm Court',       'Seattle',       'M', '1983-11-17', 85.00),
  ('E0006', 'Miss.', 'Ava Thompson',  '231 Willow Way',    'San Francisco', 'F', '1996-03-25', 58.00),
  ('E0007', 'Ms.',   'Grace Okafor',  '56 Redwood St',     'Honolulu',      'F', '1988-12-02', 60.00),
  ('E0008', 'Mr.',   'Ethan Brooks',  '301 Aspen Dr',      'Los Angeles',   'M', '1994-06-14', 75.00),
  ('E0009', 'Dr.',   'Nina Kovacs',   '18 Juniper Ln',     'Boston',        'F', '1981-05-19', 64.00),
  ('E0010', 'Mr.',   'Omar Haddad',   '77 Chestnut St',    'Austin',        'M', '1987-02-11', 82.00),
  ('E0011', 'Mrs.',  'Sofia Rossi',   '145 Spruce Ave',    'Miami',         'F', '1990-08-23', 57.00),
  ('E0012', 'Mr.',   'Liam Murphy',   '62 Walnut Ct',      'Portland',      'M', '1993-12-05', 88.00),
  ('E0013', 'Ms.',   'Hana Sato',     '208 Cypress Rd',    'San Jose',      'F', '1995-04-02', 52.00),
  ('E0014', 'Mr.',   'Noah Fischer',  '333 Magnolia Blvd', 'Houston',       'M', '1986-10-30', 91.00),
  ('E0015', 'Mrs.',  'Zara Ali',      '510 Poplar Way',    'Atlanta',       'F', '1991-03-14', 59.00),
  ('E0016', 'Dr.',   'Erik Lindberg', '94 Palm Grove',     'Phoenix',       'M', '1975-06-27', 84.00);

-- Transactions (18) ----------------------------------------------------------
INSERT INTO transaction_tbl (transaction_id, entry_id, datasetkey, amount) VALUES
  ('T0001', 'E0001', 'PR101', 245.00),
  ('T0002', 'E0002', 'PR101', 245.00),
  ('T0003', 'E0003', 'PR205', 310.50),
  ('T0004', 'E0004', 'FR120', 199.99),
  ('T0005', 'E0005', 'FR120', 199.99),
  ('T0006', 'E0006', 'FR120', 210.75),
  ('T0007', 'E0007', 'MR777', 489.00),
  ('T0008', 'E0008', 'MR777', 489.00),
  ('T0009', 'E0001', 'MR450', 175.40),
  ('T0010', 'E0004', 'MR450', 175.40),
  ('T0011', 'E0002', 'FR330', 275.00),
  ('T0012', 'E0003', 'PR310', 220.00),
  ('T0013', 'E0009', 'MR777', 512.25),
  ('T0014', 'E0010', 'IV610', 133.10),
  ('T0015', 'E0011', 'FR410', 98.80),
  ('T0016', 'E0012', 'LG520', 460.00),
  ('T0017', 'E0013', 'PR101', 245.00),
  ('T0018', 'E0016', 'MR450', 175.40);

-- Tree nodes (roots + subtrees) ----------------------------------------------
INSERT INTO tree_node (transaction_id, parent_node_id, node_name, node_type, index_key, available) VALUES
  ('T0001', NULL, 'Main Tree',      'ROOT',    'idx_12A', 1),
  ('T0001', 1,    'Record Group',   'SUBTREE', NULL,      1),
  ('T0002', NULL, 'Main Tree',      'ROOT',    'idx_12B', 1),
  ('T0003', NULL, 'Main Tree',      'ROOT',    'idx_02A', 1),
  ('T0004', NULL, 'Main Tree',      'ROOT',    'idx_07A', 1),
  ('T0005', NULL, 'Main Tree',      'ROOT',    'idx_07B', 1),
  ('T0006', NULL, 'Main Tree',      'ROOT',    'idx_07C', 1),
  ('T0007', NULL, 'Main Tree',      'ROOT',    'idx_21A', 1),
  ('T0008', NULL, 'Main Tree',      'ROOT',    'idx_21B', 1),
  ('T0009', NULL, 'Main Tree',      'ROOT',    'idx_10A', 1),
  ('T0010', NULL, 'Main Tree',      'ROOT',    'idx_10B', 1),
  ('T0011', NULL, 'Main Tree',      'ROOT',    'idx_03A', 1),
  ('T0013', NULL, 'Main Tree',      'ROOT',    'idx_21C', 1),
  ('T0014', NULL, 'Main Tree',      'ROOT',    'idx_31A', 1),
  ('T0015', NULL, 'Main Tree',      'ROOT',    'idx_41A', 1),
  ('T0016', NULL, 'Main Tree',      'ROOT',    'idx_51A', 1),
  ('T0017', NULL, 'Main Tree',      'ROOT',    'idx_12D', 1),
  ('T0018', NULL, 'Main Tree',      'ROOT',    'idx_10C', 1);

-- Branches ---------------------------------------------------------------------
INSERT INTO tree_branch (node_id, branch_name, status) VALUES
  (2,  'Archived Records',  'OPEN'),
  (2,  'Active Records',    'OPEN'),
  (4,  'Quick Lookup',      'OPEN'),
  (8,  'Standard Intake',   'OPEN'),
  (13, 'Overflow Intake',   'OPEN');

-- Sub-branches -------------------------------------------------------------------
INSERT INTO tree_sub_branch (branch_id, sub_branch_name, status) VALUES
  (1, 'Standard Record', 'OPEN'),
  (1, 'Oversize Record', 'OPEN'),
  (2, 'Temp Index',      'OPEN'),
  (3, 'Temp Index',      'OPEN'),
  (4, 'Intake Batch A',  'OPEN'),
  (5, 'Intake Batch B',  'CLOSED');

-- Modules (modular leaf units) ------------------------------------------------------
INSERT INTO tree_module (sub_branch_id, module_name, module_value) VALUES
  (1, 'retention_years',  '7'),
  (1, 'storage_fee',      '0.00'),
  (2, 'retention_years',  '10'),
  (2, 'storage_fee',      '75.00'),
  (3, 'ttl_days',         '30'),
  (4, 'ttl_days',         '14'),
  (5, 'batch_size',       '500'),
  (5, 'checksum',         'md5'),
  (6, 'batch_size',       '250'),
  (6, 'checksum',         'sha256');

-- Index keys (per dataset) -------------------------------------------------------------
INSERT INTO index_key (datasetkey, index_key, entry_id, available, amount) VALUES
  ('PR101', 'idx_12A', 'E0001', 0, 245.00),
  ('PR101', 'idx_12B', 'E0002', 0, 245.00),
  ('PR101', 'idx_12C', NULL,    1, 245.00),
  ('PR101', 'idx_12D', 'E0013', 0, 245.00),
  ('PR205', 'idx_02A', 'E0003', 0, 310.50),
  ('PR205', 'idx_02B', NULL,    1, 310.50),
  ('FR120', 'idx_07A', 'E0004', 0, 199.99),
  ('FR120', 'idx_07B', 'E0005', 0, 199.99),
  ('FR120', 'idx_07C', 'E0006', 0, 210.75),
  ('FR330', 'idx_03A', 'E0002', 0, 275.00),
  ('FR330', 'idx_03B', NULL,    1, 275.00),
  ('MR777', 'idx_21A', 'E0007', 0, 489.00),
  ('MR777', 'idx_21B', 'E0008', 0, 489.00),
  ('MR777', 'idx_21C', 'E0009', 0, 512.25),
  ('MR450', 'idx_10A', 'E0001', 0, 175.40),
  ('MR450', 'idx_10B', 'E0004', 0, 175.40),
  ('MR450', 'idx_10C', 'E0016', 0, 175.40),
  ('IV610', 'idx_31A', 'E0010', 0, 133.10),
  ('IV610', 'idx_31B', NULL,    1, 133.10),
  ('LG520', 'idx_51A', 'E0012', 0, 460.00),
  ('LG520', 'idx_51B', NULL,    1, 460.00);

COMMIT;

-- Settings ----------------------------------------------------------------
INSERT INTO app_setting (setting_key, setting_value) VALUES
  ('database.host',   'localhost'),
  ('database.port',   '3306'),
  ('database.name',   'recordbase'),
  ('api.base_url',    'http://localhost:8080/api/v1'),
  ('api.max_retries', '3'),
  ('rows.default',    '150'),
  ('currency',        'USD')
ON DUPLICATE KEY UPDATE setting_value = VALUES(setting_value);
