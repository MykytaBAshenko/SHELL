#include "ush.h"

static t_list *get_process_by_cmd(char *args, t_list *processes) {
    t_list *ret_process = NULL;
    unsigned int count_processes = 0;
    t_process *tmp = NULL;

    while (processes) {
        tmp = (t_process*)processes->data;
        if (!mx_get_substr_index(tmp->commands, args)) {
            count_processes++;
            ret_process = processes;
        }
        processes = processes->next;
    }
    if (count_processes == 1)
        return ret_process;
    else if (count_processes > 1)
        fprintf(stderr, "fg: %s: ambiguous job spec\n", args);
    else if (!count_processes)
        fprintf(stderr, "fg: %s: no such job\n", args);
    return NULL;
}

static t_list *get_process_by_id(char *args, t_list *processes) {
    int cur_pos = atoi(args);
    t_process *tmp = NULL;

    while (processes) {
        tmp = (t_process*)processes->data;
        if (tmp->pos == cur_pos) {
            return processes;
        }
        processes = processes->next;
    }
    fprintf(stderr, "fg: %s: no such job\n", args);
    return NULL;
}

t_list *mx_find_process(char *args) {
    bool is_num = true;
    int len = 0;
    t_list **processes = mx_get_plist();

    if (!args)
        return mx_get_last_process(*processes);
    args++;
    len = strlen(args);
    for (int i = 0; i < len; i++) {
        if (!isnumber(args[i])) {
            is_num = false;
            break;
        }
    }
    if (is_num)
        return get_process_by_id(args, *processes);
    else
        return get_process_by_cmd(args, *processes);
}
