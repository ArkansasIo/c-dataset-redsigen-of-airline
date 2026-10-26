// types.h
// Shared data model for the RecordBase Database System (C++23).
// Redesigned from the catalog transaction model into a records database:
//   catalog -> catalog, dataset -> dataset, entry -> entry,
//   transaction -> transaction, index key -> index key.
#pragma once

#include <cstddef>

namespace acs
{

    // Forward declarations so nodes can reference each other.
    struct dataset;
    struct entry;
    struct transaction;

    // Catalog node: name plus the head of its dataset list.
    struct catalog
    {
        char *catalogname = nullptr;
        catalog *next = nullptr;
        dataset *node1 = nullptr; // head of dataset list for this catalog
    };

    // Dataset node: record fields plus entry and transaction sub-lists.
    struct dataset
    {
        char *dataset_id = nullptr;      // unique key
        char *date_of_dataset = nullptr;
        char *source_table = nullptr;
        char *target_table = nullptr;
        char *start_time = nullptr;
        char *end_time = nullptr;
        int total_rows = 0;
        int rows_indexed = 0;

        dataset *next = nullptr;
        entry *node2 = nullptr;       // head of entry list
        transaction *node3 = nullptr; // head of transaction list
    };

    // Entry node (unique entry_id).
    struct entry
    {
        char *name_suffix = nullptr;
        char *name = nullptr;
        char *entry_id = nullptr;     // unique key
        char *address = nullptr;
        char *city = nullptr;
        char *gender = nullptr;
        char *data_of_birth = nullptr;
        char *weight = nullptr;
        entry *next = nullptr;
    };

    // Transaction node (unique transaction_id).
    struct transaction
    {
        char *transaction_id = nullptr; // unique key
        char *entry_id = nullptr;
        char *dataset_id = nullptr;
        char *cost = nullptr;           // amount
        transaction *next = nullptr;
    };

    // Index key node (array style fixed buffers, as in the original).
    struct index_key
    {
        char key_name[80] = {};        // key name/id
        char cost[80] = {};            // amount
        int  available = 1;
        index_key* next = nullptr;
    };

    // Maximum sizes used by the original fixed buffers.
    inline constexpr std::size_t kMaxNameLen = 80;
    inline constexpr std::size_t kMaxIdLen = 10;
    inline constexpr std::size_t kMaxSuffix = 6;

} // namespace acs
