#include "recordbase/dataset_engine.hpp"
#include <iostream>
int main(){
 recordbase::DatasetEngine e;
 e.add_catalog({"air","Airline Catalog"});
 e.add_dataset("air",{"flight-001","Flight Dataset","flights","analytics","2026-10-26","08:00","10:00",100,80});
 e.add_entry("flight-001",{"pax-001","Stephen Example","1 Main St","Quebec"});
 e.add_transaction("flight-001",{"txn-001","pax-001","flight-001",249.99});
 e.add_index("flight-001",{"idx-001","flight-001","seat:1A",true});
 e.tree().walk([](const recordbase::TreeNode& n){std::cout<<n.id<<" ["<<n.name<<"] children="<<n.children.size()<<" records="<<n.records.size()<<'\n';});
 return e.validate()?0:1;
}
