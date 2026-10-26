/*
 * reports.c
 * Reporting functions required by the assignment:
 *   d) show all current datasets, sorted by start time
 *   e) show all entries on a selected dataset, sorted by last name
 *   f) summary statistics for the whole system
 *
 * These operate on the same linked lists used by
 * access navigation control.cpp (start1..start5 globals).
 */
#ifndef REPORTS_H
#define REPORTS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Extract the last word of a "First Last" style name */
static const char *last_name(const char *name)
{
    const char *last = strrchr(name, ' ');
    return last ? last + 1 : name;
}

/* Compare datasets by start time (and then dataset number as tie-break).
 * Works on the dataset structure layout of the main program. */
struct dataset_rep
{
    char *dataset_id;
    char *date_of_dataset;
    char *source_table;
    char *target_table;
    char *start_time;
    char *end_time;
};

static int compare_start_time(const void *a, const void *b)
{
    const struct dataset_rep *fa = (const struct dataset_rep *)a;
    const struct dataset_rep *fb = (const struct dataset_rep *)b;
    int c = strcmp(fa->start_time, fb->start_time);
    if (c == 0)
        c = strcmp(fa->dataset_id, fb->dataset_id);
    return c;
}

/* Report (d): all datasets sorted by departure time */
void report_datasets_by_start_time(struct dataset_rep *datasets, int count)
{
    int i;
    printf("\n==================================================\n");
    printf("  DATASET SCHEDULE (sorted by start time)\n");
    printf("==================================================\n");
    if (count == 0)
    {
        printf("No datasets in the system.\n");
        return;
    }
    qsort(datasets, (size_t)count, sizeof(*datasets), compare_start_time);
    for (i = 0; i < count; ++i)
    {
        printf("%-8s %-12s %-16s -> %-16s %5s - %s\n",
               datasets[i].dataset_id,
               datasets[i].date_of_dataset,
               datasets[i].source_table,
               datasets[i].target_table,
               datasets[i].start_time,
               datasets[i].end_time);
    }
}

/* Compare entries by last name */
struct entry_rep
{
    char *entry_id;
    char *name;
};

static int compare_last_name(const void *a, const void *b)
{
    const struct entry_rep *pa = (const struct entry_rep *)a;
    const struct entry_rep *pb = (const struct entry_rep *)b;
    return strcmp(last_name(pa->name), last_name(pb->name));
}

/* Report (e): entries on a dataset, sorted by last name */
void report_entries_on_dataset(struct entry_rep *entries, int count,
                                 const char *dataset_id)
{
    int i;
    printf("\n==================================================\n");
    printf("  ENTRIES ON DATASET %s (sorted by last name)\n", dataset_id);
    printf("==================================================\n");
    if (count == 0)
    {
        printf("No entries on this dataset.\n");
        return;
    }
    qsort(entries, (size_t)count, sizeof(*entries), compare_last_name);
    for (i = 0; i < count; ++i)
        printf("%-10s %s\n", entries[i].entry_id, entries[i].name);
}

/* Report (f): simple system statistics */
void report_statistics(int catalogs, int datasets, int entries,
                       int transactions, int rows_indexed, int total_rows)
{
    printf("\n==================================================\n");
    printf("  SYSTEM STATISTICS\n");
    printf("==================================================\n");
    printf("Catalogs        : %d\n", catalogs);
    printf("Datasets        : %d\n", datasets);
    printf("Entries         : %d\n", entries);
    printf("Transactions    : %d\n", transactions);
    printf("Rows indexed    : %d of %d (%.1f%% indexed)\n",
           rows_indexed, total_rows,
           total_rows ? (100.0 * rows_indexed / total_rows) : 0.0);
}

#endif /* REPORTS_H */
