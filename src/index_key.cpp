// index_key.cpp
// Index key linked-list operations (C++23).
#include "index_key.h"
#include "util.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace acs
{

    index_key *enter_index_key_info(index_key *node)
    {
        std::printf(" Welcome to the Index Key List\n");
        std::printf("Please enter the Index Key\n");
        drain_stdin();
        read_line(node->key_name, sizeof(node->key_name));
        std::printf("Please enter the Amount\n");
        drain_stdin();
        read_line(node->cost, sizeof(node->cost));
        node->available = 1;
        return node;
    }

    index_key *create_index_key_node(index_key *start5)
    {
        std::printf("\n");
        if (start5 == nullptr)
        {
            start5 = static_cast<index_key *>(std::malloc(sizeof(index_key)));
            start5 = enter_index_key_info(start5);
            start5->next = nullptr;
            return start5;
        }

        index_key *count_node1, *count_node2 = nullptr, *node;
        node = static_cast<index_key *>(std::malloc(sizeof(index_key)));
        node = enter_index_key_info(node);

        for (count_node1 = start5;
             count_node1 != nullptr &&
             std::strcmp(count_node1->key_name, node->key_name) <= 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 == start5)
        {
            node->next = start5;
            start5 = node;
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
        return start5;
    }

    index_key *cancelled_index_key_node(index_key *start5)
    {
        std::printf("\n");
        if (start5 == nullptr)
        {
            std::printf("Sorry, the index_key list is empty\n");
            return start5;
        }

        char key_name[80];
        std::printf("Please enter the index_key you want to delete\n");
        drain_stdin();
        read_line(key_name, sizeof(key_name));

        index_key *count_node1, *count_node2 = nullptr, *temp;
        for (count_node1 = start5;
             count_node1 != nullptr &&
             std::strcmp(count_node1->key_name, key_name) != 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 != nullptr)
        {
            if (count_node1 == start5)
            {
                temp = start5;
                start5 = start5->next;
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
        return start5;
    }

    void print_index_key_list(index_key *start5)
    {
        std::printf("\n");
        if (start5 == nullptr)
        {
            std::printf("Sorry, the Index Key List is empty\n");
            return;
        }
        for (index_key *count = start5; count != nullptr; count = count->next)
        {
            std::printf("Index Key : %s\n", count->key_name);
            std::printf("Cost : %s\n", count->cost);
            std::printf("Available : %s\n", count->available ? "yes" : "no");
            std::printf("\n");
        }
    }

    index_key *index_key_menu(index_key *start5)
    {
        std::printf("\n");
        int choice;
        std::printf("Welcome to the Index Key menu\n");
        std::printf("Please enter your choice from the menu shown below\n");
        std::printf("1: Add an index key to this dataset\n");
        std::printf("2: Cancel an index key from this dataset\n");
        std::printf("3: Display the index key list for this dataset\n");
        std::printf("4: Return back to the dataset Menu\n");
        if (std::scanf("%d", &choice) != 1)
        {
            drain_stdin();
            choice = 0;
        }

        switch (choice)
        {
        case 1:
            start5 = create_index_key_node(start5);
            break;
        case 2:
            start5 = cancelled_index_key_node(start5);
            break;
        case 3:
            print_index_key_list(start5);
            break;
        case 4:
            return start5;
        default:
            return index_key_menu(start5);
        }
        return index_key_menu(start5);
    }

} // namespace acs
