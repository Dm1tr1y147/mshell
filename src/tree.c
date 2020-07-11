#include "../include/tree.h"

struct tree_node *get_new_node()
{
    struct tree_node *node = (struct tree_node *)malloc(sizeof(struct tree_node));
    node->is_leaf = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        node->child[i] = NULL;
    }

    return node;
}

void insert_tree(struct tree_node *root, char *key)
{
    struct tree_node *current = root;

    for (int i = 0; i < strlen(key); i++)
    {
        char index = key[i];
        if (current->child[index] == NULL)
        {
            current->child[index] = get_new_node();
        }

        current = current->child[index];
    }

    current->is_leaf = 1;
}

void free_tree(struct tree_node *root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->child[i] != NULL)
        {
            free_tree(root->child[i]);
        }
    }

    free(root);
    return;
}

int search_tree(struct tree_node *root, char *key)
{
    struct tree_node *current = root;

    for (int i = 0; i < strlen(key); i++)
    {
        char index = key[i];

        if (current->child[index] == NULL)
        {
            return 0;
        }

        current = current->child[index];
    }

    return (current != NULL && current->is_leaf);
}

int is_last_node(struct tree_node *root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->child[i] != NULL)
            return 0;

    return 1;
}

size_t list_strings_containing(struct tree_node *root, char *key, char ***strings)
{
    size_t amount = 0;

    free(*strings);

    *strings = malloc(amount * sizeof(char *));

    struct tree_node *current = root;

    for (int level = 0; level < strlen(key); level++)
    {
        int index = key[level];

        if (current->child[index] == NULL)
            return amount;

        current = current->child[index];
    }

    int key_presents = current->is_leaf;

    int is_last = is_last_node(current);

    if (key_presents && is_last)
    {
        amount++;
        *strings = (char **)realloc(*strings, amount * sizeof(char *));
        (*strings)[0] = strdup(key);
        return amount;
    }

    if (is_last == 0)
    {
        char *prefix = strdup(key);

        get_all_substrings(current, &amount, &prefix, strings);

        free(prefix);

        return amount;
    }

    return -1;
}

void get_all_substrings(struct tree_node *root, size_t *amount, char **curr_prefix, char ***strings)
{
    if (root->is_leaf)
    {
        (*amount)++;
        *strings = (char **)realloc(*strings, *amount * sizeof(char *));
        (*strings)[*amount - 1] = strdup(*curr_prefix);
    }

    if (is_last_node(root))
        return;

    for (int index = 0; index < ALPHABET_SIZE; index++)
    {
        if (root->child[index] != NULL)
        {
            *curr_prefix = realloc(*curr_prefix, strlen(*curr_prefix) + 2);
            *curr_prefix = strncat(*curr_prefix, (char *)(&index), 1);

            get_all_substrings(root->child[index], amount, curr_prefix, strings);

            (*curr_prefix)[strlen(*curr_prefix) - 1] = '\0';
        }
    }
}