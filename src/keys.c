#include "../include/keys.h"
#include "../include/output.h"
#include "../include/utils.h"
#include "../include/complete.h"
#include "../include/shell.h"
#include "../include/history.h"
#include "../include/input.h"

/**
 * @brief Delete key action
 * 
 * @param pos 
 * @param n 
 * @param line 
 */
void delete_key(int pos, int *n, char **line)
{
  if (pos < *n)
  {
    remove_on_pos(line, pos + 1);
    (*n)--;

    char *buff = strdup("");
    size_t buff_size = 1;

    append_to_buff(&buff, &buff_size, "\0337", 2);
    append_to_buff(&buff, &buff_size, *line + pos, *n - pos);
    append_to_buff(&buff, &buff_size, " ", 1);
    append_to_buff(&buff, &buff_size, "\0338", 2);

    print_str(buff, buff_size);
    free(buff);
  }
}

/**
 * @brief Up key action
 * 
 * @param pos 
 * @param n 
 * @param line 
 */
void up_key(int *pos, int *n, char **line)
{

  char *buff = strdup("\033[K");
  size_t buff_size = 4;

  char *entry = NULL;

  if (*pos == 0)
    entry = previous_hist_entry(NULL);
  else
  {
    (*line)[*pos] = '\0';
    entry = previous_hist_entry(*line);
  }

  if (entry == NULL)
  {
    free(buff);
    return;
  }

  free(*line);
  *line = entry;
  *n = strlen(*line);

  append_to_buff(&buff, &buff_size, "\0337", 2);
  append_to_buff(&buff, &buff_size, *line + *pos, *n - *pos);
  append_to_buff(&buff, &buff_size, "\0338", 2);

  print_str(buff, buff_size);
  free(buff);
}

/**
 * @brief Down key action
 * 
 * @param pos 
 * @param n 
 * @param line 
 */
void down_key(int *pos, int *n, char **line)
{

  char *buff = strdup("\033[K");
  size_t buff_size = 4;

  char *entry = NULL;

  if (*pos == 0)
  {
    if (term.hist.pos <= 0)
    {
      if (term.hist.pos == 0)
        term.hist.pos--;

      free(buff);
      free_input(pos, n, line);
      return;
    }

    entry = next_hist_entry(NULL);
  }
  else
  {
    (*line)[*pos] = '\0';

    if (term.hist.sub.pos <= 0)
    {
      if (term.hist.sub.pos == 0)
        term.hist.sub.pos--;

      free(buff);
      free_input(pos, n, line);
      return;
    }

    entry = next_hist_entry(*line);
  }

  if (entry == NULL)
  {
    free(buff);
    return;
  }

  *line = entry;
  *n = strlen(*line);

  append_to_buff(&buff, &buff_size, "\0337", 2);
  append_to_buff(&buff, &buff_size, *line + *pos, *n - *pos);
  append_to_buff(&buff, &buff_size, "\0338", 2);

  print_str(buff, buff_size);
  free(buff);
}

/**
 * @brief Arrow left key action
 * 
 * @param pos 
 */
void move_left(int *pos)
{
  clear_sub_history();
  if (*pos > 0)
  {
    print_str("\033[D", 3);
    (*pos)--;
  }
}

/**
 * @brief Arrow right key action
 * 
 * @param pos 
 * @param n 
 */
void move_right(int *pos, int n)
{
  if (*pos < n)
  {
    print_str("\033[C", 3);
    (*pos)++;

    clear_sub_history();
  }
}

/**
 * @brief Home key action
 * 
 * @param pos 
 */
void home_key(int *pos)
{
  clear_sub_history();

  char *buff = strdup("");
  size_t buff_size = 1;

  for (int i = 0; i < *pos; i++)
    append_to_buff(&buff, &buff_size, "\033[D", 3);

  print_str(buff, buff_size);
  *pos = 0;

  free(buff);
}

/**
 * @brief End key action
 * 
 * @param pos 
 * @param n 
 */
void end_key(int *pos, int n)
{
  char *buff = strdup("");
  size_t buff_size = 1;

  for (int i = 0; i < n - *pos; i++)
    append_to_buff(&buff, &buff_size, "\033[C", 3);

  print_str(buff, buff_size);

  *pos = n;

  free(buff);
}

/**
 * @brief Backspace key action
 * 
 * @param pos 
 * @param n 
 * @param line 
 */
void backspace_key(int *pos, int *n, char **line)
{
  if (*pos > 0)
  {
    clear_sub_history();

    remove_on_pos(line, *pos);
    (*n)--;
    (*pos)--;

    char *buff = strdup("");
    size_t buff_size = 1;

    append_to_buff(&buff, &buff_size, "\033[D", 3);
    append_to_buff(&buff, &buff_size, "\0337", 2);
    append_to_buff(&buff, &buff_size, *line + *pos, *n - *pos);
    append_to_buff(&buff, &buff_size, " ", 1);
    append_to_buff(&buff, &buff_size, "\0338", 2);

    print_str(buff, buff_size);

    free(buff);
  }
}

/**
 * @brief Enter key action
 * 
 */
void new_line(char *line)
{
  append_to_history(line);
  clear_sub_history();
  print_str("\n", 1);
}

/**
 * @brief Tab key action
 * 
 * @param pos 
 * @param n 
 * @param line 
 */
void tab_key(int *pos, int *n, char **line)
{
  clear_sub_history();

  (*line)[*pos] = '\0';
  *line = realloc(*line, strlen(*line) + 1);

  char *tmp = trim_string(*line, true);
  free(*line);
  *line = tmp;

  *pos = strlen(*line);
  *n = *pos;

  char *buff = strdup("");
  size_t buff_size = 1;

  char **complete_options = calloc(0, sizeof(char *)), *to_complete = NULL;
  size_t opts_sz = get_complete_options(&complete_options, *line, &to_complete);

  if (opts_sz == 1)
  {
    char *ending = complete_options[0] + strlen(to_complete);
    *pos += strlen(ending);
    *n = *pos;

    *line = realloc(*line, strlen(*line) + strlen(ending) + 1);
    *line = strcat(*line, ending);

    append_to_buff(&buff, &buff_size, ending, strlen(ending));
  }
  else
  {
    append_to_buff(&buff, &buff_size, "\n", 1);

    if ((int)opts_sz < 1)
    {
      append_to_buff(&buff, &buff_size, "No suggestions", strlen("No suggestions"));
    }

    for (int i = 0; i < (int)opts_sz; i++)
    {
      append_to_buff(&buff, &buff_size, complete_options[i], strlen(complete_options[i]));
      append_to_buff(&buff, &buff_size, " ", 1);
    }

    append_to_buff(&buff, &buff_size, "\n", 1);

    char *prompt = compose_prompt();
    append_to_buff(&buff, &buff_size, prompt, strlen(prompt));
    free(prompt);

    append_to_buff(&buff, &buff_size, *line, *pos);
  }

  print_str(buff, buff_size);

  free(buff);
  free(to_complete);
}

/**
 * @brief Any printable key action
 * 
 * @param pos 
 * @param n 
 * @param c 
 * @param line 
 */
void printable_key(int *pos, int *n, char c, char **line)
{
  char *buff = strdup("");
  size_t buff_size = 1;

  (*n)++;
  append_to_buff(&buff, &buff_size, &c, 1);

  append_to_pos(line, *pos, c);

  append_to_buff(&buff, &buff_size, "\0337", 2);
  (*pos)++;
  append_to_buff(&buff, &buff_size, *line + *pos, *n - *pos);
  append_to_buff(&buff, &buff_size, "\0338", 2);

  print_str(buff, buff_size);

  free(buff);
}