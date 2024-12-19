/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 03:42:20 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/19 10:52:49 by yaabdall         ###   ########.fr       */
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
		if (!current->space_after)
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
	if (!path_env)
		return (NULL);
	paths = ft_split_gc(path_env, ':', &data->gc);
	if (!paths)
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
			return (free_split(paths, &data->gc), full_path);
	}
	free_split(paths, &data->gc);
	if (access(command, X_OK))
		error("Permission denied", 126, &data->gc);
	return (NULL);
}

static int	execute_builtins(t_node *current, t_minishell *data)
{
	if (!ft_strncmp(current->value, "cd", 3))
		return (ft_cd(current->args, data), 1);
	else if (!ft_strncmp(current->value, "echo", 5))
		return (ft_echo(current->args, data), 1);
	else if (!ft_strncmp(current->value, "env", 4))
		return (ft_env(current->args, data), 1);
	else if (!ft_strncmp(current->value, "exit", 5))
		return (ft_exit(current->args, data), 1);
	else if (!ft_strncmp(current->value, "export", 7))
		return (ft_export(current->args, data), 1);
	else if (!ft_strncmp(current->value, "pwd", 4))
		return (ft_pwd(current->args, data), 1);
	else if (!ft_strncmp(current->value, "unset", 6))
		return (ft_unset(current->args, data), 1);
	return (0);
}

static void	execute_commandpath(t_node *ast, char **args, t_minishell *data)
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
	else if (!pid)
	{
		path = get_command_path(args[0], data);
		if (!path)
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
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
		else
			data->last_exit_status = 1;
	}
}

void	execute_command(t_node *ast, t_minishell *data)
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
	if (!execute_builtins(ast, data))
		execute_commandpath(ast, args, data);
	free_args(args, data);
}
