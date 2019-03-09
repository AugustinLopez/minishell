/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:51:04 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/09 12:24:21 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

static size_t	null_prot_strlen(char const *s)
{
	size_t	i;

	i = 0;
	if (s)
		while (*s++)
			i++;
	return (i);
}

char	*ft_strpjoin(char const *s1, char const *s2)
{
	char	*p;
	char	*cbuf;
	size_t	len1;
	size_t	len2;

	len1 = null_prot_strlen(s1);
	len2 = null_prot_strlen(s2);
	if (len1 && len2 && s1[len1 - 1] == s2[0])
		len1--;
	if (!(p = ft_strnew(len1 + len2)))
		return (NULL);
	cbuf = p;
	while (len1--)
		*(p++) = *(s1++);
	while (len2--)
		*(p++) = *(s2++);
	*p = 0;
	return (cbuf);
}

int		ft_strlcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	while (s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

char	*get_env_var(t_minishell *ms, char *var)
{
	size_t	i;
	char	*tmp;

	i = 0;
	while ((ms->env)[i])
	{
		tmp = ft_strjoin(var, "=");
		if (!ft_strlcmp((ms->env)[i], tmp))
		{
			free(tmp);
			return (ft_strchr((ms->env)[i], '=') + 1);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

int		get_home_path(t_minishell *ms, char *path, char *return_path, int reverse)
{
	char	*home_path;

	if (!path)
		return (0);
	home_path = get_env_var(ms, "HOME");
	if (ft_strlcmp(path, reverse ? "~" : home_path))
	{
		return_path = path;
		return (0);
	}
	if (reverse)
		return_path = ft_strpjoin(home_path, path + 1);
	else
	{
		if (*(path + ft_strlen(home_path)) == '\0')
			return_path = ft_strdup("~");
		else
			return_path = ft_strpjoin("~", path + ft_strlen(home_path));
	}
	if (return_path)
		return (1);
	else
		return (0);
}

void	show_prompt_msg(t_minishell *ms)
{
	char	*curr_dir;
	char	buff[PATH_MAX + 1];
	char	*print_path;
	int		i;

	curr_dir = getcwd(buff, PATH_MAX);
	print_path = NULL;
	i = get_home_path(ms, curr_dir, print_path, 0);
	ft_putstr(print_path);
	i ? free(print_path) : 0;
	ft_printf("%s%sMINISHELL%s ", FT_CYAN, FT_BOLD, FT_EOC);
}

int		main(int ac, char **av, char **env)
{
	pid_t		father;
	t_minishell	ms;
	size_t		i;
	char		*test;

	i = 0;
	ft_bzero(&ms, sizeof(ms));
	ms.ac = ac;
	ms.av = av;
	ms.env = env;
	father = fork();
	//ft_printf("Process: %lld\n", father);
	if (father > 0)
	{
		//ft_printf("minishell :");
		show_prompt_msg(&ms);
		/*while (env && env[i])
			ft_printf("%s\n", env[i++]);*/
		wait(&father);
	}
	else if (father == 0)
	{
		ft_gnl(0, &test); // /bin/ls
		/*while (env && env[i])
			ft_printf("%s\n", env[i++]);*/
		execve(test, av, NULL);
		if (1)
			return (0);
	}
	ft_printf("End\n");
	i = 0;
	return (0);
}
