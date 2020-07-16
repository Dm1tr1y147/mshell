#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/input.h"
#include "../include/utils.h"
#include "../include/shell.h"

// Definitions
t_ term;
void exit_shell();
t_ init_term();

// Main
int main()
{
    term = init_term();

    while (1)
    {
        process_command();
    }
}

// Init
t_ init_term()
{

    FILE *log_file = fopen("/var/log/mshell.log", "w");
    if (log_file == NULL)
    {
        fprintf(stderr, "Couldn't open log file\n");
    }
    else
    {
        fprintf(log_file, "\n New session:\n");
        fclose(log_file);
    }

    change_mode(1);

    term.hist.length = 0;
    term.hist.pos = -1;
    term.hist.content = calloc(term.hist.length, sizeof(char *));

    term.hist.sub.length = -1;
    term.hist.sub.pos = -1;
    term.hist.sub.content = calloc(0, sizeof(char *));

    term.last_status = 0;

    signal(SIGINT, SIG_IGN);

    atexit(exit_shell);

    return term;
}

// Exit
void exit_shell()
{
    change_mode(0);
}