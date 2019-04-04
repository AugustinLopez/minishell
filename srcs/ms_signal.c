/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_bonus_signal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/04 17:07:22 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** test flag with :
** ls \
** >
** ls "
** dquote>
*/

void	ms_signal_reinitialize(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr("\n");
		show_prompt(g_ms);
		g_ms->flags |= MSF_REINITIALIZE_READER;
	}
}

/*
** test flag with : cat;cat;cat
*/

/*
** Better name needed
*/

void	ms_signal_no_prompt(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr("\n");
		if (!(g_ms->input))
			show_prompt(g_ms);
		g_ms->flags |= MSF_NO_MORE_CMD;
	}
}
