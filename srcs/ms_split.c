/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_bonus_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 17:17:30 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/27 16:31:59 by aulopez          ###   ########.fr       */
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
	ms->elem->zu = 1;
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
		if (!(ms->elem->pv)  || !(ms->elem->next))
			return (-1);
		ms->elem->zu = *j;
		ms->elem = ms->elem->next;
		*j = 0;
		(ms->tmp0)[*i] = ' ';
	}
	while (ft_strchri(" \t\v\r\f", (ms->input)[*i]))
		(*i)++;
	return  (1);
}

static inline int	ms_parse_input(t_minishell *ms, size_t *i, size_t *j, int *option)
{
	int	special_case;
	
	special_case = 0;
	if (!(*option = case_quote(ms, *option, *i)))
	{
		if ((ms->input)[*i] == ';')
			special_case = case_semicolon(ms, j, i);
		else if ((ms->input)[*i] == '\\')
		{
			(ms->tmp0)[(*j)++] = (ms->input)[(*i)++];
			special_case = !(ms->input)[(*i)] ? 1 : 0;
		}
		else if (ft_strchri(" \t\v\r\f", (ms->input)[*i]))
			special_case = case_whitespace(ms, j, i);
		if (special_case == -1)
			return (-1);
	}
	if (!special_case)
		(ms->tmp0)[(*j)++] = (ms->input)[(*i)++];
	return (special_case);
}

static inline int	ms_iterate_over_input(t_minishell *ms)
{
	int	option;
	int	special_case;
	size_t	i;
	size_t	j;

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
		ms->elem->zu = j;
	}
	return (0);
}

static inline int	ms_list_to_array(t_minishell *ms)
{
	size_t	i;
	char **tmp;

	tmp = (char **)ft_memalloc(sizeof(char *) * (ft_lstsize(ms->cmd) + 1));
	if (!tmp)
		return (1);
	ms->all_cmd = tmp;
	ms->elem = ms->cmd;
	i = 0;
	while (ms->elem)
	{
		(ms->all_cmd)[i] = (char*)(ms->elem->pv);
		i++;
		ms->elem = ms->cmd;
		ms->cmd = ms->cmd->next;
		ms->elem ? ft_memdel((void**)&ms->elem): 0;
		ms->elem = ms->cmd;
	}
	return (0);
}

/*
** put alias after split_loop and before list_to_array. Careful: may contain backslash, quote, etc
*/

int						ms_split(t_minishell *ms)
{
	ms_free(ms, 2);
	if (!(ms->tmp0 = ft_strnew(ft_strlen(ms->input))))
		return (ms_error(1, "Error: not enough memory to parse command.\n"));
	if (!(ms->cmd = ft_lstnew(0, 0)))
		return (ms_error(1, "Error: not enough memory to parse command.\n"));
	if ((ms_iterate_over_input(ms)))
		return (ms_error(1, "Error: not enough memory to parse command.\n"));
	if (ms_list_to_array(ms))
		return (ms_error(1, "Error: not enough memory to parse command.\n"));
	ms_free(ms, 2);
	return (0);
}
