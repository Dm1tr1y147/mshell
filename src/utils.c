#include "../include/utils.h"

/**
 * @brief Appends a character to the specified position of string
 * 
 * @param str 
 * @param pos 
 * @param ch 
 */
void append_to_pos(char **str, int pos, char ch)
{
    size_t len = strlen(*str);
    if (pos <= len + 1)
    {
        *str = realloc(*str, len + 2);

        for (int i = len; i >= pos; i--)
        {
            (*str)[i + 1] = (*str)[i];
        }
        (*str)[pos] = ch;
        (*str)[len + 1] = '\0';
    }
    else
        fprintf(stderr, "Can't add \"%c\" symbol outside of the string\n", ch);
}

/**
 * @brief Removes a character located in the specified position
 * 
 * @param str 
 * @param pos 
 */
void remove_on_pos(char **str, int pos)
{
    size_t len = strlen(*str);
    if (pos <= len)
    {
        for (int i = pos - 1; i < len; i++)
            (*str)[i] = (*str)[i + 1];

        (*str)[len] = '\0';
        *str = realloc(*str, len);
    }
    else
        fprintf(stderr, "Can't remove symbol outside the string\n");
}

int sep_string(char *line, char ***toks, char *sep)
{
    free(*toks);
    char *tmp_line = strdup(line);
    char *free_tmp_line = tmp_line;
    int n = 0;
    *toks = malloc(sizeof(char *) * n);

    char *tmp = NULL;
    while ((tmp = strsep(&tmp_line, sep)) != NULL)
    {
        n++;
        *toks = realloc(*toks, sizeof(char *) * n);
        (*toks)[n - 1] = strdup(tmp);
    }

    free(free_tmp_line);

    return n;
}

char *trim_string(char **str)
{
    while ((*str)[0] == ' ')
        remove_on_pos(str, 1);

    for (int i = 1; i < strlen(*str); i++)
        if ((*str)[i] == ' ' && (*str)[i - 1] == ' ')
        {
            remove_on_pos(str, i);
            i--;
        }

    return *str;
}

void free_str_arr(char **arr)
{
    if (arr[0] != NULL)
        for (int i = 0; i < sizeof(arr) / sizeof(char *); i++)
            free(arr[i]);
    free(arr);
}

char **slice_array(char **arr, int beg, int end, bool asc)
{
    if (beg == -1)
        beg = 0;
    if (end == -1)
        end = get_null_term_arr_size(arr);

    char **new_arr = malloc(abs(end - beg) * sizeof(char *));

    if (asc)
        for (int i = beg, j = 0; i < end; i++, j++)
            new_arr[j] = arr[i];
    else
        for (int i = end, j = 0; i > beg; i--, j++)
            new_arr[j] = arr[i];

    return new_arr;
}

int get_null_term_arr_size(char **arr)
{
    int k = 0;
    for (int i = 0; arr[i]!= NULL; i++)
        k++;

    return k;
}