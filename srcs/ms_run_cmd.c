/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_run_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 11:48:20 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/26 17:26:22 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

int		run_cmd(t_minishell *ms, char *path)
{
	pid_t	pid;

	pid = fork();
	signal(SIGINT, ms_signal_no_prompt);
	if (ms->flags & MSF_NO_MORE_CMD)
		return (1);
	if (!pid)
		execve(path, ms->one_cmd, ms->arr_env);
	else if (pid < 0)
		return (ms_error(-1, "Error : failed to fork the process.\n"));
	wait(&pid);
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

	if (!(path = ft_strsplit(get_from_env(ms, "PATH="), ':')))
		return (ms_error(-1, "Error: not enough memory.\n"));
	i = 0;
	while (path[i])
	{
		if (ms->flags & MSF_NO_MORE_CMD)
			return (0);
		bin = ft_strlcmp((ms->one_cmd)[0], path[i]) ?
			ft_pathjoin(path[i], *(ms->one_cmd)) : ft_strdup(*(ms->one_cmd));
		if (!bin)
			return (ms_error(-1, "Error: not enough memory.\n"));
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
