// menus.cpp
// Top-level catalog menu (C++23).
#include "menus.h"
#include "catalog.h"
#include "storage.h"
#include "util.h"

#include <cstdio>
#include <cstdlib>

namespace acs
{

    catalog *catalog_menu(catalog *start1)
    {
        std::printf("Welcome to the Unknow Name catalog System Main Menu\n");
        std::printf("Please enter your choice from the menu shown below\n");
        std::printf("1: Add a new catalog to this database\n");
        std::printf("2: Delete a catalog from the database\n");
        std::printf("3: Display the Catalog List in this Database\n");
        std::printf("4: Enter dataset menu for an catalog\n");
        std::printf("5: Exit the program (saves to database.dat)\n");
        int choice;
        if (std::scanf("%d", &choice) != 1)
        {
            drain_stdin();
            choice = 0;
        }

        switch (choice)
        {
        case 1:
            start1 = create_catalog_node(start1);
            break;
        case 2:
            start1 = delete_catalog_node(start1);
            break;
        case 3:
            print_catalog_list(start1);
            break;
        case 4:
            start1 = enter_dataset_menu(start1);
            break;
        case 5:
            write_to_file(start1);
            std::exit(0);
        default:
            break;
        }

        std::printf("\n");
        return catalog_menu(start1);
    }

} // namespace acs
