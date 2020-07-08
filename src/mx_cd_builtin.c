#include "ush.h"

static int cd_with_flag(char **arg, char **env) {
    struct stat buffy;
    char buff[4096 + 1];

    if (mx_strequ(arg[0], "-P")) {
        mx_change_dir(arg[1], 0, env);
        mx_set_var("PWD",getcwd(buff, 4096), env);
        return 0;
    } 
    else if (mx_strequ(arg[0], "-s")) {
        lstat(arg[1],&buffy);
        if ((buffy.st_mode & S_IFMT) == S_IFLNK) {
            mx_errors(CD_NON_DIR, arg[0]);
            return 1;
        } 
        else {
            mx_change_dir(arg[1], 0, env);
            return 0;
        }
    }
    return -1;
}

static int has_two_args(char **args, char **env) {
	if (args[1]) {
        if (args[0][0] == '-')
            return cd_with_flag(args, env);
		else if (args[2]) {
		    mx_errors(CD_TO_MANY, "");
			return 1;
		} 
        else {
            mx_errors(CD_STR_NOT_PWD, args[0]);
			return 1;
		}
	}
	return -1;
}

static int to_change(char *path, char **env, char *cwd) {
    char buff2[4097];
    char *n_cwd;
    struct stat buffy;

    n_cwd = getcwd(buff2, 4096);
    lstat(path, &buffy);
    if ((buffy.st_mode & S_IFMT) == S_IFLNK) {
        if (mx_get_char_index(path,'/') == 0)
            mx_set_var("PWD", path, env);
        else
            mx_set_var("PWD", n_cwd, env);
    } 
    else if (mx_strequ(path, "..") || (mx_get_char_index(path, '/') == 0))
        mx_set_var("PWD", n_cwd, env);
    else
        mx_set_var("PWD", mx_pathjoin(cwd, path), env);
	mx_set_var("OLDPWD", cwd, env);
    return 0;
}

int mx_change_dir(char *path, int printh_path, char **env) {
    char buff[4097];
    char *cwd = getcwd(buff, 4096);

	if (!chdir(path)) {
		if (printh_path) {
			print_path(path, env);
		    mx_printchar('\n');
		}
        return to_change(path, env, cwd);
	} 
    else {
        if (access(path, F_OK) == -1)
			mx_errors(CD_NSFORD, path);
		else if (access(path, R_OK) == -1)
			mx_errors(CD_PERMISION, path);
		else
			mx_errors(CD_NON_DIR, path);
        return 1;
    }
}

int mx_cd_builtin(char **arg, char **env) {
    char *home_path  = mx_get_env_var("HOME", env);

    if(!arg[0]) {
        mx_change_dir(home_path, 0, env);
        return 0;
    }
    if (arg[1])
        return has_two_args(arg, env);
    else {
         if (mx_strequ(arg[0], "-")) {
			mx_change_dir(mx_get_env_var("OLDPWD", env), 1, env);
			return 0;
		} 
        else if (mx_strequ(arg[0], "--")) {
			mx_change_dir(home_path, 0, env);
			return 0;
        }
		return mx_change_dir(arg[0], 0, env);
    }
    return 0;
}
