#include "ush.h"

static void set_signals(sigset_t *signals, int im) {
    sigfillset(signals);
    sigdelset(signals, SIGINT);
    sigdelset(signals, SIGQUIT);
    sigdelset(signals, SIGCONT);
    sigdelset(signals, SIGCHLD);
    if (im == 1)
        sigdelset(signals, SIGTSTP);
}

t_process *mx_create_process(int im) {
    t_process *process = malloc(sizeof(t_process));

    process->gpid = 0;
    process->commands = NULL;
    process->pos = 0;
    process->input_mode = im;
    set_signals(&process->signals, im);
    posix_spawnattr_init(&process->attrs);
    posix_spawnattr_setpgroup(&process->attrs, process->gpid);
    posix_spawnattr_setsigmask(&process->attrs, &process->signals);
    posix_spawnattr_setflags(&process->attrs, POSIX_SPAWN_SETSIGMASK
                             | POSIX_SPAWN_SETPGROUP);
    posix_spawn_file_actions_init(&process->actions);
    posix_spawn_file_actions_adddup2(&process->actions, im, 1);
    return process;
}

void mx_del_process(t_process **process) {
    if (!MX_WIFSTOPPED((*process)->status)) {
        posix_spawnattr_destroy(&(*process)->attrs);
        posix_spawn_file_actions_destroy(&(*process)->actions);
        mx_strdel(&(*process)->commands);
        free(*process);
        process = NULL;
    }
}

void mx_kill_all_proc(void) {
    t_list **processes = mx_get_plist();
    t_process *tmp = NULL;

    for (t_list *cur = *processes; cur; cur = cur->next) {
        tmp = (t_process*)cur->data;
        kill(-tmp->gpid, SIGKILL);
        mx_del_node_list(processes, &tmp);
    }
}
