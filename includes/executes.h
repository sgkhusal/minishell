/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguilher <sguilher@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 15:41:13 by lalex-ku          #+#    #+#             */
/*   Updated: 2022/06/10 15:23:23 by sguilher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTES_H
# define EXECUTES_H

# define IN 0
# define OUT 1

// executes
int		execute_one_command(char *command, t_env	**minienv);
int		execute_multiple_commands(char **commands, t_env **minienv);

// multiple commands utils
int		*init_children_pid(char **commands);
void	clean_after_execute(int *children_pid);

// execute builtin
int		execute_forked_builtin(char **args, t_env **minienv, char **commands);
int		execute_builtin(char **args, t_env **minienv);

// execute external
int		execute_external(char **args, t_env *minienv);
char	*get_path(char *command, t_env *minienv);

// wait after fork
int		wait_for_child(int child_pid, int is_last_child);
int		wait_for_children(int children_pid[1024]);

// redirects
void	redirect_fd(int fd_to_redirect, int fd_location);
void	redirect_fds(int fd_in, int fd_out);
int		redirect_input(char *command);
int		redirect_output(char *command);
char	*redirect_position(char *str, char redirect_char);
void	close_all_fds(void);
void	close_extra_fds(void);
int		has_input_redirect(char *command);
int		has_output_redirect(char *command);
char	*redirect_file_name(char *redirect_str);

// pipes
void	handle_pipe(int original_out, char *current_command, char **commands);

#endif
