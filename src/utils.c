#include "../include/utils.h"

void die(char *s)
{
    perror(s);
    exit(1);
}

void append_to_pos(char **str, int pos, char ch)
{
    size_t len = strlen(*str);

    *str = realloc(*str, len + 2);

    for (int i = len; i >= pos; i--)
    {
        (*str)[i + 1] = (*str)[i];
    }
    (*str)[pos] = ch;
    (*str)[len + 1] = '\0';
}

void remove_on_pos(char **str, int pos)
{
    size_t len = strlen(*str);

    for (int i = pos - 1; i <= len; i++)
    {
        (*str)[i] = (*str)[i + 1];
    }
    (*str)[len] = '\0';
    *str = realloc(*str, len);
}