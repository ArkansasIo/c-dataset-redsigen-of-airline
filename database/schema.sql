-- =====================================================================
-- RecordBase / Records Database Schema (full)
-- Database hierarchy:
--   transaction (root / tree)
--     -> tree_node             (sub-node / subtree)
--        -> tree_branch        (branch)
--           -> tree_sub_branch (sub-branch)
--              -> tree_module (modular leaf units)
-- Plus: index_key, audit_log, app_setting, views, triggers, procedures
-- =====================================================================

DROP DATABASE IF EXISTS recordbase;
CREATE DATABASE recordbase
  CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE recordbase;

-- ---------------------------------------------------------------------
-- Core entity tables (unique keys, no duplicated data)
-- ---------------------------------------------------------------------
CREATE TABLE catalog (
  catalog_id     INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  catalogname    VARCHAR(80)  NOT NULL UNIQUE,
  description    VARCHAR(255),
  created_at     TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

CREATE TABLE dataset (
  dataset_id      INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  datasetkey      VARCHAR(80)  NOT NULL UNIQUE,
  date_of_dataset DATE         NOT NULL,
  source_table    VARCHAR(80)  NOT NULL,
  target_table    VARCHAR(80)  NOT NULL,
  start_time      TIME         NOT NULL,
  end_time        TIME         NOT NULL,
  total_rows      INT UNSIGNED NOT NULL DEFAULT 0,
  rows_indexed    INT UNSIGNED NOT NULL DEFAULT 0,
  status          ENUM('PENDING','RUNNING','DONE','FAILED') NOT NULL DEFAULT 'PENDING',
  catalog_id      INT UNSIGNED NOT NULL,
  CONSTRAINT fk_dataset_catalog
    FOREIGN KEY (catalog_id) REFERENCES catalog(catalog_id)
    ON DELETE CASCADE,
  INDEX idx_dataset_start (date_of_dataset, start_time)
) ENGINE=InnoDB;

CREATE TABLE entry (
  entry_id      VARCHAR(20)  PRIMARY KEY,          -- unique key
  name_suffix   VARCHAR(6),
  name          VARCHAR(120) NOT NULL,
  address       VARCHAR(200),
  city          VARCHAR(60),
  gender        ENUM('M','F','O'),
  date_of_birth DATE,
  weight_kg     DECIMAL(5,2),
  created_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  INDEX idx_entry_name (name)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- Transaction root (tree)
-- ---------------------------------------------------------------------
CREATE TABLE transaction_tbl (
  transaction_id VARCHAR(20) PRIMARY KEY,           -- unique key
  entry_id       VARCHAR(20) NOT NULL,
  datasetkey     VARCHAR(80) NOT NULL,
  amount         DECIMAL(10,2) NOT NULL DEFAULT 0.00,
  created_at     TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT fk_tx_entry
    FOREIGN KEY (entry_id) REFERENCES entry(entry_id),
  CONSTRAINT fk_tx_dataset
    FOREIGN KEY (datasetkey) REFERENCES dataset(datasetkey)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- Tree -> sub-node (subtree), self-referencing for nested subtrees
-- ---------------------------------------------------------------------
CREATE TABLE tree_node (
  node_id        INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  transaction_id VARCHAR(20)  NOT NULL,
  parent_node_id INT UNSIGNED NULL,                 -- allows nested subtrees
  node_name      VARCHAR(80)  NOT NULL,
  node_type      ENUM('ROOT','SUBTREE','BRANCH','SUB_BRANCH','MODULE')
                 NOT NULL DEFAULT 'SUBTREE',
  index_key      VARCHAR(80),
  available      TINYINT(1) NOT NULL DEFAULT 1,
  CONSTRAINT fk_node_transaction
    FOREIGN KEY (transaction_id) REFERENCES transaction_tbl(transaction_id)
    ON DELETE CASCADE,
  CONSTRAINT fk_node_parent
    FOREIGN KEY (parent_node_id) REFERENCES tree_node(node_id)
    ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE tree_branch (
  branch_id   INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  node_id     INT UNSIGNED NOT NULL,
  branch_name VARCHAR(80) NOT NULL,
  status      ENUM('OPEN','CLOSED','CANCELLED') NOT NULL DEFAULT 'OPEN',
  CONSTRAINT fk_branch_node
    FOREIGN KEY (node_id) REFERENCES tree_node(node_id)
    ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE tree_sub_branch (
  sub_branch_id   INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  branch_id       INT UNSIGNED NOT NULL,
  sub_branch_name VARCHAR(80) NOT NULL,
  status          ENUM('OPEN','CLOSED','CANCELLED') NOT NULL DEFAULT 'OPEN',
  CONSTRAINT fk_subbranch_branch
    FOREIGN KEY (branch_id) REFERENCES tree_branch(branch_id)
    ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE tree_module (
  module_id      INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  sub_branch_id  INT UNSIGNED NOT NULL,
  module_name    VARCHAR(80) NOT NULL,
  module_value   VARCHAR(255),
  created_at     TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT fk_module_subbranch
    FOREIGN KEY (sub_branch_id) REFERENCES tree_sub_branch(sub_branch_id)
    ON DELETE CASCADE
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- Index keys: total rows and rows indexed per dataset
-- ---------------------------------------------------------------------
CREATE TABLE index_key (
  key_id     INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  datasetkey VARCHAR(80) NOT NULL,
  index_key  VARCHAR(80) NOT NULL,
  entry_id   VARCHAR(20) NULL,          -- NULL when the key is free
  available  TINYINT(1) NOT NULL DEFAULT 1,
  amount     DECIMAL(10,2) NOT NULL DEFAULT 0.00,
  CONSTRAINT fk_key_dataset
    FOREIGN KEY (datasetkey) REFERENCES dataset(datasetkey)
    ON DELETE CASCADE,
  CONSTRAINT fk_key_entry
    FOREIGN KEY (entry_id) REFERENCES entry(entry_id)
    ON DELETE SET NULL,
  UNIQUE KEY uq_dataset_key (datasetkey, index_key)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- Audit log (populated by triggers)
-- ---------------------------------------------------------------------
CREATE TABLE audit_log (
  log_id     INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  log_table  VARCHAR(40) NOT NULL,
  log_action VARCHAR(10) NOT NULL,
  log_row    VARCHAR(40) NOT NULL,
  logged_at  TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- Configuration / settings / options (used by config.json + API)
-- ---------------------------------------------------------------------
CREATE TABLE app_setting (
  setting_key   VARCHAR(80) PRIMARY KEY,
  setting_value VARCHAR(255) NOT NULL,
  updated_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP
                 ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- Triggers: audit transactions
-- ---------------------------------------------------------------------
DELIMITER //
CREATE TRIGGER trg_tx_insert AFTER INSERT ON transaction_tbl
FOR EACH ROW
BEGIN
  INSERT INTO audit_log (log_table, log_action, log_row)
  VALUES ('transaction_tbl', 'INSERT', NEW.transaction_id);
END //

CREATE TRIGGER trg_tx_delete AFTER DELETE ON transaction_tbl
FOR EACH ROW
BEGIN
  INSERT INTO audit_log (log_table, log_action, log_row)
  VALUES ('transaction_tbl', 'DELETE', OLD.transaction_id);
END //
DELIMITER ;

-- ---------------------------------------------------------------------
-- Stored procedure: dataset summary
-- ---------------------------------------------------------------------
DELIMITER //
CREATE PROCEDURE sp_dataset_summary(IN p_datasetkey VARCHAR(80))
BEGIN
  SELECT d.datasetkey,
         c.catalogname,
         d.date_of_dataset,
         d.total_rows,
         d.rows_indexed,
         COUNT(t.transaction_id) AS transaction_count,
         COALESCE(SUM(t.amount), 0) AS total_amount
  FROM dataset d
  JOIN catalog c ON c.catalog_id = d.catalog_id
  LEFT JOIN transaction_tbl t ON t.datasetkey = d.datasetkey
  WHERE d.datasetkey = p_datasetkey
  GROUP BY d.datasetkey, c.catalogname, d.date_of_dataset,
           d.total_rows, d.rows_indexed;
END //
DELIMITER ;

-- ---------------------------------------------------------------------
-- Views
-- ---------------------------------------------------------------------
CREATE OR REPLACE VIEW v_transaction_tree AS
SELECT r.transaction_id,
       r.entry_id,
       r.datasetkey,
       r.amount,
       n.node_id,
       n.parent_node_id,
       n.node_name,
       n.node_type,
       b.branch_id,
       b.branch_name,
       sb.sub_branch_id,
       sb.sub_branch_name,
       m.module_id,
       m.module_name,
       m.module_value
FROM transaction_tbl r
LEFT JOIN tree_node n          ON n.transaction_id = r.transaction_id
LEFT JOIN tree_branch b        ON b.node_id = n.node_id
LEFT JOIN tree_sub_branch sb   ON sb.branch_id = b.branch_id
LEFT JOIN tree_module m        ON m.sub_branch_id = sb.sub_branch_id;

CREATE OR REPLACE VIEW v_dataset_summary AS
SELECT d.datasetkey,
       c.catalogname,
       d.date_of_dataset,
       d.start_time,
       d.end_time,
       d.total_rows,
       d.rows_indexed,
       COUNT(t.transaction_id) AS transaction_count,
       COALESCE(SUM(t.amount), 0) AS total_amount
FROM dataset d
JOIN catalog c ON c.catalog_id = d.catalog_id
LEFT JOIN transaction_tbl t ON t.datasetkey = d.datasetkey
GROUP BY d.datasetkey, c.catalogname, d.date_of_dataset,
         d.start_time, d.end_time, d.total_rows, d.rows_indexed;

CREATE OR REPLACE VIEW v_entry_transactions AS
SELECT e.entry_id,
       e.name,
       COUNT(t.transaction_id) AS transaction_count,
       COALESCE(SUM(t.amount), 0) AS total_amount
FROM entry e
LEFT JOIN transaction_tbl t ON t.entry_id = e.entry_id
GROUP BY e.entry_id, e.name;
