/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:24:44 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:20:09 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline int		copy_env(t_minishell *ms, t_list *mem)
{
	if (!(ft_strcmp(ms->one_cmd[0], "-i")))
	{
		ms->env = 0;
		(ms->one_cmd)++;
	}
	else if (!(mem == NULL) && !(ms->env = ft_lstmap(mem, *ft_lstccpy)))
	{
		ft_lstdel(&(ms->env), *ft_lstfree);
		ms->env = mem;
		return (0);
	}
	return (1);
}

static inline int		add_new_env(t_minishell *ms, t_list **tmp)
{
	if (!*tmp)
	{
		*tmp = ft_lstnew(0, 0);
		ms->env = *tmp;
	}
	else
	{
		(*tmp)->next = ft_lstnew(0, 0);
		*tmp = (*tmp)->next;
	}
	if (!*tmp || !((*tmp)->pv = ft_strdup(ms->one_cmd[0])))
		return (0);
	(*tmp)->zu = 1;
	return (1);
}

static inline int		replace_old_env(t_minishell *ms)
{
	t_list	*tmp;
	char	*s;

	tmp = ms->env;
	while (tmp)
	{
		s = ft_strchr(tmp->pv, '=');
		s[0] = 0;
		if (ft_strcmp(ms->one_cmd[0], tmp->pv) == '=')
		{
			free(tmp->pv);
			if (!(tmp->pv = ft_strdup(ms->one_cmd[0])))
				return (-1);
			tmp->zu = 1;
			return (0);
		}
		s[0] = '=';
		tmp = tmp->next;
	}
	return (1);
}

static inline int		update_env(t_minishell *ms, t_list **mem)
{
	t_list	*tmp;
	int		i;

	tmp = ms->env;
	while (tmp && tmp->next)
		tmp = tmp->next;
	while (ms->one_cmd[0] && ft_strchr(ms->one_cmd[0], '=')
		&& !((ms->one_cmd[0][0] == '=')))
	{
		i = replace_old_env(ms);
		if (i && (i == -1 || !add_new_env(ms, &tmp)))
		{
			ft_lstdel(&(ms->env), *ft_lstfree);
			ms->env = *mem;
			return (0);
		}
		(ms->one_cmd)++;
	}
	return (1);
}

int						ms_env(t_minishell *ms)
{
	t_list	*mem;
	char	**mem2;
	int		ret;

	mem = ms->env;
	mem2 = ms->one_cmd;
	if (print_env(ms))
		return (1);
	(ms->one_cmd)++;
	if (!copy_env(ms, mem) || !update_env(ms, &mem))
		return (ms_error(ms, -1, "msh: not enough memory to use env\n"));
	if (ms->one_cmd[0])
		ret = execute_single_command(ms);
	else
		ret = 1;
	ms->env ? ft_lstdel(&(ms->env), *ft_lstfree) : 0;
	ms->env = mem;
	if (load_prompt(ms) == -1)
		return (-1);
	ms->one_cmd = mem2;
	if (ret < 0)
		return (ret);
	return (1);
}
