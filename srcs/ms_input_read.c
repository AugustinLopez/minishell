/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_reader.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 17:45:30 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/13 18:42:58 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

static int			ms_gnl(t_minishell *ms, char **tmp, int option)
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

static int			ms_do_i_continue(char **input, int previous)
{
	int option;
	size_t i;

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

int	check_empty_space(char **tmp)
{
	size_t	i;

	i = 0;
	if (!tmp || !*tmp || !**tmp)
		return (0);
	while ((*tmp)[i])
	{
		if (((*tmp)[i] == '\"' && (*tmp)[i + 1] == '\"')
			|| ((*tmp)[i] == '\'' && (*tmp)[i + 1] == '\''))
		{
			(*tmp)[i] = ' ';
			(*tmp)[i + 1] = ' ';
		}
		i++;
	}
	i = 0;
	while ((*tmp)[i] == ' '
		|| (*tmp)[i] == 9 || ((*tmp)[i] >= 11 && (*tmp)[i] <= 13))
		i++;
	return (i);
}

int					ms_read(t_minishell *ms)
{
	int		option;
	int		mem_option;
	int		i;

	ms_gnl(ms, &(ms->input), 0);
	i = check_empty_space(&(ms->input));
	if (i)
	{
		ms->tmp0 = ft_strdup((ms->input) + i);
		ms->input = ms->tmp0;
		ms->tmp0 = 0;
	}
	option = ms_do_i_continue(&(ms->input), 0);
	while (option && ms_gnl(ms, &(ms->tmp0), option))
	{
		mem_option = ms_do_i_continue(&(ms->tmp0), option);
		i = mem_option == 3 ? check_empty_space(&(ms->tmp0)) : 0;
		if (ms->flags & MSF_REINITIALIZE_READER)
		{
			ms->tmp1 = ft_strdup((ms->tmp0) + i);
			ms->tmp0 ? ft_memdel((void**)&(ms->tmp0)) : 0;
			ms->flags &= ~MSF_REINITIALIZE_READER;
			mem_option = 0;
		}
		else
			ms->tmp1 = (option != 3) ?
				ft_sprintf("%s\n%s", ms->input, ms->tmp0 + i) :
				ft_strjoin(ms->input, ms->tmp0 + i);
		option = mem_option;
		ms->input ? ft_memdel((void**)&(ms->input)) : 0;
		ms->input = ms->tmp1;
		ms->tmp1 = 0;
		ms_free(ms, 2);
		if (!(ms->input))
			return (ms_error(-1, "Error: not enough memory.\n"));
	}
	return (0);
}
