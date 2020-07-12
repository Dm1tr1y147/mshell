#include "../include/history.h"
#include "../include/shell.h"

/**
 * @brief Append entry to commands history
 * 
 * @param line 
 */
void append_to_history(char *line)
{
    if (term.hist.length > 0)
        if (strcmp(line, term.hist.content[term.hist.length - 1]) == 0)
            return;

    term.hist.length++;
    term.hist.content = (char **)realloc(term.hist.content, term.hist.length * sizeof(char *));
    term.hist.content[term.hist.length - 1] = strdup(line);

    term.hist.pos = -1;
}

/**
 * @brief Clear history entries search tree
 * 
 */
void clear_search_tree()
{
    term.hist.tree.length = -1;
    term.hist.tree.pos = -1;
    free_tree(term.hist.tree.r);
}

/**
 * @brief Get previous history entry
 * 
 * @param line 
 * @return char* 
 */
char *previous_hist_entry(char *line)
{
    if (line == NULL)
    {
        term.hist.pos++;
        return term.hist.content[term.hist.length - term.hist.pos - 1];
    }
}