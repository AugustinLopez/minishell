/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_setenv.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 11:56:17 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/04 18:12:27 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	setenv_proceed(t_minishell *ms, t_list **start, t_list **tmp,
						t_list **tmpenv)
{
	if (*tmpenv)
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

int		setenv_change(t_minishell *ms, t_list *start)
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

int		setenv_set(t_minishell *ms)
{
	int		i;
	t_list	*tmp;
	t_list	*start;

	i = 1;
	tmp = NULL;
	start = NULL;
	if (setenv_mem_check(ms, &tmp, &start))
		return (ms_error(-1, "minishell: not enough memory to use setenv\n"));
	setenv_change(ms, start);
	return (0);
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

int		ms_setenv(t_minishell *ms)
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
