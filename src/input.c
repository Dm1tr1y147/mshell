#include "../include/input.h"
#include "../include/keys.h"

/**
 * @brief Switches console raw mode
 * 
 * @param on 
 */
void change_mode(int on)
{
    static struct termios oldt, newt;

    if (on)
    {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    else
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

/**
 * @brief Reads line from user input and puts it into "line"
 * 
 * @param line 
 * @return char* 
 */
char *read_line(char **line)
{
    int c;
    int n = 0, pos = 0;

    while (read(STDIN_FILENO, &c, 1))
    {
        c = process_keypress(c);

        if (c)
        {
            switch (c)
            {
            case DELETE_KEY:
                delete_key(pos, &n, line);
                break;

            case LEFT_KEY:
                move_left(&pos);
                break;

            case RIGHT_KEY:
                move_right(&pos, n);
                break;

            case HOME_KEY:
                home_key(&pos);
                break;

            case END_KEY:
                end_key(&pos, n);
                break;

            case BACKSPACE_KEY:
                backspace_key(&pos, &n, line);
                break;

            case ENTER_KEY:
                new_line();

                return *line;
                break;

            case TAB_KEY:
            {
                // TODO: autocomplete
                tab_key(&pos, &n, line);
            }
            break;

            case ESCAPE_KEY:
                break;

            default:
                if ((c > 31 && c < 127) || (c > 127 && c < 255))
                    printable_key(&pos, &n, (char)c, line);

                break;
            }
        }
    }
}

/**
 * @brief Processes user keypress, filters special characters
 * 
 * @param c 
 * @return int 
 */
int process_keypress(char c)
{
    char seq[3];
    if (c == '\x1b')
    {
        for (int i = 0; i < 2; i++)
            if (read(STDIN_FILENO, &seq[i], 1) == -1)
                return ESCAPE_KEY;
        if (seq[0] == '[')
        {
            if (seq[1] >= '0' && seq[1] <= '9')
            {
                read(STDIN_FILENO, &seq[2], 1);
                if (seq[2] == '~')
                {
                    switch (seq[1])
                    {
                    case '3':
                        return DELETE_KEY;
                        break;
                    }
                }
            }
            switch (seq[1])
            {
            case 'D':
                return LEFT_KEY;
                break;

            case 'C':
                return RIGHT_KEY;
                break;

            case 'H':
                return HOME_KEY;
                break;

            case 'F':
                return END_KEY;
                break;
            }
        }
        return ESCAPE_KEY;
    }
    else if (c == 127)
    {
        return BACKSPACE_KEY;
    }
    else if (c == '\n')
    {
        return ENTER_KEY;
    }
    else if (c == '\t')
    {
        return TAB_KEY;
    }
    else
    {
        return c;
    }
}