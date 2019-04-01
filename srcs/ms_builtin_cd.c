/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:32:06 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/01 11:50:26 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	change_dir(char *path, int flags)
{
	char	*cwd;
	char	buff[4096 + 1];

	(void)cwd;
	(void)buff;
	(void)path;
	(void)flags;
	ft_printf("%s\n", path);
	return (0);
	/*cwd = getcwd(buff, 4096);
	if (!chdir*/
}

int cd_available_option(char *av, int *flags)
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

int	cd_parsing(t_minishell *ms, int *flags)
{
	int		i;
	char	**av;

	av = ms->one_cmd;
	if (!av || !(av[0]))
		return (-1);
	i = 1;
	while (av[i] && av[i][0] == '-' && av[i][1])
	{
		if (av[i][1] == '-' && !av[i][2])
			return (i + 1);
		if (!cd_available_option(av[i], flags))
		{
			*flags = i;
			return (-1);
		}
	}
	return (i);
}

int	ms_cd(t_minishell *ms)
{
	char	*home_path;
	int		ac;
	int		flags;

	flags = 0;
	home_path = get_from_env(ms, "HOME=");
	if ((ac = cd_parsing(ms, &flags)) < 0)
	{
		ft_dprintf(2, "cd: bad argument: %s\n", ms->one_cmd[ac]);
		return (1);
	}
	if (!(ms->one_cmd[ac]))
	{
		change_dir(home_path, flags);
		return (1);
	}
	if (ms->one_cmd[ac + 1])
		return (ms_error(1, "cd: too many arguments"));
	if (ms->one_cmd[ac][0] == '-' && !ms->one_cmd[ac][1])
	{
		flags += 512;
		change_dir(get_from_env(ms, "OLDPWD="), flags);
		return (1);
	}
	change_dir(ms->one_cmd[ac], flags);
	return (1);
}
