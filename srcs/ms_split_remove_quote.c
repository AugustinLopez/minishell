/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_remove_quote.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:42:47 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/04 17:22:40 by aulopez          ###   ########.fr       */
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

static inline int	remove_quote(char **src, char **tmp)
{
	int		quote;
	size_t	i;
	size_t	j;

	quote = 0;
	j = 0;
	i = 0;
	while ((*src)[j])
	{
		quote = quote_value((*src)[j], quote);
		if (ft_strchr("\"\'", (*src)[j]))
		{
			j++;
			continue ;
		}
		if (((*src)[j] == '\\')
			&& (!quote || (quote == '\"' && ft_strchr("\"\\", (*src)[j + 1]))))
			j++;
		(*tmp)[i++] = (*src)[j++];
	}
	(*tmp)[i] = 0;
	return (i);
}

int					ms_split_remove_quote(t_minishell *ms)
{
	char	*src;
	char	*tmp;
	t_list	*elem;

	elem = ms->cmd;
	while (elem)
	{
		src = (char *)(elem->pv);
		if (src && *src)
		{
			if (!(tmp = ft_strnew(ft_strlen(src))))
				return (1);
			remove_quote(&src, &tmp);
			free(elem->pv);
			elem->pv = tmp;
		}
		elem = elem->next;
	}
	return (0);
}
