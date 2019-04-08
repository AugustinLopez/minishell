/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_replace_dollar.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 15:38:22 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 16:10:56 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline int		no_active_dollar(t_minishell *ms, int quote, size_t *i)
{
	char	*str;
	char	*tmp;

	str = (char *)(ms->elem->pv);
	if (quote == '\'' || (str[*i] != '$' && str[*i] != '\\')
		|| (str[*i] == '$' && ft_strchr(" \\\t", str[*i + 1])))
	{
		(*i)++;
		return (1);
	}
	if (str[*i] == '\\')
	{
		if (str[*i + 1] == '$')
		{
			str[(*i)++] = 0;
			if (!(tmp = ft_strjoin(str, str + *i)))
				return (-1);
			free(ms->elem->pv);
			ms->elem->pv = tmp;
			return (1);
		}
		(*i) += 2;
		return (1);
	}
	return (0);
}

static inline t_list	*find_env(t_list *env, char *str, size_t *j)
{
	char	c;
	t_list	*tmp;

	*j = 1;
	while (ft_isalnum(str[*j]) || str[*j] == '_')
		(*j)++;
	c = str[*j];
	str[*j] = 0;
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->pv, str + 1) == '=')
			break ;
		tmp = tmp->next;
	}
	str[*j] = c;
	return (tmp);
}

static inline int		no_replacement(t_minishell *ms, size_t i, size_t j)
{
	char	*str;
	char	*tmp;
	char	*val;

	str = (char *)(ms->elem->pv);
	val = NULL;
	if (str[i + 1] == '?' && (!(val = ft_itoa(ms->ret))))
		return (-1);
	if (str[i + 1] == '0' && (!(val = ft_strdup("-minishell"))))
		return (-1);
	str[i] = 0;
	if (val)
		tmp = ft_sprintf("%s%s%s", str, val, str + i + 2);
	else if (ft_strchr("~*=#@-!", str[i + 1]))
		tmp = ft_strjoin(str, str + 2);
	else
		tmp = ft_strjoin(str, str + i + j);
	if (val)
		free(val);
	if (!tmp)
		return (-1);
	ft_memdel((void **)&(ms->elem->pv));
	ms->elem->pv = tmp;
	return (0);
}

static inline int		replacement(t_minishell *ms, size_t i, size_t j,
									int quote)
{
	char	*str;
	char	*tmp;
	char	*val;
	char	*safe;

	str = (char *)(ms->elem->pv);
	safe = ms->tmp0;
	ms->tmp0 = 0;
	str[i] = 0;
	if (ms_split_protect_characters(safe, &val))
		return (-1);
	if (quote == '\"')
		tmp = ft_sprintf("%s\"%s\"%s", str, val, str + i + j);
	else
		tmp = ft_sprintf("%s%s%s", str, val, str + i + j);
	if (val)
		free(val);
	ft_memdel((void **)&(ms->elem->pv));
	ms->elem->pv = tmp;
	return (0);
}

int						ms_split_replace_dollar(t_minishell *ms)
{
	int		quote;
	size_t	i;
	size_t	j;
	t_list	*tmp;
	int		ret;

	quote = 0;
	i = 0;
	while (((char *)(ms->elem->pv))[i])
	{
		quote = quote_value(((char *)(ms->elem->pv))[i], quote);
		if ((ret = no_active_dollar(ms, quote, &i)))
		{
			if (ret == -1)
				return (-1);
			continue ;
		}
		if ((tmp = find_env(ms->env, (char *)(ms->elem->pv) + i, &j)))
			ms->tmp0 = tmp->pv;
		if ((tmp && replacement(ms, i, j, quote))
			|| (!tmp && no_replacement(ms, i, j)))
			return (-1);
	}
	return (0);
}
