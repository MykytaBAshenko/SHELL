#include "ush.h"

char **realloc_envv(int new_size, char **env) {
    int i = -1;
	char **new;
    new = (char **)malloc(sizeof(char *) * (new_size + 1));;

	while (env[++i] && i < new_size) {
		new[i] = strdup(env[i]);
		free(env[i]);
	}
	return new;
}

int envv_len(char **envv) {
	int	i = -1;
	int	count = 0;
    
	while (envv[++i])
		count++;
	return (count);
}

char *mx_get_env_var(char *var, char **env) {
    char *tmp = mx_strjoin(var, "=");
    int i = -1;
    
    if(!env)
        return NULL;
    while (env[++i]) {
        if (!(mx_get_substr_index(env[i], tmp))) {
            mx_strdel(&tmp);
            return (mx_strchr(env[i], '=') + 1);
        }
    }
    mx_strdel(&tmp);
    return NULL;
}
