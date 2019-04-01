/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:35:56 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/01 16:11:07 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	is_builtin_cmd(t_minishell *ms)
{
	if (!ft_strcmp((ms->one_cmd)[0], "exit"))
		ms_exit(ms);
	if (!ft_strcmp((ms->one_cmd)[0], "echo"))
		return (ms_echo(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "cd"))
		return (ms_cd(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "setenv"))
		return (ms_setenv(ms));
	if (!ft_strcmp(ms->one_cmd[0], "unsetenv"))
		return (ms_unsetenv(ms));
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
		return (-1);
	if ((i = is_builtin_cmd(ms)))
		return (i);
	if ((i = is_bin_cmd(ms)))
		return (i);
	if (lstat((ms->one_cmd)[0], &stat) != -1)
	{
		if ((stat.st_mode & S_IXUSR))
			return (run_cmd(ms, (ms->one_cmd)[0]));
		// in run_cmd
		ft_dprintf(2, "minishell: permission denied: %s\n", (ms->one_cmd)[0]);
		ms->ret = 126;
		return (0);
	}
	if (*(ms->one_cmd) && **(ms->one_cmd))
	{
		ft_dprintf(2, "minishell: command not found: %s\n", (ms->one_cmd)[0]);
		ms->ret = 127;
	}
	return (0);
}

int			execute_command_among_other(t_minishell *ms, size_t i, size_t *j)
{
	int		ret;

	(ms->all_cmd)[i] = NULL;
	if (*j == i)
		ret = 0;
	else
		ret = execute_single_command(ms);
	(ms->all_cmd)[i] = ms->elem->pv;
	ms->one_cmd = (ms->all_cmd) + (i + 1);
	*j = i + 1;
	return (ret);
}

int			ms_execute(t_minishell *ms)
{
	size_t	i;
	size_t	j;
	int		ret;

	ms_free(ms, 2);
	ret = 0;
	i = 0;
	j = 0;
	ms->one_cmd = ms->all_cmd;
	ms->elem = ms->cmd;
	while (ms->elem)
	{
		if (ms->elem->zu == ';')
			if ((ret = execute_command_among_other(ms, i, &j)) < 0)
				break ;
		i++;
		ms->elem = ms->elem->next;
	}
	if (ret < 0)
		return (ret);
	ret = (j < i) ? execute_single_command(ms) : ret;
	return (ret);
}
