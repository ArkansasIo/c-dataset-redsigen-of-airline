// main.cpp
// Program entry point (C++23).
//
// The original monolithic "access navigation control.cpp" has been split
// into these modules:
//   types.h            shared data model (catalog/dataset/entry/...)
//   util.h             input + allocation helpers
//   catalog.{h,cpp}    catalog linked list
//   dataset.{h,cpp}     dataset linked list + dataset menu
//   entry.{h,cpp}  entry linked list + menu
//   transaction.{h,cpp} transaction linked list + menu
//   index_key.{h,cpp} Index Key List + menu
//   storage.{h,cpp}    save/load database.dat
//   menus.{h,cpp}      top-level menu
#include "menus.h"
#include "storage.h"
#include "util.h"

int main()
{
    acs::catalog *start1 = acs::read_from_file();
    start1 = acs::catalog_menu(start1);
    return 0;
}

namespace acs
{
    // save_and_exit() is implemented in menus.cpp, which owns the root list
    // and the storage layer.
} // namespace acs
