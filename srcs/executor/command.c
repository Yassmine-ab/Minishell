/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 03:42:20 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/22 01:39:39 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	concatenate_adjacent_nodes(t_node *node, t_minishell *data)
{
	t_node	*current;
	t_node	*temp;
	char	*combined;

	current = node;
	while (current && current->next)
	{
		if (current->space_after == false)
		{
			combined = ft_strjoin_gc(current->value, \
			current->next->value, &data->gc);
			gc_free(current->value, &data->gc);
			current->value = combined;
			temp = current->next;
			current->next = temp->next;
			gc_free(temp->value, &data->gc);
			gc_free(temp, &data->gc);
		}
		else
			current = current->next;
	}
}

static char	**get_command_args(t_node *cmd_node, t_minishell *data)
{
	int		arg_count;
	t_node	*arg;
	char	**args;
	int		i;

	arg_count = 1;
	arg = cmd_node->args;
	while (arg)
	{
		arg_count++;
		arg = arg->next;
	}
	args = gc_malloc(sizeof(char *) * (arg_count + 1), &data->gc);
	args[0] = cmd_node->value;
	arg = cmd_node->args;
	i = 1;
	while (arg)
	{
		args[i++] = arg->value;
		arg = arg->next;
	}
	args[i] = NULL;
	return (args);
}

static char	*get_command_path(char *command, t_minishell *data)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	if (access(command, X_OK) == 0)
		return (command);
	path_env = getenv("PATH");
	if (path_env == NULL)
		return (NULL);
	paths = ft_split_gc(path_env, ':', &data->gc);
	if (paths == NULL)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin_gc(paths[i], "/", &data->gc);
		full_path = ft_strjoin_gc(full_path, command, &data->gc);
		if (access(full_path, X_OK))
		{
			if (errno == EACCES)
				error("Permission denied", 126, &data->gc);
		}
		else
			return (full_path);
	}
	if (access(command, X_OK))
		error("Permission denied", 126, &data->gc);
	return (NULL);
}

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

static void	execute_command_in_pipeline(t_node *ast, char **args, \
t_minishell *data)
{
	char	*path;

	path = get_command_path(args[0], data);
	if (path == 0)
	{
		data->last_exit_status = 127;
		error("Command not found", 127, &data->gc);
	}
	if (ast->redirections)
		execute_redirections(ast->redirections, data);
	execve(path, args, data->envp);
	data->last_exit_status = 127;
	error("Command execution failed", 127, &data->gc);
}

static void	launch_execve(char *path, char **args, t_minishell *data)
{
	if (execve(path, args, data->envp) == -1)
	{
		if (errno == EACCES)
		{
			error("Permission denied", 126, &data->gc);
			data->last_exit_status = 126;
		}
		else
		{
			error("Command execution failed", 127, &data->gc);
			data->last_exit_status = 127;
		}
	}
}

static void	execute_extern_command(t_node *ast, char **args, t_minishell *data)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid == -1)
	{
		data->last_exit_status = 1;
		error("Fork failed", 1, &data->gc);
	}
	else if (pid == 0)
	{
		data->is_child_process = true;
		signal_child_process();
		path = get_command_path(args[0], data);
		if (path == 0)
		{
			data->last_exit_status = 127;
			error("Command not found", 127, &data->gc);
		}
		if (ast->redirections)
			execute_redirections(ast->redirections, data);
		launch_execve(path, args, data);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		data->last_exit_status = 128 + WTERMSIG(status);
		data->child_end_with_signal = true;
	}
	else if (WIFEXITED(status))
		data->last_exit_status = WEXITSTATUS(status);
	else
		data->last_exit_status = 1;
}

void	execute_command(t_node *ast, t_minishell *data, bool in_pipeline)
{
	t_node	*tmp;
	char	**args;

	if (ast->args)
	{
		tmp = ast->args;
		while (tmp)
		{
			tmp->value = expand_variables(tmp->value, data);
			tmp = tmp->next;
		}
	}
	ast->value = expand_variables(ast->value, data);
	concatenate_adjacent_nodes(ast, data);
	concatenate_adjacent_nodes(ast->args, data);
	args = get_command_args(ast, data);
	if (execute_builtins(ast, data) == 0)
	{
		if (in_pipeline)
			execute_command_in_pipeline(ast, args, data);
		else
			execute_extern_command(ast, args, data);
	}
}
