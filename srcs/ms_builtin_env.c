/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:24:44 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/03 16:25:21 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//env "PATH=X" should update PATH
int		ms_env(t_minishell *ms)
{
	t_list	*tmp;
	t_list	*mem;
	char	**mem2;
	int		ret;

	ms->ret = 0;
	if (print_env(ms))
		return (1);
	mem2 = (ms->one_cmd);
	(ms->one_cmd)++;
	mem = ms->env;
	if (!(ft_strcmp(ms->one_cmd[0], "-i")))
	{
		ms->env = 0;
		(ms->one_cmd)++;
	}
	else if (!(mem == NULL) && !(ms->env = ft_lstmap(mem, *ft_lstccpy)))
	{
		ft_lstdel(&(ms->env), *ft_lstfree);
		ms->env = mem;
		return (ms_error(-1, "minishell: not enough memory to use env\n"));
	}
	tmp = ms->env;
	while (tmp && tmp->next)
		tmp = tmp->next;
	while (ms->one_cmd[0] && ft_strchr(ms->one_cmd[0], '=') && !((ms->one_cmd[0][0] == '=')))
	{
		if (!tmp)
		{
			tmp = ft_lstnew(0,0);
			ms->env = tmp;
		}
		else
		{
			tmp->next = ft_lstnew(0,0);
			tmp = tmp->next;
		}
		if (!tmp || !(tmp->pv = ft_strdup(ms->one_cmd[0])))
		{
			ft_lstdel(&(ms->env), *ft_lstfree);
			ms->env = mem;
			return (ms_error(-1, "minishell: not enough memory to use env\n"));
		}
		tmp->zu = 1;
		(ms->one_cmd)++;
	}
	if (ms->one_cmd[0])
		ret = execute_single_command(ms);
	else
		ret = 1;
	ms->env ? ft_lstdel(&(ms->env), *ft_lstfree) : 0;
	ms->env = mem;
	ms->one_cmd = mem2;
	if (ret < 0)
		return (ret);
	return (1);
}
