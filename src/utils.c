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
}

/**
 * @brief Separates string with "sep" characters
 * 
 * @param line 
 * @param toks 
 * @param sep 
 * @return int 
 */
int sep_string(char *line, char ***toks, char *sep)
{
    free(*toks);
    char *tmp_line = strdup(line);
    char *free_tmp_line = tmp_line;
    int n = 0;
    *toks = calloc(n, sizeof(char *));

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

/**
 * @brief Removes extra spaces
 * 
 * @param str 
 * @return char* 
 */
char *trim_string(char *str, bool leave_trailing_space)
{
    char *tmp = strdup(str);
    while (tmp[0] == ' ')
        remove_on_pos(&tmp, 1);

    for (int i = 1; i < strlen(tmp); i++)
        if (tmp[i] == ' ' && tmp[i - 1] == ' ')
        {
            remove_on_pos(&tmp, i);
            i--;
        }

    if (!leave_trailing_space)
        if (tmp[strlen(tmp) - 1] == ' ' && tmp[strlen(tmp) - 2] == ' ')
            remove_on_pos(&tmp, strlen(tmp) - 1);

    return tmp;
}

/**
 * @brief Frees array of strings
 * 
 * @param arr 
 * @param sz 
 */
void free_str_arr(char **arr, int sz)
{
    if (arr[0] != NULL)
        for (int i = 0; i < sz; i++)
            free(arr[i]);
    free(arr);
}

/**
 * @brief Creates a slice of array
 * 
 * @param arr 
 * @param beg 
 * @param end 
 * @param asc 
 * @return char** 
 */
char **slice_array(char **arr, int beg, int end, bool asc)
{
    if (beg == -1)
        beg = 0;
    if (end == -1)
        end = get_null_term_arr_size(arr);

    char **new_arr = calloc(abs(end - beg), sizeof(char *));

    if (asc)
        for (int i = beg, j = 0; i < end; i++, j++)
            new_arr[j] = arr[i];
    else
        for (int i = end, j = 0; i > beg; i--, j++)
            new_arr[j] = arr[i];

    return new_arr;
}

/**
 * @brief Gets size of null-terminated array
 * 
 * @param arr 
 * @return int 
 */
int get_null_term_arr_size(char **arr)
{
    int k = 0;
    for (int i = 0; arr[i] != NULL; i++)
        k++;

    return ++k;
}

/**
 * @brief Append string to the end of array
 * 
 * @param arr 
 * @param sz 
 * @param str 
 * @return int 
 */
int append_to_str_arr(char ***arr, ssize_t *sz, char *str)
{
    (*sz)++;

    char **tmp = realloc(*arr, sizeof(char *) * *sz);
    if (tmp == NULL)
    {
        fprintf(stderr, "Couldn't reallocate memory\n");
        return -1;
    }
    *arr = tmp;

    (*arr)[*sz - 1] = strdup(str);

    return 0;
}

/**
 * @brief Get the current location
 * 
 * @return char* 
 */
char *get_curr_dir_name()
{
    char *pwd = malloc(FILENAME_MAX);
    getcwd(pwd, FILENAME_MAX);

    return pwd;
}

/**
 * @brief Get the number of lines in file
 * 
 * @param file 
 * @return int 
 */
int get_num_of_lines(FILE *file)
{
    int n = 0;
    char ch, pch;
    long curr_pos = ftell(file);

    fseek(file, 0, SEEK_SET);

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
            n++;
        pch = ch;
    }

    if (pch != '\n')
        n++;

    fseek(file, curr_pos, SEEK_SET);

    return n;
}

bool str_is_in_arr(char **arr, size_t sz, char *str)
{
    int first = 0;
    int last = sz - 1;
    int middle;

    while (first <= last)
    {
        middle = (first + last) / 2;
        int diff = strcmp(arr[middle], str);
        if (diff > 0)
        {
            first = middle + 1;
        }
        else if (diff == 0)
            return true;
        else
        {
            last = middle - 1;
        }
    }

    return false;
}