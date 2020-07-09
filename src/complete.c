#include "../include/complete.h"
#include "../include/shell.h"
#include "../include/tree.h"

size_t get_dir_list(char ***dir_list, char *path)
{
    size_t n = 0;
    *dir_list = malloc(sizeof(char *) * n);

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(path)) == NULL)
    {
        perror("opendir");
        return -1;
    }
    while ((ent = readdir(dir)) != NULL)
    {
        n++;
        *dir_list = realloc(*dir_list, sizeof(char *) * n);
        (*dir_list)[n - 1] = strdup(ent->d_name);
    }

    closedir(dir);

    return n;
}

void complete_line(int *pos, int *n, char **line, char **out)
{
    (*line)[*pos] = '\0';
    *out = strdup("\x1b[2K");

    char **comp_list;

    char *curr_path, *tmp_line = strdup(*line);
    size_t sz;

    if ((curr_path = strtok(tmp_line, " ")) != NULL && (curr_path = strtok(NULL, " ")) != NULL)
    {
        sz = get_dir_list(&comp_list, get_current_dir_name());
    }
    else
    {
        sz = get_dir_list(&comp_list, "/usr/bin");
        curr_path = *line;
    }
    if (*pos > 0 && (*line)[*pos - 1] != ' ')
    {
        curr_path = strdup(curr_path);

        char *tmp;
        while ((tmp = strtok(NULL, " ")) != NULL)
        {
            free(curr_path);

            curr_path = strdup(tmp);
        }

        struct tree_node *child_dirs_root = get_new_node();
        for (size_t i = 0; i < sz; i++)
        {
            insert_tree(child_dirs_root, comp_list[i]);
        }

        sz = list_strings_containing(child_dirs_root, curr_path, &comp_list);
    }

    if (sz == 1)
    {
        *out = strdup(comp_list[0] + strlen(curr_path));
        *pos += strlen(*out);
        *n = *pos;

        *line = realloc(*line, strlen(*line) + strlen(*out));
        *line = strcat(*line, *out);

        return;
    }

    for (int i = 0; i < sz; i++)
    {
        *out = realloc(*out, strlen(*out) + strlen(comp_list[i]) + 2);
        *out = strcat(*out, comp_list[i]);
        *out = strcat(*out, " ");
    }

    *out = realloc(*out, strlen(*out) + 2);
    *out = strcat(*out, "\n");

    char *prompt = compose_prompt();
    *out = realloc(*out, strlen(*out) + strlen(prompt) + 1);
    *out = strcat(*out, prompt);

    *out = realloc(*out, strlen(*out) + *pos);
    *out = strncat(*out, *line, *pos + 1);

    *n = *pos;
}