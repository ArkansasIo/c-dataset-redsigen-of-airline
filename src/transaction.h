// transaction.h
// transaction linked-list operations (C++23).
#pragma once

#include "types.h"

namespace acs
{

    transaction *create_transaction_node(transaction *start4); // sorted insert by id
    transaction *enter_transaction_info(transaction *node);    // interactive input
    transaction *delete_transaction_node(transaction *start4); // delete by id
    transaction *transaction_menu(transaction *start4);
    void print_transaction_list(transaction *start4);

} // namespace acs
