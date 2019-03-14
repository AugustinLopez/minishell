/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_bonus_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 17:17:30 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/14 13:55:54 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>


int				is_there_active_quote(t_minishell *ms, int option, size_t i)
{
	if ((!option || option == 1) && (ms->input)[i] == '\"')
		option = (!option) ? 1 : 0;
	else if ((!option || option == 2) && (ms->input)[i] == '\'')
		option = (!option) ? 2 : 0;
	return (option);
}

void			case_semicolon(t_minishell *ms, size_t i, size_t j, t_list **elem)
{
	t_list	*tmp;

	if (j == 0)
	{
		((char*)((*elem)->pv))[0] = ';';
		tmp = ft_lstnew(ms->tmp0, (*elem)->zu - i);
		tmp->zu = (*elem)->zu + 2;
		(*elem)->zu = 1;
		(*elem)->next = tmp;
		(*elem) = tmp;
	}
	else if (!(i && ms->input[i - 1] == ' '))
	{
		tmp = ft_lstnew(";", 2);
		tmp->next = ft_lstnew(ms->tmp0, (*elem)->zu - i);
		tmp->next->zu = (*elem)->zu + 2;
		(*elem)->zu = j;
		(*elem)->next = tmp;
		(*elem) = tmp->next;
	}
	else
	{
		((char*)((*elem)->pv))[j] = (ms->input)[i];
		tmp = ft_lstnew(ms->tmp0, (*elem)->zu - i);
		tmp->zu = (*elem)->zu + 2;
		(*elem)->zu = j;
		(*elem)->next = tmp;
		(*elem) = tmp;
	}
	if (!*elem)
		ft_dprintf(2, "Error: not enough memory.\n");
}

int				ms_split(t_minishell *ms)
{
	t_list	*begin;
	t_list	*last;
	t_list	*tmp;
	t_list	*tmp2;
	int option;
	size_t	i;
	size_t	j;
	size_t	all;

	i = 0;
	j = 0;
	option = 0;
	all = ft_strlen(ms->input);
	ms->tmp0 = ft_strnew(all);
	begin = ft_lstnew(ms->tmp0, all + 1);
	begin->zu = all + 2;
	last = begin;
	while ((ms->input[i]))
	{
		option = is_there_active_quote(ms, option, i);
		if (!option && (ms->input)[i] == ';')
		{
			last->zu = all;
			case_semicolon(ms, i, j, &last);
			j = 0;
			i++;
			if (!last)
				break ;
			continue ;
		}
		else if (!option && (ms->input)[i] == '\\')
		{
			((char*)(last->pv))[j++] = (ms->input)[i++];
			((char*)(last->pv))[j++] = (ms->input)[i++];
			continue ;
		}
		if (!option && ft_strchri(" \t\v\r\f", (ms->input)[i]))
		{
			if (last->zu == all + 2)
				last->zu = 0;
			else
			{
				tmp = ft_lstnew(ms->tmp0, all - i);
				tmp->zu = all + 2;
				last->zu = j;
				last->next = tmp;
					tmp2 = last;
				last = tmp;
				j = 0;
				while (ft_strchri(" \t\v\r\f", (ms->input)[i]))
					i++;
				if (!(ms->input)[i])
				{
					ft_lstdelone(&last, &ft_lstfree);
					last = tmp2;
					last->next = 0;
				}
				continue ; 
			}
			while (ft_strchri(" \t\v\r\f", (ms->input)[i]))
				i++;
			continue ;
		}
		(last->zu == all + 2) ? last->zu = 0 : 0;
		((char*)(last->pv))[j++] = (ms->input)[i++];
	}
	tmp = begin;
	while (((char*)(begin->pv))[0] == ';') //might handle this in cmd running mode to reproduce the ';;' behavior with zsh
	{
		tmp = begin->next;
		ft_lstdelone(&begin, *ft_lstfree);
		begin = tmp;
	}
	i = 0;
	ms->all_cmd ? ft_memdel((void**)&(ms->all_cmd)) : 0;
	ms->all_cmd = (char **)ft_memalloc(sizeof(*(ms->all_cmd)) * (ft_lstsize(begin) + 1));
	while (tmp)
	{
		(ms->all_cmd)[i] = (char*)(tmp->pv);
		i++;
		tmp = tmp->next;
		free(begin);
		begin = tmp;
	}
	ms_free(ms, 2);
	return (0);
}
