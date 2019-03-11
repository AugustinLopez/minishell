/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/11 12:31:30 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

int		builtin_msname(t_minishell *ms)
{
	if (!(ms->one_cmd)[1])
		ft_putendl(ms->hostname);
	else
	{
		if (ft_strlen((ms->one_cmd)[1]) > HOSTNAME_SIZE)
			ft_dprintf(2, "Hostname cannot be longer than %i characters.\n", 
				HOSTNAME_SIZE);
		else
		{
			ft_bzero(ms->hostname, HOSTNAME_SIZE * sizeof(char));
			ft_strcat(ms->hostname, (ms->one_cmd)[1]);
		}
	}
	return (1);
}

int		builtin_mspath(t_minishell *ms)
{
	if (!(ms->one_cmd)[1])
	{
		if (!(ms->flags & MSF_SHOW_PATH_HOME))
			ms->flags |= MSF_SHOW_PATH_HOME;
		else
			ms->flags &= ~MSF_SHOW_PATH_HOME;
	}
	else
	{
		if (ft_strchr("yY", (ms->one_cmd)[1][0]))
			ms->flags |= MSF_SHOW_PATH_HOME;
		else
			ms->flags &= ~MSF_SHOW_PATH_HOME;
	}
	return (1);
}
