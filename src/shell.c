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

    line = trim_string(&line);
    if (line[strlen(line) - 1] == ' ')
        remove_on_pos(&line, strlen(line));

    term.com.line = line;

    process_line(line, &args);

    if (args[0] != NULL)
        if (strcmp(args[0], "!") == 0)
        {
            term.status.invert = true;
            args = slice_array(args, 1, -1, true);
        }

    int status = execute(args);

    if (term.status.invert)
    {
        if (status == 0)
            term.status.s = 1;
        else
            term.status.s = 0;

        term.status.invert = false;
    }
    else
        term.status.s = status;

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

    for (int i = 0; i < buff_size; i++)
    {
        (*args)[i] = NULL;
    }

    char *tmp = strdup(line), *free_tmp = tmp;

    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == ' ')
        {
            tmp[i] = '\0';
            (*args)[pos] = strdup(tmp);
            tmp += strlen((*args)[pos]) + 1;
            pos++;
        }
        else if (line[i] == '#')
        {
            break;
        }

        if (pos > buff_size)
        {
            buff_size += ARG_SIZE;
            *args = realloc(*args, buff_size * sizeof(char *));
            if (*args == NULL)
            {
                fprintf(stderr, "myshell: memory allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (tmp[0] != '\0' && tmp[0] != '#')
    {
        (*args)[pos] = strdup(tmp);
        pos++;
    }

    if ((*args)[0] == NULL && line[0] != '#' && line[0] != '\0')
    {
        (*args)[0] = strdup(line);
        pos++;
    }

    free(free_tmp);

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

char *get_ip_addr()
{
    struct ifaddrs *host, *tmp;
    getifaddrs(&host);
    tmp = host;
    char *ip = NULL;

    while (tmp)
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
            ip = inet_ntoa(pAddr->sin_addr);
            if (strncmp(ip, "127", 3) != 0)
                break;
        }

        tmp = tmp->ifa_next;
    }

    freeifaddrs(host);

    return ip;
}

/**
 * @brief Creates prompt string
 * 
 * @return char* 
 */
char *compose_prompt()
{
    // New line
    char *prompt = strdup("");

    // Username
    char *username = getenv("USER");
    if (username == NULL)
        username = "none";
    prompt = realloc(prompt, strlen(prompt) + strlen("\033[97;44m") + strlen(username) + 2);
    prompt = strcat(prompt, "\033[97;44m");
    prompt = strcat(prompt, username);

    // Current host ip
    char *ip = get_ip_addr();
    if (ip == NULL)
        ip = "none";
    prompt = realloc(prompt, strlen(prompt) + 1 + strlen(ip) + strlen("\033[39m") + strlen("\033[48;2;28;32;35m") + 2);
    prompt = strcat(prompt, "@");
    prompt = strcat(prompt, ip);
    prompt = strcat(prompt, "\033[39m");
    prompt = strcat(prompt, "\033[48;2;28;32;35m");
    prompt = strcat(prompt, ":");

    // Current path
    char *full_path = get_current_dir_name();
    prompt = realloc(prompt, strlen(prompt) + strlen("\033[92;1m") + strlen("\033[39;0m") + strlen(full_path) + 2);
    prompt = strcat(prompt, "\033[92;1m");
    prompt = strcat(prompt, full_path);
    prompt = strcat(prompt, "\033[39;0m");
    free(full_path);

    // Previous status
    if (term.status.s != 0)
    {
        char *status = malloc(snprintf(NULL, 0, " \033[91m%d\033[39m", term.status.s));
        sprintf(status, " \033[91m%d\033[39m", term.status.s);
        prompt = realloc(prompt, strlen(prompt) + strlen(status) + 1);
        prompt = strcat(prompt, status);
        free(status);
    }

    // Permissions specific character before user input
    prompt = realloc(prompt, strlen(prompt) + 4);
    if (getuid() == 0)
    {
        prompt = strcat(prompt, "\n# ");
    }
    else
        prompt = strcat(prompt, "\n% ");

    return prompt;
}