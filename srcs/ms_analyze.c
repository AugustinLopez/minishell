/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_analyze.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 16:29:02 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/27 16:30:48 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int			remove_unused_escape_quote(char **src, char **new)
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
	(*new)[i] = 0;
	free(*src);
	*src = *new;
	*new = NULL;
	return (ret);
}

int			ms_execute_all(t_minishell *ms)
{
	size_t	i;
	size_t	j;
	size_t	k;
	int		ret;

	ms_free(ms, 2);
	ret = 0;
	i = 0;
	j = 0;
	ms->one_cmd = ms->all_cmd;
	while ((ms->all_cmd)[i])
	{
		k = ft_strlen(ms->all_cmd[i]);
		if (k && !(ms->tmp0 = ft_strnew(k)))
			return (-1);
		if (k)
			ret = remove_unused_escape_quote(&(ms->all_cmd[i]), &(ms->tmp0));
		if ((ms->all_cmd)[i][0] == ';')
		{
			ms->tmp0 = (ms->all_cmd)[i];
			(ms->all_cmd)[i] = NULL;
			if (j == i)
				ret = 1;
			else
				ret = execute_single_command(ms);
			if (ret < 0)
				break ;
			(ms->all_cmd)[i] = ms->tmp0;
			ms->tmp0 = NULL;
			ms->one_cmd = (ms->all_cmd) + (i + 1);
			j = i + 1;
		}
		i++;
	}
	if (ret >= 0 && j < i)
		ret = execute_single_command(ms);
	return (ret);
}
