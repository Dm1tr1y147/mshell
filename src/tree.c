#include "../include/tree.h"

/**
 * @brief Create new trie tree
 * 
 * @return struct tree_node* 
 */
struct tree_node *get_new_node()
{
  struct tree_node *node = malloc(sizeof(struct tree_node));
  node->is_leaf = 0;

  for (int i = 0; i < ALPHABET_SIZE; i++)
  {
  node->child[i] = NULL;
  }

  return node;
}

/**
 * @brief Inserts a string into trie
 * 
 * @param root 
 * @param key 
 */
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

/**
 * @brief Frees memory allocated for trie
 * 
 * @param root 
 */
void free_tree(struct tree_node *root)
{
  if (root != NULL)
  {
  for (int i = 0; i < ALPHABET_SIZE; i++)
  {
    if (root->child[i] != NULL)
    {
    free_tree(root->child[i]);
    }
  }

  free(root);
  }
  return;
}

/**
 * @brief Checks if key is in trie
 * 
 * @param root 
 * @param key 
 * @return int 
 */
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

/**
 * @brief Checks if trie node is last
 * 
 * @param root 
 * @return int 
 */
int is_last_node(struct tree_node *root)
{
  for (int i = 0; i < ALPHABET_SIZE; i++)
  if (root->child[i] != NULL)
    return 0;

  return 1;
}

/**
 * @brief Lists all strings in trie begining with key
 * 
 * @param root 
 * @param key 
 * @param strings 
 * @return ssize_t 
 */
ssize_t list_strings_containing(struct tree_node *root, char *key, char ***strings)
{
  ssize_t amount = 0;

  free(*strings);

  *strings = calloc(amount, sizeof(char *));

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

/**
 * @brief Recursive substrings search for list_strings_containing
 * 
 * @param root 
 * @param amount 
 * @param curr_prefix 
 * @param strings 
 */
void get_all_substrings(struct tree_node *root, ssize_t *amount, char **curr_prefix, char ***strings)
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