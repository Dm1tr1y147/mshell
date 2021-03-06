#include "../include/output.h"

/**
 * @brief Appends to buffer string
 * 
 * @param buff 
 * @param buff_size 
 * @param ap 
 * @param ap_size 
 * @return size_t 
 */
size_t append_to_buff(char **buff, size_t *buff_size, char *ap, size_t ap_size)
{
  *buff = realloc(*buff, *buff_size + ap_size);
  memcpy(*buff + *buff_size - 1, ap, ap_size);

  *buff_size += ap_size;
  (*buff)[*buff_size - 1] = '\0';

  return *buff_size;
}

/**
 * @brief Prints buffer string
 * 
 * @param buff 
 * @param size 
 */
void print_str(char *buff, size_t size)
{
  write(STDOUT_FILENO, buff, size);
}