/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_setenv.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 11:56:17 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/03 16:11:57 by aulopez          ###   ########.fr       */
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
		if (!(s[1]) || (ms->one_cmd[i][0] == '='))
			return (ms_error(1, "setenv: name/value cannot be set as NUL\n"));
		if (ft_isdigit(ms->one_cmd[i][0]))
			return (ms_error(1, "setenv: name cannot start with a number\n"));
		s = ms->one_cmd[i];
		while (*s != '=')
		{
			if (!ft_isalnum(*s) && !(*s == '_'))
			{
				ft_dprintf(2,"setenv: all characters in name must be");
				return (ms_error(1, " a number, a letter or an underscore\n"));
			}
			s++;
		}
		i++;
	}
	return (0);
}

int		setenv_mem_check(t_minishell *ms, t_list **tmp, t_list **start)
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
	return (0);
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
			tmpenv = ms->env;
			while (tmpenv->next)
				tmpenv = tmpenv->next;
			start = tmp;
			tmp = tmp->next;
			start->next = 0;
			tmpenv->next = start;
		}
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
		return (ms_error(1, "setenv: not enough memory\n"));
	setenv_change(ms, start);
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
