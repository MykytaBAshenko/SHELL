#include "ush.h"

t_list *mx_get_process_by_cmd(char *arg, t_list *processes) {
    t_list *ret_process = NULL;
    unsigned int count_processes = 0;
    t_process *tmp = NULL;

    while (processes) {
        tmp = (t_process*)processes->data;
        if (!mx_get_substr_index(tmp->commands, arg)) {
            count_processes++;
            ret_process = processes;
        }
        processes = processes->next;
    }
    if (count_processes == 1)
        return ret_process;
    else if (count_processes > 1)
        fprintf(stderr, "fg: %s: ambiguous job spec\n", arg);
    else if (!count_processes)
        fprintf(stderr, "fg: %s: no such job\n", arg);
    return NULL;
}

int mx_fprintf_fg(char **args) {
fprintf(stderr, "%s", *args ? "" : "fg: no current jobs\n");
        return 1;   
}