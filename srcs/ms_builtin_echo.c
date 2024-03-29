/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:07:39 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/08 18:18:59 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static inline char	parse_octal(char *src, size_t *index)
{
	size_t	i;
	int		ret;

	i = 1;
	ret = 0;
	(*index)++;
	if (!src[i] || (src[i] < '0' || src[i] > '7'))
		return (src[i]);
	ret = src[i] - '0';
	i++;
	if (!src[i] || (src[i] < '0' || src[i] > '7'))
		return (ret);
	ret = ret * 8 + src[i] - '0';
	i++;
	(*index)++;
	if (!src[i] || (src[i] < '0' || src[i] > '7'))
		return (ret);
	ret = ret * 8 + src[i] - '0';
	(*index)++;
	return (ret);
}

static inline char	parse_hexa(char *src, size_t *index)
{
	size_t	i;
	int		ret;

	i = 1;
	ret = 0;
	(*index)++;
	if (!src[i]
		|| ((src[i] < '0' || src[i] > 'f') || (src[i] > '9' && src[i] < 'a')))
		return (src[i]);
	ret = (src[i] >= 'a') ? src[i] + 10 - 'a' : src[i] - '0';
	i++;
	if (!src[i]
		|| ((src[i] < '0' || src[i] > 'f') || (src[i] > '9' && src[i] < 'a')))
		return (ret);
	ret *= 16;
	ret += (src[i] >= 'a') ? src[i] + 10 - 'a' : src[i] - '0';
	(*index)++;
	return (ret);
}

static inline char	parse_escape(char *src, size_t *i)
{
	(*i)++;
	if (!ft_strchr("abfntv\\0x", src[1]) || !src[1])
		return (src[0]);
	(*i)++;
	if (src[1] == 'a')
		return ('\a');
	else if (src[1] == 'b')
		return ('\b');
	else if (src[1] == 'f')
		return ('\f');
	else if (src[1] == 'n')
		return ('\n');
	else if (src[1] == 't')
		return ('\t');
	else if (src[1] == 'v')
		return ('\v');
	else if (src[1] == '\\')
		return ('\\');
	else if (src[1] == '0')
		return (parse_octal(src + 1, i));
	else
		return (parse_hexa(src + 1, i));
}

static inline char	*parse_echo(char *src, int *stop)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (!(str = ft_strnew(ft_strlen(src))))
		return (NULL);
	i = 0;
	j = 0;
	while (src[i] && *stop != 2)
	{
		if (!(src[i] == '\\'))
			str[j++] = src[i++];
		else
		{
			if (src[i + 1] == 'c')
				*stop = 2;
			else
				str[j++] = parse_escape(src + i, &i);
		}
	}
	return (str);
}

int					ms_echo(t_minishell *ms)
{
	size_t	i;
	int		stop;
	char	*tmp;

	if (!ms->one_cmd[1])
		return (1);
	i = !ft_strcmp(ms->one_cmd[1], "-n") ? 2 : 1;
	stop = (i == 2) ? 1 : 0;
	while ((ms->one_cmd)[i])
	{
		tmp = parse_echo((ms->one_cmd)[i++], &stop);
		if (!tmp)
			return (ms_error(ms, -1, "msh: not enough memory to use echo\n"));
		ft_putstr(tmp);
		ft_memdel((void **)&(tmp));
		if (stop == 2)
			break ;
		(ms->one_cmd)[i] ? write(1, " ", 1) : 0;
	}
	!stop ? write(1, "\n", 1) : 0;
	return (1);
}
