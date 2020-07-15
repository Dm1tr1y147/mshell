#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define log(fmt, ...)                                 \
    {                                                 \
        log_file = fopen("/var/log/mshell.log", "a"); \
        fprintf(log_file, fmt, __VA_ARGS__);          \
        fclose(log_file);                             \
    }

void append_to_pos(char **str, int pos, char ch);
void remove_on_pos(char **str, int pos);
int sep_string(char *line, char ***toks, char *sep);
char *trim_string(char *str, bool leave_trailing_space);
void free_str_arr(char **arr);
char **slice_array(char **arr, int beg, int end, bool asc);
int get_null_term_arr_size(char **arr);
int append_to_str_arr(char ***arr, int *sz, char *str);
char *get_curr_dir_name();

#endif