#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/input.h"
#include "../include/utils.h"
#include "../include/shell.h"
#include "../include/history.h"

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
    // Log file
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

    // Entering raw mode
    change_mode(1);

    // Init history
    init_history();

    term.last_status = 0;

    // Disable Ctrl+C default behaviour for shell
    signal(SIGINT, SIG_IGN);

    // Set up function to run on shell exit
    atexit(exit_shell);

    return term;
}

// Exit
void exit_shell()
{
    change_mode(0);
}