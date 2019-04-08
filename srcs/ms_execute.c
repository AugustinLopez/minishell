/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:35:56 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:30:18 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline int	test_error_pre_fork(t_minishell *ms, char *path, char **arr)
{
	size_t	i;
	t_stat	st;
	t_list	*tmp;

	i = 0;
	tmp = ms->env;
	while (tmp)
	{
		(arr)[i++] = tmp->pv;
		tmp = tmp->next;
	}
	i = 1;
	ms->ret = 0;
	if (stat(path, &st) < 0 && (ms->ret = 126))
		ft_dprintf(2, "minishell: invalid command: %s\n", path);
	else if ((!(st.st_mode & S_IXUSR) || !(S_ISREG(st.st_mode)))
		&& (ms->ret = 127))
		ft_dprintf(2, "minishell: permission denied: %s\n", path);
	else if (!(ms->flags & MSF_NO_MORE_CMD))
		i = 0;
	if (i == 1)
		free(arr);
	return (i);
}

int					run_cmd(t_minishell *ms, char *path)
{
	pid_t	pid;
	int		status;
	char	**arr;

	ms_free(ms, 2);
	signal(SIGINT, ms_signal_when_executing);
	arr = (char **)ft_memalloc(sizeof(char *) * (1 + ft_lstsize(ms->env)));
	if (!arr)
		return (ms_error(ms, -1, "msh: not enough memory\n"));
	if (test_error_pre_fork(ms, path, arr))
		return (1);
	status = 0;
	pid = fork();
	if (!pid)
		execve(path, ms->one_cmd, arr);
	else if (pid < 0)
	{
		free(arr);
		return (ms_error(ms, -1, "msh: failed to fork the process.\n"));
	}
	free(arr);
	waitpid(pid, &status, 0);
	ret_value(ms, status);
	return (1);
}

int					execute_single_command(t_minishell *ms)
{
	t_stat	stat;
	int		i;

	if (ms->flags & MSF_NO_MORE_CMD)
		return (-1);
	if ((i = is_builtin_cmd(ms)))
		return (i);
	if ((i = is_bin_cmd(ms)))
		return (i);
	if (lstat((ms->one_cmd)[0], &stat) != -1 && ft_strchr(ms->one_cmd[0], '/'))
	{
		if ((stat.st_mode & S_IXUSR))
			return (run_cmd(ms, (ms->one_cmd)[0]));
		ft_dprintf(2, "msh: permission denied: %s\n", (ms->one_cmd)[0]);
		ms->ret = 126;
	}
	else if (*(ms->one_cmd) && **(ms->one_cmd))
	{
		ft_dprintf(2, "msh: command not found: %s\n", (ms->one_cmd)[0]);
		ms->ret = 127;
	}
	return (0);
}

static inline int	execute_command_among_other(t_minishell *ms, size_t i,
												size_t *j)
{
	int		ret;

	(ms->arr_cmd)[i] = NULL;
	if (*j == i)
		ret = 0;
	else
		ret = execute_single_command(ms);
	(ms->arr_cmd)[i] = ms->elem->pv;
	ms->one_cmd = (ms->arr_cmd) + (i + 1);
	*j = i + 1;
	return (ret);
}

int					ms_execute(t_minishell *ms)
{
	size_t	i;
	size_t	j;
	int		ret;

	ms_free(ms, 2);
	ret = 0;
	i = 0;
	j = 0;
	ms->one_cmd = ms->arr_cmd;
	ms->elem = ms->cmd;
	while (ms->elem)
	{
		ms->ret = 0;
		if (ms->elem->zu == ';')
			if ((ret = execute_command_among_other(ms, i, &j)) < 0)
				break ;
		i++;
		ms->elem = ms->elem->next;
	}
	ms->ret = 0;
	if (ret < 0)
		return (ret);
	ret = (j < i && ms->arr_cmd[j]) ? execute_single_command(ms) : ret;
	return (ret);
}
