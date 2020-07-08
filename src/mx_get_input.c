#include "ush.h"

char *read_speed(char *name) {
    char *bfer = 0;
    long length;

    FILE * f = fopen (name, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell (f);
        fseek(f, 0, SEEK_SET);
        bfer = malloc(length);
        if (bfer) {
            fread(bfer, 1, length - 1, f);
        }
    fclose(f);
    bfer[length - 1] = '\0';
    }
    return bfer;
}

static char *check_alias(char *input, t_ush data) {
    int i = -1;

    while(data.alias[++i]) {
        if ((mx_get_substr_index(input, 
             *mx_strsplit(data.alias[i], '='))) == 0) {
            input = mx_replace_substr(input, 
            mx_strdup(*mx_strsplit(data.alias[i], '=')), 
            mx_get_env_var(mx_strdup(*mx_strsplit(data.alias[i], '=')), 
            data.alias));
        }
    }
    return input;
}

static char *mx_get_var_input(char *in, char **var) {
    int i = -1;
    int c = -1;
    char **com = mx_strsplit(in, ' ');

    while (com[++i]) 
        if (mx_get_char_index(com[i], '=') == -1) {
            while (com[++c] && mx_get_char_index(com[c], '=') != -1) {
                in = mx_replace_substr(in, 
                                       get_word_by_char(com[c], '='), "");
            }
            return in;
        }
    while (com[++c]){
        mx_set_var(*mx_strsplit(com[c], '='), *(mx_strsplit(com[c], '=') + 1),
                                var);
        in = mx_replace_substr(in, get_word_by_char(com[c], '='), "");
    }
    mx_del_strarr(&com);
    return in;
}

char *replace_tild(char *rep, char **env, int *i) {
    struct passwd *buf;
    int it;

    if (mx_get_char_index(rep, '~') < 0)
        return rep;
    it = mx_get_char_index(rep, '~');
    buf = getpwuid(getuid());
    i++; 
    rep = mx_replace_substr(rep, "~+", buf->pw_dir);
    rep = mx_replace_substr(rep, "~-", mx_get_env_var("OLDPWD", env));
    if (rep[it + 1] == ' ' || rep[it + 1] == '\0' || rep[it + 1] == '/')
        rep = mx_replace_substr(rep, "~", mx_get_env_var("HOME", env));
    else
        rep = mx_replace_substr(rep, "~", "/Users/");
    return rep;
}

char *logical(char *str, char **var) {
    if (mx_get_substr_index(str, "&&") > -1)
        mx_set_var("logical", "&&", var);
    if (mx_get_substr_index(str, "||") > -1)
        mx_set_var("logical", "||", var);
    char *st = mx_replace_substr(mx_replace_substr(str, "&&", "^"), "||", "^");
    return st;
}

static char **mx_parse_input(char *input, t_ush data, char ***env) {
    int codetilda = 0;
    char *rep = check_alias(logical(input, data.var), data);
    char *nah_tild = replace_tild(rep, *env, &codetilda);
    char **commands = mx_split_commands(nah_tild);

    if (!(mx_checkclosequots(input)) || (mx_checkdotkoma(input)))
        commands[0] = NULL;
    for (int k = 0; commands[k] != NULL; k++){
        commands[k] = strdup(mx_strtrim(mx_get_var_input(commands[k], 
                                                         data.var)));
        if (mx_strlen(commands[k]) == 1 && commands[k][0] == '\\') {
            mx_printstr("ush: \\ not closen \n");
            commands[k] = NULL;
        }
    }
    mx_strdel(&rep);
    if (codetilda)
        mx_strdel(&nah_tild);
    return commands;
}

static char **mx_check_pipe(char **text, t_ush data, char ***env);

char **mx_get_input(char **input, t_ush data, char ***env) {
    int ret;
    char b;
    int i = 0;
    char **parse_input;
    int count = 1;

    *input = mx_strnew(1);
    while ((ret = read(0, &b, 1)) && b != '\n') { 
        *(*input + i++) = b;
		*input = mx_realloc(*input, count + 1);
		count++;
    }
    *(*input + i) = '\0';
    if (!ret) {
		exit(0);
	}
    parse_input = mx_check_pipe(mx_parse_input(*input, data, env), data, env);
    return parse_input;
}

static char *replace_exedol(char ***env, char **text, t_ush data, int i) {
    char * bfer = 0;
    char **filecommand = NULL;
    char *b = NULL;
    
    b = mx_get_substr_start_end(text[i], '$', ')');
    b = mx_del_substr(mx_del_substr(b, "$("), ")");
    if (mx_get_substr_index(b, "$(") > -1 && mx_get_char_index(b, ')') > -1){
        b = replace_exedol(env, &b, data, 0);
    }
    filecommand = mx_strsplit(b, ' ');
    freopen("output.txt", "w+", stdout);
    mx_run_command(filecommand, data, env, 1);
    freopen("/dev/tty", "w", stdout);
    bfer = read_speed("output.txt");
    remove("output.txt");
    mx_del_strarr(&filecommand);
    return mx_replace_substr(text[i], 
                             mx_get_substr_start_end(text[i], '$', ')'), 
                             bfer);
}

static void mx_copy_file(char *pipe, char *pipe2) {
    FILE * in, * out;
    int  n;
    char b [512];

    in = fopen(pipe, "rb");
    out = fopen(pipe2, "w");
    while ((n = read(fileno(in), b, sizeof b)) > 0) {
        write(fileno(out), b, n);  
    }
}

static void pipeexe(char ***env, char **text, t_ush data, int i) {
    char **pipecommand = mx_strsplit(text[i], '|');
    char **filecommand = NULL;
    int j = -1;
    int len;
    
    len = envv_len(pipecommand);
    while (pipecommand[++j]) {
        if (j > 0)
            pipecommand[j] = mx_strjoin(pipecommand[j], " src/pipe2\0");
        filecommand = mx_strsplit(mx_strtrim(pipecommand[j]), ' ');
        if (j < len - 1) {
            freopen("src/pipe", "w+", stdout);
            mx_run_command(filecommand, data, env, 1);
            freopen("/dev/tty", "w", stdout);
            mx_copy_file("src/pipe", "src/pipe2");
        } 
        else
            mx_run_command(filecommand, data, env, 1);
    }
    mx_del_strarr(&pipecommand);
    mx_strdel(&text[i]);
    remove("src/pipe");
    remove("src/pipe2");
}

static char **mx_check_pipe(char **text, t_ush data, char ***env) {
    int i = -1;
    while (text[++i])
        if ((mx_get_char_index(text[i], '|') > -1 &&
             mx_get_substr_index(text[i], "||") < 0) ||
             mx_get_char_index(text[i], '|') > -1) {
            pipeexe(env, text, data, i);
        }
    return text;
}

static void  redirect(char ***env, char **text, t_ush data, int i) {
    char **filecommand = NULL;

    if (mx_get_substr_index(text[i], ">>") > -1) {
        freopen(mx_strtrim(*(mx_strsplit(text[i], '>') + 1)), "a+", stdout);
        text[i] = mx_del_substr(text[i], mx_strchr(text[i], '>'));
        filecommand = mx_strsplit(text[i], ' ');
        mx_run_command(filecommand, data, env, 1);
        freopen("/dev/tty", "w", stdout);
        mx_strdel(&text[i]);
    } 
    else if (mx_get_char_index(text[i], '>') > -1) {
        freopen(mx_strtrim(*(mx_strsplit(text[i], '>') + 1)), "w+", stdout);
        text[i] = mx_del_substr(text[i], mx_strchr(text[i], '>'));
        filecommand = mx_strsplit(mx_strtrim(text[i]), ' ');
        mx_run_command(filecommand, data, env, 1);
        freopen("/dev/tty", "w", stdout);
        mx_strdel(&text[i]);
    } 
    else if (mx_get_char_index(text[i], '<') > -1)
        text[i] = mx_del_substr(text[i], "<");
}


char **mx_check_exec(char **text, t_ush data, char ***env) {
    int i = -1;

    while (text[++i]) {
        if (mx_get_char_index_reverse(text[i], '`') > -1)
            text[i][mx_get_char_index_reverse(text[i], '`')] = ')';
        text[i] = mx_replace_substr(text[i], "`", "$(");
        if (mx_get_substr_index(text[i], "$(") > -1 
        && mx_get_char_index(text[i], ')') > -1 )
            text[i] = replace_exedol(env, text, data, i);
        else if (mx_get_char_index(text[i], '(') > -1 &&
                 mx_get_char_index(text[i], ')') > -1) {
            text[i] = mx_replace_substr(text[i], "(", "");
            text[i] = mx_replace_substr(text[i], ")", "");
        } 
        else if (mx_get_char_index(text[i], '>') > -1 ||
                   mx_get_substr_index(text[i], ">>") > -1 ||
                   mx_get_char_index(text[i], '<') > -1 )
            redirect(env, text, data, i);
    }
    return text;
}
