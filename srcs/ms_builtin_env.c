/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 15:39:50 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/01 11:56:02 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

inline static int env_parsing(t_minishell *ms, int *flags)
{
	int	i;
	
}

int	ms_env(t_minishell *ms)
{
	int		flags;
	t_list	*tmp;

	flags = 0;
	(ms->one_cmd)++;
	if (!(ms->one_cmd))
	{
		temp = ms->env;
		while (temp)
		{
			ft_putendl((char *)(temp->pv));
			temp = temp->next;
		}
	}
/*	if (ms->one_cmd)
		ms_parsing(ms, &flags);
	temp = ms->env;
	while (temp)
	{
		ft_putendl((char *)(temp->pv));
		temp = temp->next;
	}*/
	ms->ret = 0;
	return (1);
}
