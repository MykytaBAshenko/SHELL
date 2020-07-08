#include "ush.h"

int mx_isemptystr(char *str, int consider_space) {
	int	i = -1;
	int	min = 32 + consider_space;
	int max = 126;

	while (str[++i]) {
		if (str[i] >= min && str[i] <= max)
			return 0;
	}
	return 1;
}

char *mx_parse_home_path(char *path, char **env) {
    char *new;
    char *home_path = mx_get_env_var("HOME", env);

    if (!path)
        return NULL;
    if (!home_path)
        return NULL;
    if (strcmp(home_path, path) == 0)
        return strdup(home_path);
    new = mx_replace_substr(path,home_path, "~");
    return new;
}
