// entry.h
// entry linked-list operations (C++23).
#pragma once

#include "types.h"

namespace acs
{

    entry *create_entry_node(entry *start3); // sorted insert by name
    entry *enter_entry_info(entry *node);    // interactive input
    entry *delete_entry_node(entry *start3); // delete by name
    entry *entry_menu(entry *start3);
    void print_entry_list(entry *start3);

} // namespace acs
