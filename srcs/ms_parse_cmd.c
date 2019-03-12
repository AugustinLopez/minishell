/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:35:56 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/12 16:53:58 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

static int	is_builtin_cmd(t_minishell *ms)
{
	size_t	i;
	int		j;
	char	*tmp;

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
	if (!ft_strcmp((ms->one_cmd)[0], "echo"))
	{
		if (!ft_strcmp((ms->one_cmd)[1], "-n"))
			j = 1;
		i = 1 + j;
		while ((ms->one_cmd)[i])
		{
			tmp = ft_strrstr((ms->one_cmd)[i], "\\c");
			if (tmp && !(tmp[2]))
			{
				*tmp = 0;
				j = 2;
			}
			ft_putstr((ms->one_cmd)[i++]);
			if (j == 2)
				break ;
			if ((ms->one_cmd)[i])
				write(1, " ", 1);
		}
		!j ? write(1, "\n", 1) : 0;
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
	size_t	j;
	size_t	k;
	size_t	l;
	int		ret;
	char	*tmp;

	i = 0;
	j = 0;
	ms->one_cmd = ms->all_cmd;
	while ((ms->all_cmd)[i])
	{
		if ((k = ft_strlen(ms->all_cmd[i])))
		{
			ret = 0;
			tmp = ft_strnew(k);
			l = 0;
			k = 0;
			while ((ms->all_cmd[i][l]))
			{
				if (!ret && ms->all_cmd[i][l] == '\\')
					l++;
				if (ft_strchr("\'\"",(ms->all_cmd)[i][l]))
				{
					ret = 1;
					l++;
					continue;
				}
				tmp[k++] = (ms->all_cmd)[i][l++];
			}
			free((ms->all_cmd)[i]);
			(ms->all_cmd)[i] = tmp;
		}
		if ((ms->all_cmd)[i][0] == ';')
		{
			tmp = (ms->all_cmd)[i];
			(ms->all_cmd)[i] = NULL;
			//need to remove possible quote, single quote et '\'
			ret = execute_single_command(ms);
			if (ret < 0)
				break ;
			(ms->all_cmd)[i] = tmp;;
			ms->one_cmd = (ms->all_cmd) + (i + 1);
			j = i + 1;
		}
		i++;
	}
	if (j < i)
		ret = execute_single_command(ms);
	return (ret);
}
