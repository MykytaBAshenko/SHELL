#include "ush.h"
static int check_b1(char **command, t_ush data, char ***env) {
    if (!mx_get_substr_index(command[0], "exit"))
        return mx_exit_builtin(command + 1);
    else if (!mx_get_substr_index(command[0], "bye"))
        return bye_builtin(command);
    else if (!mx_get_substr_index(command[0], "echo")) {
        mx_set_var("?", mx_itoa(mx_echo_builtin(command)), data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "alias")) {
        mx_set_var("?", mx_itoa(mx_alias(command + 1, data.alias)), data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "cd")) {
        mx_set_var("?", mx_itoa(mx_cd_builtin(command + 1, *env)), data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "jobs")) {
        mx_set_var("?", mx_itoa(mx_jobs(command + 1, 1)), data.var);
        return 1;
    }
    return 0;
}
static int check_b2(char **command, t_ush data, char ***env) {
    if (!mx_get_substr_index(command[0], "export")) {
        mx_set_var("?", mx_itoa(mx_export_builtin(command + 1, data, *env)),
                    data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "unset")) {
        mx_set_var("?", mx_itoa(mx_unsetenv_builtin(command + 1, *env)),
                    data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "which")) {
        mx_set_var("?", mx_itoa(mx_which_builtin(command + 1, *env)), 
                    data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "env")) {
        mx_set_var("?", mx_itoa(mx_env_builtin(command + 1, data, *env)),
                    data.var);
        return 1;
    }
    return 0;
}
static int check_b3(char **command, t_ush data, char ***env) {
    if (!mx_get_substr_index(command[0], "True"))
        return true_builtin(data.var);
    else if (!mx_get_substr_index(command[0], "env")) {
        mx_set_var("?", mx_itoa(mx_env_builtin(command + 1, data, *(env))),
                    data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "unalias")) {
        mx_set_var("?", mx_itoa(mx_unalias(command + 1, data.alias)), 
                    data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "set")) {
        mx_print_env(data.var);
        return 1;
    }
    return 0;
}
static int check_b4(char **command, t_ush data, char ***env) {
    if (!mx_get_substr_index(command[0], "return"))
        return return_builtin(data.var, command + 1);
    else if (!mx_get_substr_index(command[0], "builtin")) {
        mx_set_var("?", mx_itoa(builtin(command + 1, data, env)),
                   data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "pwd")) {
        mx_set_var("?", mx_itoa(mx_pwd_builtin(command + 1, *env)), data.var);
        return 1;
    }
    else if (!mx_get_substr_index(command[0], "False"))
        return false_builtin(data.var);
    else if (!mx_get_substr_index(command[0], "fg")) {
        mx_set_var("?", mx_itoa(mx_fg(command + 1, 1)), data.var);
        return 1;
    }
    return 0;
}
int check_builtins(char **command, t_ush data, char ***env) {
    command = mx_rep(mx_check_exec(command, data, env), *(env), data.var);
    int q = check_b1(command, data, env);
    if (q != 1 && q != 0)
        return q;
    else
        if (check_b2(command, data, env) != 1)
            if (check_b3(command, data, env) != 1)
                if (check_b4(command, data, env) != 1)
                    if (q != 1)
                        return 0;
    return 1;
}
