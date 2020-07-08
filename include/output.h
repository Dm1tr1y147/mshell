#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

size_t append_to_buff(char **buff, size_t *buff_size, char *ap, size_t ap_size);
void print_str(char *buff, size_t size);

#endif