#include "ush.h"

bool mx_check_substitutions(char *command) {
    bool g_quotes = false;
    size_t len = strlen(command);
    
    for (unsigned int i = 0; i < len; i++) {
        mx_skip_quotes(command, &i, '\'');
        mx_skip_expansion(command, &i);
        if (command[i] == '`' && !mx_isescape_char(command, i)) {
            g_quotes = !g_quotes;
        }
    }
    return !g_quotes;
}

int mx_checkclosequots(char *input) {
    int result = 0;
    if (!(result = mx_check_quotes(input))) {
        mx_printerr("ush: syntax error: missing terminating character\n");
    }
    else if (!(result = mx_check_brackets(input))) {
        mx_printerr("ush: syntax error: missing brace character\n");
    }
    else if (!(result = mx_check_substitutions(input))) {
        mx_printerr("ush: syntax error: missing subsitution character\n");
    }
    return result;
}

void mx_set_var(char *key, char *value, char **var) {
    char *tmp = mx_strjoin("=", value);
    int	pos;

    if (mx_get_char_index(key,'=') > 0)
        key[mx_strlen(key) - 1] = '\0';
	pos = mx_find_env_var(key, var);
	if (var[pos]) {
		free(var[pos]);
		if (value)
			var[pos] = mx_strjoin(key, tmp);
		else
			var[pos] = mx_strjoin(key, "=");
	} 
    else {
		if (value)
			var[pos] = mx_strjoin(key, tmp);
		else
			var[pos] = mx_strjoin(key, "=");
	}
	free(tmp);
}
