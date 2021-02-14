#include "../include/shell.h"
#include "../include/utils.h"
#include "../include/input.h"
#include "../include/output.h"

/**
 * @brief Function for main loop. It prints prompt, reads user's input and executes it
 */
void process_command()
{
  char *prompt = compose_prompt();
  print_str(prompt, strlen(prompt));

  char *line = read_line();

  char *tmp = trim_string(line, false);
  free(line);
  line = tmp;

  cmds_p *coms = process_line(line);

  cmds_p *curr = coms;
  int status = 0;
  while (curr != NULL)
  {
    status = execute_with_pipes(curr);

    if (curr->stat.invert)
    {
      if (status == 0)
        status = 1;
      else
        status = 0;
    }

    curr->stat.s = term.last_status = status;

    if (curr->sep_next == AND_SEP)
    {
      if (term.last_status != 0)
        break;
    }
    else if (curr->sep_next == OR_SEP)
    {
      if (term.last_status == 0)
        break;
    }

    curr = curr->next;
  }

  free(line);
  free(prompt);
  free_cmds_p(coms);
}

/**
 * @brief Initialize command structure
 * 
 * @return cmds_p*
 */
cmds_p *new_cmd()
{
  cmds_p *new = malloc(sizeof(cmds_p));

  new->pipe = new_cmd_pipe();
  new->pipes_am = 1;

  new->envs = calloc(0, sizeof(char *));
  new->envs_am = 0;

  new->stat.s = 0;
  new->sep_next = NO_SEP;
  new->stat.invert = false;
  new->next = NULL;

  return new;
}

/**
 * @brief Initialize command pipe structure
 * 
 * @return cmd_pipe* 
 */
cmd_pipe *new_cmd_pipe()
{
  cmd_pipe *new = malloc(sizeof(cmd_pipe));
  new->args = calloc(0, sizeof(char *));
  new->args_am = 0;
  new->next = NULL;
  new->pipefd[0] = new->pipefd[1] = -1;

  return new;
}

void free_cmds_p(cmds_p *root)
{
  if (root != NULL)
  {
    if (root->pipe != NULL)
      free_cmd_pipe(root->pipe);

    if (root->next != NULL)
      free_cmds_p(root->next);

    free(root);
  }
  return;
}

void free_cmd_pipe(cmd_pipe *root)
{
  if (root != NULL)
  {
    if (root->next != NULL)
      free_cmd_pipe(root->next);

    free(root);
  }
  return;
}