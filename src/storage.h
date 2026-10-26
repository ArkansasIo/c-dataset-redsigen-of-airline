// storage.h
// Save/load the linked lists to and from database.dat (C++23).
#pragma once

#include "types.h"

namespace acs
{

    // Save all lists to "database.dat" (length-prefixed binary format).
    void write_to_file(catalog *start1);

    // Load the catalog list (and nested lists) from "database.dat".
    // Returns the head of the catalog list, or nullptr if no file exists.
    catalog *read_from_file();

} // namespace acs
