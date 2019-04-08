/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:59:02 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

inline static int	set_list(t_minishell *ms, t_list **elem, char *str)
{
	if (!(*elem))
	{
		if (!(*elem = ft_lstnew(0, 0)))
			return (-1);
		ms->env = *elem;
	}
	else
	{
		if (!((*elem)->next = ft_lstnew(0, 0)))
			return (-1);
		*elem = (*elem)->next;
	}
	if (!((*elem)->pv = ft_strdup(str)))
		return (-1);
	return (0);
}

inline static int	ms_init(t_minishell *ms, int ac, char **av, char **env)
{
	size_t	i;
	t_list	*tmp;

	ft_bzero(ms, sizeof(*ms));
	g_ms = ms;
	(ms->hostname)[0] = '$';
	(void)ac;
	(void)av;
	tmp = NULL;
	i = 0;
	while (env[i])
		if (set_list(ms, &tmp, env[i++]))
			return (ms_error(ms, -1, "msh: not enough memory to launch.\n"));
	return (0);
}

int					main(int ac, char **av, char **env)
{
	int			err;
	int			i;
	t_minishell	ms;

	signal(SIGINT, ms_signal_when_executing);
	i = 0;
	err = ms_init(&ms, ac, av, env);
	while (!err)
	{
		ms_free(&ms, 1);
		if ((err = ms_read(&ms)))
			break ;
		if ((err = ms_split(&ms)))
			break ;
		if (ms.cmd && (ms.cmd->pv))
			i = ms_execute(&ms);
		if (ms.flags & MSF_NO_MORE_CMD)
			continue ;
		else if (i < 0)
			break ;
	}
	ms_free(&ms, 0);
	err = (i < 0) ? i : err;
	return (err);
}
