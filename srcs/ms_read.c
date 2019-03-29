/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_read.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 11:05:59 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/29 12:48:57 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline int	ms_gnl(t_minishell *ms, char **tmp, int option)
{
	int		j;

	signal(SIGINT, ms_signal_reinitialize);
	option == 0 ? show_prompt(ms) : 0;
	option == 1 ? ft_printf("quote> ") : 0;
	option == 2 ? ft_printf("dquote> ") : 0;
	option == 3 ? ft_printf("> ") : 0;
	j = ft_gnl(0, tmp, 0);
	if (j == -1)
		return (ms_error(1, "Fatal error: could not read from stdin.\n"));
	if (!j)
		return (ms_error(1, "\n"));
	return (0);
}

static inline int	ms_continue_reading(char **input, int previous)
{
	int		option;
	size_t	i;

	option = (previous == 3) ? 0 : previous;
	i = 0;
	while ((*input)[i])
	{
		if ((*input)[i] == '\\' && (*input)[i + 1])
		{
			i++;
			if (option != 1)
				i++;
			continue ;
		}
		if ((!option || option == 1) && (*input)[i] == '\'')
			option = !option ? 1 : 0;
		if ((!option || option == 2) && (*input)[i] == '\"')
			option = !option ? 2 : 0;
		if (!option && (*input)[i] == '\\' && !((*input)[i + 1]))
		{
			(*input)[i] = 0;
			return (3);
		}
		i++;
	}
	return (option);
}

static inline int	ms_read_loop(t_minishell *ms, int option)
{
	int	new_option;

	if (ms->flags & MSF_REINITIALIZE_READER)
	{
		new_option = ms_continue_reading(&(ms->tmp0), 0);
		ms->tmp1 = ft_strdup((ms->tmp0));
		ms->flags &= ~MSF_REINITIALIZE_READER;
	}
	else
	{
		new_option = ms_continue_reading(&(ms->tmp0), option);
		ms->tmp1 = (option != 3) ?
			ft_sprintf("%s\n%s", ms->input, ms->tmp0) :
			ft_strjoin(ms->input, ms->tmp0);
	}
	ms->input ? ft_memdel((void**)&(ms->input)) : 0;
	ms->input = ms->tmp1;
	ms->tmp1 = 0;
	ms_free(ms, 2);
	!(ms->input) ? new_option = -1 : 0;
	return (new_option);
}

int					ms_read(t_minishell *ms)
{
	int	option;

	if (ms_gnl(ms, &(ms->input), 0))
		return (1);
	option = ms_continue_reading(&(ms->input), 0);
	while (option)
	{
		if (ms_gnl(ms, &(ms->tmp0), option))
			return (1);
		option = ms_read_loop(ms, option);
		if (option == -1)
			return (ms_error(1, "Error: not enough memory.\n"));
	}
	return (0);
}
