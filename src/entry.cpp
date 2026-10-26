// entry.cpp
// entry linked-list operations (C++23).
#include "entry.h"
#include "util.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace acs
{

    entry *enter_entry_info(entry *node)
    {
        char buf[kMaxNameLen];

        std::printf(" Welcome to the entry List\n");

        std::printf("Please enter entry Name Suffix\n");
        drain_stdin();
        node->name_suffix = new char[kMaxSuffix];
        read_line(node->name_suffix, static_cast<int>(kMaxSuffix));

        std::printf("Please enter entry Full Name\n");
        drain_stdin();
        node->name = new char[kMaxNameLen];
        read_line(node->name, static_cast<int>(kMaxNameLen));

        std::printf("Please enter entry ID\n"); // unique key
        drain_stdin();
        node->entry_id = new char[kMaxIdLen];
        read_line(node->entry_id, static_cast<int>(kMaxIdLen));

        std::printf("Please enter entry Address\n");
        drain_stdin();
        node->address = new char[kMaxNameLen];
        read_line(node->address, static_cast<int>(kMaxNameLen));

        std::printf("Please enter entry City\n");
        drain_stdin();
        node->city = new char[30];
        read_line(node->city, 30);

        std::printf("Please enter Gender\n");
        drain_stdin();
        node->gender = new char[10];
        read_line(node->gender, 10);

        std::printf("Please enter Date Of Birth\n");
        drain_stdin();
        node->data_of_birth = new char[10];
        read_line(node->data_of_birth, 10);

        std::printf("Please enter Weight\n");
        drain_stdin();
        node->weight = new char[10];
        read_line(node->weight, 10);

        return node;
    }

    entry *create_entry_node(entry *start3)
    {
        std::printf("\n");
        if (start3 == nullptr)
        {
            start3 = static_cast<entry *>(std::malloc(sizeof(entry)));
            start3 = enter_entry_info(start3);
            start3->next = nullptr;
            return start3;
        }

        entry *count_node1, *count_node2 = nullptr, *node;
        node = static_cast<entry *>(std::malloc(sizeof(entry)));
        node = enter_entry_info(node);

        for (count_node1 = start3;
             count_node1 != nullptr &&
             std::strcmp(count_node1->name, node->name) <= 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 == start3)
        {
            node->next = start3;
            start3 = node;
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
        return start3;
    }

    entry *delete_entry_node(entry *start3)
    {
        std::printf("\n");
        if (start3 == nullptr)
        {
            std::printf("Sorry, the entry list is empty\n");
            return start3;
        }

        char name[80];
        std::printf("Please enter the name of the entry you want to delete\n");
        drain_stdin();
        read_line(name, 80);

        entry *count_node1, *count_node2 = nullptr, *temp;
        for (count_node1 = start3;
             count_node1 != nullptr &&
             std::strcmp(count_node1->name, name) != 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 != nullptr)
        {
            if (count_node1 == start3)
            {
                temp = start3;
                start3 = start3->next;
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
            std::printf("\nSorry, this name was not found in this list\n");
        }
        return start3;
    }

    void print_entry_list(entry *start3)
    {
        std::printf("\n");
        if (start3 == nullptr)
        {
            std::printf("Sorry, the entry list is empty\n");
            return;
        }
        for (entry *count = start3; count != nullptr; count = count->next)
        {
            std::printf("entry Name Suffix : %s\n", count->name_suffix);
            std::printf("entry Name : %s\n", count->name);
            std::printf("entry ID : %s\n", count->entry_id);
            std::printf("entry Address : %s\n", count->address);
            std::printf("entry City : %s\n", count->city);
            std::printf("entry Gender : %s\n", count->gender);
            std::printf("entry Date Of Birth : %s\n", count->data_of_birth);
            std::printf("entry Weight : %s\n", count->weight);
            std::printf("\n");
        }
    }

    entry *entry_menu(entry *start3)
    {
        std::printf("\n");
        int choice;
        std::printf("Welcome to the entry menu\n");
        std::printf("Please enter your choice from the menu shown below\n");
        std::printf("1: Add a entry to this dataset\n");
        std::printf("2: Delete a entry from this dataset\n");
        std::printf("3: Display the entry list for this dataset\n");
        std::printf("4: Return back to the dataset Menu\n");
        if (std::scanf("%d", &choice) != 1)
        {
            drain_stdin();
            choice = 0;
        }

        switch (choice)
        {
        case 1:
            start3 = create_entry_node(start3);
            break;
        case 2:
            start3 = delete_entry_node(start3);
            break;
        case 3:
            print_entry_list(start3);
            break;
        case 4:
            return start3;
        default:
            return entry_menu(start3);
        }
        return entry_menu(start3);
    }

} // namespace acs
