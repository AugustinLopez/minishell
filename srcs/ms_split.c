/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 14:54:53 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/03 13:35:00 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
		if ((*src)[j] == '\'' && (ret == 0 || ret == 1))
		{
			ret = (ret == 0) ? 1 : 0;
			j++;
			continue ;
		}
		if ((*src)[j] == '\"' && (ret == 0 || ret == 2))
		{
			ret = (ret == 0) ? 2 : 0;
			j++;
			continue ;
		}
		if (!ret && (*src)[j] == '\\')
			j++;
		if (ret == 2 && (*src)[j] == '\\' && ft_strchr("\"\\", (*src)[j + 1]))
			j++;
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

static inline int	ansi_value(char *str, char **result)
{
	char	*s;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	s = ft_strchr(str, '=');
	if (!s || !(s + 1))
		return (1);
	s++;
	while (s[i])
		if (ft_strchr("\'\"\\~$", str[i++]))
			j++;
	*result = ft_strnew(i + j);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (ft_strchr("\'\"\\~$", str[i]))
			(*result)[j++] = '\\';
		(*result)[j++] = s[i++];
	}
	return (1);
}

static inline int	find_and_replace_dollar(t_minishell *ms)
{
	char	*str;
	char	*s;
	char	*value;
	char	c;
	int		quote;
	size_t	i;
	size_t	j;
	t_list	*tmp;

	quote = 0;
	str = (char *)(ms->elem->pv);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] != '$' && !(quote == '\\'))
		{
			i += 2;
			continue ;
		}
		if (str[i] == '\\' && str[i + 1] == '$' && !(quote == '\''))
		{
			str[i] = 0;
			s = ft_strjoin(str, str + i + 1);
			free(ms->elem->pv);
			ms->elem->pv = s;
			continue ;
		}
		if (str[i] == '\'' && !(quote == '\''))
			quote = (quote == 0) ? '\'' : 0;
		if (str[i] == '\'' && !(quote == '\"'))
			quote = (quote == 0) ? '\"' : 0;
		if (quote == '\'' || str[i] != '$')
		{
			i++;
			continue ;
		}
		j = 1;
		while (ft_isalnum(str[i + j]) || str[i] == '_')
			j++;
		c = str[i + j];
		str[i + j] = 0;
		tmp = ms->env;
		while (tmp)
		{
			if (ft_strcmp(tmp->pv, str + i + 1) == '=')
				break ;
			tmp = tmp->next;
		}
		str[i + j] = c;
		if (!tmp)
		{
			if (j == 1 && c == '?')
				value = ft_itoa(ms->ret);
			else
				value = NULL;
			str[i] = 0;
			if (value)
				s = ft_sprintf("%s%s%s", str, value, str + i + j + 1);
			else
				s = ft_strjoin(str, str + i + j);
			free(ms->elem->pv);
			ms->elem->pv = s;
		}
		else
		{
			str[i] = 0;
			ansi_value(tmp->pv, &value);
			if (quote == '\"')
				s = ft_sprintf("%s\"%s\"%s", str, value, str + i + j);
			else
				s = ft_sprintf("%s%s%s", str, value, str + i + j);
			free(ms->elem->pv);
			ms->elem->pv = s;
		}
	}
	return (1);
}

static inline int	find_and_replace_tilde(t_minishell *ms)
{
	char	*str;
	char	*s;
	int		option;
	size_t	i;

	str = (char *)(ms->elem->pv);
	i = 1;
	if (str[i] == '+')
		option = 1;
	else if (str[i] == '-')
		option = 2;
	else
		option = 0;
	if (option)
		i++;
	if (option == 0)
		s = get_from_env(ms, "HOME=");
	else if (option == 1)
		s = get_from_env(ms, "PWD=");
	else
		s = get_from_env(ms, "OLDPWD=");
	if (!s)
		return (1);
	s = ft_strjoin(s, str + i);
	free(ms->elem->pv);
	ms->elem->pv = s;
	return (1);
}

static inline int	ms_handle_dollar(t_minishell *ms)
{
	ms->elem = ms->cmd;
	while (ms->elem)
	{
		if (ms->elem->pv && ft_strchr(ms->elem->pv, '$'))
			find_and_replace_dollar(ms);
		if (ms->elem->pv && ((char *)(ms->elem->pv))[0] == '~')
			find_and_replace_tilde(ms);
		ms->elem = ms->elem->next;
	}
	return (0);
}

int					ms_split(t_minishell *ms)
{
	ms_free(ms, 2);
	if (ms_split_to_argv(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	ms_free(ms, 2);
	if (ms_handle_dollar(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	if (ms_handle_quoting(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	if (ms_list_to_array(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	ms_free(ms, 2);
	return (0);
}


