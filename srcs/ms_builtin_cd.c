/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:32:06 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/27 18:46:59 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ms_cd(t_minishell *ms)
{
	char	*home_path;

	home_path = get_from_env(ms, "HOME=");
	if (!(ms->one_cmd[1]))
	{
		change_dir(home_path, 0);
		return (1);
	}
	if (ms->one_cmd[2])
		
}
