/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_cd_posix.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 18:31:25 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/04 18:36:31 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** cd should perform as described by - http:
** //pubs.opengroup.org/onlinepubs/9699919799/utilities/cd.html#tag_20_14_08
** but with -P always active. I implemented the parsing but I won't use
** those flags in the minishell.
*/

int					cd_posix_minus(t_minishell *ms, int ac, int flags)
{
	if (ms->one_cmd[ac][0] == '-' && !ms->one_cmd[ac][1])
	{
		flags += 512;
		if (change_dir(ms, get_from_env(ms, "OLDPWD="), flags) == -1)
			return (-1);
		return (1);
	}
	return (0);
}

int					cd_posix_step_1_2(t_minishell *ms, int ac, char *home_path,
									int flags)
{
	if (!(ms->one_cmd[ac]))
	{
		if (home_path)
			if (change_dir(ms, home_path, flags) == -1)
				return (-1);
		return (1);
	}
	if (ms->one_cmd[ac + 1])
		return (ms_error(1, "cd: too many arguments\n"));
	return (0);
}

char				*cd_posix_step_3_to_6(t_minishell *ms, int ac, int *flags)
{
	char	*tmp;
	char	**split;
	size_t	i;
	t_stat	st;

	if ((ms->one_cmd)[ac][0] == '/' || (ms->one_cmd)[ac][0] == '.')
		return (ft_strdup(ms->one_cmd[ac]));
	tmp = get_from_env(ms, "CDPATH=");
	if (!tmp || !(split = ft_strsplit(tmp, ':')))
		return (ft_strdup(ms->one_cmd[ac]));
	i = 0;
	while (split[i])
	{
		tmp = (split[i][0]) ? ft_sprintf("%s/%s", split[i], (ms->one_cmd)[ac])
			: ft_sprintf("./%s", (ms->one_cmd)[ac]);
		if ((!tmp) || stat(tmp, &st) >= 0 || S_ISDIR(st.st_mode))
		{
			ft_free_sarray(&split);
			*flags += (tmp != NULL) ? 512 : 0;
			return (tmp);
		}
		free(tmp);
		i++;
	}
	return (ft_strdup(ms->one_cmd[ac]));
}

static inline int	cd_available_option(char *av, int *flags)
{
	char	*s;

	while (*(av++))
	{
		if (!(s = ft_strchr("LP", av[0])))
			return (0);
		*flags = s[0];
	}
	return (1);
}

int					cd_posix_parsing(t_minishell *ms, int *flags)
{
	int		i;
	char	**av;

	av = ms->one_cmd;
	if (!av || !(av[0]))
	{
		ft_dprintf(2, "cd: invalid argument: %s\n", ms->one_cmd[0]);
		return (-1);
	}
	i = 1;
	while (av[i] && av[i][0] == '-' && av[i][1])
	{
		if (av[i][1] == '-' && !av[i][2])
			return (i + 1);
		if (!cd_available_option(av[i], flags))
		{
			ft_dprintf(2, "cd: invalid argument: %s\n", ms->one_cmd[i]);
			return (-1);
		}
		i++;
	}
	return (i);
}
