#include "ush.h"

int mx_unsetenv_builtin(char **arg, char **data) {
    int i = -1;
    int var_pos;
    int max;

	if (!arg[0]) {
		mx_errors(UNSET_ERR, "uns");
		return 1;
	}
	while (arg[++i]) {
        var_pos = mx_find_env_var(arg[i], data);
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

int mx_find_env_var(char *var,char **env) {
    int i = -1;
    char *tmp = mx_strjoin(var, "=");
    
    while (env[++i]) {
        if (mx_get_substr_index(env[i], tmp) == 0) {
            mx_strdel(&tmp);
            return i;
        }    
    }
    mx_strdel(&tmp);
    return i;
}

static int export_with_argument(int i, char **arg, char **env, t_ush data) {
    char *val;

    if (mx_get_char_index(arg[i], '=') > 0) {
        if (mx_get_char_index(arg[i], '=') == 0 && mx_strlen(arg[i]) == 1) {
                mx_errors(USH_BAD_ASSIGN, ":(");
                return 1;
        } 
        else
            mx_set_var(*(mx_strsplit(arg[i], '=')),
                       *(mx_strsplit(arg[i], '=') + 1), env);
        }
        else {
            val = mx_get_env_var(arg[i], data.var);
            mx_set_var(arg[i], val, env);
        }
    return 0;
}

int mx_export_builtin(char **arg, t_ush data, char **env) {
    int i = -1;
    char **sorted_env = mx_init_export(env);
    
    if (!arg[0]) {
        mx_bubble_sort(sorted_env, envv_len(env));
        mx_print_env(sorted_env);
        mx_del_strarr(&sorted_env);
        return 0;
    }
    while (arg[++i]) {
        export_with_argument(i, arg, env, data);
    }
	return 0;
}
