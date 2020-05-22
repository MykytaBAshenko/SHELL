#include "ush.h"

static int with_logic(t_ush data, char ***env, int i) {
    char **logical = mx_strsplit(data.commands[i], '^');
    char **command;
    int m_exit = 0;
    int i_l = -1;

    while (logical[++i_l]) {
        command = mx_interpretate(logical[i_l]);
        m_exit = mx_run_command(command, data, env, 1);
        if (((atoi(mx_get_env_var("?",data.var)) == 1) 
            && mx_strequ(mx_get_env_var("logical", data.var), "&&")) || 
            ((atoi(mx_get_env_var("?",data.var)) == 0)
            && mx_strequ(mx_get_env_var("logical", data.var), "||")))
            break;
    }
    return m_exit;
}

static int exec_commands(t_ush data, char ***env) {
    int i = -1;
    int m_exit = 0;
    char **command;

    while (data.commands[++i]) {
        if (mx_isemptystr(data.commands[i], 1))
            continue;
        if (!mx_strequ(mx_get_env_var("logical", data.var), "0"))
            m_exit = with_logic(data, env, i);
        else {
            command = mx_interpretate(data.commands[i]);
            m_exit = mx_run_command(command, data, env, 1);
        } 
        if (m_exit != 1 && m_exit != 0)
            break;
    }
    mx_del_strarr(&command);
    return m_exit;
}

static t_ush *init(void) {
    t_ush *data = malloc(sizeof(t_ush));
    char **var = (char **)malloc(sizeof(char *) * 1000);//arr variable
   	char **alias = (char **)malloc(sizeof(char *) * 1000);//arr alias
   	char **commands = (char **)malloc(sizeof(char *) * 1000);//arr commands
    char *buf = read_speed("/etc/paths");

    buf = mx_replace_substr(buf,"\n",":");
    data->var = var;
    data->alias = alias;
    data->commands = commands;
    data->var[0] = strdup("?=0");
    data->var[1] = strdup("logical=0");
    data->var[2] = strdup("PROMPT=standart");
    data->var[4] = strdup("PROMPT1=minimal");
    data->var[3] = mx_strjoin("Path=",buf);
    data->var[5] = NULL;
    return data;
}

static int circle_main(char **env, t_ush data) {
    char *input;
    int ret;

    while (1) {
        if (isatty(0))
            mx_display(env, data.var);
        signal(SIGINT, signal_handler);
        signal(SIGTSTP, signal_handler);
        data.commands = mx_get_input(&input, data, &env);
        if (mx_isemptystr(input, 1))
            continue;
        free(input);
        ret = exec_commands(data, &env);
        if (ret != 0 && ret != 1) 
            break;
	}
    return ret;
}

int main(int argc, char **argv, char **envr) {
    char **env;
    int q;

    (void)argv;
    (void)argc;
    env = mx_init_envr(envr);
    t_ush *data = init();
    mx_init_signals();
    tcgetattr(STDIN_FILENO, mx_get_tty());
    setvbuf(stdout, NULL, _IONBF, 0);
    mx_set_input_mode();
    mx_unset_input_mode();
    q = circle_main(env, *data);
    free(data);
    mx_unset_input_mode();
    mx_kill_all_proc();
    mx_del_strarr(&env);
    exit(q);
}
