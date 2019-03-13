/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_bonus_signal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/13 18:32:55 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void	ms_signal_reinitialize(int signo)
{
	if (signo == SIGINT)
	{
		signal(SIGINT, ms_signal_reinitialize);
		ft_putstr("\n");
		show_prompt(g_ms);
		g_ms->flags |= MSF_REINITIALIZE_READER;
	}
}

void	ms_signal_no_prompt(int signo)
{
	if (signo == SIGINT)
	{
		signal(SIGINT, ms_signal_no_prompt);
		ft_putstr("\n");
		if (!(g_ms->input))
			show_prompt(g_ms);
	}
}
