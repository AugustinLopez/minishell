/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/07 11:21:42 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/03 17:15:16 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
** unistd : fork, exceve
** sys/wait : wait
** limits : PATH_MAX. We use getcwd (we shouldn't but whatever).
** signal : (bonus) signal
** sys/stat
** sys/type
** dirent
*/

#include <libft.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

/*
** global variable necessary to pass info to signal
*/

#define HOSTNAME_SIZE 16
#define HISTO_SIZE 10

#define MSF_SHOW_PATH_HOME 1
#define MSF_REINITIALIZE_READER 4
#define MSF_NO_MORE_CMD 8

typedef struct		s_minishell
{
	int				ac;
	char			**av;
	t_list			*env;
	char			**arr_env;
	int				flags;
	int				ret;
	char			hostname[HOSTNAME_SIZE + 1];
	char			*curr_path;
	char			*historique[HISTO_SIZE];
	char			*input;
	t_list			*cmd;
	char			**all_cmd;
	char			**one_cmd;
	char			*tmp0;
	char			*tmp1;
	t_list			*elem;
}					t_minishell;

t_minishell *g_ms;
typedef struct stat	t_stat;

int		ms_initialize(t_minishell *ms, int ac, char **av, char **env);

int		load_prompt(t_minishell *ms);
void	show_prompt(t_minishell *ms);

int		get_home_path(t_minishell *ms, char *path, char **return_path, int reverse);
char	*get_from_env(t_minishell *ms, char *var);

int		ms_error(int ret, char *s);
void	ms_free(t_minishell *ms, int option);
void	ms_exit(t_minishell *ms);
int		ms_setenv(t_minishell *ms);
int		ms_unsetenv(t_minishell *ms);
int		ms_echo(t_minishell *ms);
int		ms_cd(t_minishell *ms);
void	ms_signal_reinitialize(int signo);
void	ms_signal_no_prompt(int signo);
int		ms_split(t_minishell *ms);
int		ms_split_to_list(t_minishell *ms);
int		ms_split_remove_quote(t_minishell *ms);
int		ms_read(t_minishell *ms);

int		need_new_prompt(void);


int		run_cmd(t_minishell *ms, char *path);
int		is_bin_cmd(t_minishell *ms);
int		builtin_mspath(t_minishell *ms);
int		builtin_msname(t_minishell *ms);
int		ms_execute(t_minishell *ms);
void	ms_inputsplit(t_minishell *ms);
#endif
