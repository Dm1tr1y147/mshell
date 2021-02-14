#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <ifaddrs.h>
#include <glob.h>
#include <stdbool.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <arpa/inet.h>

#include "../include/tree.h"

// Defines
#define BUFF_SIZE 1024
#define ARG_SIZE 32

// Types definitions
struct hist_sub
{
  char **content;
  int length;
  int pos;
};

struct history
{
  char **content;
  ssize_t length;
  int pos;
  FILE *file;

  struct hist_sub sub;
};

struct status
{
  int s;
  bool invert;
};

typedef enum
{
  NO_SEP,
  SEMICOLON_SEP,
  AND_SEP,
  OR_SEP
} cmd_sep;

typedef struct pipes
{
  char **args;
  ssize_t args_am;
  int pipefd[2];
  struct pipes *next;
} cmd_pipe;

typedef struct commands
{
  cmd_pipe *pipe;
  ssize_t pipes_am;

  char **envs;
  ssize_t envs_am;

  struct status stat;
  struct commands *next;
  cmd_sep sep_next;
} cmds_p;

typedef struct
{
  struct history hist;
  int last_status;
} t_;

//Globals defenition
extern t_ term;
extern char **environ;
extern char *builtin[];

// shell.c file
void process_command();
cmds_p *new_cmd();
cmd_pipe *new_cmd_pipe();
void free_cmds_p(cmds_p *root);
void free_cmd_pipe(cmd_pipe *root);

// line_parce.c file
cmds_p *process_line(char *line);
int expand_wildcatrd(char ***arr, char *input);

// execute.c file
int execute_with_pipes(cmds_p *command);
int execute(cmd_pipe *command, char **envp);
int launch(cmd_pipe *command, char **envp);
int mexecvpe(char *file, char **argv, char **envp);
int complete_envs(char ***envp);

int sh_cd(char **args);
int sh_exec(char **args, char **envp);
int sh_export(char **args);
int sh_exit(char **args);

void redirect_fd(int old, int new);

// prompt.c
char *get_ip_addr();
char *compose_prompt();

#define BUILTIN_NUM sizeof(builtin) / sizeof(char *)

#endif