/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 03:42:20 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/26 02:49:13 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtins(t_node *current, t_minishell *data)
{
	if (ft_strncmp(current->value, "cd", 3) == 0)
		return (ft_cd(current->args, data), 1);
	else if (ft_strncmp(current->value, "echo", 5) == 0)
		return (ft_echo(current->args, data), 1);
	else if (ft_strncmp(current->value, "env", 4) == 0)
		return (ft_env(current->args, data), 1);
	else if (ft_strncmp(current->value, "exit", 5) == 0)
		return (ft_exit(current->args, data), 1);
	else if (ft_strncmp(current->value, "export", 7) == 0)
		return (ft_export(current->args, data), 1);
	else if (ft_strncmp(current->value, "pwd", 4) == 0)
		return (ft_pwd(current->args, data), 1);
	else if (ft_strncmp(current->value, "unset", 6) == 0)
		return (ft_unset(current->args, data), 1);
	return (0);
}

static void
	execute_command_in_child(t_node *ast, char **args, t_minishell *data)
{
	char	*path;

	path = get_command_path(args[0], data);
	if (path == NULL)
	{
		if (data->last_exec_error == EXEC_NOT_REGULAR)
			error(ast->value, ": Not a regular file", 126, data);
		if (data->last_exec_error == EXEC_NO_PERMISSION)
			error(ast->value, ": Permission denied", 126, data);
		error(ast->value, ": Command not found", 127, data);
	}
	if (ast->redirections)
	{
		while (ast->redirections)
		{
			execute_redirections(ast->redirections, data);
			ast->redirections = ast->redirections->next;
		}
		data->tmp_fd = open(data->tmp_file, O_RDONLY, 0644);
		dup2(data->tmp_fd, STDIN_FILENO);
		safe_close(&data->tmp_fd);
		unlink(data->tmp_file);
	}
	execve(path, args, data->envp);
	error(ast->value, ": Command execution failed", 127, data);
}

static void	execute_extern_command(t_node *ast, char **args, t_minishell *data)
{
	pid_t	pid;
	int		status;

	pid = safe_fork(data);
	if (pid == 0)
		execute_command_in_child(ast, args, data);
	else
	{
		waitpid(pid, &status, 0);
		handle_child_exit(status, data);
	}
}

void	execute_command(t_node *ast, t_minishell *data, bool in_child_process)
{
	char	**args;

	if (ast->is_single_quoted == false)
		ast->value = expand_variables(ast->value, data);
	concatenate_adjacent_nodes(ast, data);
	concatenate_adjacent_nodes(ast->args, data);
	args = get_command_args(ast, data);
	data->in_command = true;
	if (execute_builtins(ast, data) == 0)
	{
		if (in_child_process)
			execute_command_in_child(ast, args, data);
		else
			execute_extern_command(ast, args, data);
	}
	data->in_command = false;
}
