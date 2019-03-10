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

int		run_cmd(char *path, char **args)
{
	pid_t	pid;

	pid = fork();
	signal(SIGINT, signal_no_display);
	if (!pid)
		execve(path, args, g_env);
	else if (pid < 0)
	{
		ft_dprintf(2, "Failed to fork the process.\n");
		return (-1);
	}
	wait(&pid);
	return (1);
}

int		is_bin_executable(char *bin, t_stat stat, char **command)
{
	if (stat.st_mode & S_IFREG)
	{
		if (stat.st_mode & S_IXUSR)
			return (run_cmd(bin, command));
		else
			ft_dprintf(2, "%s: permission denied.\n", bin);
		return (1);
	}
	return (0);
}

int		is_bin_cmd(char **command)
{
	size_t	i;
	int		j;
	char	*bin;
	char	**path;
	t_stat	stat;

	if (!(path = ft_strsplit(get_from_env("PATH="), ':')))
	{
		ft_dprintf(2, "Error: not enough memory.\n");
		return (-1);
	}
	i = 0;
	while (path[i])
	{
		j = 0;
		if (!(ft_strlcmp(command[0], path[i])))
			bin = ft_strdup(command[0]);
		else if ((j = 1))
			bin = ft_pathjoin(path[i], command[0]);
		if (!bin)
		{
			ft_dprintf(2, "Error: not enough memory.\n");
			return (-1);
		}
		if (lstat(bin, &stat) < 0)
			bin ? free(bin) : 0;
		else
		{
			ft_free_sarray(path);
			i = is_bin_executable(bin, stat, command);
			bin ? free(bin) : 0;
			return (i);
		}
		i++;
	}
	ft_free_sarray(path);
	return (0);
}

int	print_env(void)
{
	size_t	i;

	i = 0;
	while (g_env[i])
		ft_putendl(g_env[i++]);
	return (1);
}

int		is_builtin_cmd(char **command)
{
	if (!ft_strcmp(command[0], "exit"))
		return (-2);
	if (!ft_strcmp(command[0], "env"))
		return (print_env());
	return (0);
}

int		execute_single_command(char **command)
{
	t_stat	stat;
	int		i;

	if ((i = is_bin_cmd(command)))
		return (i);
	if ((i = is_builtin_cmd(command)))
		return (i);
	if (lstat(command[0], &stat) != -1)
	{
		if (stat.st_mode & S_IXUSR)
			return (run_cmd(command[0], command));
		//else if (stat.st_mode & S_IFDIR) // cd
	}
	ft_dprintf(2, "%s: command not found.\n", command[0]);
	return (0);
}

int		execute_all_commands(char **commands)
{
	size_t	i;
	int		ret;
	char	**command;

	i = 0;
	while (commands[i])
	{
		if (!(command = ft_strssplit(commands[i], " \t\v\f\r")))
		{
			ft_dprintf(2, "Error: not enough memory.\n");
			return (-1);
		}
		ret = execute_single_command(command);
		ft_free_sarray(command);
		if (ret < 0)
			break;
		i++;
	}
	return (ret);
}

void	show_prompt_msg(void)
{
	char	*curr_dir;
	char	buff[PATH_MAX + 1];
	char	*print_path;
	int		i;

	i = 0;
	if (!(curr_dir = getcwd(buff, PATH_MAX)))
		ft_dprintf(2, "Error: could not get path to current directory.\n");
	else
		i = get_home_path(curr_dir, &print_path, 0);
	if (!i && !print_path)
	{
		buff[0] = 0;
		print_path = buff;
	}
	ft_printf("%s%sMINISHELL:%s%s%s%s ", FT_CYAN, FT_BOLD,
		FT_EOC, FT_CYAN, print_path, FT_EOC);
	i ? free(print_path) : 0;
}

int		main(int ac, char **av, char **env)
{
	char		*input;
	int			i;
	char		**commands;

	initialize_env(ac, av, env);
	while (1)
	{
		show_prompt_msg();
		signal(SIGINT, signal_display); // Ctrl + C
		i = ft_gnl(0, &input);
		if (i == -1)
		{
			ft_dprintf(2, "Error: could not read stdin.\n");
			break ;
		}
		if (ft_iswhitespace(input, 1))
		{
			i == 1 && input ? free(input) : 0;
			i == 0 ? write(1, "\n", 1) : 0;
			continue ;
		}
		//gestion $ et ~ ici
		commands = ft_strsplit(input, ';');
		input ? free(input) : 0;
		i = execute_all_commands(commands);
		ft_free_sarray(commands);
		if (i == -2)
			break ;
	}
	ft_free_sarray(g_env);
	return (-1);
}
