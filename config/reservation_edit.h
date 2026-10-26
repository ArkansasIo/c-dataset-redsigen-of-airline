/*
 * reservation_edit.h
 * C structures mirroring the transaction database tree so the
 * console program (database navigation control.cpp) can edit the same
 * hierarchy that the MySQL schema and API expose.
 *
 *   transaction (tree root)
 *     -> tree_node        (sub-node / subtree)
 *        -> tree_branch   (branch)
 *           -> tree_sub_branch (sub-branch)
 *              -> tree_module (modular leaf)
 */
#ifndef RESERVATION_EDIT_H
#define RESERVATION_EDIT_H

#include <stdlib.h>
#include <string.h>

/* Modular leaf unit */
struct tree_module
{
    char *module_name;
    char *module_value;
    struct tree_module *next;
};

/* Sub-branch */
struct tree_sub_branch
{
    char *sub_branch_name;
    struct tree_module *modules;
    struct tree_sub_branch *next;
};

/* Branch */
struct tree_branch
{
    char *branch_name;
    struct tree_sub_branch *sub_branches;
    struct tree_branch *next;
};

/* Sub-node / subtree */
struct tree_node
{
    char *node_name;
    char *index_key;
    int available;
    struct tree_branch *branches;
    struct tree_node *next;     /* sibling */
    struct tree_node *children; /* subtree */
};

/* Transaction tree root */
struct transaction_tree
{
    char *transaction_id; /* unique key */
    char *entry_id;
    char *dataset_id;
    double amount;
    struct tree_node *nodes;
    struct transaction_tree *next;
};

/* Helper: allocate a modifiable copy of a string */
static inline char *res_strdup(const char *src)
{
    size_t n = (src ? strlen(src) : 0) + 1;
    char *dst = (char *)malloc(n);
    if (dst && src)
        memcpy(dst, src, n);
    else if (dst)
        dst[0] = 0;
    return dst;
}

#endif /* RESERVATION_EDIT_H */
