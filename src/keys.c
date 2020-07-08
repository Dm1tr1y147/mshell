#include "../include/keys.h"
#include "../include/output.h"
#include "../include/utils.h"
#include "../include/complete.h"

/**
 * @brief Delete key action
 * 
 * @param pos 
 * @param n 
 * @param line 
 */
void delete_key(int pos, int *n, char **line)
{
    if (pos < *n)
    {
        remove_on_pos(line, pos + 1);
        (*n)--;

        char *buff = malloc(1);
        buff[0] = '\0';
        size_t buff_size = 1;

        append_to_buff(&buff, &buff_size, "\0337", 2);
        append_to_buff(&buff, &buff_size, *line + pos, *n - pos);
        append_to_buff(&buff, &buff_size, " ", 1);
        append_to_buff(&buff, &buff_size, "\0338", 2);

        print_str(buff, buff_size);
    }
}

/**
 * @brief Arrow left key action
 * 
 * @param pos 
 */
void move_left(int *pos)
{
    if (*pos > 0)
    {
        print_str("\033[D", 3);
        (*pos)--;
    }
}

/**
 * @brief Arrow right key action
 * 
 * @param pos 
 * @param n 
 */
void move_right(int *pos, int n)
{
    if (*pos < n)
    {
        print_str("\033[C", 3);
        (*pos)++;
    }
}

/**
 * @brief Home key action
 * 
 * @param pos 
 */
void home_key(int *pos)
{
    char *buff = malloc(1);
    buff[0] = '\0';
    size_t buff_size = 1;

    for (int i = 0; i < *pos; i++)
        append_to_buff(&buff, &buff_size, "\033[D", 3);

    print_str(buff, buff_size);
    *pos = 0;
}

/**
 * @brief End key action
 * 
 * @param pos 
 * @param n 
 */
void end_key(int *pos, int n)
{
    char *buff = malloc(1);
    buff[0] = '\0';
    size_t buff_size = 1;

    for (int i = 0; i < n - *pos; i++)
        append_to_buff(&buff, &buff_size, "\033[C", 3);

    print_str(buff, buff_size);

    *pos = n;
}

/**
 * @brief Backspace key action
 * 
 * @param pos 
 * @param n 
 * @param line 
 */
void backspace_key(int *pos, int *n, char **line)
{
    if (*pos > 0)
    {
        remove_on_pos(line, *pos);
        (*n)--;
        (*pos)--;

        char *buff = malloc(1);
        buff[0] = '\0';
        size_t buff_size = 1;

        append_to_buff(&buff, &buff_size, "\033[D", 3);
        append_to_buff(&buff, &buff_size, "\0337", 2);
        append_to_buff(&buff, &buff_size, *line + *pos, *n - *pos);
        append_to_buff(&buff, &buff_size, " ", 1);
        append_to_buff(&buff, &buff_size, "\0338", 2);

        print_str(buff, buff_size);
    }
}

/**
 * @brief Enter key action
 * 
 */
void new_line()
{
    print_str("\n", 1);
}

void tab_key(int *pos, int *n, char **line)
{
    char *buff = malloc(1);
    buff[0] = '\0';
    size_t buff_size = 1;

    char *options;

    complete_line(pos, n, line, &options);
    append_to_buff(&buff, &buff_size, options, strlen(options));

    print_str(buff, buff_size);
}

/**
 * @brief Any printable key action
 * 
 * @param pos 
 * @param n 
 * @param c 
 * @param line 
 */
void printable_key(int *pos, int *n, char c, char **line)
{
    char *buff = malloc(1);
    buff[0] = '\0';
    size_t buff_size = 1;

    (*n)++;
    append_to_buff(&buff, &buff_size, &c, 1);

    append_to_pos(line, *pos, c);

    append_to_buff(&buff, &buff_size, "\0337", 2);
    (*pos)++;
    append_to_buff(&buff, &buff_size, *line + *pos, *n - *pos);
    append_to_buff(&buff, &buff_size, "\0338", 2);

    print_str(buff, buff_size);
}