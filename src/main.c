#include <signal.h>

#include "../include/input.h"
#include "../include/utils.h"
#include "../include/shell.h"

void exit_shell()
{
    change_mode(0);
}

int main(int argc, char **argv)
{
    change_mode(1);
    signal(SIGINT, SIG_IGN);

    atexit(exit_shell);

    while (1)
    {
        process_command();
    }
}