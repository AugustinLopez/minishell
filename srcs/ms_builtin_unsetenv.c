/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_unsetenv.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 13:48:46 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/04 18:03:18 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline int	special_case(t_minishell *ms)
{
	size_t	i;

	i = 1;
	ms->ret = 1;
	if (!(ms->one_cmd[i]))
		return (1);
	while (ms->one_cmd[i])
	{
		if (!(ms->one_cmd[i][0]))
			return (ms_error(1, "unsetenv: argument cannot be NUL\n"));
		if (ft_strchr(ms->one_cmd[i], '='))
			return (ms_error(1, "unsetenv: argument cannot contains '='\n"));
		i++;
	}
	ms->ret = 0;
	return (0);
}

static inline int	remove_env_var(t_minishell *ms, t_list **tmp, t_list *tmp2)
{
	int	i;

	i = ft_strcmp((*tmp)->pv, "HOME");
	if (*tmp == ms->env)
	{
		ms->env = ms->env->next;
		ft_lstdelone(tmp, *ft_lstfree);
	}
	else
	{
		tmp2->next = (*tmp)->next;
		ft_lstdelone(tmp, *ft_lstfree);
	}
	if (!i)
		if (load_prompt(ms) == -1)
			return (-1);
	return (0);
}

int					ms_unsetenv(t_minishell *ms)
{
	t_list	*tmp;
	t_list	*tmp2;
	int		i;

	if (special_case(ms))
		return (1);
	i = 1;
	while (ms->one_cmd[i])
	{
		tmp = ms->env;
		while (tmp)
		{
			if (ft_strcmp(tmp->pv, ms->one_cmd[i]) == '=')
				if (remove_env_var(ms, &tmp, tmp2))
					return (-1);
			if (tmp)
			{
				tmp2 = tmp;
				tmp = tmp->next;
			}
		}
		i++;
	}
	return (1);
}
