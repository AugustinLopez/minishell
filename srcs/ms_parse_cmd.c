/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:35:56 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/27 18:52:58 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	is_builtin_cmd(t_minishell *ms)
{
	t_list	*temp;

	if (!ft_strcmp((ms->one_cmd)[0], "exit"))
		ms_exit(ms);
	if (!ft_strcmp((ms->one_cmd)[0], "echo"))
		return (ms_echo(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "msname"))
		return (builtin_msname(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "mspath"))
		return (builtin_mspath(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "env"))
	{
		temp = ms->env;
		while (temp)
		{
			ft_putendl((char *)(temp->pv));
			temp = temp->next;
		}
		return (1);
	}
	if (!ft_strcmp((ms->one_cmd)[0], "setenv"))
	{
		if (!(ms->one_cmd)[1])
		{
			temp = ms->env;
			while (temp)
			{
				ft_putendl((char *)(temp->pv));
				temp = temp->next;
			}
			return (1);
		}
		if ((ms->one_cmd)[2])
		{
			ft_dprintf(2, "setenv: too many arguments.\n");
			return (0);
		}
		return (1);
	}
	return (0);
}

static int	execute_single_command(t_minishell *ms)
{
	t_stat	stat;
	int		i;

	if (ms->flags & MSF_NO_MORE_CMD)
	{
		ms->flags &= ~MSF_NO_MORE_CMD;
		return (-1);
	}
	if ((i = is_builtin_cmd(ms)))
		return (i);
	if ((i = is_bin_cmd(ms)))
		return (i);
	if (lstat((ms->one_cmd)[0], &stat) != -1)
	{
		if (stat.st_mode & S_IXUSR)
			return (run_cmd(ms, (ms->one_cmd)[0]));
		//else if (stat.st_mode & S_IFDIR) // cd
	}
	if (*(ms->one_cmd) && **(ms->one_cmd))
		ft_dprintf(2, "%s: command not found.\n", (ms->one_cmd)[0]);
	return (0);
}

int			clean_quote(char **src, char **new)
{
	int		ret;
	size_t	i;
	size_t	j;

	ret = 0;
	j = 0;
	i = 0;
	while ((*src)[j])
	{
		if (!ret && (*src)[j] == '\\')
			j++;
		else if (ft_strchr("\'\"",(*src)[j]))
		{
			ret = 1;
			j++;
			continue;
		}
		(*new)[i++] = (*src)[j++];
	}
	ret = (ret == 1) || ((*src)[0] == '\\') ? 1 : 0;
	(*new)[i] = 0;
	free(*src);
	*src = *new;
	*new = NULL;
	return (ret);
}

int			execute_command_among_other(t_minishell *ms, size_t i, size_t *j)
{
	char	*save;
	int		ret;

	save = (ms->all_cmd)[i];
	(ms->all_cmd)[i] = NULL;
	if (*j == i)
		ret = 1;
	else
		ret = execute_single_command(ms);
	(ms->all_cmd)[i] = save;
	ms->one_cmd = (ms->all_cmd) + (i + 1);
	*j = i + 1;
	return (ret);
}

int			ms_execute_all(t_minishell *ms)
{
	size_t	i;
	size_t	j;
	size_t	k;
	int		ret;

	ms_free(ms, 2);
	ret = 0;
	i = 0;
	j = 0;
	ms->one_cmd = ms->all_cmd;
	while ((ms->all_cmd)[i])
	{
		k = ft_strlen(ms->all_cmd[i]);
		if (k && !(ms->tmp0 = ft_strnew(k)))
			return (ms_error(-1, "Error: not enough memory to parse command.\n"));
		ret = (k != 0) ? clean_quote(&(ms->all_cmd[i]), &(ms->tmp0)) : 0;
		if (!ret && (ms->all_cmd)[i][0] == ';')
			if ((ret = execute_command_among_other(ms, i, &j) < 0))
				break ;
		i++;
	}
	if (ret < 0)
		return (ret);
	ret = (j < i) ? execute_single_command(ms) : ret;
	return (ret);
}
