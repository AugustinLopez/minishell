/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/13 18:33:50 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

//Pensez a remettre les pointeurs a 0 une fois free si necessaire

int	ms_error(int ret, char *s)
{
	ft_dprintf(2, "%s", s);
	return (ret);
}

void	ms_free(t_minishell *ms, int option)
{
	if (!option)
	{
		ms->env ? ft_free_sarray(&(ms->env)) : 0;
		ms->curr_path ? ft_memdel((void**)&(ms->curr_path)) : 0;
		ft_gnl(-1, NULL);
	}
	if (option <= 1)
	{
		ms->all_cmd ? ft_free_sarray(&(ms->all_cmd)) : 0;
		ms->input ? ft_memdel((void**)&(ms->input)) : 0;
	}
	ms->tmp0 ? ft_memdel((void**)&(ms->tmp0)) : 0;
	ms->tmp1 ? ft_memdel((void**)&(ms->tmp1)) : 0;
}

void	ms_exit(t_minishell *ms, int exit_status)
{
	ms_free(ms, 0);
	exit(exit_status);
}

/*
** Show prompt can be called by signal_handler
** So i make this function as simple as possible
*/

void	show_prompt(t_minishell *ms)
{
	if ((ms->flags & MSF_SHOW_PATH_HOME) && ms->curr_path)
	{
		ft_putstr(FT_CYAN);
		ft_putstr(FT_BOLD);
		ft_putstr(ms->hostname);
		ft_putstr(">:");
		ft_putstr(FT_EOC);
		ft_putstr(FT_CYAN);
		ft_putstr(ms->curr_path);
		ft_putchar(' ');
		ft_putstr(FT_EOC);
	}
	else
	{
		ft_putstr(FT_CYAN);
		ft_putstr(FT_BOLD);
		ft_putstr(ms->hostname);
		ft_putstr("> ");
		ft_putstr(FT_EOC);
	}
}

void	load_prompt(t_minishell *ms)
{

	char	buff[4096 + 1];
	char	*curr_dir;

	ms->curr_path ? ft_memdel((void**)&(ms->curr_path)) : 0;
	if ((ms->flags & MSF_SHOW_PATH_HOME))
	{
		if (!(curr_dir = getcwd(buff, 4096)))
			ft_dprintf(2, "Error: could not get path to current directory.\n");
		else
			get_home_path(ms, curr_dir, &(ms->curr_path), 0);
	}
}

int		main(int ac, char **av, char **env)
{
	int			i;
	t_minishell	ms;

	signal(SIGINT, ms_signal_no_prompt);
	initialize_env(&ms, ac, av, env);
	while (1)
	{
		ms_free(&ms, 1);
		ms_read(&ms);
		ms_split(&ms);
		i = execute_all_commands(&ms);
		if (ms.flags & MSF_BUILTIN_EXIT)
			break ;
	}
	ms_free(&ms, 0);
	return (0);
}
