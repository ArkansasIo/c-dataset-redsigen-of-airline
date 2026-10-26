#pragma once
#include "recordbase/hierarchy.hpp"
#include <string>
#include <vector>
#include <optional>

namespace recordbase {
struct Dataset { std::string id,name,source,target,date,start_time,end_time; std::int64_t total_rows=0,rows_indexed=0; };
struct Catalog { std::string id,name; };
struct Entry { std::string id,name,address,city; };
struct Transaction { std::string id,entry_id,dataset_id; double amount=0.0; };
struct IndexKey { std::string id,dataset_id,key; bool available=true; };
struct EngineConfig { std::string data_dir="data"; std::string database_dir="database"; std::string config_dir="config"; std::string log_level="info"; int api_port=8080; bool strict_validation=true; };
class DatasetEngine {
public:
 bool add_catalog(Catalog c); bool add_dataset(const std::string& catalog_id,Dataset d); bool add_entry(const std::string& dataset_id,Entry e); bool add_transaction(const std::string& dataset_id,Transaction t); bool add_index(const std::string& dataset_id,IndexKey k);
 bool remove(const std::string& id); const Dataset* dataset(const std::string& id) const; const Entry* entry(const std::string& id) const;
 bool validate() const; std::vector<const TreeNode*> datasets() const; const HierarchyTree& tree() const noexcept{return tree_;} HierarchyTree& tree() noexcept{return tree_;}
 void set_config(EngineConfig c){config_=std::move(c);} const EngineConfig& config() const noexcept{return config_;}
private: HierarchyTree tree_; EngineConfig config_{};
};
} // namespace recordbase
