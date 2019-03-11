/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/11 12:38:54 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

int	ms_error(int ret, char *s)
{
	ft_dprintf(2, "%s", s);
	return (ret);
}

void	ms_exit(t_minishell *ms, int exit_status)
{
	ft_free_sarray(ms->env);
	exit(exit_status);
}

void	show_prompt_msg(t_minishell *ms)
{
	char	*curr_dir;
	char	buff[PATH_MAX + 1];
	char	*print_path;
	int		i;

	i = 0;
	if (ms->flags && MSF_SHOW_PATH_HOME)
	{
		if (!(curr_dir = getcwd(buff, PATH_MAX)))
			ft_dprintf(2, "Error: could not get path to current directory.\n");
		else
			i = get_home_path(ms, curr_dir, &print_path, 0);
		if (!i && !print_path)
		{
			buff[0] = 0;
			print_path = buff;
		}
		ft_printf("%s%s%s>:%s%s%s%s ", FT_CYAN, FT_BOLD, ms->hostname,
			FT_EOC, FT_CYAN, print_path, FT_EOC);
	}
	else
		ft_printf("%s%s%s>%s ", FT_CYAN, FT_BOLD, ms->hostname, FT_EOC);
	i ? free(print_path) : 0;
}

int		main(int ac, char **av, char **env)
{
	int			i;
	t_minishell	ms;

	initialize_env(&ms, ac, av, env);
	while (1)
	{
		show_prompt_msg(&ms);
		signal(SIGINT, ms_signal_prompt);
		i = ft_gnl(0, &(ms.input));
		if (i == -1)
		{
			ft_dprintf(2, "Error: could not read stdin.\n");
			break ;
		}
		if (ft_iswhitespace(ms.input, 1))
		{
			i == 1 && ms.input ? free(ms.input) : 0;
			i == 0 ? write(1, "\n", 1) : 0;
			continue ;
		}
		//gestion $ et ~ ici
		ms.all_cmd = ft_strsplit(ms.input, ';');
		ms.input ? free(ms.input) : 0; //feed_historique here
		i = execute_all_commands(&ms);
		ft_free_sarray(ms.all_cmd);
		if (ms.flags & MSF_BUILTIN_EXIT)
			break ;
	}
	ft_free_sarray(ms.env); //function to free ms here
	return (-1);
}
