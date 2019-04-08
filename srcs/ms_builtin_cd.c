/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:32:06 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:20:24 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** cd should perform as described by - http:
** //pubs.opengroup.org/onlinepubs/9699919799/utilities/cd.html#tag_20_14_08
** but with -P always active. I implemented the parsing but I won't use
** those flags in the minishell.
*/

static inline t_list	*cd_set(char *name, char *value)
{
	t_list	*tmp;

	if (!(tmp = ft_lstnew(0, 0)))
		return (0);
	if (!(tmp->pv = ft_strjoin(name, value)))
	{
		ft_lstdelone(&tmp, *ft_lstfree);
		return (0);
	}
	return (tmp);
}

static inline void		cd_setenv(t_minishell *ms, t_list *list)
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

static inline void		could_not_change_dir(t_minishell *ms,
												t_list **list, char *path)
{
	ft_putstr_fd("cd: ", 2);
	if (access(path, F_OK) == -1 && (ms->ret = 1))
		ft_putstr_fd("no such file or directory: ", 2);
	else if (access(path, R_OK) == -1 && (ms->ret = 126))
		ft_putstr_fd("permission denied: ", 2);
	else if ((ms->ret = 1))
		ft_putstr_fd("not a directory: ", 2);
	ft_putendl_fd(path, 2);
	ft_lstdel(list, *ft_lstfree);
}

int						change_dir(t_minishell *ms, char *path, int flags)
{
	char	*cwd;
	char	buff[PATH_MAX + 1];
	t_list	*list;

	if (!(cwd = getcwd(buff, PATH_MAX)))
		return (ms_error(ms, 1, "cd: bad path to current directory.\n"));
	if (!(list = cd_set("OLDPWD=", cwd)))
		return (ms_error(ms, -1, "msh: not enough memory to use cd\n"));
	if (!chdir(path))
	{
		if (!(cwd = getcwd(buff, PATH_MAX)))
			return (ms_error(ms, 1, "cd: bad path to current directory.\n"));
		if (flags & 512)
			ft_printf("%s\n", cwd);
		if (!(list->next = cd_set("PWD=", cwd)))
			return (ms_error(ms, -1, "msh: not enough memory to use cd\n"));
		if (ms->flags & MSF_SHOW_PATH_HOME && !(ms->flags & MSF_ENV_CMD))
			if (get_home_path(ms, cwd, &(ms->cwd), 0))
				return (-1);
		cd_setenv(ms, list->next);
		cd_setenv(ms, list);
	}
	else
		could_not_change_dir(ms, &list, path);
	return (1);
}

int						ms_cd(t_minishell *ms)
{
	char	*home_path;
	char	*curpath;
	int		ac;
	int		flags;
	int		ret;

	flags = 0;
	home_path = get_from_env(ms, "HOME=");
	if ((ac = cd_posix_parsing(ms, &flags)) < 0)
		return (1);
	if ((ret = cd_posix_step_1_2(ms, ac, home_path, flags)))
		return (ret);
	if ((ret = cd_posix_minus(ms, ac, flags)))
		return (ret);
	if ((curpath = cd_posix_step_3_to_6(ms, ac, &flags)))
	{
		ret = change_dir(ms, curpath, flags);
		free(curpath);
		return (ret);
	}
	else
		return (ms_error(ms, -1, "msh: not enough memory to use cd\n"));
	return (1);
}
