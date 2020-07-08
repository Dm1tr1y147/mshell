#ifndef _COMPLETE_H
#define _COMPLETE_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <features.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

void complete_line(int *pos, int *n, char **line, char **out);

#endif