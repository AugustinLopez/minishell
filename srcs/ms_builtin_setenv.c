/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_setenv.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 11:56:17 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/01 14:04:08 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int		setenv_error_check(t_minishell *ms)
{
	int		i;
	char	*s;

	i = 1;
	while (ms->one_cmd[i])
	{
		if (!(s = ft_strchr(ms->one_cmd[i], '=')) || ft_strchr(s + 1, '='))
		{
			ft_putstr_fd("setenv: ", 2);
			return (ms_error(1, "argument format is not \'name=value\'\n"));
		}
		if (!(s[1]))
			return (ms_error(1, "setenv: variable cannot be set as NUL\n"));
		i++;
	}
	return (0);
}

int		setenv_change(t_minishell *ms, t_list *start)
{
	t_list	*tmp;
	t_list	*tmpenv;

	tmp = start;
	while (tmp)
	{
		tmpenv = ms->env;
		while (tmpenv)
		{
			if (ft_strcmp(tmp->pv, tmpenv->pv) == '=')
				break ;
			tmpenv = tmpenv->next;
		}
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
			ft_lstadd(&(ms->env), start);
		}
	}
	return (0);
}

int		setenv_mem_check(t_minishell *ms, t_list **tmp, t_list **start, char ***arr)
{
	int	i;

	i = 0;
	while (ms->one_cmd[++i])
	{
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
		if (!*tmp || !((*tmp)->pv = ft_strdup(ms->one_cmd[i])))
		{
			*start ? ft_lstdel(start, *ft_lstfree) : 0;
			return (1);
		}
	}
	i += ft_lstsize(ms->env);
	if (!(*arr = (char **)ft_memalloc(sizeof(char *) * i)))
		return (1);
	return (0);
}

int		setenv_set(t_minishell *ms)
{
	int		i;
	t_list	*tmp;
	t_list	*start;
	char	**arr;

	i = 1;
	tmp = NULL;
	start = NULL;
	if (setenv_mem_check(ms, &tmp, &start, &arr))
		return (ms_error(1, "setenv: not enough memory\n"));
	setenv_change(ms, start);
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
}

int		ms_setenv(t_minishell *ms)
{
	t_list	*tmp;

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
	if (setenv_error_check(ms) || setenv_set(ms))
	{
		ms->ret = -1;
		return (1);
	}
	return (1);
}
