#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void append_to_pos(char **str, int pos, char ch);
void remove_on_pos(char **str, int pos);

#endif