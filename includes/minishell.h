/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/07 11:21:42 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/09 12:24:22 by aulopez          ###   ########.fr       */
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

char	**g_env;
typedef struct stat	t_stat;

void	show_prompt_msg(void);
void	signal_display(int signo);
void	signal_no_display(int signo);
int		get_home_path(char *path, char **return_path, int reverse);
char	*get_from_env(char *var);
void	ms_exit(int exit_status);
void	initialize_env(int ac, char **av, char **env);
#endif
