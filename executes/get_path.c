/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex-ku <lalex-ku@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 18:15:41 by sguilher          #+#    #+#             */
/*   Updated: 2022/06/22 17:00:00 by lalex-ku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_path(char base[], char *part1, char *part2, char *part3)
{
	*base = 0;
	ft_strlcat(base, part1, PATH_MAX);
	ft_strlcat(base, part2, PATH_MAX);
	ft_strlcat(base, part3, PATH_MAX);
}

static int	is_local_path(char *command)
{
	return (command[0] == '.' && command[1] == '/');
}

static int	is_on_current_dir(char *command, t_env *minienv)
{
	char	*pwd;
	char	current_path[PATH_MAX];
	
	pwd = minienv_value("PWD", minienv);
	create_path(current_path, pwd, "/", command);
	return (access(current_path, F_OK) == 0);
}

static char	*local_path(char *command, t_env *minienv)
{
	char	full_path[PATH_MAX];

	full_path[0] = 0;
	create_path(full_path, minienv_value("PWD", minienv), "/", command);
	return (ft_strdup(full_path));
}

char	*get_path(char *command, t_env *minienv)
{
	char	*path_env;
	char	**paths;
	char	current_path[PATH_MAX];
	char	**paths_start;

	if (is_local_path(command))
		return (local_path(command, minienv));
	path_env = minienv_value("PATH", minienv);
	paths = ft_split(path_env, ':');
	paths_start = paths;
	while (*paths)
	{
		create_path(current_path, *paths, "/", command);
		if (access(current_path, F_OK) == 0)
		{
			free_array(paths_start);
			return (ft_strdup(current_path));
		}
		paths++;
	}
	free_array(paths_start);
	if (is_on_current_dir(command, minienv))
		return (local_path(command, minienv));
	return (NULL);
}
