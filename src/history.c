#include "../include/history.h"
#include "../include/shell.h"
#include "../include/utils.h"

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

    if (line[0] == '\0')
        return;

    term.hist.length++;
    term.hist.content = (char **)realloc(term.hist.content, term.hist.length * sizeof(char *));
    term.hist.content[term.hist.length - 1] = strdup(line);

    term.hist.pos = -1;
}

void clear_sub_history()
{
    free(term.hist.sub.content);

    term.hist.sub.content = malloc(0);
    term.hist.sub.length = -1;
    term.hist.sub.pos = -1;
}

/**
 * @brief Get previous history entry
 * 
 * @param line 
 * @return char* 
 */
char *previous_hist_entry(char *line)
{
    static int h_size;
    if (line == NULL)
    {
        if (term.hist.pos + 1 == term.hist.length)
            return NULL;

        term.hist.pos++;
        return strdup(term.hist.content[term.hist.length - term.hist.pos - 1]);
    }

    if (term.hist.sub.length < 0)
    {
        term.hist.sub.length++;
        for (int i = 0; i < term.hist.length; i++)
            if (strncmp(term.hist.content[i], line, strlen(line)) == 0)
            {
                term.hist.sub.length++;
                term.hist.sub.content = realloc(term.hist.sub.content, term.hist.sub.length * sizeof(char *));
                term.hist.sub.content[term.hist.sub.length - 1] = term.hist.content[i];
            }
        h_size = term.hist.sub.length;
    }
    else
    {
        int tmp_len = term.hist.sub.length;
        term.hist.sub.length = 0;
        for (int i = 0; i < tmp_len; i++)
            if (strncmp(term.hist.sub.content[i], line, strlen(line)) == 0)
            {
                term.hist.sub.length++;
                term.hist.sub.content[term.hist.sub.length - 1] = term.hist.sub.content[i];
            }
        term.hist.sub.content = realloc(term.hist.sub.content, term.hist.sub.length * sizeof(char *));

        if (term.hist.sub.length != h_size)
        {
            h_size = term.hist.sub.length;
            term.hist.sub.pos = -1;
        }
    }

    term.hist.sub.pos++;

    if (term.hist.sub.pos >= term.hist.sub.length)
    {
        term.hist.sub.pos = term.hist.sub.length - 1;
        return NULL;
    }

    return strdup(term.hist.sub.content[term.hist.sub.length - term.hist.sub.pos - 1]);
}

/**
 * @brief Get next history entry
 * 
 * @param line 
 * @return char* 
 */
char *next_hist_entry(char *line)
{
    if (line == NULL)
    {
        if (term.hist.pos == 0)
            return NULL;

        term.hist.pos--;
        return strdup(term.hist.content[term.hist.length - term.hist.pos - 1]);
    }

    if (term.hist.sub.pos <= 0)
        return NULL;

    term.hist.sub.pos--;
    return strdup(term.hist.sub.content[term.hist.sub.length - term.hist.sub.pos - 1]);
}