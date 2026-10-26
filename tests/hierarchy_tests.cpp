#include "recordbase/dataset_engine.hpp"
#include <cassert>
int main(){
 recordbase::HierarchyTree t;
 assert(t.create_root("root","Root"));
 assert(t.add("subtree","Subtree",recordbase::NodeKind::Subtree,"root"));
 assert(t.add("branch","Branch",recordbase::NodeKind::Branch,"subtree"));
 assert(t.add("subbranch","Subbranch",recordbase::NodeKind::SubBranch,"branch"));
 assert(t.add("node","Node",recordbase::NodeKind::Node,"subbranch"));
 assert(t.add("subnode","Subnode",recordbase::NodeKind::SubNode,"node"));
 assert(t.add("module","Module",recordbase::NodeKind::Module,"subnode"));
 assert(t.add("submodule","Submodule",recordbase::NodeKind::SubModule,"module"));
 assert(t.add("data","Data",recordbase::NodeKind::Data,"submodule"));
 assert(t.attach_record("data",{"r1","record",{{"value",recordbase::DataValue::string("ok")}}}));
 assert(t.validate());
 assert(t.descendants_of("root").size()==8);
 assert(t.remove("subtree"));
 assert(t.size()==1);
 assert(t.validate());
 return 0;
}
