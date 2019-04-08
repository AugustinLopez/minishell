/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_type.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 11:48:20 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:30:01 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void				ret_value(t_minishell *ms, int status)
{
	if (WIFEXITED(status))
		ms->ret = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ms->ret = 128 + WTERMSIG(status);
}

static inline int	is_bin_executable(t_minishell *ms, char *bin, t_stat stat)
{
	int	i;

	if (stat.st_mode & S_IFREG)
	{
		i = 1;
		if (stat.st_mode & S_IXUSR)
			i = run_cmd(ms, bin);
		else if ((ms->ret = 126))
			ft_dprintf(2, "%s: permission denied.\n", bin);
		bin ? ft_memdel((void*)&bin) : 0;
		return (i);
	}
	bin ? ft_memdel((void*)&bin) : 0;
	return (0);
}

static inline int	is_bin_loop(t_minishell *ms, char ***path, size_t i)
{
	t_stat	stat;
	char	*bin;

	bin = ft_strlcmp((ms->one_cmd)[0], (*path)[i]) ?
		ft_pathjoin((*path)[i], *(ms->one_cmd)) : ft_strdup(*(ms->one_cmd));
	if (!bin)
	{
		ft_free_sarray(path);
		return (ms_error(ms, -1, "Error: not enough memory.\n"));
	}
	if (lstat(bin, &stat) < 0)
		ft_memdel((void*)&bin);
	else
	{
		ft_free_sarray(path);
		return (is_bin_executable(ms, bin, stat));
	}
	return (0);
}

int					is_bin_cmd(t_minishell *ms)
{
	size_t	i;
	int		ret;
	char	**path;

	if (!get_from_env(ms, "PATH="))
		return (0);
	if (!(path = ft_strsplit(get_from_env(ms, "PATH="), ':')))
		return (ms_error(ms, -1, "Error: not enough memory.\n"));
	i = 0;
	while (path[i])
	{
		if (ms->flags & MSF_NO_MORE_CMD)
			break ;
		ret = is_bin_loop(ms, &path, i);
		if (!path)
			return (ret);
		i++;
	}
	ft_free_sarray(&path);
	return (0);
}

int					is_builtin_cmd(t_minishell *ms)
{
	int	ret;

	if (!ft_strcmp((ms->one_cmd)[0], "exit"))
		ms_exit(ms);
	ms->ret = 0;
	if (!ft_strcmp((ms->one_cmd)[0], "echo"))
		return (ms_echo(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "cd"))
	{
		ret = ms_cd(ms);
		load_prompt(ms);
		return (ret);
	}
	if (!ft_strcmp((ms->one_cmd)[0], "setenv"))
		return (ms_setenv(ms));
	if (!ft_strcmp(ms->one_cmd[0], "unsetenv"))
		return (ms_unsetenv(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "msname"))
		return (builtin_msname(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "mspath"))
		return (builtin_mspath(ms));
	if (!ft_strcmp((ms->one_cmd)[0], "env"))
		return (ms_env(ms));
	return (0);
}
