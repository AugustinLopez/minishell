/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_info.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/09 12:24:21 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void	ms_exit(int exit_status)
{
	ft_free_sarray(g_env);
	exit(exit_status);
}

void	initialize_env(int ac, char **av, char **env)
{
	size_t	i;

	(void)ac;
	(void)av;
	i = 0;
	while (env[i])
		i++;
	g_env = (char **)ft_memalloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		if (!(g_env[i] = ft_strdup(env[i])))
		{
			ft_dprintf(2,"Error: not enough memory to initialize minishell.\n");
			ms_exit(EXIT_FAILURE);
		}
		i++;
	}
}

char	*get_from_env(char *var)
{
	size_t	i;

	i = 0;
	while ((g_env)[i])
	{
		if (!ft_strlcmp((g_env)[i], var))
			return (ft_strchr((g_env)[i], '=') + 1);
		i++;
	}
	return (NULL);
}

int		get_home_path(char *path, char **return_path, int reverse)
{
	char	*home_path;

	home_path = get_from_env("HOME=");
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
