/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_run_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 11:48:20 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/04 17:25:54 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void	bonus_return(t_minishell *ms, int status)
{
	int	i;

	i = 0;
	if (WIFEXITED(status))
		ms->ret = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		ms->ret = 128 + WTERMSIG(status);
		if (!(ms->flags & MSF_NO_MORE_CMD))
		{
			ft_dprintf(2, "minishell: core dumped: ");
			while (ms->one_cmd[i])
				ft_dprintf(2, " %s", ms->one_cmd[i++]);
			ft_dprintf(2,"\n");
		}
	}
}

int		run_cmd(t_minishell *ms, char *path)
{
	pid_t	pid;
	int		status;
	t_stat	st;
	t_list	*tmp;
	char	**arr;

	ms_free(ms, 2);
	signal(SIGINT, ms_signal_no_prompt);
	if (!(arr = (char **)ft_memalloc(sizeof(char *) * (1 + ft_lstsize(ms->env)))))
		return (ms_error(-1, "minishell: not enough memory\n"));
	status = 0;
	tmp = ms->env;
	while (tmp)
	{
		(arr)[status++] = tmp->pv;
		tmp = tmp->next;
	}
	if (stat(path, &st) < 0)
	{
		free(arr);
		ft_dprintf(2, "minishell: invalid command: %s\n", path);
		return (1);
	}
	if ((!(st.st_mode & S_IXUSR) || !(S_ISREG(st.st_mode))))
	{
		free(arr);
		ft_dprintf(2, "minishell: permission denied: %s\n", path);
		return (1);
	}
	status = 0;
	pid = fork();
	if (ms->flags & MSF_NO_MORE_CMD)
	{
		free(arr);
		return (1);
	}
	if (!pid)
		execve(path, ms->one_cmd, arr);
	else if (pid < 0)
	{
		free(arr);
		return (ms_error(-1, "minishell: failed to fork the process.\n"));
	}
	free(arr);
	waitpid(pid, &status, 0);
	bonus_return(ms, status);
	return (1);
}

static int		is_bin_executable(t_minishell *ms, char *bin, t_stat stat)
{
	int	i;

	if (stat.st_mode & S_IFREG)
	{
		i = 1;
		if (stat.st_mode & S_IXUSR)
			i = run_cmd(ms, bin);
		else
			ft_dprintf(2, "%s: permission denied.\n", bin);
		bin ? ft_memdel((void*)&bin) : 0;
		return (i);
	}
	bin ? ft_memdel((void*)&bin) : 0;
	return (0);
}

int		is_bin_cmd(t_minishell *ms)
{
	size_t	i;
	char	*bin;
	char	**path;
	t_stat	stat;

	if (!get_from_env(ms, "PATH="))
		return (0);
	if (!(path = ft_strsplit(get_from_env(ms, "PATH="), ':')))
		return (ms_error(1, "Error: not enough memory.\n"));
	i = 0;
	while (path[i])
	{
		if (ms->flags & MSF_NO_MORE_CMD)
			return (0);
		bin = ft_strlcmp((ms->one_cmd)[0], path[i]) ?
			ft_pathjoin(path[i], *(ms->one_cmd)) : ft_strdup(*(ms->one_cmd));
		if (!bin)
			return (ms_error(1, "Error: not enough memory.\n"));
		if (lstat(bin, &stat) < 0)
			ft_memdel((void*)&bin);
		else
		{
			ft_free_sarray(&path);
			return (is_bin_executable(ms, bin, stat));
		}
		i++;
	}
	ft_free_sarray(&path);
	return (0);
}
