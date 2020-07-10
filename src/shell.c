#include "../include/shell.h"
#include "../include/input.h"
#include "../include/output.h"

/* Global definitions */
char *builtin[] = {
    "cd",
    "exit"};

int (*builtin_func[])(char **) = {
    &sh_cd,
    &sh_exit};

/**
 * @brief Function for main loop. It prints prompt, reads user's input and executes it
 */
void process_command()
{
    char **args;
    int status;

    do
    {
        char *line = malloc(1);
        line[0] = '\0';

        char *prompt = compose_prompt();

        print_str(prompt, strlen(prompt));

        line = read_line(&line);

        process_line(line, &args);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
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
    char *tok, *rest = strdup(line);

    while ((tok = strtok_r(rest, " ", &rest)) != NULL)
    {
        (*args)[pos] = tok;
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
        change_mode(0);
        signal(SIGINT, SIG_DFL);

        if (execvp(args[0], args) < 0)
        {
            perror("myshell");
        }

        exit(EXIT_FAILURE);
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
        fprintf(stderr, "myshell: expected arguments for \"cd\"\n");
    else if (chdir(args[1]) < 0)
        perror("myshell");

    return 1;
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

char *compose_prompt()
{
    char *prompt = malloc(1);
    prompt[0] = '\0';

    prompt = realloc(prompt, strlen(prompt) + 4);
    if (getuid() == 0)
    {
        prompt = strcat(prompt, "\n# ");
    }
    else
        prompt = strcat(prompt, "\n% ");

    return prompt;
}