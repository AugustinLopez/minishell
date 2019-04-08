/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/07 11:21:42 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:35:36 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
** unistd: fork, exceve
** sys/wait: waitpid
** stdlib: malloc, free
** limits: PATH_MAX.
** signal: (bonus) signal
** sys/stat: struct stat
*/

# include <libft.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <limits.h>
# include <signal.h>
# include <sys/stat.h>

/*
** --- Define ------------------------------------------------------------------
*/

/*
** HOSTNAME_SIZE: text of the prompt
** HISTO_SIZE: size of the historic
** MSF_SHOW_PATH_HOME: flags - prompt show the path to home
** MSF_ENV_CMD: flags - prevent modification of the prompt when 'env [cmd]'
** MSF_REINITIALIZE_READER: flags - to handle signal <C-c> when writing.
** MSF_NO_MORE_CMD: flags - to handle signel <C-c> in case of 'cat;[cmd]'
*/

# define HOSTNAME_SIZE 16
# define HISTO_SIZE 10

# define MSF_SHOW_PATH_HOME 1
# define MSF_ENV_CMD 2
# define MSF_REINITIALIZE_READER 4
# define MSF_NO_MORE_CMD 8

/*
** --- Typedef & Global --------------------------------------------------------
*/

/*
** s_minishell:
** - env: global variable
** - flags: see previous define
** - ret: return value of command. See with echo $?
** - hostname: text of the prompt
** - cwd: current working directory
** - histo: command historic
** - input: input in stdin
** - cmd: input in list form.
** - elem: current active command from 'cmd'.
** - arr_cmd: input in array form for exceve. Contains all char* from 'cmd'
** - one_cmd: current active command from 'arr_cmd'.
** - tmp0, tmp1: general purpose temporary char. Always freed by ms_free
**
** g_ms: global pointer to t_minishell: for signal function.
*/

typedef struct stat	t_stat;
typedef struct		s_minishell
{
	t_list			*env;
	int				flags;
	int				ret;
	char			hostname[HOSTNAME_SIZE + 1];
	char			*cwd;
	char			*histo[HISTO_SIZE];
	char			*input;
	t_list			*cmd;
	t_list			*elem;
	char			**arr_cmd;
	char			**one_cmd;
	char			*tmp0;
	char			*tmp1;
}					t_minishell;
t_minishell			*g_ms;

/*
** =============================================================================
** --- Prototypes --------------------------------------------------------------
** =============================================================================
*/

/*
** Utilities
*/

int					ms_error(t_minishell *ms, int ret, char *s);
void				ms_free(t_minishell *ms, int option);
int					print_env(t_minishell *ms);
char				*get_from_env(t_minishell *ms, char *var);
int					get_home_path(t_minishell *ms, char *path, char **ret_path,
								int revers);

/*
** Prompt & Signal
*/

int					load_prompt(t_minishell *ms);
void				show_prompt(t_minishell *ms);
void				ms_signal_when_executing(int signo);
void				ms_signal_when_reading(int signo);

/*
** Read & Split Input
*/

int					ms_read(t_minishell *ms);
int					quote_value(char c, int quote);
int					ms_split(t_minishell *ms);
int					ms_split_to_list(t_minishell *ms);
int					ms_split_remove_quote(t_minishell *ms);
int					ms_split_replace(t_minishell *ms);
int					ms_split_replace_dollar(t_minishell *ms);
int					ms_split_protect_characters(char *str, char **result);

/*
** Execution
*/

int					ms_execute(t_minishell *ms);
int					is_bin_cmd(t_minishell *ms);
int					is_builtin_cmd(t_minishell *ms);
int					execute_single_command(t_minishell *ms);
int					run_cmd(t_minishell *ms, char *path);
void				ret_value(t_minishell *ms, int status);

/*
** Builtins
*/

void				ms_exit(t_minishell *ms);
int					ms_setenv(t_minishell *ms);
int					ms_unsetenv(t_minishell *ms);
int					ms_echo(t_minishell *ms);
int					ms_cd(t_minishell *ms);
int					ms_env(t_minishell *ms);
int					builtin_mspath(t_minishell *ms);
int					builtin_msname(t_minishell *ms);

/*
** Builtins Helper
*/

int					setenv_error_check(t_minishell *ms);
int					setenv_mem_check(t_minishell *ms, t_list **tmp,
									t_list **start);
int					cd_posix_minus(t_minishell *ms, int ac, int flags);
int					cd_posix_step_1_2(t_minishell *ms, int ac, char *home_path,
										int flags);
char				*cd_posix_step_3_to_6(t_minishell *ms, int ac, int *flags);
int					cd_posix_parsing(t_minishell *ms, int *flags);
int					change_dir(t_minishell *ms, char *path, int flags);

#endif
