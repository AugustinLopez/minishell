/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:35:56 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/27 16:28:37 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>


static int	is_builtin_cmd(t_minishell *ms)
{
	size_t	i;
	int		j;
	char	*tmp;
	t_list	*temp;

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
			return (1);
		}
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
	if (!ft_strcmp((ms->one_cmd)[0], "mspath"))
		return (builtin_mspath(ms));
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

//bonus_semicolon

/*int			execute_with_semicolon(t_minishell *ms)
{
	size_t	i;

	ms_free(ms, 2);
	ms->one_cmd = ms->all_cmd;
	i = 0;
	while ((ms->all_cmd)[i])
	{
		
	}
}*/

int			remove_unused_escape_quote(char **src, char **new)
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
	(*new)[i] = 0;
	free(*src);
	*src = *new;
	*new = NULL;
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
			return (-1);
		if (k)
			ret = remove_unused_escape_quote(&(ms->all_cmd[i]), &(ms->tmp0));
		if ((ms->all_cmd)[i][0] == ';')
		{
			ms->tmp0 = (ms->all_cmd)[i];
			(ms->all_cmd)[i] = NULL;
			if (j == i)
				ret = 1;
			else
				ret = execute_single_command(ms);
			if (ret < 0)
				break ;
			(ms->all_cmd)[i] = ms->tmp0;
			ms->tmp0 = NULL;
			ms->one_cmd = (ms->all_cmd) + (i + 1);
			j = i + 1;
		}
		i++;
	}
	if (ret >= 0 && j < i)
		ret = execute_single_command(ms);
	return (ret);
}
