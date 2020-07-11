#ifndef _SHELL_H
#define _SHELL_H

#define _GNU_SOURCE

#include <features.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#define BUFF_SIZE 1024
#define ARG_SIZE 32

int process_line(char *line, char ***args);
int launch(char **args);
void process_command();
int execute(char **args);

void sig_handler();

int sh_cd(char **args);
int sh_exit(char **args);
int sh_exec(char **args);

char *compose_prompt();

#define BUILTIN_NUM sizeof(builtin) / sizeof(char *)

#endif