/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 14:54:53 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/03 19:04:10 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
	if (!(*result = ft_strnew(i + j)))
		return (0);
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

static inline int	no_replace(t_minishell *ms, int quote, size_t *i)
{
	char	*str;
	char	*tmp;

	str = (char *)(ms->elem->pv);
	if (quote == '\'' || (str[*i] != '$' && str[*i] != '\\'))
	{
		(*i)++;
		return (1);
	}
	if (str[*i] == '\\')
	{
		if (str[*i + 1] == '$')
		{
			str[*i] = 0;
			if (!(tmp = ft_strjoin(str, str + *i + 1)))
				return (2);
			free(ms->elem->pv);
			ms->elem->pv = tmp;
			return (1);
		}
		(*i) += 2;
		return (1);
	}
	return (0);
}

static inline t_list	*find_replacement(t_list *env, char *str, size_t *j)
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
	if (str[i + 1] == '?')
	{
		if (!(val = ft_itoa(ms->ret)))
			return (1);
	}
	else
		val = NULL;
	str[i] = 0;
	if (val)
		tmp = ft_sprintf("%s%s%s", str, val, str + i + 2);
	else
		tmp = ft_strjoin(str, str + i + j);
	if (val)
		free(val);
	if (!tmp)
		return (1);
	ft_memdel((void **)&(ms->elem->pv));
	ms->elem->pv = tmp;
	return (0);
}

static inline int	replacement(t_minishell *ms, size_t i, size_t j, int quote)
{
	char	*str;
	char	*tmp;
	char	*val;

	str = (char *)(ms->elem->pv);
	str[i] = 0;
	if (!ansi_value(ms->tmp0, &val))
	{
		ms->tmp0 = 0;
		return (1);
	}
	if (quote == '\"')
		tmp = ft_sprintf("%s\"%s\"%s", str, val, str + i + j);
	else
		tmp = ft_sprintf("%s%s%s", str, val, str + i + j);
	if (val)
		free(val);
	ft_memdel((void **)&(ms->elem->pv));
	ms->elem->pv = tmp;
	ms->tmp0 = 0;
	return (0);
}

static inline int	find_and_replace_dollar(t_minishell *ms)
{
	char	*str;
	int		quote;
	size_t	i;
	size_t	j;
	t_list	*tmp;

	quote = 0;
	str = (char *)(ms->elem->pv);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' && !(quote == '\''))
			quote = (quote == 0) ? '\"' : 0;
		if (str[i] == '\'' && !(quote == '\"'))
			quote = (quote == 0) ? '\'' : 0;
		if ((j = no_replace(ms, quote, &i)))
		{
			str = (char *)(ms->elem->pv);
			if (j == 2)
				return (1);
			continue ;
		}
			str = (char *)(ms->elem->pv);
		tmp = find_replacement(ms->env, str + i, &j);
		if (!tmp)
		{
			if (no_replacement(ms, i, j))
				return (1);
		}
		else
		{
			ms->tmp0 = tmp->pv;
			if (replacement(ms, i, j, quote))
				return (1);
		}
		str = (char *)(ms->elem->pv);
	}
	return (0);
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
		return (0);
	if (!(s = ft_strjoin(s, str + i)))
		return (1);
	free(ms->elem->pv);
	ms->elem->pv = s;
	return (0);
}

static inline int	ms_handle_dollar(t_minishell *ms)
{
	ms_free(ms, 2);
	ms->elem = ms->cmd->next;
	while (ms->elem)
	{
		if (ms->elem->pv
			&& ft_strcmp(ms->elem->pv, "$") && ft_strchr(ms->elem->pv, '$'))
		{
			if (find_and_replace_dollar(ms))
				return (1);
		}
		if (ms->elem->pv && ((char *)(ms->elem->pv))[0] == '~')
			if (find_and_replace_tilde(ms))
				return (1);
		ms->elem = ms->elem->next;
	}
	return (0);
}

int					ms_split(t_minishell *ms)
{
	if (ms_split_to_list(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	if (ms_handle_dollar(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	if (ms_split_remove_quote(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	if (ms_list_to_array(ms))
		return (ms_error(1, "minishell: not enough memory to parse input.\n"));
	ms_free(ms, 2);
	return (0);
}
