#ifndef USH_H 
#define USH_H

#include <stdio.h>
#include <signal.h>
#include <wchar.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <malloc/malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/acl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/acl.h>
#include <sys/xattr.h>
#include <time.h>
#include <termios.h>
#include <spawn.h>
#include "../libmx/inc/libmx.h"

enum e_eror{
    UNSET_ERR,
    USH_NF,
    USH_PERMISION,
    FORK_FAIL,
    USH_BAD_ASSIGN,
    CD_NSFORD,
    CD_PERMISION,
    CD_NON_DIR,
    CD_TO_MANY,
    CD_STR_NOT_PWD,
    ENV_ILL,
    ENV_OPTION_REQ,
    WHC_NF,
    ERR_NAR,
    PWD_ERR,
};

typedef struct s_ush {
    char **commands;
    char **alias;
    char **var;
}              t_ush;

#define MX_W_INT(m) (*(int*) & (m))
#define MX_WSTOPSIG(m) (MX_W_INT(m) >> 8)
#define MX_WSTATUS(m) (MX_W_INT(m) & 0177)
#define MX_WIFSTOPPED(m) (MX_WSTATUS(m) == _WSTOPPED && MX_WSTOPSIG(m) != 0x13)
#define MX_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define MX_WAIT_TO_INT(m) (*(int *) & (m))
#define MX_WEXITSTATUS(x) ((MX_WAIT_TO_INT(x) >> 8) & 0x000000ff)

typedef struct s_process {
    int input_mode;
    char *commands;
    pid_t pid;
    pid_t gpid;
    int pos;
    int status;
    sigset_t signals;
    posix_spawn_file_actions_t actions;
    posix_spawnattr_t attrs;
}              t_process;

#define MX_OK 00
char **mx_check_exec(char **text, t_ush data, char ***env);
char *read_speed(char *name);
void signal_handler(int signo);
void proc_signal_handler(int signo);
char **replace_on_koskav(char **shit);
char **mx_interpretate(char *command);
int mx_unalias(char **arg, char **data);
int check_builtins(char **command, t_ush data, char ***env);
int mx_exit_builtin(char **arg);
void mx_set_var(char *key, char *value, char **var);
int bye_builtin(char **command);
int false_builtin(char **var);
int true_builtin(char **var);
int return_builtin(char **var, char **arg);
void key_handler(t_ush data);
int envv_len(char **envv);
void mx_errors(enum e_eror errors, char *a);
void print_path(char *path, char **env);
char **mx_init_envr(char **envr);
void mx_display(char **env, char **var);
char *mx_parse_home_path(char *path, char **env);
int mx_isemptystr(char *str, int consider_space);
void mx_exit_shell(char **env);
char *mx_get_env_var(char *var, char **env);
char *mx_pathjoin(char *p1, char *p2);
char **mx_get_input(char **input, t_ush data, char ***env);
void signal_handler(int signo);
void proc_signal_handler(int signo); 
int mx_run_command(char **commands, t_ush data, char ***env, int run_mode);
void mx_print_env(char **env);
int mx_export_builtin(char **arg, t_ush data, char **env);
int mx_cd_builtin(char **arg, char **env);
int mx_change_dir(char *path, int printh_path, char **env);
int mx_find_env_var(char *var,char **env);
void mx_set_env_var(char *key, char *value, char ***env);
char **realloc_envv(int new_size, char **env);
int mx_pwd_builtin(char **arg, char **env);
int mx_which_builtin(char **arg, char **env);
int mx_env_builtin(char **arg, t_ush data, char **env);
char **mx_init_export(char **env);
int mx_alias(char **arg, char **data);
int mx_unsetenv_builtin(char **arg, char **data);
int	mx_isinenv(char **env, char *var);
int mx_exit_builtin(char **arg);
int mx_echo_builtin(char **str);
t_process *mx_create_process(int im);
t_list **mx_get_plist(void);
int mx_exec_cmd(t_process *proc, char *path, char **args, char **env);
void mx_init_signals(void);
void mx_del_process(t_process **process);
void mx_del_node_list(t_list **list, t_process **del_proc);
void mx_kill_process(void);
bool mx_check_quotes(char *input);
int mx_checkclosequots(char *input);
void mx_continue_process(t_process *process, t_list **all_processes, int fd);
int mx_fg(char **args, int fd);
t_list *mx_get_last_process(t_list *processes);
struct termios *mx_get_tty(void);
void set_input_mode(void);
void unset_input_mode(void);
void mx_skip_expansion(char *input, unsigned int *i);
char **get_result(char *command);
char *get_formated_arg(char *str);
t_list *mx_split_command(char *command);
char **mx_split_commands(char *command);
bool mx_isescape_char(char *input, int i);
int mx_skip_quotes(char *input, unsigned int *i, char c);
int mx_jobs(char **args, int fd);
int mx_checkdotkoma(char *input);
int builtin(char **arg, t_ush data, char ***env);
char *repwithskob(char *n, char ***env, char **s, int *z);
char *repwithoutskob(char *n, char ***env, char **s, int *z);
char **mx_rep(char **command, char **env, char **var);
char *rec_cord(char *str, char *word1, char *word2, int coord);
char *replaceslesh(char *n);
char **get(char *n);
char **getslesh(char *n);
void mx_set_input_mode(void);
void mx_unset_input_mode(void);
void mx_kill_all_proc(void);
bool mx_check_brackets(char *c);
bool mx_check_quotes(char *input);
void mx_skip_expansion(char *input, unsigned int *i);
t_list *mx_find_process(char *args);

#endif
