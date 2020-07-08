#include "ush.h"

static int arr_size(char **arr) {
    int result = 0;

    while (*arr) {
        result++;
        arr++;
    }
    return result;
}

static bool check_args(char **args) {
    if (!arr_size(args))
        return true;
    if (arr_size(args) > 1) {
        fprintf(stderr, "fg: too many arguments\n");
        return false;
    }
    if (args[0][0] != '%' || !args[0][1]) {
        fprintf(stderr, "fg: invalid argument: %s\n", args[0]);
        return false;
    }
    return true;
}

static void continue_process(t_process *process, int fd) {
    t_list **all_processes = mx_get_plist();
    
    mx_unset_input_mode();
    tcsetpgrp(STDOUT_FILENO, process->gpid);
    mx_continue_process(process, all_processes, fd);
    tcsetpgrp(STDOUT_FILENO, getpgrp());
    mx_set_input_mode();
}

int mx_fg(char **args, int fd) {
    t_list *process = NULL;
    t_process *f_process = NULL;

    if (!check_args(args))
        return 1;
    process = mx_find_process(args[0]);
    if (process) {
        f_process = (t_process*)process->data;
        continue_process(f_process, fd);
    }
    else {
        if (*args) {
            fprintf(stderr, "%s", "");
        }
        else {
            fprintf(stderr, "%s", "fg: no current jobs\n");
        }
        return 1;
    }
    mx_unset_input_mode();
    return f_process->status;
}
