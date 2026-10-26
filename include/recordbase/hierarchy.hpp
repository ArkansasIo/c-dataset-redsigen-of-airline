#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <optional>

namespace recordbase {
enum class NodeKind : std::uint8_t { Tree, Subtree, Branch, SubBranch, Node, SubNode, Module, SubModule, Data };
enum class DataType : std::uint8_t { Null, String, Integer, Decimal, Boolean };
struct DataValue { DataType type{DataType::Null}; std::string text; static DataValue string(std::string v); static DataValue integer(std::int64_t v); static DataValue decimal(double v); static DataValue boolean(bool v); };
struct DataRecord { std::string id; std::string name; std::unordered_map<std::string, DataValue> fields; };
struct TreeNode { std::string id; std::string name; NodeKind kind{NodeKind::Node}; std::optional<std::string> parent_id; std::vector<std::string> children; std::vector<DataRecord> records; };
class HierarchyTree {
public:
 bool create_root(std::string id,std::string name); bool add(std::string id,std::string name,NodeKind kind,const std::string& parent_id); bool remove(const std::string& id); bool contains(const std::string& id) const; const TreeNode* find(const std::string& id) const; TreeNode* find(const std::string& id); std::vector<const TreeNode*> children_of(const std::string& id) const; std::vector<const TreeNode*> descendants_of(const std::string& id) const; bool attach_record(const std::string& node_id,DataRecord record); std::vector<const TreeNode*> find_by_kind(NodeKind kind) const; bool validate() const; std::size_t size() const noexcept { return nodes_.size(); } void walk(const std::function<void(const TreeNode&)>& visitor) const; const std::string& root_id() const noexcept { return root_id_; }
private: bool remove_recursive(const std::string& id); std::unordered_map<std::string,TreeNode> nodes_; std::string root_id_;
};
} // namespace recordbase
