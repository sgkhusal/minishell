/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguilher <sguilher@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:39:20 by lalex-ku          #+#    #+#             */
/*   Updated: 2022/06/08 18:55:52 by sguilher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_folder(char *command)
{
	struct stat	statbuf;

	if (stat(command, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

static void	handle_execve_errors(char **args, char *path)
{
	int	error;

	error = EXIT_FAILURE;
	print_perror_msg("execve", args[0]);
	free_array(args);
	if (access(path, F_OK) != 0)
		error = CMD_NOT_FOUND;
	else if (access(path, X_OK) != 0)
		error = NOT_EXECUTABLE;
	free(path);
	exit(error);
}

int	execute_external(char **args, t_env *minienv)
{
	char	*path;
	int		child_pid;
	char	*command;

	command = args[0];
	child_pid = fork();
	define_execute_signals(child_pid);
	if (child_pid == -1)
		print_perror_msg("fork", command);
	else if (child_pid == 0)
	{
		close_extra_fds();
		if (is_empty(command))
			exit (EXIT_SUCCESS);
		if (is_folder(command))
			exit_with_error(command, NOT_EXECUTABLE_MSG, NOT_EXECUTABLE);
		path = get_path(command, minienv);
		if (path == NULL)
			exit_with_error(command, CMD_NOT_FOUND_MSG, CMD_NOT_FOUND);
		if (execve(path, args, minienv_to_envp(minienv)) == -1)
			handle_execve_errors(args, path);
	}
	return (child_pid);
}