/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_commands.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguilher <sguilher@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 13:29:31 by lalex-ku          #+#    #+#             */
/*   Updated: 2022/06/03 15:41:54 by sguilher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_for_children(int children_pid[1024])
{
	int	i;
	int	exit_status;

	i = 0;
	while (children_pid[i] != 0)
	{
		exit_status = wait_for_child(children_pid[i]);
		i++;
	}
	return (exit_status);
}

int	execute_multiple_commands(char **commands, t_env **minienv)
{
	char	**args;
	int		original_fds[2];
	int		exit_status;
	int		children_pid[1024]; // TODO: podia ser uma lista linkada
	int		is_first_command;

	original_fds[1] = dup(STDOUT_FILENO);
	is_first_command = TRUE;

	// cat <"./tests/test_files/infile_big" | echo hi
	int i = 0;
	while (commands[i])
	{
		original_fds[0] = dup(STDIN_FILENO);
		prepare_io(original_fds[1], is_first_command, (commands[i + 1] != NULL));
		if(handle_input_redirect(commands[i]) == EXIT_FAILURE)
		{
			redirect_fd(original_fds[0], STDIN_FILENO);
			original_fds[0] = dup(STDIN_FILENO);
			continue;
		}
		args = split_args(commands[i]);
		// TODO: fechar o pipe de leitura nos filhos (menos no último comando, que não tem)
		// TODO: filhos precisam fechar os fds duplicados - principalmente a exit!!
		if (is_builtin(args[0]))
			children_pid[i] = execute_forked_builtin(args, minienv, commands);
		else
			children_pid[i] = execute_command(args, *minienv);
		is_first_command = FALSE;
		free_array(args);
		redirect_fd(original_fds[0], STDIN_FILENO);
		i++;
	}
	children_pid[i] = 0;
	exit_status = wait_for_children(children_pid);
	return (exit_status);
}
