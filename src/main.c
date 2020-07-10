#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/input.h"
#include "../include/utils.h"
#include "../include/shell.h"

FILE *log_file;

void exit_shell()
{
    change_mode(0);
}

int main(int argc, char **argv)
{
    log_file = fopen("/var/log/mshell.log", "w");
    if (log_file == NULL)
    {
        fprintf(stderr, "Couldn't open log file\n");
    }
    else {
        fprintf(log_file, "\n New session:\n");
        fclose(log_file);
    }

    change_mode(1);
    signal(SIGINT, SIG_IGN);

    atexit(exit_shell);

    while (1)
    {
        process_command();
    }
}