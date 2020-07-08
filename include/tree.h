#ifndef _TREE_H
#define _TREE_H

#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256

struct tree_node
{
    struct tree_node *child[ALPHABET_SIZE];

    int is_leaf;
};

struct tree_node *get_new_node();
void insert_tree(struct tree_node *root, char *key);
void free_tree(struct tree_node *root);
int search_tree(struct tree_node *root, char *key);
int is_last_node(struct tree_node *root);
size_t list_strings_containing(struct tree_node *root, char *key, char ***strings);
void get_all_substrings(struct tree_node *root, size_t *amount, char **curr_prefix, char ***strings);

#endif