#ifndef _KEYS_H
#define _KEYS_H

#include <stdlib.h>

void delete_key(int pos, int *n, char **line);
void up_key(int *pos, int *n, char **line);
void down_key(int *pos, int *n, char **line);
void move_left(int *pos);
void move_right(int *pos, int n);
void home_key(int *pos);
void end_key(int *pos, int n);
void backspace_key(int *pos, int *n, char **line);
void new_line(char *line);
void tab_key(int *pos, int *n, char **line);
void printable_key(int *pos, int *n, char c, char **line);

#endif