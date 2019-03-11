/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:42:35 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/11 13:29:00 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

//Need to use another gnl here, with quote> and dquote>
//in order to do that i need to verify if s + 1 exist
//0 : escape till no number 3 numbers
//a, b, t, m, v, f, r, c are special
static void	putecho(char *s)
{
	size_t	start;
	size_t	len;

	start = ft_strchr("\"\'", s[0]) ? 1 : 0;
	len = ft_strlen(s);
	ft_strchr("\"\'", s[len - 1]) ? s[len - 1] = '\0' : 0;
	ft_putstr(s + start);
}

int			builtin_echo(t_minishell *ms)
{
	size_t	i;
	int		n;

	n = 0;
	if (!(ms->one_cmd)[0])
	{
		write(1, "\n", 1);
		return (1);
	}
	if (!ft_strcmp((ms->one_cmd)[1], "-n"))
		n = 1;
	i = n;
	while ((ms->one_cmd[i]))
	{
		putecho(one_cmd[i]);
		if (ms->one_cmd[++i])
			write(1, " ", 1);
	}
	!n ? write(1, "\n", 1) : 0;
	return (1);
}
