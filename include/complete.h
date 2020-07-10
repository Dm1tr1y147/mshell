#ifndef _COMPLETE_H
#define _COMPLETE_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <features.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>

bool check_if_executable(char *path, char *file_name);
size_t get_dir_list(char ***dir_list, char *path, int ex);
size_t get_complete_options(char ***opts, char *line, char **to_complete);
size_t complete_line(int *pos, int *n, char **line, char **out);
size_t filter_options(char ***comp_list, size_t *size, char *filter_string);

#endif