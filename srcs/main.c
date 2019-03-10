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

char	**g_env;

char	*get_env_var(char *var)
{
	size_t	i;
	char	*tmp;

	i = 0;
	while ((g_env)[i])
	{
		tmp = ft_strjoin(var, "=");
		if (!ft_strlcmp((g_env)[i], tmp))
		{
			free(tmp);
			return (ft_strchr((g_env)[i], '=') + 1);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

int		get_home_path(char *path, char **return_path, int reverse)
{
	char	*home_path;

	if (!path)
		return (0);
	home_path = get_env_var("HOME");
	if (ft_strlcmp(path, reverse ? "~" : home_path))
	{
		*return_path = path;
		return (0);
	}
	if (reverse)
		*return_path = ft_strjoin(home_path, path + 1); //pathjoin ?
	else
	{
		if (*(path + ft_strlen(home_path)) == '\0')
			*return_path = ft_strdup("~");
		else
			*return_path = ft_strjoin("~", path + ft_strlen(home_path)); //pathjoin ?
	}
	if (*return_path)
		return (1);
	return (0);
}

void	show_prompt_msg(void)
{
	char	*curr_dir;
	char	buff[PATH_MAX + 1];
	char	*print_path;
	int		i;

	curr_dir = getcwd(buff, PATH_MAX);
	i = get_home_path(curr_dir, &print_path, 0);
	ft_printf("%s%sMINISHELL:%s%s%s%s ", FT_CYAN, FT_BOLD,
		FT_EOC, FT_CYAN, print_path, FT_EOC);
	i ? free(print_path) : 0;
}

void	signal_display(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr("\n");
		show_prompt_msg();
		signal(SIGINT, signal_display);
	}
}

void	signal_no_display(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr("\n");
		signal(SIGINT, signal_display);
	}
}


int		run_cmd(char *path, char **args)
{
	pid_t	pid;

	pid = fork();
	signal(SIGINT, signal_no_display);
	if (!pid)
		execve(path, args, g_env);
	else if (pid < 0)
	{
		ft_putendl("Failed to fork the process.");
		return (-1);
	}
	wait(&pid);
	return (0);
}

int		execute_command(char **command)
{
	return (run_cmd(command[0], command));
}

int		execute_commands(char **commands)
{
	size_t	i;
	size_t	j;
	int		ret;
	char	**command;

	i = 0;
	ret = 0;
	while (commands[i])
	{
		command = ft_strssplit(commands[i], " \t\v\f\r\n");
		ret = execute_command(command);
		if (command)
		{
			j = 1;
			while (command[j])
				free(command[j++]);
			free(command[0]);
			free(command);
		}
		if (ret == -1)
			break;
		i++;
	}
	return (ret);
}

int		main(int ac, char **av, char **env)
{
	t_minishell	ms;
	char		*test;
	int			i;
	char		**commands;

	ft_bzero(&ms, sizeof(ms));
	ms.ac = ac;
	ms.av = av;
	ms.env = env;
	g_env = ms.env;
	while (1)
	{
		show_prompt_msg();
		signal(SIGINT, signal_display); // Ctrl + C
		i = ft_gnl(0, &test); // /bin/ls
		if (!i)
		{
			test ? free(test) : 0;
			write(1, "\n", 1);
			exit(0);
		}
		//gestion $ et ~ ici
		if (ft_iswhitespace(test, 1))
		{
			test ? free(test) : 0;
			continue;
		}
		commands = ft_strsplit(test, ';');
		test ? free(test) : 0;
		i = execute_commands(commands);
		i = 1;
		if (commands)
		{
			while (commands[i])
				free(commands[i++]);
			free(commands[0]);
			free(commands);
		}
		if (i == -1)
			return (-1);
	}
	return (0);
}
