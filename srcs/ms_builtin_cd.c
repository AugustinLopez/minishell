/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:32:06 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/02 14:10:31 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*cd_set(char *name, char *value)
{
	t_list	*tmp;

	tmp = ft_lstnew(0, 0);
	if (!tmp || !(tmp->pv = ft_strjoin(name, value)))
	{
		ft_lstdelone(&tmp, *ft_lstfree);
		return (0);
	}
	return (tmp);
}

void	cd_setenv(t_minishell *ms, t_list *list)
{
	t_list	*tmp;

	tmp = ms->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->pv, list->pv, ft_strchri(list->pv, '=') - 1))
		{
			free(tmp->pv);
			tmp->pv = list->pv;
			list->pv = 0;
			ft_lstdelone(&list, *ft_lstfree);
			return ;
		}
		tmp = tmp->next;
	}
	if (!ms->env)
		ms->env = list;
	else
		ft_lstadd(&(ms->env), list);
}

int	change_dir(t_minishell *ms, char *path, int flags)
{
	char	*cwd;
	char	buff[4096 + 1];
	t_list	*list;

	(void)path;
	(void)flags;
	cwd = getcwd(buff, 4096);
	list = cd_set("OLDPWD=", cwd);
	if (!chdir(path))
	{
		cwd = getcwd(buff, 4096);
		if (flags & 512)
			ft_printf("%s\n", cwd);
		list->next = cd_set("PWD=", cwd);
		if (ms->flags & MSF_SHOW_PATH_HOME)
			get_home_path(ms, cwd, &(ms->curr_path), 0);
		cd_setenv(ms, list->next);
		cd_setenv(ms, list);
	}
	else
	{
		ft_putstr_fd("cd: ", 2);
		if (access(path, F_OK) == -1)
			ft_putstr_fd("no such file or directory: ", 2);
		else if (access(path, R_OK) == -1)
			ft_putstr_fd("permission denied: ", 2);
		else
			ft_putstr_fd("not a directory: ", 2);
		ft_putendl_fd(path, 2);
		return (1);
	}
	return (0);
}

int cd_available_option(char *av, int *flags)
{
	char	*s;

	while (*(av++))
	{
		if (!(s = ft_strchr("LP", av[0])))
			return (0);
		*flags = s[0];
	}
	return (1);
}

int	cd_parsing(t_minishell *ms, int *flags)
{
	int		i;
	char	**av;

	av = ms->one_cmd;
	if (!av || !(av[0]))
		return (-1);
	i = 1;
	while (av[i] && av[i][0] == '-' && av[i][1])
	{
		if (av[i][1] == '-' && !av[i][2])
			return (i + 1);
		if (!cd_available_option(av[i], flags))
		{
			*flags = i;
			return (-1);
		}
		i++;
	}
	return (i);
}

int	ms_cd(t_minishell *ms)
{
	char	*home_path;
	int		ac;
	int		flags;

	flags = 0;
	home_path = get_from_env(ms, "HOME=");
	if ((ac = cd_parsing(ms, &flags)) < 0)
	{
		ft_dprintf(2, "cd: invalid argument: %s\n", ms->one_cmd[flags]);
		return (1);
	}
	if (!(ms->one_cmd[ac]))
	{
		if (!home_path)
			return (1);
		change_dir(ms, home_path, flags);
		return (1);
	}
	if (ms->one_cmd[ac + 1])
		return (ms_error(1, "cd: too many arguments\n"));
	if (ms->one_cmd[ac][0] == '-' && !ms->one_cmd[ac][1])
	{
		flags += 512;
		change_dir(ms, get_from_env(ms, "OLDPWD="), flags);
		return (1);
	}
	change_dir(ms, ms->one_cmd[ac], flags);
	return (1);
}
