#include "ush.h"

struct termios *mx_get_tty(void) {
    static struct termios tty;

    return &tty;
}

void mx_set_input_mode(void) {
    struct termios tty;

    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tty);
}

void mx_unset_input_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, mx_get_tty());
}
