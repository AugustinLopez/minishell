/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_bonus_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 17:17:30 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/12 19:10:38 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

static int			do_i_need_to_read(char **input, int previous)
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

static int			read_again(t_minishell *ms, char **tmp, int option)
{
	int		j;

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
		ms_exit(ms, 0);
	}
	return (1);
}
//> echo \
//> "
//dquote> "\
//>
//corriger ca
int					read_if_needed(t_minishell *ms)
{
	char	*tmp;
	char	*tmp2;
	int		option;

	option = do_i_need_to_read(&(ms->input), 0);
	tmp = 0;
	tmp2 = 0;
	while (option && read_again(ms, &tmp, option))
	{
		if ((option == 3) && tmp[0] == '\\' && ft_iswhitespace(tmp + 1, 1))
		{
			free(tmp);
			continue ;
		}
		ft_printf("%d.%s\n",option, tmp);
		tmp2 = (option != 3) ? ft_sprintf("%s\n%s", ms->input, tmp) :
			ft_strjoin(ms->input, tmp);
		option = do_i_need_to_read(&tmp, option);
		tmp ? ft_memdel((void**)&tmp) : 0;
		ms->input ? ft_memdel((void**)&(ms->input)) : 0;
		if (!tmp2)
			return (ms_error(-1, "Error: not enough memory.\n"));
		ms->input = tmp2;
	}
	return (0);
}
/*
int				case_semicolon(t_minishell *ms, size_t (*ij)[2], t_list *last)
{

}*/

int				split_ms(t_minishell *ms)
{
	t_list	*begin;
	t_list	*last;
	t_list	*tmp;
	char	*zero;
	int option;
	size_t	i;
	size_t	j;
	size_t	all;

	option = 0;
	i = 0;
	j = 0;
	all = ft_strlen(ms->input);
	zero = ft_strnew(all);
	begin = ft_lstnew(zero, all + 1);
	last = begin;
	while ((ms->input[i]))
	{
		if ((!option || option == 1) && (ms->input)[i] == '\"')
			option = (!option) ? 1 : 0;
		else if ((!option || option == 2) && (ms->input)[i] == '\'')
			option = (!option) ? 2 : 0;
		else if (!option && (ms->input)[i] == ';')
		{
			if (!(i && ms->input[i - 1] == ' '))
			{
				tmp = ft_lstnew(";", 2);
				tmp->next = ft_lstnew(zero, all - i++);
				tmp->next->zu = all + 2;
				last->zu = j;
				last->next = tmp;
				last = tmp->next;
			}
			else
			{
				((char*)(last->pv))[j] = (ms->input)[i];
				tmp = ft_lstnew(zero, all - i++);
				tmp->zu = all + 2;
				last->zu = j;
				last->next = tmp;
				last = tmp;
			}
			j = 0;
		}
		else if (!option && (ms->input)[i] == '\\')
		{
			((char*)(last->pv))[j++] = (ms->input)[i++];
			((char*)(last->pv))[j++] = (ms->input)[i++];
			continue ;
		}
		if (!option && (ms->input)[i] == ' ')
		{
			if (last->zu == all + 2)
				last->zu = 0;
			else
			{
				tmp = ft_lstnew(zero, all - i);
				last->zu = j;
				last->next = tmp;
				last = tmp;
				j = 0;
			}
			while (ms->input[i] == ' ')
				i++;
			continue ;
		}
		(last->zu == all + 2) ? last->zu = 0 : 0;
		((char*)(last->pv))[j++] = (ms->input)[i++];
	}
	tmp = begin;
	i = 0;
	ms->all_cmd = (char **)ft_memalloc(sizeof(*(ms->all_cmd)) * (ft_lstsize(begin) + 1));
	while (tmp)
	{
		(ms->all_cmd)[i] = (char*)(tmp->pv);
		i++;
		tmp = tmp->next;
		free(begin);
		begin = tmp;
	}
	free(zero);
	return (0);
}

void			ms_inputsplit(t_minishell *ms)
{
	if (read_if_needed(ms))
		return ;
		//return (-1);
	split_ms(ms);
}
