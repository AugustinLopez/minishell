/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:35:56 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/11 18:41:20 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

static int	is_builtin_cmd(t_minishell *ms)
{
	size_t	i;
	int		j;

	i = 0;
	j = 0;
	if (!ft_strcmp((ms->one_cmd)[0], "exit"))
	{
		if ((ms->one_cmd)[1])
			j = ft_atoi(ms->one_cmd[1]);
		if (j)
			ms_exit(ms, j);
		ms->flags |= MSF_BUILTIN_EXIT;
		return (-1);
	}
	if (!ft_strcmp((ms->one_cmd)[0], "env"))
	{
		while ((ms->env)[i])
			ft_putendl((ms->env)[i++]);
		return (1);
	}
	if (!ft_strcmp((ms->one_cmd)[0], "msname"))
		return (builtin_msname(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "mshomepath"))
		return (builtin_mspath(ms));
	return (0);
}

static int	execute_single_command(t_minishell *ms)
{
	t_stat	stat;
	int		i;

	if ((i = is_bin_cmd(ms)))
		return (i);
	if ((i = is_builtin_cmd(ms)))
		return (i);
	if (lstat((ms->one_cmd)[0], &stat) != -1)
	{
		if (stat.st_mode & S_IXUSR)
			return (run_cmd(ms, (ms->one_cmd)[0]));
		//else if (stat.st_mode & S_IFDIR) // cd
	}
	ft_dprintf(2, "%s: command not found.\n", (ms->one_cmd)[0]);
	return (0);
}

int			execute_all_commands(t_minishell *ms)
{
	size_t	i;
	int		ret;

	i = 0;
	while ((ms->all_cmd)[i])
	{
		if (!(ms->one_cmd = ft_strssplit((ms->all_cmd)[i], " \t\v\f\r")))
		{
			ft_dprintf(2, "Error: not enough memory.\n");
			return (-1);
		}
		ret = execute_single_command(ms);
		ft_free_sarray(&(ms->one_cmd));
		if (ret < 0)
			break;
		i++;
	}
	return (ret);
}
