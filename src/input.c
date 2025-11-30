// input.c

#include "../include/input.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static struct termios terminal_old, terminal_new;

static bool key_pressed[INPUT_KEY_COUNT];

void input_init()
{
        memset(key_pressed, 0, sizeof(key_pressed));

        tcgetattr(STDIN_FILENO, &terminal_old);

        terminal_new = terminal_old;
        terminal_new.c_lflag &= ~(ICANON | ECHO);
        terminal_new.c_cc[VMIN] = 0;
        terminal_new.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO, TCSANOW, &terminal_new);

        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // sets std input to non blocking
}

void input_restore()
{
        tcsetattr(STDIN_FILENO, TCSANOW, &terminal_old);
}

void input_update()
{
        memset(key_pressed, 0, sizeof(key_pressed));

        unsigned char key;

        while (read(STDIN_FILENO, &key, 1) > 0) 
        {
                if (!key_pressed[key]) 
                {
                        key_pressed[key] = true;
                }
        }
}

bool input_key_pressed(int key) 
{
        return key_pressed[key];
}