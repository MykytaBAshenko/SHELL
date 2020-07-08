#include "ush.h"

bool mx_isescape_char(char *input, int i) {
    if (i > 0 && input[i - 1] == '\\' && !mx_isescape_char(input, i - 1)) {
        return true;
    }
    if (i == 1 && input[i - 1] == '\\')
        return true;
    return false;
}

int mx_skip_quotes(char *input, unsigned int *i, char c) {
    if (input[*i] == c && !mx_isescape_char(input, *i)) {
        (*i)++;
        while (input[*i]) {
            if (input[*i] == c
                && !mx_isescape_char(input, *i)) {
                break;
            }
            (*i)++;
        }
    return 1;
    }
    return 0;
}


t_list *mx_split_command(char *command) {
    t_list *result = NULL;
    unsigned int save = 0;

    for (unsigned int i = 0; i < strlen(command); i++) {
        mx_skip_quotes(command, &i, '\"');
        mx_skip_quotes(command, &i, '\'');
        if (isspace(command[i]) && !mx_isescape_char(command, i)) {
            save++;
            continue;
        }
        if (!command[i + 1] || (isspace(command[i + 1])
            && !mx_isescape_char(command, i + 1))) {
            mx_push_back(&result, strndup(command + save, i - save + 1));
            save = i + 1;
        }
    }
    return result;
}

char **replace_on_koskav(char **shit) {
    if (mx_strcmp("echo", shit[0]) == 0 && shit[1] != NULL && shit[1][0] == '`' 
        && shit[1][strlen(shit[1]) - 1] == '`' && shit[2] == NULL)
        return mx_interpretate(strndup(++shit[1], strlen(shit[1]) - 1));
    return shit;
}

char **mx_interpretate(char *command) {
    if (!strlen(command))
        return NULL;
    return replace_on_koskav(get_result(command));
}
