#include "ush.h"

int get_next_command(char *command) {
    for (unsigned int i = 0; i < strlen(command); i++) {
        mx_skip_quotes(command, &i, '`');
        mx_skip_quotes(command, &i, '\'');
        mx_skip_quotes(command, &i, '\"');
        mx_skip_expansion(command, &i);
        if (command[i] == ';')
            return i;
        if (!command[i + 1] && command[i] != ';')
            return i + 1;
    }
    return -1;
}

static t_list *split(char *command) {
    t_list *commands = NULL;
    int len = 0;
    char *tmp_cmd = strdup(command);
    char *save = tmp_cmd;
    
    for (unsigned int i = 0; len != -1; i++) {
        len = get_next_command(tmp_cmd);
        mx_push_back(&commands, strndup(tmp_cmd, len));
        if ((tmp_cmd[len] == ';' && !tmp_cmd[len + 1]) || !tmp_cmd[len])
            break;
        tmp_cmd += len + 1;
    }
    mx_strdel(&save);
    return commands;
}

char **mx_split_commands(char *command) {
    t_list *commands = split(command);
    size_t size = mx_list_size(commands);
    char **cmds = malloc(sizeof(char *) * (size + 1));
    unsigned int index = 0;

    cmds[size] = NULL;
    for (t_list *cur = commands; cur; cur = cur->next) {
        cmds[index++] = strdup(cur->data);
    }
    mx_del_list(&commands);
    return cmds;
}

int mx_checkdotkoma(char *input) {
    int t = 0;

    for (int h = 0; input[h] != '\0'; h++)
        if (input[h] == ';' && input[h + 1] == ';')
            t++;
    if (t) {
        mx_printerr("ush: parse error near `;;'\n");
        return t;
    }
    return t;
}
