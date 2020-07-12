#include "../include/shell.h"
#include "../include/input.h"
#include "../include/output.h"
#include "../include/utils.h"

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
 * @brief Function for main loop. It prints prompt, reads user's input and executes it
 */
void process_command()
{
    char **args = NULL;

    char *prompt = compose_prompt();
    print_str(prompt, strlen(prompt));

    char *line = read_line();
    process_line(line, &args);
    int status = execute(args);

    free(line);
    free(prompt);
    free_str_arr(args);
}

/**
 * @brief Extracts command and its arguments from line
 * 
 * @param line 
 * @param args 
 */
int process_line(char *line, char ***args)
{
    int buff_size = ARG_SIZE, pos = 0;
    *args = malloc(buff_size * sizeof(char *));
    char *tok = NULL, *rest = strdup(line), *free_rest = rest;

    while ((tok = strtok_r(rest, " ", &rest)) != NULL)
    {
        (*args)[pos] = strdup(tok);
        pos++;

        if (pos > buff_size)
        {
            buff_size += ARG_SIZE;
            *args = realloc(*args, buff_size * sizeof(char *));
            if (*args == NULL)
            {
                fprintf(stderr, "myshell: allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }

    free(free_rest);

    (*args)[pos] = NULL;
    return pos;
}

/**
 * @brief Executes either builtin or system command
 * 
 * @param args 
 * @return int 
 */
int execute(char **args)
{
    if (args[0] == NULL)
        return 1;

    for (int i = 0; i < BUILTIN_NUM; i++)
        if (strcmp(args[0], builtin[i]) == 0)
            return (*builtin_func[i])(args);

    return launch(args);
}

/**
 * @brief Launches separate process and executes specified command
 * 
 * @param args 
 * @return int 
 */
int launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        sh_exec(args);
    }
    else if (pid < 0)
    {
        perror("myshell");
    }
    else
        do
            wpid = waitpid(pid, &status, WUNTRACED);
        while (!WIFEXITED(status) && !WIFSIGNALED(status));

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
        perror("myshell");

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
        perror("myshell");
    }

    exit(EXIT_FAILURE);
}

/**
 * @brief Creates prompt string
 * 
 * @return char* 
 */
char *compose_prompt()
{
    char *prompt = strdup("\n");

    char *full_path = get_current_dir_name();
    prompt = realloc(prompt, strlen(prompt) + strlen(full_path) + 2);
    prompt = strcat(prompt, full_path);

    free(full_path);

    prompt = realloc(prompt, strlen(prompt) + 4);
    if (getuid() == 0)
    {
        prompt = strcat(prompt, "\n# ");
    }
    else
        prompt = strcat(prompt, "\n% ");

    return prompt;
}