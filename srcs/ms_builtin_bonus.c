/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 10:37:16 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/23 10:37:17 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int		builtin_msname(t_minishell *ms)
{
	if (ms->flags & MSF_ENV_CMD)
		return (1);
	if (!(ms->one_cmd)[1])
		ft_putendl(ms->hostname);
	else if (ft_strlen((ms->one_cmd)[1]) > HOSTNAME_SIZE)
	{
		ft_dprintf(2, "Hostname cannot be longer than %i characters\n",
			HOSTNAME_SIZE);
		ms->ret = 1;
	}
	else
	{
		ft_bzero(ms->hostname, HOSTNAME_SIZE * sizeof(char));
		ft_strcat(ms->hostname, (ms->one_cmd)[1]);
	}
	return (1);
}

int		builtin_mspath(t_minishell *ms)
{
	if (ms->flags & MSF_ENV_CMD)
		return (1);
	if (!(ms->flags & MSF_SHOW_PATH_HOME))
		ms->flags |= MSF_SHOW_PATH_HOME;
	else
		ms->flags &= ~MSF_SHOW_PATH_HOME;
	if ((load_prompt(ms) == -1))
		return (-1);
	return (1);
}
