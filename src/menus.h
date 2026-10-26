// menus.h
// Top-level menu (catalog menu) and program entry point support (C++23).
#pragma once

#include "types.h"

namespace acs
{

    // The main menu. Returns the (possibly updated) head of the catalog list.
    catalog *catalog_menu(catalog *start1);

} // namespace acs
