#include "recordbase/hierarchy.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace recordbase {
DataValue DataValue::string(std::string v){return {DataType::String,std::move(v)};}
DataValue DataValue::integer(std::int64_t v){return {DataType::Integer,std::to_string(v)};}
DataValue DataValue::decimal(double v){std::ostringstream s;s<<std::setprecision(15)<<v;return {DataType::Decimal,s.str()};}
DataValue DataValue::boolean(bool v){return {DataType::Boolean,v?"true":"false"};}

bool HierarchyTree::create_root(std::string id,std::string name){ if(id.empty()||nodes_.contains(id)||!root_id_.empty()) return false; TreeNode n{std::move(id),std::move(name),NodeKind::Tree,std::nullopt,{},{}}; root_id_=n.id; nodes_.emplace(n.id,std::move(n)); return true; }
bool HierarchyTree::add(std::string id,std::string name,NodeKind kind,const std::string& parent_id){ if(id.empty()||nodes_.contains(id)||!nodes_.contains(parent_id)) return false; auto &p=nodes_.at(parent_id); TreeNode n{std::move(id),std::move(name),kind,parent_id,{},{}}; p.children.push_back(n.id); nodes_.emplace(n.id,std::move(n)); return true; }
const TreeNode* HierarchyTree::find(const std::string& id) const { auto i=nodes_.find(id); return i==nodes_.end()?nullptr:&i->second; }
TreeNode* HierarchyTree::find(const std::string& id){ auto i=nodes_.find(id); return i==nodes_.end()?nullptr:&i->second; }
bool HierarchyTree::contains(const std::string& id) const{return nodes_.contains(id);}
std::vector<const TreeNode*> HierarchyTree::children_of(const std::string& id) const { std::vector<const TreeNode*> r; auto n=find(id); if(!n)return r; for(auto& c:n->children) if(auto x=find(c))r.push_back(x); return r; }
std::vector<const TreeNode*> HierarchyTree::descendants_of(const std::string& id) const { std::vector<const TreeNode*> r; std::function<void(const TreeNode*)> f=[&](const TreeNode*n){for(auto& c:n->children)if(auto x=find(c)){r.push_back(x);f(x);}}; if(auto n=find(id))f(n); return r; }
bool HierarchyTree::attach_record(const std::string& node_id,DataRecord record){auto n=find(node_id);if(!n||record.id.empty())return false; if(std::any_of(n->records.begin(),n->records.end(),[&](const auto&r){return r.id==record.id;}))return false;n->records.push_back(std::move(record));return true;}
std::vector<const TreeNode*> HierarchyTree::find_by_kind(NodeKind kind) const {std::vector<const TreeNode*>r;for(auto&[id,n]:nodes_)if(n.kind==kind)r.push_back(&n);return r;}
bool HierarchyTree::remove_recursive(const std::string&id){auto n=find(id);if(!n||id==root_id_)return false;auto parent=n->parent_id;if(parent&&nodes_.contains(*parent)){auto&v=nodes_.at(*parent).children;v.erase(std::remove(v.begin(),v.end(),id),v.end());}auto children=n->children;for(auto&c:children)remove_recursive(c);nodes_.erase(id);return true;}
bool HierarchyTree::remove(const std::string&id){return remove_recursive(id);}
bool HierarchyTree::validate() const {if(root_id_.empty()||!nodes_.contains(root_id_))return false;std::unordered_map<std::string,bool> seen;std::function<bool(const TreeNode&)> f=[&](const TreeNode&n){if(seen[n.id])return false;seen[n.id]=true;for(auto&c:n.children){auto i=nodes_.find(c);if(i==nodes_.end()||i->second.parent_id!=n.id||!f(i->second))return false;}return true;};if(!f(nodes_.at(root_id_)))return false;return seen.size()==nodes_.size();}
void HierarchyTree::walk(const std::function<void(const TreeNode&)>& visitor) const {if(auto n=find(root_id_)){std::function<void(const TreeNode&)>f=[&](const TreeNode&x){visitor(x);for(auto&c:x.children)if(auto y=find(c))f(*y);};f(*n);}}
} // namespace recordbase
