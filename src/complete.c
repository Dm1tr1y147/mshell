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
    char **args, **folders = malloc(0);
    size_t sz;

    int am = sep_string(line, &args, " ");

    char *last_arg = args[am - 1];

    if (am > 0)
    {
        if (last_arg[0] == '/')
        {
            int path_depth = sep_string(last_arg, &folders, "/");
            *to_complete = strdup(folders[path_depth - 1]);
            sz = get_dir_list(opts, last_arg, 0); // pass / + all $folders instead of last_arg
        }
        else if (last_arg[0] == '.' && last_arg[1] == '/')
        {
            int path_depth = sep_string(last_arg + 1, &folders, "/");
            *to_complete = strdup(folders[path_depth - 1]);
            sz = get_dir_list(opts, get_current_dir_name(), 0); // pass get_current_dir_name() + all $folders instead of get_current_dir_name()
        }
        else if (strchr(line, ' '))
        {
            int path_depth = sep_string(last_arg, &folders, "/");
            *to_complete = strdup(folders[path_depth - 1]);
            sz = get_dir_list(opts, get_current_dir_name(), 0); // pass get_current_dir_name() + all $folders instead of get_current_dir_name()
        }
        else
            goto ABSOLUTE;
    }
    else
    {
    ABSOLUTE:
        *to_complete = strdup(line);
        sz = get_dir_list(opts, "/usr/bin", 1);
    }

    free_str_arr(args);
    free_str_arr(folders);

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

    *size = list_strings_containing(child_dirs_root, filter_string, comp_list);

    free_tree(child_dirs_root);

    return *size;
}