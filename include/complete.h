#ifndef _COMPLETE_H
#define _COMPLETE_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <features.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

size_t get_dir_list(char ***dir_list, char *path);
void complete_line(int *pos, int *n, char **line, char **out);

#endif