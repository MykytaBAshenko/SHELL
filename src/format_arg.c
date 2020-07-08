#include "ush.h"

char **get_result(char *command) {
    char **result = NULL;
    t_list *arguments = NULL;
    int len = 0;
    unsigned int i = 0;

    arguments = mx_split_command(command);
    len = mx_list_size(arguments);
    result = malloc(sizeof(char*) * (len + 1));
    result[len] = NULL;
    for (t_list *cur = arguments; cur; cur = cur->next) {
        result[i++] = get_formated_arg(cur->data);
    }
    mx_del_list(&arguments);
    mx_strdel(&command);
    return result;
}

char *get_formated_arg(char *str) {
    char *result = mx_strnew(ARG_MAX);
    unsigned int len = strlen(str);
    unsigned int index = 0;
    bool is_quotes[2];
    
    for (unsigned int i = 0; i < len; i++) {
        if ((str[i] == '\'') && !mx_isescape_char(str, i) && !is_quotes[1]) {
            is_quotes[0] = !is_quotes[0];
            continue;
        }
        if ((str[i] == '\"') && !mx_isescape_char(str, i) && !is_quotes[0]) {
            is_quotes[1] = !is_quotes[1];
            continue;
        }
        if (mx_isescape_char(str, i + 1) && str[i + 1] != '$' && !is_quotes[0] 
            && !is_quotes[1])
            i++;
        result[index++] = str[i];
    }
    return result;
}
