/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_bonus_signal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:55:05 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ms_signal_when_reading(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr("\n");
		show_prompt(g_ms);
		g_ms->flags |= MSF_REINITIALIZE_READER;
	}
}

void	ms_signal_when_executing(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr("\n");
		if (!(g_ms->input))
			show_prompt(g_ms);
		g_ms->flags |= MSF_NO_MORE_CMD;
		g_ms->ret = 130;
	}
}
