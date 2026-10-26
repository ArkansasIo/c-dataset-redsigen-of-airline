CREATE DATABASE IF NOT EXISTS recordbase CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE recordbase;
CREATE TABLE IF NOT EXISTS tree_node (
 id VARCHAR(128) PRIMARY KEY,
 parent_id VARCHAR(128) NULL,
 name VARCHAR(255) NOT NULL,
 kind ENUM('tree','subtree','branch','subbranch','node','subnode','module','submodule','data') NOT NULL,
 depth INT NOT NULL DEFAULT 0,
 created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
 CONSTRAINT fk_tree_parent FOREIGN KEY(parent_id) REFERENCES tree_node(id) ON DELETE CASCADE
);
CREATE INDEX ix_tree_parent ON tree_node(parent_id);
CREATE INDEX ix_tree_kind ON tree_node(kind);
CREATE TABLE IF NOT EXISTS data_record (
 id VARCHAR(128) PRIMARY KEY,
 node_id VARCHAR(128) NOT NULL,
 name VARCHAR(255) NOT NULL,
 created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
 CONSTRAINT fk_record_node FOREIGN KEY(node_id) REFERENCES tree_node(id) ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS data_value (
 record_id VARCHAR(128) NOT NULL,
 field_name VARCHAR(128) NOT NULL,
 data_type ENUM('null','string','integer','decimal','boolean') NOT NULL,
 value_text TEXT NULL,
 PRIMARY KEY(record_id,field_name),
 CONSTRAINT fk_value_record FOREIGN KEY(record_id) REFERENCES data_record(id) ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS app_config (
 config_key VARCHAR(128) PRIMARY KEY,
 config_value TEXT NOT NULL,
 updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
CREATE TABLE IF NOT EXISTS dataset_registry (
 dataset_id VARCHAR(128) PRIMARY KEY,
 node_id VARCHAR(128) NOT NULL,
 source_table VARCHAR(255), target_table VARCHAR(255), total_rows BIGINT NOT NULL DEFAULT 0,
 rows_indexed BIGINT NOT NULL DEFAULT 0,
 CONSTRAINT fk_dataset_node FOREIGN KEY(node_id) REFERENCES tree_node(id) ON DELETE CASCADE
);
