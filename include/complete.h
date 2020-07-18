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
ssize_t get_dir_list(char ***dir_list, char *path, int ex);
ssize_t get_complete_options(char ***opts, char *line, char **to_complete);
ssize_t complete_line(int *pos, int *n, char **line, char **out);
ssize_t filter_options(char ***comp_list, ssize_t *size, char *filter_string);
ssize_t get_path_commands_list(char ***opts);

#endif