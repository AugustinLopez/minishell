/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_setenv_error.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 18:04:36 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/04 18:17:46 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int		setenv_error_loop(char *s)
{
	while (*s != '=')
	{
		if (!ft_isalnum(*s) && !(*s == '_'))
		{
			ft_dprintf(2, "setenv: all characters in name must be ");
			ft_dprintf(2, " a number, a letter or an underscore\n");
			return (1);
		}
		s++;
	}
	return (0);
}

int		setenv_error_check(t_minishell *ms)
{
	int		i;
	char	*s;

	i = 1;
	while (ms->one_cmd[i])
	{
		if (!(s = ft_strchr(ms->one_cmd[i], '=')) || ft_strchr(s + 1, '='))
		{
			ft_dprintf(2, "setenv: argument format is not \'name=value\'\n");
			return (1);
		}
		if (!(s[1]) || (ms->one_cmd[i][0] == '='))
			return (ms_error(1, "setenv: name/value cannot be set as NUL\n"));
		if (ft_isdigit(ms->one_cmd[i][0]))
			return (ms_error(1, "setenv: name cannot start with a number\n"));
		s = ms->one_cmd[i];
		if (setenv_error_loop(s))
			return (1);
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
