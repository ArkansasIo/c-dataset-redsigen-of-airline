// storage.cpp
// Save/load the linked lists to and from database.dat (C++23).
// Length-prefixed strings in a flat binary format:
//   [int catalog_count]
//   per catalog: [int len][name][int dataset_count]
//     per dataset: 6 x [int len][str] + [int total_rows][int booked]
//       + [int entry_count] per entry: 8 x [int len][str]
//       + [int transaction_count] per transaction: 4 x [int len][str]
#include "storage.h"
#include "util.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace acs
{

    static void save_str(FILE *fp, const char *s)
    {
        const int length = s ? static_cast<int>(std::strlen(s)) : 0;
        std::fwrite(&length, sizeof(int), 1, fp);
        if (length > 0)
            std::fwrite(s, 1, static_cast<size_t>(length), fp);
    }

    static void save_int(FILE *fp, int v)
    {
        std::fwrite(&v, sizeof(int), 1, fp);
    }

    void write_to_file(catalog *start1)
    {
        FILE *fp = std::fopen("database.dat", "wb");
        if (!fp)
        {
            std::printf("Error: could not open database.dat for writing\n");
            return;
        }

        int catalog_count = 0;
        for (catalog *a = start1; a != nullptr; a = a->next)
            ++catalog_count;
        save_int(fp, catalog_count);

        for (catalog *a = start1; a != nullptr; a = a->next)
        {
            save_str(fp, a->catalogname);

            int dataset_count = 0;
            for (dataset *f = a->node1; f != nullptr; f = f->next)
                ++dataset_count;
            save_int(fp, dataset_count);

            for (dataset *f = a->node1; f != nullptr; f = f->next)
            {
                save_str(fp, f->dataset_id);
                save_str(fp, f->date_of_dataset);
                save_str(fp, f->source_table);
                save_str(fp, f->target_table);
                save_str(fp, f->start_time);
                save_str(fp, f->end_time);
                save_int(fp, f->total_rows);
                save_int(fp, f->rows_indexed);

                int entry_count = 0;
                for (entry *p = f->node2; p != nullptr; p = p->next)
                    ++entry_count;
                save_int(fp, entry_count);

                for (entry *p = f->node2; p != nullptr; p = p->next)
                {
                    save_str(fp, p->name_suffix);
                    save_str(fp, p->name);
                    save_str(fp, p->entry_id);
                    save_str(fp, p->address);
                    save_str(fp, p->city);
                    save_str(fp, p->gender);
                    save_str(fp, p->data_of_birth);
                    save_str(fp, p->weight);
                }

                int transaction_count = 0;
                for (transaction *r = f->node3; r != nullptr; r = r->next)
                    ++transaction_count;
                save_int(fp, transaction_count);

                for (transaction *r = f->node3; r != nullptr; r = r->next)
                {
                    save_str(fp, r->transaction_id);
                    save_str(fp, r->entry_id);
                    save_str(fp, r->dataset_id);
                    save_str(fp, r->cost);
                }
            }
        }
        std::fclose(fp);
        std::printf("Saved to database.dat\n");
    }

    static bool load_str(FILE *fp, char **out)
    {
        int length = 0;
        if (std::fread(&length, sizeof(int), 1, fp) != 1)
            return false;
        *out = static_cast<char *>(std::malloc(static_cast<size_t>(length) + 1));
        if (length > 0 &&
            std::fread(*out, 1, static_cast<size_t>(length), fp) !=
                static_cast<size_t>(length))
        {
            std::free(*out);
            *out = nullptr;
            return false;
        }
        (*out)[length] = 0;
        return true;
    }

    static bool load_int(FILE *fp, int *out)
    {
        return std::fread(out, sizeof(int), 1, fp) == 1;
    }

    catalog *read_from_file()
    {
        FILE *fp = std::fopen("database.dat", "rb");
        if (!fp)
            return nullptr;

        catalog *start = nullptr;
        catalog *curr = nullptr;
        int catalog_count = 0;

        while (load_int(fp, &catalog_count))
        {
            for (; catalog_count > 0; --catalog_count)
            {
                catalog *a = static_cast<catalog *>(std::malloc(sizeof(catalog)));
                a->next = nullptr;
                a->node1 = nullptr;
                load_str(fp, &a->catalogname);

                if (start == nullptr)
                    start = curr = a;
                else
                {
                    curr->next = a;
                    curr = curr->next;
                }

                int dataset_count = 0;
                load_int(fp, &dataset_count);

                dataset *lastdataset = nullptr;
                for (; dataset_count > 0; --dataset_count)
                {
                    dataset *f = static_cast<dataset *>(std::malloc(sizeof(dataset)));
                    f->next = nullptr;
                    f->node2 = nullptr;
                    f->node3 = nullptr;

                    load_str(fp, &f->dataset_id);
                    load_str(fp, &f->date_of_dataset);
                    load_str(fp, &f->source_table);
                    load_str(fp, &f->target_table);
                    load_str(fp, &f->start_time);
                    load_str(fp, &f->end_time);
                    load_int(fp, &f->total_rows);
                    load_int(fp, &f->rows_indexed);

                    if (a->node1 == nullptr)
                        a->node1 = f;
                    else
                        lastdataset->next = f;
                    lastdataset = f;

                    int entry_count = 0;
                    load_int(fp, &entry_count);

                    entry *lastpass = nullptr;
                    for (; entry_count > 0; --entry_count)
                    {
                        entry *p = static_cast<entry *>(std::malloc(sizeof(entry)));
                        p->next = nullptr;
                        load_str(fp, &p->name_suffix);
                        load_str(fp, &p->name);
                        load_str(fp, &p->entry_id);
                        load_str(fp, &p->address);
                        load_str(fp, &p->city);
                        load_str(fp, &p->gender);
                        load_str(fp, &p->data_of_birth);
                        load_str(fp, &p->weight);

                        if (f->node2 == nullptr)
                            f->node2 = p;
                        else
                            lastpass->next = p;
                        lastpass = p;
                    }

                    int transaction_count = 0;
                    load_int(fp, &transaction_count);

                    transaction *lastres = nullptr;
                    for (; transaction_count > 0; --transaction_count)
                    {
                        transaction *r = static_cast<transaction *>(std::malloc(sizeof(transaction)));
                        r->next = nullptr;
                        load_str(fp, &r->transaction_id);
                        load_str(fp, &r->entry_id);
                        load_str(fp, &r->dataset_id);
                        load_str(fp, &r->cost);

                        if (f->node3 == nullptr)
                            f->node3 = r;
                        else
                            lastres->next = r;
                        lastres = r;
                    }
                }
            }
        }
        std::fclose(fp);
        return start;
    }

} // namespace acs
