/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 14:54:53 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/01 19:04:50 by aulopez          ###   ########.fr       */
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
	while (ft_strchri(" \t", (ms->input)[*i]))
		(*i)++;
	return  (1);
}

/*static inline int	case_dollar(t_minishell *ms, size_t *j, size_t *i)
{
	int	k;
	int	l;
	t_list	*tmp;
	char	*str;

	k = *i;
	while (ft_isalnum((ms->input)[*i]) || (ms->input)[*i] == '_')
		(*i)++;
	tmp = ms->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->pv, ms->input + k) == '=')
			break ;
		tmp = tmp->next;
	}
	if (tmp)
	{
		l = ft_strlen(ft_strchr(tmp->pv, '='));
		ft_printf("|%d %d %s|\n", *j, l, ms->tmp0);
		str = ft_strnew(ft_strlen(ms->input) + l);
		str = ft_strcat(ms->tmp0, ft_strchr(tmp->pv, '=') + 1);
		free(ms->tmp0);
		ms->tmp0 = str;
		*j = ft_strlen(ms->tmp0);
	}
	if (!(ms->input[*i]))
		return (1);
	return (0);
}*/

static inline int	ms_parse_input_loop(t_minishell *ms, size_t *i, size_t *j, int *option)
{
	int	special_case;
	
	special_case = 0;
	*option = case_quote(ms, *option, *i);
	/*if (*option < 2 && (ms->input)[*i] == '$')
	{
		(*i)++;
		special_case = case_dollar(ms, j, i);
		ft_printf("%s\n", ms->tmp0);
		return (special_case);
	}*/
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
	/*if (*option == 1 && ms->input[*i] == '\\' && !((ms->input)[*i + 1] == '$'))
		(ms->tmp0)[(*j)++] = (ms->input)[(*i)++];*/
	if (!special_case)
		(ms->tmp0)[(*j)++] = (ms->input)[(*i)++];
	return (special_case);
}

static inline int	ms_parse_input(t_minishell *ms)
{
	int		option;
	int		special_case;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	special_case = 0;
	option = 0;
	ms->elem = ms->cmd;
	while (ms->input[i])
		if ((special_case = ms_parse_input_loop(ms, &i, &j, &option)) == -1)
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

static inline int			remove_quote(char **src, char **new)
{
	int		ret;
	size_t	i;
	size_t	j;

	ret = 0;
	j = 0;
	i = 0;
	while ((*src)[j])
	{
		if (!ret && (*src)[j] == '\\')
			j++;
		else if (ft_strchr("\'\"",(*src)[j]))
		{
			ret = 1;
			j++;
			continue;
		}
		(*new)[i++] = (*src)[j++];
	}
	ret = (ret == 1) || ((*src)[0] == '\\') ? 1 : 0;
	(*new)[i] = 0;
	free(*src);
	*src = *new;
	*new = NULL;
	return (ret);
}

static inline int	ms_handle_quoting(t_minishell *ms)
{
	char	c;

	ms->elem = ms->cmd;
	while (ms->elem)
	{
		if (ms->elem->pv && (((char *)(ms->elem->pv))[0]))
		{
			c = ((char *)(ms->elem->pv))[0];
			if (!(ms->tmp1 = ft_strnew(ft_strlen((char *)(ms->elem->pv)))))
				return (1);
			remove_quote(((char **)&(ms->elem->pv)), &(ms->tmp1));
			ms->elem->zu = c;
		}
		ms->elem = ms->elem->next;
	}
	return (0);
}

/*static inline int	ms_replace_dollar(t_minishell *ms, int i)
{
	char	*s1;
	char	*s2;
	char	*name;
	char	*value;
	t_list	*tmp;

	tmp = ms->env;
	while (tmp)
	{
		
	}
}*/

/*static inline int	ms_handle_dollar(t_minishell *ms)
{
	int	i;
	int	j;
	int	k;
	char	*s;

	ms->elem = ms->cmd;
	while (ms->elem)
	{
		if (ms->elem->pv && (((char *)(ms->elem->pv))[0]) && ms->elem->zu != '\'')
		{
			i = 0;
			k = 0;
			s = (char *)(ms->elem->pv);
			while ((i = ft_strchri(s + k, '$')))
			{
				if (i != 1)
				{
					j = 1;
					while (j < i && ((char *)(ms->elem->pv + i + k - j))[0] == '\\')
						j++;
				ft_printf("%d %d\n", i, j);
					if (!(j % 2))
						ft_printf("Hello\n");
				}
				else
					ft_printf("Hello\n");
				k += i + 1;
			}
		}
		ms->elem = ms->elem->next;
	}
	return (0);
}*/


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
		ms->elem=ms->elem->next;
	}
	return (0);
}


int					ms_split(t_minishell *ms)
{
	ms_free(ms, 2);
	if (!(ms->tmp0 = ft_strnew(ft_strlen(ms->input))))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	if (!(ms->cmd = ft_lstnew(0, 0)))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	if (ms_parse_input(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	ms_free(ms, 2);
	if (ms_handle_quoting(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	/*if (ms_handle_dollar(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));*/
	if (ms_list_to_array(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	ms_free(ms, 2);
	return (0);
}
