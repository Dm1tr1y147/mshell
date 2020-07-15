#ifndef _SHELL_H
#define _SHELL_H

#define _GNU_SOURCE

#include <features.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <ifaddrs.h>
#include <stdbool.h>

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
    int length;
    int pos;
    char *curr_command;

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

typedef struct commands
{
    char **args;
    struct status stat;
    struct commands *next;
    cmd_sep sep_next;
} cmds_p;

typedef struct
{
    struct history hist;
    char **envs;
    int last_status;
} t_;

//Globals defenition
extern t_ term;
extern char *builtin[];

// Functions prototypes
int process_line(char *line, cmds_p **coms);
int launch(char **args);
void process_command();
int execute(cmds_p *command);

void sig_handler();

int sh_cd(char **args);
int sh_exit(char **args);
int sh_exec(char **args);

char *compose_prompt();

cmds_p *new_cmd();

#define BUILTIN_NUM sizeof(builtin) / sizeof(char *)

#endif