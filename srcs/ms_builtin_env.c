/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:24:44 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/03 16:25:21 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*int		env_change(t_list *env, t_list *start)
{
	t_list	*tmp;
	t_list	*tmpenv;
	char	*s;

	tmp = start;
	while (tmp)
	{
		s = ft_strchr(tmp->pv, '=');
		s[0] = 0;
		tmpenv = env;
		while (tmpenv)
		{
			if (ft_strcmp(tmpenv->pv, tmp->pv) == '=')
				break ;
			tmpenv = tmpenv->next;
		}
		s[0] = '=';
		if (tmpenv)
		{
			free(tmpenv->pv);
			tmpenv->pv = tmp->pv;
			start = tmp;
			tmp = tmp->next;
			start->pv = 0;
			ft_lstdelone(&start, *ft_lstfree);
		}
		else
		{
			start = tmp;
			tmp = tmp->next;
			start = 0;
			tmpenv = env;

		}
	}
	return (0);
}

int		env_mem_check(t_minishell *ms, t_list **tmp, t_list **start)
{
	int	i;

	while (ms->one_cmd[0])
	{
		if (!ft_strchr(ms->one_cmd[0], '=') || (ms->one_cmd[0][0] == '='))
			return (0);
		if (!*tmp)
		{
			*tmp = ft_lstnew(0,0);
			*start = *tmp;
		}
		else
		{
			(*tmp)->next = ft_lstnew(0,0);
			*tmp = (*tmp)->next;
		}
		if (!*tmp || !((*tmp)->pv = ft_strdup(ms->one_cmd[0])))
		{
			*start ? ft_lstdel(start, *ft_lstfree) : 0;
			return (1);
		}
		(ms->one_cmd)++;
	}
	return (0);
}

int		env_set(t_minishell *ms, t_list *env)
{
	int		i;
	t_list	*start;
	t_list	*tmp;
	char	**arr;

	i = 1;
	start = NULL;
	if (env_mem_check(ms, &tmp, &start))
		return (ms_error(1, "env: not enough memory\n"));
	env_change(ms, start);
	tmp = ms->env;
	i = 0;
	while (tmp)
	{
		arr[i++] = tmp->pv;
		tmp = tmp->next;
	}
	free(ms->arr_env);
	ms->arr_env = arr;
	return (0);
}*/


// need ft_lstcpy
int		ms_env(t_minishell *ms)
{
	t_list	*tmp;
	t_list	*mem;

	tmp = ms->env;
	ms->ret = 0;
	if (!(ms->one_cmd[1]))
	{
		while (tmp)
		{
			ft_putendl((char *)(tmp->pv));
			tmp = tmp->next;
		}
		return (1);
	}
	(ms->one_cmd)++;
	mem = ms->env;
	tmp = ms->env;
	while (tmp->next)
		tmp = tmp->next;
	if (!(ft_strcmp(ms->one_cmd[0], "-i")))
	{
		tmp = 0;
		ms->env = 0;
		(ms->one_cmd)++;
	}
	if (env_set(ms, tmp))
	{
		ms->ret = -1;
		return (1);
	}
	return (1);
}
