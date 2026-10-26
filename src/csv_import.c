/*
 * csv_import.c
 * Standalone import tool: reads the CSV files in data/ and prints a
 * summary of what was loaded. Can be linked with the main program or
 * built on its own to verify the dataset:
 *
 *   clang++ src/csv_import.c src/csv_loader_test.c -o csv_import.exe
 */
#include <stdio.h>
#include "csv_loader.h"

int main(void)
{
    char **airlines = NULL;
    int na = csv_load_catalogs(&airlines);
    printf("catalogs.csv            : %d record(s)\n", na);

    struct csv_dataset *flights = NULL;
    int nf = csv_load_datasets(&flights);
    printf("datasets.csv             : %d record(s)\n", nf);

    struct csv_entry *passengers = NULL;
    int np = csv_load_entries(&passengers);
    printf("entries.csv          : %d record(s)\n", np);

    struct csv_transaction *res = NULL;
    int nr = csv_load_transactions(&res);
    printf("transactions.csv        : %d record(s)\n", nr);

    struct csv_index_key *seats = NULL;
    int ns = csv_load_indexes(&seats);
    printf("indexes.csv    : %d record(s)\n", ns);

    printf("total records loaded    : %d\n", na + nf + np + nr + ns);
    return 0;
}
