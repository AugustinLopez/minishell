/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_setenv.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 11:56:17 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:13:02 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline void	setenv_proceed(t_minishell *ms, t_list **start,
									t_list **tmp, t_list **tmpenv)
{
	if (!ms->env)
	{
		ms->env = *start;
		*tmp = (*tmp)->next;
	}
	else if (*tmpenv)
	{
		free((*tmpenv)->pv);
		(*tmpenv)->pv = (*tmp)->pv;
		*start = *tmp;
		(*tmp) = (*tmp)->next;
		(*start)->pv = 0;
		ft_lstdelone(start, *ft_lstfree);
	}
	else
	{
		(*tmpenv) = ms->env;
		while ((*tmpenv)->next)
			*tmpenv = (*tmpenv)->next;
		*start = *tmp;
		*tmp = (*tmp)->next;
		(*start)->next = 0;
		(*tmpenv)->next = *start;
	}
}

static inline int	setenv_change(t_minishell *ms, t_list *start)
{
	t_list	*tmp;
	t_list	*tmpenv;
	char	*s;

	tmp = start;
	while (tmp)
	{
		s = ft_strchr(tmp->pv, '=');
		s[0] = 0;
		tmpenv = ms->env;
		while (tmpenv)
		{
			if (ft_strcmp(tmpenv->pv, tmp->pv) == '=')
				break ;
			tmpenv = tmpenv->next;
		}
		s[0] = '=';
		setenv_proceed(ms, &start, &tmp, &tmpenv);
	}
	return (0);
}

static inline int	setenv_set(t_minishell *ms)
{
	t_list	*tmp;
	t_list	*start;

	tmp = NULL;
	start = NULL;
	if (setenv_mem_check(ms, &tmp, &start))
		return (ms_error(ms, -1, "msh: not enough memory to use setenv\n"));
	setenv_change(ms, start);
	return (0);
}

int					ms_setenv(t_minishell *ms)
{
	char	**s;
	size_t	i;

	ms->ret = 0;
	if (print_env(ms))
		return (1);
	ms->ret = -1;
	if (setenv_error_check(ms))
		return (1);
	if (setenv_set(ms))
		return (-1);
	s = ms->one_cmd;
	i = 0;
	ms->ret = 0;
	while (s[i])
	{
		if (!ft_strncmp(s[i], "HOME=", 5))
			if (load_prompt(ms) == -1)
				return (-1);
		i++;
	}
	return (1);
}
