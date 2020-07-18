#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

void append_to_pos(char **str, int pos, char ch);
void remove_on_pos(char **str, int pos);
int sep_string(char *line, char ***toks, char *sep);
char *trim_string(char *str, bool leave_trailing_space);
void free_str_arr(char **arr, int sz);
char **slice_array(char **arr, int beg, int end, bool asc);
int get_null_term_arr_size(char **arr);
int append_to_str_arr(char ***arr, ssize_t *sz, char *str);
char *get_curr_dir_name();
int get_num_of_lines(FILE *file);
bool str_is_in_arr(char **arr, size_t sz, char *str);

#endif