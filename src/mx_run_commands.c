#include "ush.h"

static int run_cmd(char *path, char **args, char **env) {
	int retval = 0;
    t_process *process = mx_create_process(1);

    signal(SIGINT, proc_signal_handler);
    retval = mx_exec_cmd(process, path, args, env);
    mx_del_process(&process);
    return 1;
}

static int is_exec(char *bin_path, struct stat f, char **command, char **env) {
	if (f.st_mode & S_IFREG) {
		if (f.st_mode & S_IXUSR)
			return (run_cmd(bin_path, command, env));
		else 
            mx_errors(USH_PERMISION, bin_path);
		return 1;
	}
	return 0;
}

static int check_bins(char **commands, char **env, t_ush data) {
    int	i = -1;
	char *bin_path;
	char **path;
	struct stat	f;

    if ((mx_get_env_var("PATH", env) == NULL))
        path = mx_strsplit(mx_strjoin(mx_get_env_var("Path", data.var), 
                    mx_strjoin(":", mx_get_env_var("PWD", env))), ':');
    else
        path = mx_strsplit(mx_strjoin(mx_get_env_var("PATH", env), 
                    mx_strjoin(":", mx_get_env_var("PWD", env))), ':');
    while (path && path[++i]) {
        if (mx_get_substr_index(commands[0], path[i]) == 0)
            bin_path = mx_strdup(commands[0]);
        else
            bin_path = mx_pathjoin(path[i], commands[0]);
        if (lstat(bin_path, &f) == -1)
			mx_strdel(&bin_path);
		else {
			mx_del_strarr(&path);
			return (is_exec(bin_path, f, commands, env));
		}
	}
	mx_del_strarr(&path);
	return 0;
}

int mx_run_command(char **commands, t_ush data, char ***env, int run_mode) {
    int is_builtin = 0;

    if (run_mode == 0 || run_mode == 2) {
        if (check_bins(commands, *env, data))
            return 0;
    } 
    else if (run_mode == 1) {
        if ((is_builtin = check_builtins(commands, data, env)) == 1 
            || check_bins(commands, *env, data) )
            return 0;
    }
    if (is_builtin != 1 && is_builtin != 0)
        return is_builtin;
    mx_errors(USH_NF, commands[0]);
    mx_set_var("?", "1", data.var);
    return 0;
}
