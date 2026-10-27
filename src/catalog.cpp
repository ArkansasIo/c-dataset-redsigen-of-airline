// catalog.cpp
// catalog linked-list operations (C++23).
#include "catalog.h"
#include "dataset.h"
#include "util.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace acs
{

    catalog *enter_catalog_info(catalog *node)
    {
        std::printf("\n");
        std::printf(" Welcome To RecordBase Database System\n");
        std::printf("Please enter the catalog Name\n");
        std::printf("\n");
        node->catalogname = new char[kMaxNameLen];
        read_line(node->catalogname, static_cast<int>(kMaxNameLen));
        node->node1 = nullptr;
        return node;
    }

    catalog *create_catalog_node(catalog *start1)
    {
        if (start1 == nullptr)
        {
            start1 = static_cast<catalog *>(std::malloc(sizeof(catalog)));
            start1 = enter_catalog_info(start1);
            start1->next = nullptr;
            return start1;
        }

        catalog *count_node1, *count_node2 = nullptr, *node;
        node = static_cast<catalog *>(std::malloc(sizeof(catalog)));
        node = enter_catalog_info(node);

        for (count_node1 = start1;
             count_node1 != nullptr &&
             std::strcmp(count_node1->catalogname, node->catalogname) <= 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 == start1)
        {
            node->next = start1;
            start1 = node;
        }
        else if (count_node1 == nullptr)
        {
            count_node2->next = node;
            node->next = nullptr;
        }
        else
        {
            count_node2->next = node;
            node->next = count_node1;
        }
        return start1;
    }

    catalog *delete_catalog_node(catalog *start1)
    {
        if (start1 == nullptr)
        {
            std::printf("\nSorry, this Database is not operational at this moment\n");
            return start1;
        }

        char catalogname[80];
        std::printf("\nPlease enter the  name of the catalog you want to delete\n");
        read_line(catalogname, 80);

        catalog *count_node1, *count_node2 = nullptr, *temp;
        for (count_node1 = start1;
             count_node1 != nullptr &&
             std::strcmp(count_node1->catalogname, catalogname) != 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 != nullptr)
        {
            if (count_node1 == start1)
            {
                temp = start1;
                start1 = start1->next;
                std::free(temp);
            }
            else if (count_node1->next == nullptr)
            {
                temp = count_node1;
                count_node2->next = nullptr;
                std::free(temp);
            }
            else
            {
                temp = count_node1;
                count_node2->next = count_node1->next;
                std::free(temp);
            }
        }
        else
        {
            std::printf("\nSorry, this catalog is currently not in this database\n");
        }
        return start1;
    }

    void print_catalog_list(catalog *start1)
    {
        std::printf("\n");
        if (start1 == nullptr)
        {
            std::printf("Sorry, this Database is not operational at this moment\n");
            return;
        }
        for (catalog *count = start1; count != nullptr; count = count->next)
            std::printf("%s\n", count->catalogname);
    }

    catalog *enter_dataset_menu(catalog *start1)
    {
        char catalogname[80];
        std::printf("\nWelcome to the dataset List Menu\n");
        std::printf("Enter the catalog name you wish to check\n");
        read_line(catalogname, 80);

        catalog *count;
        for (count = start1;
             count != nullptr &&
             std::strcmp(count->catalogname, catalogname) != 0;
             count = count->next)
        {
        }

        if (count == nullptr)
        {
            std::printf("\nSorry, this catalog does not exist in this database\n");
            return start1;
        }
        std::printf("\n");
        count->node1 = dataset_menu(count->node1);
        return start1;
    }

} // namespace acs
