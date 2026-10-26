// transaction.cpp
// transaction linked-list operations (C++23).
#include "transaction.h"
#include "util.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace acs
{

    transaction *enter_transaction_info(transaction *node)
    {
        std::printf(" Welcome to the transaction List\n");

        std::printf("Please enter your transaction ID\n"); // unique key
        drain_stdin();
        node->transaction_id = new char[kMaxNameLen];
        read_line(node->transaction_id, static_cast<int>(kMaxNameLen));

        std::printf("Please enter your entry ID\n");
        drain_stdin();
        node->entry_id = new char[kMaxIdLen];
        read_line(node->entry_id, static_cast<int>(kMaxIdLen));

        std::printf("Please enter your dataset Number\n");
        drain_stdin();
        node->dataset_id = new char[kMaxNameLen];
        read_line(node->dataset_id, static_cast<int>(kMaxNameLen));

        std::printf("Please enter your Cost\n");
        drain_stdin();
        node->cost = new char[30];
        read_line(node->cost, 30);

        return node;
    }

    transaction *create_transaction_node(transaction *start4)
    {
        std::printf("\n");
        if (start4 == nullptr)
        {
            start4 = static_cast<transaction *>(std::malloc(sizeof(transaction)));
            start4 = enter_transaction_info(start4);
            start4->next = nullptr;
            return start4;
        }

        transaction *count_node1, *count_node2 = nullptr, *node;
        node = static_cast<transaction *>(std::malloc(sizeof(transaction)));
        node = enter_transaction_info(node);

        for (count_node1 = start4;
             count_node1 != nullptr &&
             std::strcmp(count_node1->transaction_id, node->transaction_id) <= 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 == start4)
        {
            node->next = start4;
            start4 = node;
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
        return start4;
    }

    transaction *delete_transaction_node(transaction *start4)
    {
        std::printf("\n");
        if (start4 == nullptr)
        {
            std::printf("Sorry, the transaction list is empty\n");
            return start4;
        }

        char transaction_id[80];
        std::printf("Please enter the transaction id you want to delete\n");
        drain_stdin();
        read_line(transaction_id, 80);

        transaction *count_node1, *count_node2 = nullptr, *temp;
        for (count_node1 = start4;
             count_node1 != nullptr &&
             std::strcmp(count_node1->transaction_id, transaction_id) != 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 != nullptr)
        {
            if (count_node1 == start4)
            {
                temp = start4;
                start4 = start4->next;
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
        return start4;
    }

    void print_transaction_list(transaction *start4)
    {
        std::printf("\n");
        if (start4 == nullptr)
        {
            std::printf("Sorry, the transaction list is empty\n");
            return;
        }
        for (transaction *count = start4; count != nullptr; count = count->next)
        {
            std::printf("transaction ID : %s\n", count->transaction_id);
            std::printf("entry ID : %s\n", count->entry_id);
            std::printf("dataset Number : %s\n", count->dataset_id);
            std::printf("Cost : %s\n", count->cost);
            std::printf("\n");
        }
    }

    transaction *transaction_menu(transaction *start4)
    {
        std::printf("\n");
        int choice;
        std::printf("Welcome to the transaction menu\n");
        std::printf("Please enter your choice from the menu shown below\n");
        std::printf("1: Add a transaction to this dataset\n");
        std::printf("2: Delete a transaction from this dataset\n");
        std::printf("3: Display the transaction list for this dataset\n");
        std::printf("4: Return back to the dataset Menu\n");
        if (std::scanf("%d", &choice) != 1)
        {
            drain_stdin();
            choice = 0;
        }

        switch (choice)
        {
        case 1:
            start4 = create_transaction_node(start4);
            break;
        case 2:
            start4 = delete_transaction_node(start4);
            break;
        case 3:
            print_transaction_list(start4);
            break;
        case 4:
            return start4;
        default:
            return transaction_menu(start4);
        }
        return transaction_menu(start4);
    }

} // namespace acs
