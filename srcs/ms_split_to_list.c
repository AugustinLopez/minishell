/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_in_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 11:15:53 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/04 17:13:23 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline int	case_quote(t_minishell *ms, int option, size_t i)
{
	if ((!option || option == 1) && (ms->input)[i] == '\"')
		option = (!option) ? 1 : 0;
	else if ((!option || option == 2) && (ms->input)[i] == '\'')
		option = (!option) ? 2 : 0;
	return (option);
}

static inline int	case_semicolon(t_minishell *ms, size_t *j, size_t *i)
{
	if (*j)
	{
		(ms->tmp0)[*j] = 0;
		ms->elem->pv = ft_strdup(ms->tmp0);
		ms->elem->zu = *j;
		ms->elem->next = ft_lstnew(0, 0);
		if (!(ms->elem->next) || !((ms->elem->pv)))
			return (-1);
		ms->elem = ms->elem->next;
	}
	ms->elem->pv = ft_strdup(";");
	ms->elem->zu = ';';
	ms->elem->next = ft_lstnew(0, 0);
	if (!(ms->elem->next) || !((ms->elem->pv)))
		return (-1);
	ms->elem = ms->elem->next;
	*j = 0;
	*i += 1;
	return (1);
}

static inline int	case_whitespace(t_minishell *ms, size_t *j, size_t *i)
{
	if (*j)
	{
		(ms->tmp0)[*j] = 0;
		ms->elem->pv = ft_strdup(ms->tmp0);
		ms->elem->next = ft_lstnew(0, 0);
		if (!(ms->elem->pv) || !(ms->elem->next))
			return (-1);
		ms->elem->zu = 1;
		ms->elem = ms->elem->next;
		*j = 0;
		(ms->tmp0)[*i] = ' ';
	}
	while (ft_strchri(" \t", (ms->input)[*i]))
		(*i)++;
	return (1);
}

static inline int	ms_parse_input(t_minishell *ms, size_t *i, size_t *j,
									int *option)
{
	int	special_case;

	special_case = 0;
	*option = case_quote(ms, *option, *i);
	if (!*option)
	{
		if ((ms->input)[*i] == ';')
			special_case = case_semicolon(ms, j, i);
		else if ((ms->input)[*i] == '\\')
		{
			(ms->tmp0)[(*j)++] = (ms->input)[(*i)++];
			special_case = !(ms->input)[(*i)] ? 1 : 0;
		}
		else if (ft_strchri(" \t", (ms->input)[*i]))
			special_case = case_whitespace(ms, j, i);
		if (special_case == -1)
			return (-1);
	}
	if (!special_case)
		(ms->tmp0)[(*j)++] = (ms->input)[(*i)++];
	return (special_case);
}

int					ms_split_to_list(t_minishell *ms)
{
	int		option;
	int		special_case;
	size_t	i;
	size_t	j;

	ms_free(ms, 2);
	ms->tmp0 = ft_strnew(ft_strlen(ms->input));
	if (!ms->tmp0 || !(ms->cmd = ft_lstnew(0, 0)))
		return (1);
	i = 0;
	j = 0;
	special_case = 0;
	option = 0;
	ms->elem = ms->cmd;
	while (ms->input[i])
		if ((special_case = ms_parse_input(ms, &i, &j, &option)) == -1)
			return (1);
	if (j && !special_case)
	{
		(ms->tmp0)[j] = 0;
		ms->elem->pv = ft_strdup(ms->tmp0);
		if (!(ms->elem->pv))
			return (1);
	}
	return (0);
}
