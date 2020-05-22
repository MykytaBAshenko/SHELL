#include "ush.h"

void print_path(char *path, char **env) {
	char *parsed_home = mx_parse_home_path(path, env);

	mx_printstr(parsed_home);
	mx_strdel(&parsed_home);
}

void mx_print_env(char **env) {
    int i = 0;
    int len = envv_len(env);
    
    if (env) {
        while (i < len){
            mx_printstr(env[i]);
            mx_printstr("\n");
            i++;
        }
    }
}
