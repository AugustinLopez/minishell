/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_info.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/04 18:34:36 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

inline static int	set_list(t_minishell *ms, t_list **elem, char *str)
{
	if (!(*elem))
	{
		if (!(*elem = ft_lstnew(0, 0)))
			return (0);
		ms->env = *elem;
	}
	else
	{
		if (!((*elem)->next = ft_lstnew(0, 0)))
			return (0);
		*elem = (*elem)->next;
	}
	if (!((*elem)->pv = ft_strdup(str)))
		return (0);
	return (1);
}

int					ms_initialize(t_minishell *ms, int ac, char **av,
									char **env)
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
		if (!(set_list(ms, &tmp, env[i++])))
			return (ms_error(1, "minishell: not enough memory to launch.\n"));
	return (0);
}

char				*get_from_env(t_minishell *ms, char *var)
{
	size_t	i;
	t_list	*tmp;
	char	*s;

	i = 0;
	tmp = ms->env;
	while (tmp)
	{
		s = (char *)(tmp->pv);
		if (!ft_strlcmp(s, var))
			return (ft_strchr(s, '=') + 1);
		i++;
		if (tmp)
			tmp = tmp->next;
	}
	return (NULL);
}

int					get_home_path(t_minishell *ms, char *path,
								char **return_path, int reverse)
{
	char	*home_path;

	home_path = get_from_env(ms, "HOME=");
	return_path ? ft_memdel((void **)return_path) : 0;
	if (!home_path)
		return (0);
	if (ft_strlcmp(path, reverse ? "~" : home_path))
		*return_path = ft_strdup(path);
	else if (reverse)
		*return_path = ft_pathjoin(home_path, path + 1);
	else if (*(path + ft_strlen(home_path)) == '\0')
		*return_path = ft_strdup("~");
	else
		*return_path = ft_pathjoin("~", path + ft_strlen(home_path));
	if (*return_path)
		return (0);
	return (ms_error(-1, "minishell: not enough memory to load home path.\n"));
}
