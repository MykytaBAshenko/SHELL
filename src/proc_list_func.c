#include "ush.h"

t_list **mx_get_plist(void) {
    static t_list *list = NULL;

    return &list;
}

t_list *mx_get_last_process(t_list *processes) {
    t_list *current = processes;

    while(current) {
        if (!current->next)
            return current;
    }
    return NULL;
}

void mx_del_node_list(t_list **list, t_process **del_proc) {
    t_list *cur = *list;
    t_list *tmp = NULL;
    t_process *find_proc = (t_process*)cur->data;

    if (find_proc->pid == (*del_proc)->pid) {
        mx_del_process(del_proc);
        tmp = *list;
        *list = (*list)->next;
        free(tmp);
        return;
    }
    for (cur = *list; cur; cur = cur->next) {
        find_proc = (t_process*)cur->next->data;
        if (find_proc->pid == (*del_proc)->pid)
            break;
    }
    tmp = cur->next;
    cur->next = tmp->next;
    free(tmp);
    mx_del_process(del_proc);
}
