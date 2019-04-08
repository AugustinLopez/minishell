/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:03:02 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:08:59 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ms_exit(t_minishell *ms)
{
	int		j;
	size_t	i;

	j = 0;
	if ((ms->one_cmd)[1])
	{
		i = 0;
		while (ms->one_cmd[1][i])
		{
			if (!ft_isdigit(ms->one_cmd[1][i]))
			{
				if (!(!i && (ft_strchr("+-", ms->one_cmd[1][0]))))
				{
					ms_free(ms, 0);
					ft_dprintf(2, "exit: non numeric argument\n");
					exit(1);
				}
			}
			i++;
		}
		j = ft_atoi(ms->one_cmd[1]);
		ms->ret = j;
	}
	ms_free(ms, 0);
	exit(ms->ret);
}
