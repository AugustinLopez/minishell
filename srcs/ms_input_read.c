/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_read.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 12:31:56 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/14 15:56:59 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

inline static int	ms_gnl(t_minishell *ms, char **tmp, int option)
{
	int		j;

	signal(SIGINT, ms_signal_reinitialize);
	option == 0 ? show_prompt(ms) : 0;
	option == 1 ? ft_printf("quote> ") : 0;
	option == 2 ? ft_printf("dquote> ") : 0;
	option == 3 ? ft_printf("> ") : 0;
	j = ft_gnl(0, tmp);
	if (j == -1)
	{
		ft_dprintf(2, "Error: could not read stdin.\n");
		ms_exit(ms, EXIT_FAILURE);
}
	if (!j)
	{
		write(1, "\n", 1);
		ms_exit(ms, EXIT_FAILURE);
	}
	return (1);
}

inline static int	ms_do_i_continue(char **input, int previous)
{
	int		option;
	size_t	i;

	option = (previous == 3) ? 0 : previous;
	i = 0;
	while ((*input)[i])
	{
		if ((*input)[i] == '\\' && (*input)[i + 1])
		{
			i += 2;
			continue ;
		}
		if ((!option || option == 1) && (*input)[i] == '\'')
			option = !option ? 1 : 0;
		if ((!option || option == 2) && (*input)[i] == '\"')
			option = !option ? 2 : 0;
		if (!option && (*input)[i] == '\\' && !((*input)[i + 1]))
		{
			(*input)[i] = 0;
			return (3);
		}
		i++;
	}
	return (option);
}

inline static int	ms_read_loop(t_minishell *ms, int option)
{
	int	new_option;

	new_option = ms_do_i_continue(&(ms->tmp0), option);
	if (ms->flags & MSF_REINITIALIZE_READER)
	{
		ms->tmp1 = ft_strdup((ms->tmp0));
		ms->tmp0 ? ft_memdel((void**)&(ms->tmp0)) : 0;
		ms->flags &= ~MSF_REINITIALIZE_READER;
		new_option = 0;
	}
	else
		ms->tmp1 = (option != 3) ?
			ft_sprintf("%s\n%s", ms->input, ms->tmp0) :
			ft_strjoin(ms->input, ms->tmp0);
	ms->input ? ft_memdel((void**)&(ms->input)) : 0;
	ms->input = ms->tmp1;
	ms->tmp1 = 0;
	ms_free(ms, 2);
	!(ms->input) ? new_option = -1 : 0;
	return (new_option);
}

int					ms_read(t_minishell *ms)
{
	int	option;

	ms_gnl(ms, &(ms->input), 0);
	option = ms_do_i_continue(&(ms->input), 0);
	while (option && ms_gnl(ms, &(ms->tmp0), option))
	{
		option = ms_read_loop(ms, option);
		if (option == -1)
			return (ms_error(-1, "Error: not enough memory.\n"));
	}
	return (0);
}
