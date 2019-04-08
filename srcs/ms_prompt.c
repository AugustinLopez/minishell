/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 10:21:01 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 17:51:55 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** Show_prompt may be called by signal_handler.
** As such, I will only use async-signal-safe functions, and keep things simple.
** ft_putstr will call to write (safe) and ft_strlen (whose implementation
** should be safe).
** No variable will be modified during a signal.
*/

void	show_prompt(t_minishell *ms)
{
	if ((ms->flags & MSF_SHOW_PATH_HOME) && ms->cwd)
	{
		ft_putstr(FT_CYAN);
		ft_putstr(FT_BOLD);
		ft_putstr(ms->hostname);
		ft_putstr(">:");
		ft_putstr(FT_EOC);
		ft_putstr(FT_CYAN);
		ft_putstr(ms->cwd);
		ft_putstr(" ");
		ft_putstr(FT_EOC);
	}
	else
	{
		ft_putstr(FT_CYAN);
		ft_putstr(FT_BOLD);
		ft_putstr(ms->hostname);
		ft_putstr("> ");
		ft_putstr(FT_EOC);
	}
}

int		load_prompt(t_minishell *ms)
{
	char	buff[PATH_MAX + 1];
	char	*curr_dir;

	ms->cwd ? ft_memdel((void**)&(ms->cwd)) : 0;
	if ((ms->flags & MSF_SHOW_PATH_HOME))
	{
		if (!(curr_dir = getcwd(buff, PATH_MAX)))
		{
			ft_dprintf(2, "minishell: ");
			ft_dprintf(2, "could not get path to current directory.\n");
			ms->ret = 1;
			return (0);
		}
		else if (get_home_path(ms, curr_dir, &(ms->cwd), 0) == -1)
			return (-1);
	}
	return (0);
}
