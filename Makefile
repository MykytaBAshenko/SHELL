NAME = ush

SRCD = src
INCD = inc
OBJD = obj
LBMXD = libmx

LMBX = libmx.a
INC = ush.h
SRC = builint_run.c \
		export_env.c \
		main.c \
		mx_alias.c \
		mx_cd_builtin.c \
		mx_dispay.c \
		mx_echo_builtin.c \
		mx_env_builtin.c \
		mx_errors.c \
		mx_exit_builtin.c \
		mx_get_env_var.c \
		mx_get_input.c \
		mx_init_env.c \
		mx_parse_home_path.c \
		mx_pathjoin.c \
		mx_print_env.c \
		mx_run_commands.c \
		other_builtin.c \
		replace_var.c \
		which_pwd_builtin.c \
		mx_exec_cmd.c \
		mx_fg.c \
		mx_init_signals.c \
		mx_jobs.c \
		mx_tty_funcs.c \
		process_functions.c \
		signals.c \
		interpr.c \
		format_arg.c \
		input_on_speeed.c \
		list_input.c \
		mx_cont_proc.c \
		proc_list_func.c \
		input_on_speeed2.c \
		mx_find_proc.c \

INCLUDE = -I $(LBMXD) \
	-I $(INCD) \

LBMXS = $(addprefix $(LBMXD)/, $(LMBX))
INCS = $(addprefix $(INCD)/, $(INC))
SRCS = $(addprefix $(SRCD)/, $(SRC))
OBJS = $(addprefix $(OBJD)/, $(SRC:%.c=%.o))
	
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror

CC = clang

all: $(NAME)

install: $(NAME) clean

$(LBMXS):
	@make -sC $(LBMXD)

$(NAME): $(OBJS) $(LBMXS)
	@$(CC) -ltermcap $(CFLAGS) $(LBMXS) $(OBJS) -o $@
	@printf "\x1b[32;1m$@ created\x1b[0m\n"

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	@printf "\x1b[32mcompiled: \x1b[0m[$(<:$(SRCD)/%.c=%.c)]\n"

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

uninstall: clean
	@make -sC $(LBMXD) $@
	@rm -rf $(NAME)
	@printf "\x1b[34;1mdeleted $(NAME)\x1b[0m\n"

clean:
	@make -sC $(LBMXD) $@
	@rm -rf $(OBJD)
	@printf "\x1b[34;1mdeleted $(OBJD)\x1b[0m\n"

reinstall: uninstall install
