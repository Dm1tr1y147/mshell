#include "../include/shell.h"
#include "../include/utils.h"
#include "../include/input.h"

/**
 * @brief Extracts command and its arguments from line
 * 
 * @param line 
 * @param args 
 */
cmds_p *process_line(char *line)
{
    int line_size = strlen(line);

    if (line_size < 1)
        return NULL;

    char *tmp = strdup(line), *free_tmp = tmp;
    cmds_p *coms = new_cmd();

    cmds_p *curr_cmd = coms;
    cmd_pipe *curr_pipe = curr_cmd->pipe;
    curr_pipe->pipefd[0] = STDIN_FILENO;

    for (int i = 0; i < line_size; i++)
    {
        if (line[i] == '"')
        {
            tmp++;
            i++;
            int j = i;
            for (; j < line_size; j++)
                if (line[j] == '"')
                {
                    free_tmp[j] = '\0';

                    append_to_str_arr(&curr_pipe->args, &curr_pipe->args_am, trim_string(tmp, false));

                    tmp += strlen(curr_pipe->args[curr_pipe->args_am - 1]) + 1;

                    break;
                }

            if (j >= line_size)
            {
                char *ap_line = read_line();

                line = realloc(line, strlen(line) + strlen(ap_line) + 1);
                line = strcat(line, ap_line);

                line_size = strlen(line);

                free(ap_line);
                i--;
                continue;
            }

            i = j;

            if (tmp[0] == ' ')
            {
                tmp++;
                i++;
            }
        }
        else if (line[i] == ';' || (line[i] == '&' && line[i + 1] == '&') || (line[i] == '|' && line[i + 1] == '|'))
        {
            free_tmp[i] = '\0';
            if (line[i - 1] != ' ')
            {
                append_to_str_arr(&curr_pipe->args, &curr_pipe->args_am, trim_string(tmp, false));
                tmp += strlen(curr_pipe->args[curr_pipe->args_am - 1]) + 1;
            }
            else
                tmp++;

            curr_pipe->args = realloc(curr_pipe->args, sizeof(char *) * (curr_pipe->args_am + 1));
            curr_pipe->args[curr_pipe->args_am] = NULL;

            switch (line[i])
            {
            case ';':
                curr_cmd->sep_next = SEMICOLON_SEP;
                break;

            case '&':
                curr_cmd->sep_next = AND_SEP;
                i++;
                tmp++;
                break;

            case '|':
                curr_cmd->sep_next = OR_SEP;
                i++;
                tmp++;
                break;
            }

            if (tmp[0] == ' ')
            {
                tmp++;
                i++;
            }

            if (curr_pipe->args[0] != NULL)
            {
                if (strcmp(curr_pipe->args[0], "!") == 0)
                {
                    curr_cmd->stat.invert = true;
                }
            }

            cmds_p *next = new_cmd();

            curr_cmd->next = next;

            curr_cmd = curr_cmd->next;

            curr_pipe = curr_cmd->pipe;
        }
        else if (line[i] == '|')
        {
            if (line[i - 1] != ' ')
            {
                append_to_str_arr(&curr_pipe->args, &curr_pipe->args_am, trim_string(tmp, false));
                tmp += strlen(curr_pipe->args[curr_pipe->args_am - 1]) + 1;
            }
            else
                tmp++;

            curr_pipe->args = realloc(curr_pipe->args, sizeof(char *) * (curr_pipe->args_am + 1));
            curr_pipe->args[curr_pipe->args_am] = NULL;

            if (tmp[0] == ' ')
            {
                tmp++;
                i++;
            }

            if (curr_pipe->args[0] != NULL)
            {
                if (strcmp(curr_pipe->args[0], "!") == 0)
                {
                    curr_cmd->stat.invert = true;
                }
            }

            cmd_pipe *next = new_cmd_pipe();

            curr_pipe->next = next;
            curr_cmd->pipes_am++;
            curr_pipe = curr_pipe->next;
        }
        else if (line[i] == '>' || line[i] == '<')
        {
            int flags = 0;
            int f = 0;

            if (line[i] == '>' && line[i + 1] == '>')
            {
                flags = O_CREAT | O_APPEND | O_WRONLY;
                i++;
                tmp++;
                f = 0;
            }
            else if (line[i] == '>')
            {
                flags = O_CREAT | O_TRUNC | O_WRONLY;
                f = 0;
            }
            else if (line[i] == '<' && line[i + 1] == '>')
            {
                flags = O_CREAT | O_RDWR;
                i++;
                tmp++;
                f = 1;
            }
            else if (line[i] == '<')
            {
                flags = O_RDONLY;
                f = 2;
            }

            i++;
            tmp++;

            if (line[i] == ' ')
            {
                i++;
                tmp++;
            }

            int j = i;
            for (; j < line_size; j++)
                if (line[j] == ' ' || line[j] == '|' || line[j] == '&' || line[j] == ';')
                    break;

            free_tmp[j] = '\0';

            if (tmp[0] == '"')
                tmp++;
            if (free_tmp[j - 1] == '"')
                free_tmp[j - 1] = '\0';

            int ffd;
            if ((ffd = open(tmp, flags)) < 0)
            {
                perror("file redirection: open");
                return NULL;
            }

            if (f == 0)
                curr_pipe->pipefd[1] = ffd;
            else if (f == 1)
                for (int k = 0; k < 2; k++)
                    curr_pipe->pipefd[k] = ffd;
            else if (f == 2)
                curr_pipe->pipefd[0] = ffd;

            tmp += j - i + 1;
            i = j;
        }
        else if (line[i] == '$')
        {
            tmp++;
            i++;
            int j = i;

            for (; j < line_size; j++)
                if (line[j] == ' ')
                    break;

            free_tmp[j] = '\0';

            char *msg = getenv(tmp);
            if (msg != NULL)
                append_to_str_arr(&curr_pipe->args, &curr_pipe->args_am, "\n");

            tmp += j - i + 1;
            i = j;
        }
        else if (line[i] == ' ')
        {
            free_tmp[i] = '\0';

            char *ap_string = trim_string(tmp, false), **exp = calloc(0, sizeof(char *));

            int sz = expand_wildcatrd(&exp, ap_string);

            for (int l = 0; l < sz; l++)
                append_to_str_arr(&curr_pipe->args, &curr_pipe->args_am, exp[l]);

            tmp += strlen(ap_string) + 1;
        }
        else if (line[i] == '#')
        {
            break;
        }
    }

    if (tmp[0] != '\0' && tmp[0] != '#')
    {
        if (tmp[strlen(tmp) - 1] != '"')
        {
            char *ap_string = trim_string(tmp, false), **exp = calloc(0, sizeof(char *));

            int sz = expand_wildcatrd(&exp, ap_string);

            for (int l = 0; l < sz; l++)
                append_to_str_arr(&curr_pipe->args, &curr_pipe->args_am, exp[l]);
        }
        else
            append_to_str_arr(&curr_pipe->args, &curr_pipe->args_am, trim_string(tmp, false));
    }

    if (curr_pipe->args[0] != NULL)
    {
        if (strcmp(curr_pipe->args[0], "!") == 0)
        {
            curr_cmd->stat.invert = true;
        }
    }

    free(free_tmp);

    if (curr_pipe->args[curr_pipe->args_am] != NULL)
    {
        curr_pipe->args = realloc(curr_pipe->args, sizeof(char *) * (curr_pipe->args_am + 1));
        curr_pipe->args[curr_pipe->args_am] = NULL;
    }
    if (curr_pipe->pipefd[1] < 0)
        curr_pipe->pipefd[1] = STDOUT_FILENO;

    return coms;
}

/**
 * @brief Expands wildcard in argument
 * 
 * @param arr 
 * @param input 
 * @return int 
 */
int expand_wildcatrd(char ***arr, char *input)
{
    int size = 0;

    if (strchr(input, '*') || strchr(input, '~') || strchr(input, '?'))
    {
        glob_t globbuf;
        int t = glob(input, GLOB_TILDE, NULL, &globbuf);
        if (t != 0)
            perror("Glob");

        for (int j = 0; j < globbuf.gl_pathc; j++)
            append_to_str_arr(arr, &size, globbuf.gl_pathv[j]);
    }
    else
        append_to_str_arr(arr, &size, input);

    return size;
}