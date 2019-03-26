/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_info.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/26 17:27:40 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void	initialize_env(t_minishell *ms, int ac, char **av, char **env)
{
	size_t	i;
	t_list	*tmp;

	ft_bzero(ms, sizeof(*ms));
	(ms->hostname)[0] = '$';
	ms->ac = ac;
	ms->av = av;
	i = 0;
	tmp = NULL;
	while (env[i])
		i++;
	ms->arr_env = (char **)ft_memalloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		if (!tmp)
		{
			tmp = ft_lstnew(0, 0);
			ms->env = tmp;
		}
		else
		{
			tmp->next = ft_lstnew(0, 0);
			tmp = tmp->next;
		}
		if (!(tmp->pv = ft_strdup(env[i])))
		{
			ft_dprintf(2,"Error: not enough memory to initialize minishell.\n");
			ms_exit(ms, EXIT_FAILURE);
		}
		(ms->arr_env)[i] = (char*)(tmp->pv);
		i++;
	}
	g_ms = ms;
}

char	*get_from_env(t_minishell *ms, char *var)
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

int		get_home_path(t_minishell *ms, char *path, char **return_path,
		int reverse)
{
	char	*home_path;

	home_path = get_from_env(ms, "HOME=");
	if (ft_strlcmp(path, reverse ? "~" : home_path))
		*return_path = ft_strdup(path);
	else if (reverse)
		*return_path = ft_pathjoin(home_path, path + 1);
	else if (*(path + ft_strlen(home_path)) == '\0')
		*return_path = ft_strdup("~");
	else
		*return_path = ft_pathjoin("~", path + ft_strlen(home_path));
	if (*return_path)
		return (1);
	else
		ft_dprintf(2, "Error: not enough memory.\n");
	return (0);
}
