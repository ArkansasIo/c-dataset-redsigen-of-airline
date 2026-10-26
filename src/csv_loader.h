/*
 * csv_loader.h
 * CSV data import for the catalog transaction System.
 *
 * Loads the files in the data/ directory:
 *   catalogs.csv           -> list of catalog names
 *   datasets.csv           -> catalog + dataset records
 *   entries.csv            -> entry records
 *   transactions.csv       -> transaction records (unique keys)
 *   indexes.csv            -> index key map per dataset
 *
 * These mirror the same structures used by access navigation control.cpp
 * and the MySQL schema in database/schema.sql.
 */
#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CSV_DIR "data/"
#define MAX_LINE 1024

/* ---------- small helpers ---------- */

/* Trim trailing CR/LF from a line read with fgets */
static void csv_chomp(char *s)
{
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r'))
        s[--n] = 0;
}

/* Get the n-th comma separated field of a line (0 based).
 * Returns a malloc'd copy, or an empty string if the field is missing.
 * Fields may be quoted; simple unquoting is applied. */
static char *csv_field(const char *line, int n)
{
    const char *p = line;
    int field = 0;
    int in_quotes = 0;

    while (field < n && *p)
    {
        if (*p == '"')
            in_quotes = !in_quotes;
        else if (*p == ',' && !in_quotes)
            ++field;
        ++p;
    }

    /* collect until next unquoted comma or end of line */
    size_t cap = 256, len = 0;
    char *out = (char *)malloc(cap);
    if (!out)
        return NULL;
    out[0] = 0;

    while (*p && (in_quotes || *p != ','))
    {
        if (*p == '"')
        {
            in_quotes = !in_quotes;
            ++p;
            continue;
        }
        if (len + 2 > cap)
        {
            cap *= 2;
            char *tmp = (char *)realloc(out, cap);
            if (!tmp)
            {
                free(out);
                return NULL;
            }
            out = tmp;
        }
        out[len++] = *p++;
    }
    out[len] = 0;
    return out;
}

/* Open a data file, skipping the header line. Returns NULL if missing. */
static FILE *csv_open(const char *filename)
{
    char path[256];
    snprintf(path, sizeof(path), CSV_DIR "%s", filename);
    FILE *fp = fopen(path, "r");
    if (!fp)
        return NULL;
    char line[MAX_LINE];
    if (fgets(line, sizeof(line), fp) == NULL) /* discard header */
    {
        fclose(fp);
        return NULL;
    }
    return fp;
}

/* ---------- CSV record loaders ---------- */

/* Load catalog names: "catalogname" */
static int csv_load_catalogs(char ***names_out)
{
    FILE *fp = csv_open("catalogs.csv");
    if (!fp)
        return 0;

    int count = 0, cap = 16;
    char **names = (char **)malloc(cap * sizeof(char *));
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp))
    {
        csv_chomp(line);
        if (!line[0])
            continue;
        if (count == cap)
        {
            cap *= 2;
            names = (char **)realloc(names, cap * sizeof(char *));
        }
        names[count++] = csv_field(line, 0);
    }
    fclose(fp);
    *names_out = names;
    return count;
}

/* One dataset record */
struct csv_dataset
{
    char *catalog;
    char *dataset_id;
    char *date;
    char *departure;
    char *arrival;
    char *takeoff;
    char *arrivetime;
    int total_rows;
    int rows_indexed;
};

/* Load datasets: "catalog,dataset_id,date,departure,arrival,takeoff,arrivetime,total,booked" */
static int csv_load_datasets(struct csv_dataset **out)
{
    FILE *fp = csv_open("datasets.csv");
    if (!fp)
        return 0;

    int count = 0, cap = 16;
    struct csv_dataset *rows = (struct csv_dataset *)malloc(cap * sizeof(*rows));
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp))
    {
        csv_chomp(line);
        if (!line[0])
            continue;
        if (count == cap)
        {
            cap *= 2;
            rows = (struct csv_dataset *)realloc(rows, cap * sizeof(*rows));
        }
        struct csv_dataset *r = &rows[count];
        r->catalog = csv_field(line, 0);
        r->dataset_id = csv_field(line, 1);
        r->date = csv_field(line, 2);
        r->departure = csv_field(line, 3);
        r->arrival = csv_field(line, 4);
        r->takeoff = csv_field(line, 5);
        r->arrivetime = csv_field(line, 6);
        char *ts = csv_field(line, 7);
        r->total_rows = atoi(ts);
        free(ts);
        char *sb = csv_field(line, 8);
        r->rows_indexed = atoi(sb);
        free(sb);
        ++count;
    }
    fclose(fp);
    *out = rows;
    return count;
}

/* One entry record */
struct csv_entry
{
    char *suffix;
    char *name;
    char *id;
    char *address;
    char *city;
    char *gender;
    char *dob;
    char *weight;
};

/* Load entries: "suffix,name,id,address,city,gender,dob,weight" */
static int csv_load_entries(struct csv_entry **out)
{
    FILE *fp = csv_open("entries.csv");
    if (!fp)
        return 0;

    int count = 0, cap = 16;
    struct csv_entry *rows = (struct csv_entry *)malloc(cap * sizeof(*rows));
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp))
    {
        csv_chomp(line);
        if (!line[0])
            continue;
        if (count == cap)
        {
            cap *= 2;
            rows = (struct csv_entry *)realloc(rows, cap * sizeof(*rows));
        }
        struct csv_entry *r = &rows[count];
        r->suffix = csv_field(line, 0);
        r->name = csv_field(line, 1);
        r->id = csv_field(line, 2);
        r->address = csv_field(line, 3);
        r->city = csv_field(line, 4);
        r->gender = csv_field(line, 5);
        r->dob = csv_field(line, 6);
        r->weight = csv_field(line, 7);
        ++count;
    }
    fclose(fp);
    *out = rows;
    return count;
}

/* One transaction record */
struct csv_transaction
{
    char *id;
    char *entry_id;
    char *dataset_id;
    char *cost;
};

/* Load transactions: "id,entry_id,dataset_id,cost" */
static int csv_load_transactions(struct csv_transaction **out)
{
    FILE *fp = csv_open("transactions.csv");
    if (!fp)
        return 0;

    int count = 0, cap = 16;
    struct csv_transaction *rows = (struct csv_transaction *)malloc(cap * sizeof(*rows));
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp))
    {
        csv_chomp(line);
        if (!line[0])
            continue;
        if (count == cap)
        {
            cap *= 2;
            rows = (struct csv_transaction *)realloc(rows, cap * sizeof(*rows));
        }
        rows[count].id = csv_field(line, 0);
        rows[count].entry_id = csv_field(line, 1);
        rows[count].dataset_id = csv_field(line, 2);
        rows[count].cost = csv_field(line, 3);
        ++count;
    }
    fclose(fp);
    *out = rows;
    return count;
}

/* One index key record */
struct csv_index_key
{
    char *dataset_id;
    char *index_key;
    char *entry_id;
    int available;
    char *cost;
};

/* Load index keys: "dataset_id,index_key,entry_id,available,amount" */
static int csv_load_indexes(struct csv_index_key **out)
{
    FILE *fp = csv_open("indexes.csv");
    if (!fp)
        return 0;

    int count = 0, cap = 16;
    struct csv_index_key *rows = (struct csv_index_key *)malloc(cap * sizeof(*rows));
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp))
    {
        csv_chomp(line);
        if (!line[0])
            continue;
        if (count == cap)
        {
            cap *= 2;
            rows = (struct csv_index_key *)realloc(rows, cap * sizeof(*rows));
        }
        struct csv_index_key *r = &rows[count];
        r->dataset_id = csv_field(line, 0);
        r->index_key = csv_field(line, 1);
        r->entry_id = csv_field(line, 2);
        char *av = csv_field(line, 3);
        r->available = atoi(av);
        free(av);
        r->cost = csv_field(line, 4);
        ++count;
    }
    fclose(fp);
    *out = rows;
    return count;
}

#endif /* CSV_LOADER_H */
