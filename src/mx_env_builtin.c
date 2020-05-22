#include "ush.h"

static int p_env_arg(char **arg, t_ush data, char **env) {
    if (!arg[0]) {
        mx_errors(ENV_OPTION_REQ, arg[0]);
        return 1;
    } 
    else {
        if (!arg[1]) {
            mx_print_env(env);
            return 0;
        } 
        else {
            mx_set_var("PATH", arg[1], env);
            return mx_run_command(arg + 2, data, &env, 2);
        }
    }
    return 1;
}

static int u_env_arg(char **arg, char **env, t_ush data) {
    if (!arg[0]) {
        mx_errors(ENV_OPTION_REQ, arg[0]);
        return 1;
    } 
    else {
        mx_unsetenv_builtin(arg, env);
        if (!arg[1]) {
            mx_print_env(env);
            return 0;
        } 
        else 
            return mx_run_command(arg + 1, data, &env, 0);
    }
    return 1;
}

static int i_env_arg(char **arg, char **new_env, t_ush data, int i) {
    if (mx_strequ(arg[0], "-i") && !arg[1])
        return 0;
    else if (mx_strequ(arg[0], "-i")) {
        mx_del_strarr(&new_env);
        mx_run_command(arg + 1, data, &new_env, i);
        return 1;
    }
    else {
        mx_errors(ENV_ILL,arg[0]);
        return 1;
    }
}

int mx_env_builtin(char **arg, t_ush data, char **env) {
    char **new_env = mx_init_export(env);
    int i = 0;

    if (!arg[0]) {
        mx_print_env(env);
        return 0;
    }
    if (mx_strequ(arg[0], "-P"))
        return p_env_arg(arg, data, new_env);
    else if (mx_strequ(arg[0], "-u"))
        return u_env_arg(arg + 1, new_env, data);
    else {
        return i_env_arg(arg, new_env, data, i);
    }
    return 0;
}
