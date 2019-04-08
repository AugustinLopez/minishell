/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 14:54:53 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:28:08 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline int	ms_list_to_array(t_minishell *ms)
{
	size_t	i;
	char	**tmp;

	tmp = (char **)ft_memalloc(sizeof(char *) * (ft_lstsize(ms->cmd) + 1));
	if (!tmp)
		return (1);
	ms->arr_cmd = tmp;
	ms->elem = ms->cmd;
	i = 0;
	while (ms->elem)
	{
		(ms->arr_cmd)[i] = (char*)(ms->elem->pv);
		i++;
		ms->elem = ms->elem->next;
	}
	return (0);
}

/*
** Note: $ should "probably" not be modified immediately after a ;
** But changing this is not worth atm.
*/

int					ms_split(t_minishell *ms)
{
	if (ms_split_to_list(ms))
		return (ms_error(ms, -1, "msh: not enough memory to parse input.\n"));
	if (ms_split_replace(ms))
		return (ms_error(ms, -1, "msh: not enough memory to parse input.\n"));
	if (ms_split_remove_quote(ms))
		return (ms_error(ms, -1, "msh: not enough memory to parse input.\n"));
	if (ms_list_to_array(ms))
		return (ms_error(ms, -1, "msh: not enough memory to parse input.\n"));
	ms_free(ms, 2);
	return (0);
}
