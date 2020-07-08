#include "ush.h"

int true_builtin(char **var) {
    mx_set_var("?", "0", var);
    return 1;
}

int false_builtin(char **var) {
    mx_set_var("?", "1", var);
    return 1;
}

int bye_builtin(char **command) {
    return mx_exit_builtin(command + 1);
}

int return_builtin(char **var, char **arg) {
    int num;

    if (!arg[0])
        return 1;
    else if (!mx_isdigit(arg[0][0])) {
        mx_set_var("?", "0", var);
        return 1;
    } 
    else if (arg[1]) {
        mx_printerr("return: too many arguments\n");
        mx_set_var("?", "1", var);
        return 1;
    } 
    else {
        num = atoi(arg[0]);
        mx_set_var("?", mx_itoa(num), var);
        return 1;
    }
}

int builtin(char **arg, t_ush data, char ***env) {
    if (check_builtins(arg, data, env))
        return 0;
    else {
        mx_printerr("ush: no such builtin: ");
        mx_printerr(arg[0]);
        mx_printerr("\n");
        return 1;
    }
}
