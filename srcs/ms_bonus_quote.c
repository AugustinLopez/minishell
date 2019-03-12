/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_bonus_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 17:17:30 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/12 13:32:59 by aulopez          ###   ########.fr       */
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
		tmp2 = !backsl ? ft_sprintf("%s\n%s", ms->input, tmp) :
			ft_strjoin(ms->input, tmp);
		ms->input ? free(ms->input) : 0;
		ms->input = tmp2;
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
		free(tmp);
	}
	return (0);
}

int				split_ms(t_minishell *ms)
{
	t_list	*begin;
	t_list	*last;
	t_list	*tmp;
	char	*zero;
	int quote;
	int dquote;
	size_t	i;
	size_t	j;
	size_t	all;

	quote = 0;
	dquote = 0;
	i = 0;
	j = 0;
	all = ft_strlen(ms->input);
	zero = ft_strnew(all);
	begin = ft_lstnew(zero, all + 1);
	last = begin;
	while ((ms->input[i]))
	{
		if (!quote && (ms->input)[i] == '\"')
			dquote = (!dquote) ? 1 : 0;
		else if (!dquote && (ms->input)[i] == '\'')
			quote = (!quote) ? 1 : 0;
		else if (!quote && !dquote && (ms->input)[i] == ';')
		{
			if (!(i && ms->input[i - 1] == ' '))
			{
				tmp = ft_lstnew(";", 2);
				tmp->next = ft_lstnew(zero, all - i++);
				tmp->next->zu = all + 2;
				last->zu = j;
				last->next = tmp;
				last = tmp->next;
			}
			else
			{
				((char*)(last->pv))[j] = (ms->input)[i];
				tmp = ft_lstnew(zero, all - i++);
				tmp->zu = all + 2;
				last->zu = j;
				last->next = tmp;
				last = tmp;
			}
			j = 0;
		}
		else if (!quote && !dquote && (ms->input)[i] == '\\')
		{
			((char*)(last->pv))[j++] = (ms->input)[i++];
			((char*)(last->pv))[j++] = (ms->input)[i++];

			continue ;
		}
		if (!quote && !dquote && (ms->input)[i] == ' ')
		{
			if (last->zu == all + 2)
				last->zu = 0;
			else
			{
				tmp = ft_lstnew(zero, all - i);
				last->zu = j;
				last->next = tmp;
				last = tmp;
				j = 0;
			}
			while (ms->input[i] == ' ')
				i++;
			continue ;
		}
		(last->zu == all + 2) ? last->zu = 0 : 0;
		((char*)(last->pv))[j++] = (ms->input)[i++];
	}
	tmp = begin;
	i = 0;
	ms->all_cmd = (char **)ft_memalloc(sizeof(*(ms->all_cmd)) * (ft_lstsize(begin) + 1));
	while (tmp)
	{
		(ms->all_cmd)[i] = (char*)(tmp->pv);
		i++;
		tmp = tmp->next;
		free(begin);
		begin = tmp;
	}
	return (0);
}

void			ms_inputsplit(t_minishell *ms)
{

/*	char	*p;
	
	p = ft_strdup(ms->input);*/
	read_if_necessary(ms);
	split_ms(ms);
	/*ft_printf("1.%s\nEnd.\n", ms->input);
	free(ms->input);
	ms->input = p;
	(void)p;
	(void)ms;*/
}
