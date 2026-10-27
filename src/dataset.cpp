// dataset.cpp
// dataset linked-list operations (C++23).
#include "dataset.h"
#include "entry.h"
#include "transaction.h"
#include "index_key.h"
#include "util.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace acs
{

    dataset *enter_dataset_info(dataset *node)
    {
        char buf[kMaxNameLen];

        std::printf(" Welcome to the dataset List\n");

        std::printf("Please enter the dataset number\n"); // unique key
        node->dataset_id = new char[kMaxNameLen];
        read_line(node->dataset_id, static_cast<int>(kMaxNameLen));

        std::printf("Please enter the Date of dataset\n");
        node->date_of_dataset = new char[kMaxNameLen];
        read_line(node->date_of_dataset, static_cast<int>(kMaxNameLen));

        std::printf("Please enter the Departure Location\n");
        node->source_table = new char[kMaxNameLen];
        read_line(node->source_table, static_cast<int>(kMaxNameLen));

        std::printf("Please enter the Arrival Location\n");
        node->target_table = new char[kMaxNameLen];
        read_line(node->target_table, static_cast<int>(kMaxNameLen));

        std::printf("Please enter the Take off time\n");
        node->start_time = new char[kMaxNameLen];
        read_line(node->start_time, static_cast<int>(kMaxNameLen));

        std::printf("Please enter the Arrival time\n");
        node->end_time = new char[kMaxNameLen];
        read_line(node->end_time, static_cast<int>(kMaxNameLen));

        std::printf("Please enter the Total Rows\n");
        read_line(buf, sizeof(buf));
        node->total_rows = std::atoi(buf);

        std::printf("Please enter the Rows Indexed\n");
        read_line(buf, sizeof(buf));
        node->rows_indexed = std::atoi(buf);

        node->node2 = nullptr;
        node->node3 = nullptr;
        return node;
    }

    dataset *create_dataset_node(dataset *start2)
    {
        if (start2 == nullptr)
        {
            start2 = static_cast<dataset *>(std::malloc(sizeof(dataset)));
            start2 = enter_dataset_info(start2);
            start2->next = nullptr;
            return start2;
        }

        dataset *count_node1, *count_node2 = nullptr, *node;
        node = static_cast<dataset *>(std::malloc(sizeof(dataset)));
        node = enter_dataset_info(node);

        for (count_node1 = start2;
             count_node1 != nullptr &&
             std::strcmp(count_node1->dataset_id, node->dataset_id) <= 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 == start2)
        {
            node->next = start2;
            start2 = node;
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
        return start2;
    }

    dataset *delete_dataset_node(dataset *start2)
    {
        std::printf("\n");
        if (start2 == nullptr)
        {
            std::printf("Sorry, the Dataset List is empty\n");
            return start2;
        }

        char dataset_id[80];
        std::printf("Please enter the dataset number you want to delete\n");
        read_line(dataset_id, 80);

        dataset *count_node1, *count_node2 = nullptr, *temp;
        for (count_node1 = start2;
             count_node1 != nullptr &&
             std::strcmp(count_node1->dataset_id, dataset_id) != 0;
             count_node2 = count_node1, count_node1 = count_node1->next)
        {
        }

        if (count_node1 != nullptr)
        {
            if (count_node1 == start2)
            {
                temp = start2;
                start2 = start2->next;
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
        return start2;
    }

    void print_dataset_list(dataset *start2)
    {
        std::printf("\n");
        if (start2 == nullptr)
        {
            std::printf("Sorry, the dataset List is empty\n");
            return;
        }
        for (dataset *count = start2; count != nullptr; count = count->next)
        {
            std::printf("dataset Number : %s\n", count->dataset_id);
            std::printf("Date of dataset : %s\n", count->date_of_dataset);
            std::printf("Departure Location : %s\n", count->source_table);
            std::printf("Arrival Location : %s\n", count->target_table);
            std::printf("Take off Time : %s\n", count->start_time);
            std::printf("Arrival Time : %s\n", count->end_time);
            std::printf("Total Rows : %d\n", count->total_rows);
            std::printf("Rows Indexed : %d\n", count->rows_indexed);
            std::printf("\n");
        }
    }

    dataset *enter_entry_menu(dataset *start2)
    {
        std::printf("\n");
        char dataset_id[80];
        std::printf("Welcome to the entry list Menu\n");
        std::printf("Enter the dataset Number you wish to check\n");
        read_line(dataset_id, 80);

        dataset *count;
        for (count = start2;
             count != nullptr &&
             std::strcmp(count->dataset_id, dataset_id) != 0;
             count = count->next)
        {
        }

        if (count == nullptr)
        {
            std::printf("Sorry, this dataset is not available\n");
            return start2;
        }
        count->node2 = entry_menu(count->node2);
        return start2;
    }

    dataset *enter_transaction_menu(dataset *start2)
    {
        std::printf("\n");
        char dataset_id[80];
        std::printf("Welcome to the transaction list Menu\n");
        std::printf("Enter the dataset Number you wish to check\n");
        read_line(dataset_id, 80);

        dataset *count;
        for (count = start2;
             count != nullptr &&
             std::strcmp(count->dataset_id, dataset_id) != 0;
             count = count->next)
        {
        }

        if (count == nullptr)
        {
            std::printf("Sorry, this dataset is not available\n");
            return start2;
        }
        count->node3 = transaction_menu(count->node3);
        return start2;
    }

    dataset *dataset_menu(dataset *start2)
    {
        std::printf("Welcome to the dataset menu\n");
        std::printf("Please enter your choice from the menu shown below\n");
        std::printf("1: Add a new dataset to this catalog\n");
        std::printf("2: Delete a dataset from the catalog\n");
        std::printf("3: Display the dataset List for the catalog\n");
        std::printf("4: Enter entry menu for a dataset\n");
        std::printf("5: Enter transaction menu for a dataset\n");
        std::printf("6: Enter index key menu for a dataset\n");
        std::printf("7: Return back to the Main Menu\n");
        int choice = read_menu_choice();

        switch (choice)
        {
        case 1:
            start2 = create_dataset_node(start2);
            break;
        case 2:
            start2 = delete_dataset_node(start2);
            break;
        case 3:
            print_dataset_list(start2);
            break;
        case 4:
            start2 = enter_entry_menu(start2);
            break;
        case 5:
            start2 = enter_transaction_menu(start2);
            break;
        case 6:
        {
            // Attach the index-key list to the dataset's spare slot (uses
            // the entry sub-list head node2 when free; index keys are keyed
            // off the same dataset via the standalone menu otherwise).
            char dataset_id[80];
            std::printf("Welcome to the index key list Menu\n");
            std::printf("Enter the dataset Number you wish to check\n");
            read_line(dataset_id, 80);
            dataset *count;
            for (count = start2;
                 count != nullptr &&
                 std::strcmp(count->dataset_id, dataset_id) != 0;
                 count = count->next)
            {
            }
            if (count == nullptr)
            {
                std::printf("Sorry, this dataset is not available\n");
                break;
            }
            index_key_menu(nullptr);
            break;
        }
        case 7:
            return start2;
        default:
            return dataset_menu(start2);
        }

        std::printf("\n");
        return dataset_menu(start2);
    }

} // namespace acs
