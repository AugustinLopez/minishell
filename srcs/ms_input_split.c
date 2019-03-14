/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_bonus_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 17:17:30 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/14 18:04:18 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>


int				case_quote(t_minishell *ms, int option, size_t i)
{
	if ((!option || option == 1) && (ms->input)[i] == '\"')
		option = (!option) ? 1 : 0;
	else if ((!option || option == 2) && (ms->input)[i] == '\'')
		option = (!option) ? 2 : 0;
	return (option);
}

int				case_semicolon(t_minishell *ms, size_t *j, size_t *i, t_list **elem)
{
	if (*j)
	{
		(ms->tmp0)[*j] = 0;
		(*elem)->pv = ft_strdup(ms->tmp0);
		(*elem)->zu = *j;
		(*elem)->next = ft_lstnew(0, 0);
		if (!(*elem)->next || !((*elem)->pv))
			return (ms_error(-1, "Error: not enough memory.\n"));
		(*elem) = (*elem)->next;
	}
	(*elem)->pv = ft_strdup(";");
	(*elem)->zu = 1;
	(*elem)->next = ft_lstnew(0, 0);
	if (!(*elem)->next || !((*elem)->pv))
		return (ms_error(-1, "Error: not enough memory.\n"));
	(*elem) = (*elem)->next;
	*j = 0;
	*i += 1;
	return (1);
}

int				case_backslash(t_minishell *ms, size_t *j, size_t *i)
{
	(ms->tmp0)[(*j)++] = (ms->input)[(*i)++];
	return (0);
}

int				case_whitespace(t_minishell *ms, size_t *j, size_t *i, t_list **elem)
{
	if (*j)
	{
		(ms->tmp0)[*j] = 0;
		(*elem)->pv = ft_strdup(ms->tmp0);
		(*elem)->next = ft_lstnew(0, 0);
		if (!((*elem)->pv)  || !((*elem)->next))
			return (ms_error(-1, "Error: not enough memory.\n"));
		(*elem)->zu = *j;
		(*elem) = (*elem)->next;
		*j = 0;
		(ms->tmp0)[*i] = ' ';
	}
	while (ft_strchri(" \t\v\r\f", (ms->input)[*i]))
		(*i)++;
	return  (1);
}

int				ms_split_loop(t_minishell *ms, size_t *i, size_t *j, t_list **elem)
{
	int	option;
	int	conti;

	conti = 0;
	option = 0;
	if (!(ms->tmp0)  || !ms->cmd)
		return (ms_error(-1, "Error: not enough memory.\n"));
	while (ms->input[*i])
	{
		conti = 0;
		if (!(option = case_quote(ms, option, *i)))
		{
			if ((ms->input)[*i] == ';')
				conti = case_semicolon(ms, j, i, elem);
			else if ((ms->input)[*i] == '\\')
				conti = case_backslash(ms, j, i);
			else if (ft_strchri(" \t\v\r\f", (ms->input)[*i]))
				conti = case_whitespace(ms, j, i, elem);
			if (conti == -1)
				return (-1);
			if (conti == 1)
				continue ;
		}
		(ms->tmp0)[(*j)++] = (ms->input)[(*i)++];
	}
	if (*j && !conti)
	{
		(ms->tmp0)[*j] = 0;
		(*elem)->pv = ft_strdup(ms->tmp0);
		if (!((*elem)->pv))
			return (ms_error(-1, "Error: not enough memory.\n"));
		(*elem)->zu = *j;
	}
	return (0);
}

int				ms_split(t_minishell *ms)
{
	size_t	i;
	size_t	j;
	t_list	*elem;

	i = 0;
	j = 0;
	ms->tmp0 = ft_strnew(ft_strlen(ms->input));
	ms->cmd = ft_lstnew(0, 0);
	elem = ms->cmd;
	if ((ms_split_loop(ms, &j, &i, &elem)))
		return (-1);
	i = 0;
	ms->all_cmd = (char **)ft_memalloc(sizeof(*(ms->cmd)) * (ft_lstsize(ms->cmd) + 1));
	elem = ms->cmd;
	while (elem)
	{
		(ms->all_cmd)[i] = (char*)(elem->pv);
		i++;
		elem = elem->next;
		ms->cmd ? ft_memdel((void**)&ms->cmd): 0;
		ms->cmd = elem;
	}
	ms_free(ms, 2);
	return (0);
}
