#include "ush.h"

static int which_check_built(char *command, char *flag) {
    int i = -1;
    char *builtin[] = {"exit", "alias", "cd", "echo", "export", "unset",
                       "which", "pwd", "env", "fg", "bye", "return", "unalias",
                       "True", "False", "set", "jobs", NULL};
    while (builtin[++i] != NULL) {
        if (mx_strequ(command, builtin[i])) {
            if (!mx_strequ(flag, "-s")) {
                mx_printstr(command);
                mx_printstr(": ush built-in command");
                mx_printstr("\n");
            }
            return 0;
        }
    }
    return 1;
}

static int which_check_path(char *command, char **path, char *flag) {
    int i = -1;
    char *bin_path;

    while (path && path[++i]) {
        bin_path = mx_pathjoin(path[i], command);
        if (access(bin_path, MX_OK) != -1) {
            if (!mx_strequ(flag, "-s")) {
                mx_printstr(bin_path);
                mx_printstr("\n");
            }
            return 0;
        }
    }
    return 1;
}

int mx_pwd_builtin(char **arg, char **env) {
    char *cwd;
    char buff[4096 + 1];

    if (!arg[0] || mx_strequ(arg[0], "-L")) {
        mx_printstr(mx_get_env_var("PWD", env));
        mx_printstr("\n");
        return 0;
    } 
    else if (mx_strequ(arg[0], "-P")){
        cwd = getcwd(buff, 4096);
        mx_printstr(cwd);
        mx_printstr("\n");
        return 0;
    } 
    else if (arg[0][0] == '-') {
       mx_errors(PWD_ERR, arg[0]);
    } 
    else 
        mx_printerr("pwd: too many arguments\n");
    return 1;
}

static int which_with_flag(char **path, char **arg) {
    int flag = 0;
    int	i = 1;

    while (arg[i]){
        flag += which_check_built(arg[i], arg[0]);
        flag += which_check_path(arg[i], path, arg[0]);
        if (flag == 2) {
            mx_errors(WHC_NF, arg[i]);
            return 1;
        }
    flag = 0;
    i++;
    }
    return 0;
}

int mx_which_builtin(char **arg, char **env) {
    int c = -1;
	char **path = mx_strsplit(mx_get_env_var("PATH", env), ':');
    
    if (!arg[0])
        return 1;
    if (mx_get_char_index(arg[0], '-') == 0) {
        return which_with_flag(path, arg);
    } 
    else {
        while (arg[++c]) {
            if (which_check_built(arg[c], NULL))
                if (which_check_path(arg[c], path, NULL))
                    mx_errors(WHC_NF, arg[c]);
        }
    }
	return 0;
}
