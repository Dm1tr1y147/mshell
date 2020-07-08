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

        for (int i = pos - 1; i <= len; i++)
        {
            (*str)[i] = (*str)[i + 1];
        }
        (*str)[len] = '\0';
        *str = realloc(*str, len);
    }
    else
        fprintf(stderr, "Can't remove symbol outside the string\n");
}