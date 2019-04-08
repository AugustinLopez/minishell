/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_replace.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 16:39:15 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 15:50:46 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int					ms_split_protect_characters(char *str, char **result)
{
	char	*s;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	s = ft_strchr(str, '=');
	if (!s || !(s + 1))
		return (0);
	s++;
	while (s[i])
		if (ft_strchr("\'\"\\~$", str[i++]))
			j++;
	if (!(*result = ft_strnew(i + j)))
		return (-1);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (ft_strchr("\'\"\\~$", str[i]))
			(*result)[j++] = '\\';
		(*result)[j++] = s[i++];
	}
	return (0);
}

static inline int	find_and_replace_tilde(t_minishell *ms)
{
	char	*str;
	char	*s;
	size_t	i;

	str = (char *)(ms->elem->pv);
	i = 1;
	if (str[i] == '+')
		s = get_from_env(ms, "PWD=");
	else if (str[i] == '-')
		s = get_from_env(ms, "OLDPWD=");
	else
		s = get_from_env(ms, "HOME=");
	if (!s)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!(s = ft_strjoin(s, str + i)))
		return (-1);
	free(ms->elem->pv);
	ms->elem->pv = s;
	return (0);
}

int					ms_split_replace(t_minishell *ms)
{
	ms_free(ms, 2);
	ms->elem = ms->cmd->next;
	while (ms->elem)
	{
		if (ms->elem->pv && ft_strcmp(ms->elem->pv, "$")
			&& ft_strchr(ms->elem->pv, '$'))
		{
			if (ms_split_replace_dollar(ms))
				return (-1);
		}
		if (ms->elem->pv && ((char *)(ms->elem->pv))[0] == '~')
			if (find_and_replace_tilde(ms))
				return (-1);
		ms->elem = ms->elem->next;
	}
	return (0);
}
