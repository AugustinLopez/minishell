/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_unsetenv.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 13:48:46 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/02 13:05:18 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_unsetenv(t_minishell *ms)
{
	t_list	*tmp;
	t_list	*tmp2;
	int		i;

	ms->ret = 0;
	i = 1;
	if (!(ms->one_cmd[i]))
		return (1);
	while (ms->one_cmd[i])
	{
		if (!(ms->one_cmd[i][0]))
		{
			ms->ret = -1;
			return (ms_error(1, "unsetenv: argument cannot be NUL\n"));
		}
		if (ft_strchr(ms->one_cmd[i], '='))
		{
			ms->ret = -1;
			return (ms_error(1, "unsetenv: argument cannot contains '='\n"));
		}
		i++;
	}
	i = 1;
	while (ms->one_cmd[i])
	{
		tmp = ms->env;
		while (tmp)
		{
			if (ft_strcmp(tmp->pv, ms->one_cmd[i]) == '=')
			{
				if (tmp == ms->env)
				{
					ms->env = ms->env->next;
					ft_lstdelone(&tmp, *ft_lstfree);
				}
				else
				{
					tmp2->next = tmp->next;
					ft_lstdelone(&tmp, *ft_lstfree);
				}
			}
			if (tmp)
			{
				tmp2 = tmp;
				tmp = tmp->next;
			}
		}
		i++;
	}
	//need to modify env_arr
	return (1);
}
