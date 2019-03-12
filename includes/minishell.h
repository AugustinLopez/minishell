/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/07 11:21:42 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/11 17:02:38 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
** unistd : fork, exceve
** sys/wait : wait
** limits : PATH_MAX. We use getcwd (we shouldn't but whatever).
*/

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
#define MSF_BUILTIN_EXIT 2

typedef struct		s_minishell
{
	int				ac;
	char			**av;
	char			**env;
	int				flags;
	char			hostname[HOSTNAME_SIZE + 1];
	char			*historique[HISTO_SIZE];
	int				histo_begin;
	char			*input;
	char			**all_cmd;
	char			**one_cmd;
}					t_minishell;

char	**g_env;
t_minishell *g_ms;
typedef struct stat	t_stat;

int		ms_error(int ret, char *s);
void	show_prompt_msg(t_minishell *ms);
void	ms_signal_prompt(int signo);
void	ms_signal_no_prompt(int signo);
int		get_home_path(t_minishell *ms, char *path, char **return_path, int reverse);
char	*get_from_env(t_minishell *ms, char *var);
void	ms_exit(t_minishell *ms, int exit_status);
void	initialize_env(t_minishell *ms, int ac, char **av, char **env);
int		run_cmd(t_minishell *ms, char *path);
int		is_bin_cmd(t_minishell *ms);
int		builtin_mspath(t_minishell *ms);
int		builtin_msname(t_minishell *ms);
int		execute_all_commands(t_minishell *ms);
void	ms_inputsplit(t_minishell *ms);
#endif
