/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utilities.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:52:52 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** option == 0 : free everything
** option == 1 : free current command cycle
** option == 2 : free temp variable
*/

void	ms_free(t_minishell *ms, int option)
{
	if (!option)
	{
		ms->env ? ft_lstdel(&(ms->env), *ft_lstfree) : 0;
		ms->cwd ? ft_memdel((void**)&(ms->cwd)) : 0;
		ft_gnl(-1, NULL, 0);
	}
	if (option <= 1)
	{
		ms->input ? ft_memdel((void**)&(ms->input)) : 0;
		ms->cmd ? ft_lstdel(&(ms->cmd), *ft_lstfree) : 0;
		ms->arr_cmd ? ft_memdel((void**)&(ms->arr_cmd)) : 0;
		ms->flags &= ~MSF_NO_MORE_CMD;
	}
	ms->tmp0 ? ft_memdel((void**)&(ms->tmp0)) : 0;
	ms->tmp1 ? ft_memdel((void**)&(ms->tmp1)) : 0;
}

int		ms_error(t_minishell *ms, int ret, char *s)
{
	ft_dprintf(2, "%s", s);
	ms->ret = ret;
	return (ret);
}

char	*get_from_env(t_minishell *ms, char *var)
{
	size_t	i;
	t_list	*tmp;
	char	*s;

	i = 0;
	tmp = ms->env;
	while (tmp)
	{
		s = (char *)(tmp->pv);
		if (!ft_strlcmp(s, var))
			return (ft_strchr(s, '=') + 1);
		i++;
		if (tmp)
			tmp = tmp->next;
	}
	return (NULL);
}

int		get_home_path(t_minishell *ms, char *path, char **ret_path, int revers)
{
	char	*home_path;

	home_path = get_from_env(ms, "HOME=");
	ret_path ? ft_memdel((void **)ret_path) : 0;
	if (!home_path)
		return (0);
	if (ft_strlcmp(path, revers ? "~" : home_path))
		*ret_path = ft_strdup(path);
	else if (revers)
		*ret_path = ft_pathjoin(home_path, path + 1);
	else if (*(path + ft_strlen(home_path)) == '\0')
		*ret_path = ft_strdup("~");
	else
		*ret_path = ft_pathjoin("~", path + ft_strlen(home_path));
	if (*ret_path)
		return (0);
	return (ms_error(ms, -1, "msh: not enough memory to load home path.\n"));
}

int		print_env(t_minishell *ms)
{
	t_list	*tmp;

	if (!(ms->one_cmd[1]))
	{
		tmp = ms->env;
		while (tmp)
		{
			ft_putendl((char *)(tmp->pv));
			tmp = tmp->next;
		}
		return (1);
	}
	return (0);
}
