#include "../include/shell.h"
#include "../include/utils.h"
#include "../include/input.h"

/* Global definitions */
char *builtin[] = {
    "cd",
    "exit",
    "exec"};

int (*builtin_func[])(char **) = {
    &sh_cd,
    &sh_exit,
    &sh_exec};

/**
 * @brief Redirects command input/output and executes it
 * 
 * @param command 
 * @return int 
 */
int execute_with_pipes(cmds_p *command)
{
    cmd_pipe *curr = command->pipe;
    int status = 0, tmp_fd[2] = {-1};

    for (int i = 0; i < command->pipes_am - 1 && curr != NULL && status == 0; i++)
    {

        if (pipe(tmp_fd) < 0)
        {
            perror("pipe");
            return 1;
        }

        curr->pipefd[1] = tmp_fd[1];
        curr->next->pipefd[0] = tmp_fd[0];

        status = execute(curr);

        curr = curr->next;
    }

    status = execute(curr);

    return status;
}

/**
 * @brief Executes either builtin or system command
 * 
 * @param args 
 * @return int 
 */
int execute(cmd_pipe *command)
{
    if (command->args[0] == NULL)
        return 1;

    char **args = command->args;

    if (strcmp(args[0], "!") == 0)
    {
        args = slice_array(args, 1, -1, true);
        command->args = args;
    }

    for (int i = 0; i < BUILTIN_NUM; i++)
        if (strcmp(args[0], builtin[i]) == 0)
            return (*builtin_func[i])(args);

    return launch(command);
}

/**
 * @brief Launches separate process and executes specified command
 * 
 * @param args 
 * @return int 
 */
int launch(cmd_pipe *command)
{
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        redirect_fd(command->pipefd[0], STDIN_FILENO);

        redirect_fd(command->pipefd[1], STDOUT_FILENO);

        sh_exec(command->args);
    }
    else if (pid < 0)
    {
        perror("mshell");
        return -1;
    }
    else
    {
        for (int j = 0; j < 2; j++)
            if (command->pipefd[j] > 2)
                close(command->pipefd[j]);
        do
            wpid = waitpid(pid, &status, WUNTRACED);
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    change_mode(1);

    return status;
}

/**
 * @brief Shell builtin command. Changes current working directory
 * 
 * @param args 
 * @return int 
 */
int sh_cd(char **args)
{
    if (args[1] == NULL)
        chdir(getenv("HOME"));
    else if (chdir(args[1]) < 0)
    {
        perror("cd");
        return 1;
    }

    return 0;
}

/**
 * @brief Shell builtin command. Exits shell
 * 
 * @param args 
 * @return int 
 */
int sh_exit(char **args)
{
    exit(0);
}

/**
 * @brief Shell builtin command. Executes command replacing shell with it
 * 
 * @param args 
 * @return int 
 */
int sh_exec(char **args)
{
    change_mode(0);
    signal(SIGINT, SIG_DFL);

    if (strcmp(args[0], "exec") == 0)
        args = slice_array(args, 1, -1, 1);

    if (execvp(args[0], args) < 0)
    {
        perror("mshell");
    }

    exit(EXIT_FAILURE);
}

void redirect_fd(int old, int new)
{
    if (old > 0)
        if (old != new)
        {
            if (dup2(old, new) < 0)
                perror("dup2");
            else
                close(old);
        }
}