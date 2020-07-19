#include "../include/history.h"
#include "../include/shell.h"
#include "../include/utils.h"

/**
 * @brief Append entry to commands history
 * 
 * @param line 
 */
void append_to_history(char *line)
{
    if (term.hist.length > 0)
        if (strcmp(line, term.hist.content[term.hist.length - 1]) == 0)
            return;

    if (strncmp(line, "exit", strlen("exit")) == 0)
        return;

    if (line[0] == '\0')
        return;

    term.hist.length++;
    term.hist.content = (char **)realloc(term.hist.content, term.hist.length * sizeof(char *));
    term.hist.content[term.hist.length - 1] = strdup(line);

    fseek(term.hist.file, 0, SEEK_END);
    fprintf(term.hist.file, "%s\n", line);
    fflush(term.hist.file);

    term.hist.pos = -1;
}

void clear_sub_history()
{
    free(term.hist.sub.content);

    term.hist.sub.content = malloc(0);
    term.hist.sub.length = -1;
    term.hist.sub.pos = -1;
}

/**
 * @brief Get previous history entry
 * 
 * @param line 
 * @return char* 
 */
char *previous_hist_entry(char *line)
{
    static int h_size;
    int f_len = get_num_of_lines(term.hist.file);

    if (term.hist.length != f_len)
    {
        clear_sub_history();
        term.hist.pos = -1;

        char **tmp_h = calloc(sizeof(char *), 0);
        int tmp_h_pos = 0;

        fseek(term.hist.file, 0, SEEK_END);

        while (term.hist.length + tmp_h_pos < f_len && ftell(term.hist.file) > 1)
        {
            char ch = '\0';
            size_t llen = 0;
            while (ch != '\n')
            {
                fseek(term.hist.file, -2, SEEK_CUR);
                if (ftell(term.hist.file) < 1)
                    break;
                ch = fgetc(term.hist.file);
            }

            long f_pos = ftell(term.hist.file);

            tmp_h = realloc(tmp_h, sizeof(char *) * (tmp_h_pos + 1));
            getline(&tmp_h[tmp_h_pos], &llen, term.hist.file);

            fseek(term.hist.file, f_pos, SEEK_SET);

            if (tmp_h[tmp_h_pos][strlen(tmp_h[tmp_h_pos]) - 1] == '\n')
                tmp_h[tmp_h_pos][strlen(tmp_h[tmp_h_pos]) - 1] = '\0';

            tmp_h_pos++;

            fseek(term.hist.file, -2, SEEK_CUR);
        }

        for (int i = 0; i < tmp_h_pos; i++)
            append_to_str_arr(&term.hist.content, &term.hist.length, tmp_h[i]);

        free_str_arr(tmp_h, tmp_h_pos);
    }

    if (line == NULL)
    {
        if (term.hist.pos + 1 == term.hist.length)
            return NULL;

        term.hist.pos++;
        return strdup(term.hist.content[term.hist.length - term.hist.pos - 1]);
    }

    if (term.hist.sub.length < 0)
    {
        term.hist.sub.length++;
        for (int i = 0; i < term.hist.length; i++)
            if (strncmp(term.hist.content[i], line, strlen(line)) == 0)
            {
                term.hist.sub.length++;
                term.hist.sub.content = realloc(term.hist.sub.content, term.hist.sub.length * sizeof(char *));
                term.hist.sub.content[term.hist.sub.length - 1] = term.hist.content[i];
            }
        h_size = term.hist.sub.length;
    }
    else
    {
        int tmp_len = term.hist.sub.length;
        term.hist.sub.length = 0;
        for (int i = 0; i < tmp_len; i++)
            if (strncmp(term.hist.sub.content[i], line, strlen(line)) == 0)
            {
                term.hist.sub.length++;
                term.hist.sub.content[term.hist.sub.length - 1] = term.hist.sub.content[i];
            }
        term.hist.sub.content = realloc(term.hist.sub.content, term.hist.sub.length * sizeof(char *));

        if (term.hist.sub.length != h_size)
        {
            h_size = term.hist.sub.length;
            term.hist.sub.pos = -1;
        }
    }

    term.hist.sub.pos++;

    if (term.hist.sub.pos >= term.hist.sub.length)
    {
        term.hist.sub.pos = term.hist.sub.length - 1;
        return NULL;
    }

    return strdup(term.hist.sub.content[term.hist.sub.length - term.hist.sub.pos - 1]);
}

/**
 * @brief Get next history entry
 * 
 * @param line 
 * @return char* 
 */
char *next_hist_entry(char *line)
{
    if (line == NULL)
    {
        if (term.hist.pos == 0)
            return NULL;

        term.hist.pos--;
        return strdup(term.hist.content[term.hist.length - term.hist.pos - 1]);
    }

    if (term.hist.sub.pos <= 0)
        return NULL;

    term.hist.sub.pos--;
    return strdup(term.hist.sub.content[term.hist.sub.length - term.hist.sub.pos - 1]);
}

void init_history()
{
    term.hist.length = 0;
    term.hist.pos = -1;
    term.hist.content = calloc(term.hist.length, sizeof(char *));

    term.hist.sub.length = -1;
    term.hist.sub.pos = -1;
    term.hist.sub.content = calloc(0, sizeof(char *));

    open_history_file();

    size_t sz = 0;
    int len = 0;
    char *buf = NULL;

    while ((len = getline(&buf, &sz, term.hist.file)) != -1)
    {
        char *tmp = strdup(buf);
        if (tmp[strlen(tmp) - 1] == '\n')
            tmp[strlen(tmp) - 1] = '\0';

        append_to_str_arr(&term.hist.content, &term.hist.length, tmp);
        free(tmp);
    }
    free(buf);
}

void open_history_file()
{
    char *data_path;
    if ((data_path = get_env_var("XDG_DATA_HOME")) == NULL)
    {
        char *user_home;
        if ((user_home = get_env_var("HOME")) == NULL)
            exit(EXIT_FAILURE);

        data_path = user_home;
        data_path = realloc(data_path, strlen(data_path) + strlen("/.local/share/") + 1);
        data_path = strcat(data_path, "/.local/share/");
    }

    struct stat st = {0};
    if (stat(data_path, &st) < 0)
    {
        printf("No %s dir, tying to create\n", data_path);
        char *buff = malloc(strlen("mkdir -p ") + strlen(data_path) + 1);
        buff = strdup("mkdir -p ");
        buff = strcat(buff, data_path);

        system(buff);
    }

    data_path = realloc(data_path, strlen(data_path) + strlen(".mshistory") + 1);
    data_path = strcat(data_path, ".mshistory");

    if ((term.hist.file = fopen(data_path, "a+")) == NULL)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    fseek(term.hist.file, 0, SEEK_SET);

    free(data_path);
}

void close_history_file()
{
    fclose(term.hist.file);
}