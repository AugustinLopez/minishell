/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_info.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/11 12:38:18 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void	initialize_env(t_minishell *ms, int ac, char **av, char **env)
{
	size_t	i;

	ft_bzero(ms, sizeof(*ms));
	ft_strcat(ms->hostname, "$");
	ms->ac = ac;
	ms->av = av;
	i = 0;
	while (env[i])
		i++;
	ms->env = (char **)ft_memalloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		if (!((ms->env)[i] = ft_strdup(env[i])))
		{
			ft_dprintf(2,"Error: not enough memory to initialize minishell.\n");
			ms_exit(ms, EXIT_FAILURE);
		}
		i++;
	}
	g_env = ms->env;
	g_ms = ms;
}

char	*get_from_env(t_minishell *ms, char *var)
{
	size_t	i;

	i = 0;
	while ((ms->env)[i])
	{
		if (!ft_strlcmp((ms->env)[i], var))
			return (ft_strchr((ms->env)[i], '=') + 1);
		i++;
	}
	return (NULL);
}

int		get_home_path(t_minishell *ms, char *path, char **return_path, int reverse)
{
	char	*home_path;

	home_path = get_from_env(ms, "HOME=");
	if (ft_strlcmp(path, reverse ? "~" : home_path))
	{
		*return_path = path;
		return (0);
	}
	if (reverse)
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
