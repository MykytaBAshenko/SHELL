#include "ush.h"

int mx_alias(char **arg, char **data) {
    int i = -1;

    if (!arg[0])
        mx_print_env(data);
    else {
        while (arg[++i]) {
            if (mx_get_char_index(arg[i], '=') == 0 && mx_strlen(arg[i]) == 1){
                mx_errors(USH_BAD_ASSIGN, ":(");
                return 1;
            } 
            else 
                mx_set_var(*mx_strsplit(arg[i],'='),
                           *(mx_strsplit(arg[i],'=') + 1),data);
        }
    }
    return 0;
}

int mx_unalias(char **arg, char **data) {
    int i = -1;
    int var_pos;
    int max;

    if (!arg[0]) {
        mx_printerr("unalias: not enough arguments\n");
        return 1;
    }
    while (arg[++i]) {
        var_pos = mx_find_env_var(arg[i],data);
        max = envv_len(data) - 1;
        mx_strdel(&data[var_pos]);
        while (var_pos < max) {     
            data[var_pos] = mx_strdup(data[var_pos + 1]);
            var_pos++;
        }
        mx_strdel(&data[var_pos]);
    }
    return 0;
}
