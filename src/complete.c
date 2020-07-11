#include "../include/complete.h"
#include "../include/shell.h"
#include "../include/tree.h"
#include "../include/utils.h"

bool check_if_executable(char *path, char *file_name)
{
    char *file_path = malloc(strlen(path) + strlen(file_name) + 2);
    file_path[0] = '\0';
    file_path = strcat(file_path, path);
    file_path = strcat(file_path, "/");
    file_path = strcat(file_path, file_name);

    bool ret = access(file_path, X_OK) == 0;
    free(file_path);

    return ret;
}

size_t get_dir_list(char ***dir_list, char *path, int ex)
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(path)) == NULL)
    {
        perror("\nOpendir");
        return -1;
    }

    size_t n = 0;
    while ((ent = readdir(dir)) != NULL)
    {
        if (ex != 0 && !check_if_executable(path, ent->d_name))
            continue;

        n++;
        *dir_list = realloc(*dir_list, sizeof(char *) * n);
        (*dir_list)[n - 1] = strdup(ent->d_name);

        if (ent->d_type == DT_DIR)
        {
            (*dir_list)[n - 1] = realloc((*dir_list)[n - 1], strlen((*dir_list)[n - 1]) + 2);
            (*dir_list)[n - 1] = strcat((*dir_list)[n - 1], "/");
        }
    }

    closedir(dir);

    return n;
}

size_t get_complete_options(char ***opts, char *line, char **to_complete)
{
    char **args = NULL, **folders = malloc(0);
    size_t sz;

    int am = sep_string(line, &args, " ");

    char *last_arg = args[am - 1];

    if (am > 0)
    {
        int path_depth = sep_string(last_arg, &folders, "/");
        *to_complete = strdup(folders[path_depth - 1]);

        char *curr_pos = NULL;
        int i = 0;

        if (last_arg[0] == '/')
        {
            curr_pos = strdup("");
        }
        else if (strchr(line, ' '))
        {
            curr_pos = strdup(".");
        }
        else if (last_arg[0] == '.' && last_arg[1] == '/')
        {
            curr_pos = strdup(folders[0]);
            i++;
        }
        else
            goto ABSOLUTE;

        for (int i = 0; i < (path_depth - 1); i++)
        {
            curr_pos = realloc(curr_pos, strlen(curr_pos) + strlen(folders[i]) + 2);
            curr_pos = strcat(curr_pos, "/");
            curr_pos = strcat(curr_pos, folders[i]);
        }
        sz = get_dir_list(opts, curr_pos, 0);

        free(curr_pos);
    }
    else
    {
    ABSOLUTE:
        *to_complete = strdup(line);
        sz = get_dir_list(opts, "/usr/bin", 1);
    }

    free_str_arr(args);
    free_str_arr(folders);

    if (sz == -1)
        return sz;

    if ((*to_complete)[0] != '\0')
    {
        sz = filter_options(opts, &sz, *to_complete);
    }

    return sz;
}

size_t filter_options(char ***comp_list, size_t *size, char *filter_string)
{
    struct tree_node *child_dirs_root = get_new_node();
    for (size_t i = 0; i < *size; i++)
        insert_tree(child_dirs_root, (*comp_list)[i]);

    char **folders = NULL;
    int path_depth = sep_string(filter_string, &folders, "/");

    char *last_option = strdup(filter_string);

    if (path_depth > 0)
    {
        free(last_option);
        last_option = folders[path_depth - 1];
    }

    *size = list_strings_containing(child_dirs_root, last_option, comp_list);

    free_tree(child_dirs_root);
    free_str_arr(folders);

    return *size;
}