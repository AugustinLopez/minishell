/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:07:39 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/01 11:08:17 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_echo(t_minishell *ms)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 1;
	j = 0;
	if (!ft_strcmp((ms->one_cmd)[1], "-n"))
		j = 1;
	i += j;
	while ((ms->one_cmd)[i])
	{
		if ((tmp = ft_strrstr((ms->one_cmd)[i], "\\c")))
			*tmp = 0;
		ft_putstr((ms->one_cmd)[i++]);
		if (tmp)
		{
			*tmp = '\\';
			break ;
		}
		if ((ms->one_cmd)[i])
			write(1, " ", 1);
	}
	!j && !tmp ? write(1, "\n", 1) : 0;
	ms->ret = 0;
	return (1);
}
