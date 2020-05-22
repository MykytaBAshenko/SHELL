#include "ush.h"

void mx_init_signals(void) {
    sigset_t mask;

    sigfillset(&mask);
    sigdelset(&mask, SIGINT);
    sigdelset(&mask, EOF);
    sigdelset(&mask, SIGTSTP);
    sigprocmask(SIG_SETMASK, &mask, NULL);
}
