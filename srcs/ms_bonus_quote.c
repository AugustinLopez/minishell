/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_bonus_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 17:17:30 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/11 19:01:35 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

int			read_if_necessary(t_minishell *ms)
{
	size_t	i;
	int		j;
	int		quote;
	int		dquote;
	int		backsl;
	char	*tmp;
	char	*tmp2;

	if (!ms->input)
		return (0);
	i = 0;
	quote = 0;
	dquote = 0;
	backsl = 0;
	while ((ms->input)[i])
	{
		if ((ms->input)[i] == '\\' && (ms->input)[i + 1])
		{
			i+= 2;
			continue ;
		}
		if (!dquote && !backsl && (ms->input)[i] == '\'')
			quote = (!quote) ? 1 : 0;
		if (!quote && !backsl && (ms->input)[i] == '\"')
			dquote = (!dquote) ? 1 : 0;
		if (!quote && !dquote && (ms->input)[i] == '\\' && !((ms->input)[i + 1]))
			backsl = 1;
		i++;
	}
	backsl ? ms->input[i - 1] = '\0' : 0;
	while (dquote || quote || backsl)
	{
		quote ? ft_printf("quote> ") : 0;
		dquote ? ft_printf("dquote> ") : 0;
		backsl ? ft_printf("> ") : 0;
		j = ft_gnl(0, &tmp);
		if (j == -1)
		{
			ft_dprintf(2, "Error: could not read stdin.\n");
			ms_exit(ms, EXIT_FAILURE);
		}
		if (ft_iswhitespace(tmp, 1) && backsl)
		{
			j == 1 && tmp ? free(tmp) : 0;
			j == 0 ? write(1, "\n", 1) : 0;
			if (!j)
				ms_exit(ms, 0);
			continue ;
		}
		ft_printf("2.%s\n", tmp);
		tmp2 = !backsl ? ft_str3join(ms->input, "\n", tmp) : ft_strjoin(ms->input, tmp);
		ms->input ? free(ms->input) : 0;
		ms->input = tmp2;
		ft_printf("3.%s\n", ms->input);
		backsl = 0;
		i = 0;
		while (tmp[i])
		{
			if (tmp[i] == '\\' && tmp[i + 1])
			{
				i+= 2;
				continue ;
			}
			if (!dquote && !backsl && tmp[i] == '\'')
				quote = (!quote) ? 1 : 0;
			if (!quote && !backsl && tmp[i] == '\"')
				dquote = (!dquote) ? 1 : 0;
			if (!quote && !dquote && tmp[i] == '\\' && !(tmp[i + 1]))
				backsl = 1;
			i++;
		}
		ft_printf("%i %i %i\n", dquote, quote, backsl);
		free(tmp);
	}
	return (0);
}

int				split_ms(t_minishell *ms)
{
	//if " or ' : copy all until next " or '
	//if / not in quote or ', copy next
	return (0);
}

void			ms_inputsplit(t_minishell *ms)
{

	char	*p;
	
	/*p = ft_strdup(ms->input);
	read_if_necessary(ms);
	split_ms(ms);
	ft_printf("1.%s\nEnd.\n", ms->input);
	free(ms->input);
	ms->input = p;*/
	(void)p;
	(void)ms;
}
