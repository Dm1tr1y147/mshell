#ifndef _HISTORY_H
#define _HISTORY_H

#include <sys/stat.h>

void append_to_history(char *line);
void clear_sub_history();
char *previous_hist_entry(char *line);
char *next_hist_entry(char *line);
void init_history();
void open_history_file();
void close_history_file();

#endif