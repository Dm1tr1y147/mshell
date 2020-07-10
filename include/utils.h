#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern FILE *log_file;

#define log(fmt, ...)                                 \
    {                                                 \
        log_file = fopen("/var/log/mshell.log", "a"); \
        fprintf(log_file, fmt, __VA_ARGS__);          \
        fclose(log_file);                             \
    }

void append_to_pos(char **str, int pos, char ch);
void remove_on_pos(char **str, int pos);
int sep_string(char *line, char ***toks, char *sep);
char *trim_string(char **str);
void free_str_arr(char **arr);

#endif