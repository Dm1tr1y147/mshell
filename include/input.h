#ifndef _INPUT_H
#define _INPUT_H

#include <termio.h>
#include <unistd.h>
#include <stdlib.h>

enum keys {
    DELETE_KEY = 1000,
    LEFT_KEY,
    RIGHT_KEY,
    HOME_KEY,
    END_KEY,
    BACKSPACE_KEY,
    ENTER_KEY,
    TAB_KEY,
    ESCAPE_KEY,
    CTRL_C_KEY
};

void change_mode(int on);
char *read_line(char **line);
int process_keypress(char c);

#endif