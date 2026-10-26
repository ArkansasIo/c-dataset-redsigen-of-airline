// catalog.h
// catalog linked-list operations (C++23).
#pragma once

#include "types.h"

namespace acs
{

    catalog *create_catalog_node(catalog *start1); // sorted insert by name
    catalog *enter_catalog_info(catalog *node);    // interactive input
    catalog *delete_catalog_node(catalog *start1); // delete by name
    void print_catalog_list(catalog *start1);
    catalog *enter_dataset_menu(catalog *start1); // descend into dataset menu

} // namespace acs
