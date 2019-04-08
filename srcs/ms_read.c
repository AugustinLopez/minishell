/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_read.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 11:05:59 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:18:15 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int					quote_value(char c, int quote)
{
	if (c == '\'' && !(quote == '\"'))
		quote = (quote == 0) ? '\'' : 0;
	else if (c == '\"' && !(quote == '\''))
		quote = (quote == 0) ? '\"' : 0;
	return (quote);
}

static inline int	ms_gnl(t_minishell *ms, char **tmp, int quote)
{
	int		j;

	signal(SIGINT, ms_signal_when_reading);
	quote == 0 ? show_prompt(ms) : 0;
	quote == '\'' ? ft_printf("quote> ") : 0;
	quote == '\"' ? ft_printf("dquote> ") : 0;
	quote == '\\' ? ft_printf("> ") : 0;
	j = ft_gnl(0, tmp, 0);
	if (j == -1)
		return (ms_error(ms, -1, "msh: input contains null bytes.\n"));
	if (!j)
	{
		ft_printf("\n");
		ms->flags |= MSF_EOF;
		return (1);
	}
	return (0);
}

static inline int	ms_continue_reading(char **input, int previous)
{
	int		quote;
	size_t	i;

	quote = (previous == '\\') ? 0 : previous;
	i = 0;
	while ((*input)[i])
	{
		if ((*input)[i] == '\\' && (*input)[i + 1])
		{
			i++;
			i += (quote != '\'') ? 1 : 0;
			continue ;
		}
		quote = quote_value((*input)[i], quote);
		if (!quote && (*input)[i] == '\\' && !((*input)[i + 1]))
		{
			(*input)[i] = 0;
			return ('\\');
		}
		i++;
	}
	return (quote);
}

static inline int	ms_read_loop(t_minishell *ms, int quote)
{
	int	new_quote;

	if (ms->flags & MSF_REINITIALIZE_READER)
	{
		new_quote = ms_continue_reading(&(ms->tmp0), 0);
		ms->tmp1 = ft_strdup((ms->tmp0));
		ms->flags &= ~MSF_REINITIALIZE_READER;
	}
	else
	{
		new_quote = ms_continue_reading(&(ms->tmp0), quote);
		ms->tmp1 = (quote != '\\') ?
			ft_sprintf("%s\n%s", ms->input, ms->tmp0) :
			ft_strjoin(ms->input, ms->tmp0);
	}
	ms->input ? ft_memdel((void**)&(ms->input)) : 0;
	ms->input = ms->tmp1;
	ms->tmp1 = 0;
	!(ms->input) ? new_quote = -1 : 0;
	return (new_quote);
}

int					ms_read(t_minishell *ms)
{
	int	quote;

	if (ms_gnl(ms, &(ms->input), 0))
		return (1);
	quote = ms_continue_reading(&(ms->input), 0);
	while (quote)
	{
		ms_free(ms, 2);
		if (ms_gnl(ms, &(ms->tmp0), quote))
			return (1);
		quote = ms_read_loop(ms, quote);
		if (quote == -1)
			return (ms_error(ms, -1, "msh: not enough memory to read.\n"));
	}
	return (0);
}
