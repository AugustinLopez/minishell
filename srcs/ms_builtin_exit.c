/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:03:02 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/16 10:20:42 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int		ms_exit(t_minishell *ms)
{
	size_t	i;

	ft_putendl("exit");
	if ((ms->one_cmd)[1])
	{
		if (ms->one_cmd[2])
			return (ms_error(ms, 1, "exit: too many arguments\n"));
		i = 0;
		while (ms->one_cmd[1][i])
		{
			if (!ft_isdigit(ms->one_cmd[1][i])
				&& (!(!i && (ft_strchr("+-", ms->one_cmd[1][0])))))
			{
				ms_free(ms, 0);
				ft_dprintf(2, "exit: %s: numeric argument required\n",
					ms->one_cmd[1]);
				exit(1);
			}
			i++;
		}
		ms->ret = ft_atoi(ms->one_cmd[1]);
	}
	ms_free(ms, 0);
	exit(ms->ret);
	return (0);
}
