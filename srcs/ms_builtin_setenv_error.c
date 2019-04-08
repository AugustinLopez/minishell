/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_setenv_error.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 18:04:36 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:30:57 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline int	setenv_error_loop(t_minishell *ms, char *s)
{
	while (*s != '=')
	{
		if (!ft_isalnum(*s) && !(*s == '_'))
		{
			ft_dprintf(2, "setenv: all characters in name must be ");
			return (ms_error(ms, 1, " a number, a letter or an underscore\n"));
		}
		s++;
	}
	return (0);
}

int					setenv_error_check(t_minishell *ms)
{
	int		i;
	char	*s;

	i = 1;
	while (ms->one_cmd[i])
	{
		if (!(s = ft_strchr(ms->one_cmd[i], '=')) || ft_strchr(s + 1, '='))
			return (ms_error(ms, 1, "setenv: format is \'name=value\'\n"));
		if (!(s[1]) || (ms->one_cmd[i][0] == '='))
			return (ms_error(ms, 1, "setenv: name/value cannot be NUL\n"));
		if (ft_isdigit(ms->one_cmd[i][0]))
			return (ms_error(ms, 1, "setenv: name cannot start with [0-9]\n"));
		s = ms->one_cmd[i];
		if (setenv_error_loop(ms, s))
			return (1);
		i++;
	}
	return (0);
}

int					setenv_mem_check(t_minishell *ms, t_list **tmp,
										t_list **start)
{
	int	i;

	i = 0;
	while (ms->one_cmd[++i])
	{
		if (!*tmp)
		{
			*tmp = ft_lstnew(0, 0);
			*start = *tmp;
		}
		else
		{
			(*tmp)->next = ft_lstnew(0, 0);
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
