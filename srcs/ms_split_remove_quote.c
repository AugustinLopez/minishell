/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_remove_quote.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:42:47 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 17:45:23 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline void	remove_quote(char **src, char **tmp)
{
	int		quote;
	int		prev;
	size_t	i;
	size_t	j;

	quote = 0;
	prev = 0;
	j = 0;
	i = 0;
	while ((*src)[j])
	{
		quote = quote_value((*src)[j], prev);
		if (prev != quote)
		{
			j++;
			prev = quote;
			continue ;
		}
		prev = quote;
		if (((*src)[j] == '\\')
			&& (!quote || (quote == '\"' && ft_strchr("\"\\", (*src)[j + 1]))))
			j++;
		(*tmp)[i++] = (*src)[j++];
	}
	(*tmp)[i] = 0;
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
