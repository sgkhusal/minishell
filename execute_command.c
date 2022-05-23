/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex-ku <lalex-ku@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:39:20 by lalex-ku          #+#    #+#             */
/*   Updated: 2022/05/23 14:39:29 by lalex-ku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: Adicionar um novo path e tentar executar um programa naquele path
// ex: Adicionar o minishell no path, dar um cd, e tentar executar minishell
void	ft_free_arr(char	**arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
	return ;
}

char *get_executable(char *cmd, t_env *minienv)
{
	char *path_env;
	char **paths;
	char current_path[PATH_MAX];
	int	i;

	current_path[0] = 0;
	if ( cmd[0] == '.' && cmd[1] == '/' )
	{
		strs_cat(current_path, minienv_value("PWD", minienv), "/", cmd);
		if (access(current_path, X_OK) == 0)
			return (ft_strdup(current_path));	
	}
	path_env = minienv_value("PATH", minienv);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		current_path[0] = 0;
		strs_cat(current_path, paths[i], "/", cmd);
		if (access(current_path, X_OK) == 0)
		{
			ft_free_arr(paths);
			return (ft_strdup(current_path));
		}
		i++;
	}
	ft_free_arr(paths);
	return(NULL);
}

int	wait_for_child(int child_pid)
{
	int	status;

	if (waitpid(child_pid, &status, 0) < 0)
	{
		perror("minishell: waitpid error");
		exit(EXIT_FAILURE);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (INTERRUPT + status);
	}
}

int	execute_command(char **args, t_env *minienv)
{
	char *path;
	int child_pid;

	child_pid = fork();
	define_execute_signals(child_pid);
	if (child_pid == -1)
		perror("minishell : ");
	else if (child_pid == 0)
	{
		path = get_executable(args[0], minienv);
		if (execve(path, args, minienv_to_envp(minienv)) == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO); // TODO: Colocar em uma aux (ex: exit_with_message)
			ft_putstr_fd(args[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		exit(EXIT_FAILURE);
	}
	else
		return (wait_for_child(child_pid));
	return (0); // TODO: errno no fork = 11
}
