// input.c

#include "../include/input.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios terminal_old, terminal_new;

void input_init()
{
        tcgetattr(STDIN_FILENO, &terminal_old);

        terminal_new = terminal_old;
        terminal_new.c_lflag &= ~(ICANON | ECHO);
        terminal_new.c_cc[VMIN]  = 0;
        terminal_new.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO, TCSANOW, &terminal_new);

        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // sets std input to non blocking
}

void input_restore()
{
        tcsetattr(STDIN_FILENO, TCSANOW, &terminal_old);
}
