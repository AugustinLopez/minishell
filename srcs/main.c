/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/11 18:43:45 by aulopez          ###   ########.fr       */
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

void	ms_exit(t_minishell *ms, int exit_status)
{
	ms->env ? ft_free_sarray(&(ms->env)) : 0;
	ms->all_cmd ? ft_free_sarray(&(ms->all_cmd)) : 0;
	ms->one_cmd ? ft_free_sarray(&(ms->one_cmd)) : 0;
	ms->input ? ft_memdel((void**)&(ms->input)) : 0;
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
	i ? ft_memdel((void**)&print_path) : 0;
}

int		main(int ac, char **av, char **env)
{
	int			i;
	t_minishell	ms;
	char		*tmp;

	initialize_env(&ms, ac, av, env);
	tmp = 0;
	while (1)
	{
		show_prompt_msg(&ms);
		signal(SIGINT, ms_signal_prompt);
		i = ft_gnl(0, &tmp);
		if (i == -1)
		{
			ft_dprintf(2, "Error: could not read stdin.\n");
			break ;
		}
		if (ft_iswhitespace(tmp, 1))
		{
			i == 1 && tmp ? free(tmp) : 0;
			i == 0 ? write(1, "\n", 1) : 0;
			if (i == 0)
				break;
			continue ;
		}
		//gestion $ et ~ ici
		ms.input = ft_strtrim(tmp);
		ms_inputsplit(&ms);
		tmp ? ft_memdel((void**)&tmp) : 0;
		ms.all_cmd = ft_strsplit(ms.input, ';');
		ms.input ? ft_memdel((void**)&(ms.input)) : 0; //feed_historique here
		i = execute_all_commands(&ms);
		ft_free_sarray(&(ms.all_cmd));
		ms.all_cmd = 0;
		if (ms.flags & MSF_BUILTIN_EXIT)
			break ;
	}
	ft_free_sarray(&(ms.env)); //function to free ms here
	return (-1);
}
