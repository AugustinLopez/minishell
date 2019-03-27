/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/27 16:28:39 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int		ms_error(int ret, char *s)
{
	ft_dprintf(2, "%s", s);
	return (ret);
}

void	ms_exit(t_minishell *ms, int exit_status)
{
	ms_free(ms, 0);
	exit(exit_status);
}

/*
** option == 0 : free everything
** option == 1 : free current command cycle
** option == 2 : free temp variable
*/

void	ms_free(t_minishell *ms, int option)
{
	if (!option)
	{
		ms->env ? ft_lstdel(&(ms->env), *ft_lstfree) : 0;
		ms->arr_env ? ft_memdel((void**)&(ms->arr_env)) : 0;
		ms->curr_path ? ft_memdel((void**)&(ms->curr_path)) : 0;
		ft_gnl(-1, NULL, 0);
	}
	if (option <= 1)
	{
		ms->all_cmd ? ft_free_sarray(&(ms->all_cmd)) : 0;
		ms->input ? ft_memdel((void**)&(ms->input)) : 0;
		ms->cmd ? ft_lstdel(&(ms->cmd), *ft_lstfree) : 0;
		ms->elem ? ft_lstdel(&(ms->elem), *ft_lstfree) : 0;
	}
	ms->tmp0 ? ft_memdel((void**)&(ms->tmp0)) : 0;
	ms->tmp1 ? ft_memdel((void**)&(ms->tmp1)) : 0;
}

int		main(int ac, char **av, char **env)
{
	int			i;
	int			err;
	t_minishell	ms;

	signal(SIGINT, ms_signal_no_prompt);
	err = 0;
	if ((err = ms_initialize(&ms, ac, av, env)))
	{
		ms_free(&ms, 0);
		return (err);
	}
	while (!err)
	{
		ms_free(&ms, 1);
		if ((err = ms_read(&ms)))
			break ;
		if ((err = ms_split(&ms)))
			break ;
		ms.flags &= ~MSF_NO_MORE_CMD;
		if (ms.all_cmd)
			i = ms_execute_all(&ms);
		if (ms.flags & MSF_BUILTIN_EXIT)
			break ;
	}
	ms_free(&ms, 0);
	return (err);
}
