// dataset.h
// dataset linked-list operations (C++23).
#pragma once

#include "types.h"

namespace acs
{

    dataset *create_dataset_node(dataset *start2); // sorted insert by dataset number
    dataset *enter_dataset_info(dataset *node);    // interactive input
    dataset *delete_dataset_node(dataset *start2); // delete by dataset number
    void print_dataset_list(dataset *start2);
    dataset *enter_entry_menu(dataset *start2);   // descend into entry menu
    dataset *enter_transaction_menu(dataset *start2); // descend into transaction menu
    dataset *dataset_menu(dataset *start2);            // dataset list menu

} // namespace acs
