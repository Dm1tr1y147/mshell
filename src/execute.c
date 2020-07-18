#include "../include/shell.h"
#include "../include/utils.h"
#include "../include/input.h"

/* Global definitions */
char *builtin[] = {
    "cd",
    "exec",
    "export",
    "exit"};

int (*builtin_func[])(char **) = {
    &sh_cd,
    &sh_exec,
    &sh_export,
    &sh_exit};

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

        status = execute(curr, command->envs);

        curr = curr->next;
    }

    status = execute(curr, command->envs);

    return status;
}

/**
 * @brief Executes either builtin or system command
 * 
 * @param args 
 * @return int 
 */
int execute(cmd_pipe *command, char **envp)
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

    return launch(command, envp);
}

/**
 * @brief Launches separate process and executes specified command
 * 
 * @param args 
 * @return int 
 */
int launch(cmd_pipe *command, char **envp)
{
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        redirect_fd(command->pipefd[0], STDIN_FILENO);

        redirect_fd(command->pipefd[1], STDOUT_FILENO);

        sh_exec(command->args, envp);
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
 * @brief Shell builtin command. Executes command replacing shell with it
 * 
 * @param args 
 * @return int 
 */
int sh_exec(char **args, char **envp)
{
    change_mode(0);
    signal(SIGINT, SIG_DFL);

    if (strcmp(args[0], "exec") == 0)
        args = slice_array(args, 1, -1, 1);

    if (mexecvpe(args[0], args, envp) < 0)
    {
        perror("mshell");
    }

    exit(EXIT_FAILURE);
}

int sh_export(char **args)
{
    if (args[1] == NULL)
    {
        for (int i = 0; environ[i] != NULL; i++)
            printf("declare -x %s\n", environ[i]);

        return 0;
    }

    for (int i = 1; args[i] != NULL; i++)
    {
        char *name = strtok(args[i], "="), *val = strtok(NULL, "=");

        if (name == NULL || val == NULL)
        {
            fprintf(stderr, "export: wrong arguments\n");
            return 1;
        }

        setenv(name, val, true);

        printf("Set %s variable to %s\n", name, getenv(name));
    }
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

int mexecvpe(char *file, char **argv, char **envp)
{
    if (file == NULL || file[0] == '\0')
        return -1;

    if (envp == NULL)
        execvp(file, argv);

    if (strchr(file, '/') != NULL)
        execve(file, argv, envp);

    char *path = getenv("PATH");

    size_t file_len = strlen(file) + 1, path_len = strlen(path) + 1;

    char buff[file_len + path_len + 1], *subp;

    for (char *p = path;; p = subp)
    {
        subp = strchr(p, ':');
        memcpy(buff, p, subp - p);

        buff[subp - p] = '/';

        memcpy(buff + (subp - p) + (p < subp), file, file_len);

        execve(buff, argv, envp);

        if (*subp++ == '\0')
            break;
    }
}