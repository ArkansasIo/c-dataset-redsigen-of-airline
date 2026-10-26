// index_key.h
// Index key linked-list operations (C++23).
#pragma once

#include "types.h"

namespace acs
{

    index_key *create_index_key_node(index_key *start5);
    index_key *enter_index_key_info(index_key *node);
    index_key *cancelled_index_key_node(index_key *start5);
    index_key *index_key_menu(index_key *start5);
    void print_index_key_list(index_key *start5);

} // namespace acs
