#include "ush.h"

static char *mx_get_sub(char *str, char start, char end) {
    int b = mx_get_char_index(str, end);
    int a = mx_get_char_index(str, start);
    char *finish = mx_strnew(b - a);
    int i = 0;

    if (!str || !start || !end)
        return NULL;
    while(1) {
        finish[i] = str[a + i];
        if (str[i + a] == end && i > 0 && a + i == b)
            break;
        i++;
    }
    finish[i + 1] = '\0';
    return finish;
}

static char *replace_with(char *n, char **var, char ***env, 
                          char **word, int i) {
    char *buf;
    char *buffer;

    buf = mx_get_sub(word[i], '$', '}');
    buf = mx_replace_substr(mx_replace_substr(buf, "${", ""), "}", "");
    buffer = mx_get_env_var(buf, *env);
    if (buffer == NULL)
        buffer = mx_get_env_var(buf, var);
        if (buffer == NULL)
    buffer = "nahui=tuda";
    n = mx_replace_substr(n, mx_get_sub(word[i], '$', '}'), buffer);
    return n;
}

static char *replace_wothout(char *n, char **var, char ***env, 
                          char **word, int i) {
    char *buf;
    char *buffer;
    
    buf = mx_replace_substr(word[i], "$", "");
    buffer = mx_get_env_var(buf, *env);
    if (buffer == NULL)
        buffer = mx_get_env_var(buf, var);
    if (buffer == NULL)
        buffer = "nahui=tuda";
    n = mx_replace_substr(n, word[i], buffer);
    return n;
}

static char *mx_replace_dolars(char *n, char ***env, char **var) {
    char **word = NULL;
    int i = -1;

    if (mx_get_char_index(n, '$') > -1 ) {
        word = mx_strsplit(n, ' ');
        while (word[++i]) {
            if (mx_get_substr_index(n, "${") > -1) 
                n = replace_with(n, var, env, word, i);
            else 
                n = replace_wothout(n, var, env, word, i);
        }
    }
    return n;
}

char **mx_rep(char **command, char **env, char **var) {
    char **dolar = (char **)malloc(sizeof(char *) * 10);
    int q = -1;
    dolar[0] = mx_strdup("nahui");
    dolar[1] = NULL;
    for (int k = 0; command[k] != NULL; k++) 
        command[k] = mx_replace_dolars(command[k], &env, var);
    while (q < envv_len(command)) {
        mx_unsetenv_builtin(dolar, command);
        q++;
    }
    return command;
}
