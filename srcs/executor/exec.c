/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:04:05 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/20 17:08:25 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_pipeline(t_node *ast, t_minishell *data)
{
	int pipe_fd[2];
	int prev_fd = -1;
	pid_t pid;

	while (ast && ast->type == NODE_PIPE)
	{
		if (pipe(pipe_fd) == -1)
			error("Failed to create pipe", 1, &data->gc);
		pid = fork();
		if (pid == -1)
			error("Fork failed", 1, &data->gc);
		else if (pid == 0)
		{
			// Child process
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close_fd(&prev_fd);
			}
			dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
			close_fd(&pipe_fd[WRITE_END]);
			close_fd(&pipe_fd[READ_END]);
			execute_ast(ast->left, data, true);
			exit(data->last_exit_status);
		}
		else
		{
			// Parent process
			if (prev_fd != -1)
				close_fd(&prev_fd);
			close_fd(&pipe_fd[WRITE_END]);
			prev_fd = pipe_fd[READ_END];
			ast = (ast->type == NODE_PIPE) ? ast->right : NULL;
		}
	}

	// Wait for all child processes
	int status;
	while (wait(&status) > 0)
		;
	if (WIFEXITED(status))
		data->last_exit_status = WEXITSTATUS(status);
	else
		data->last_exit_status = 1;
}

static void	execute_and(t_node *ast, t_minishell *data)
{
	execute_ast(ast->left, data, false);
	if (data->last_exit_status == 0)
		execute_ast(ast->right, data, false);
}

static void	execute_or(t_node *ast, t_minishell *data)
{
	execute_ast(ast->left, data, false);
	if (data->last_exit_status != 0)
		execute_ast(ast->right, data, false);
}

static void	execute_group(t_node *ast, t_minishell *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error("Fork failed", 1, &data->gc);
	else if (pid == 0)
	{
		execute_ast(ast->left, data, false);
		exit(data->last_exit_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		data->last_exit_status = WEXITSTATUS(status);
	}
}

void	execute_ast(t_node *ast, t_minishell *data, bool in_pipeline)
{
	if (!ast)
		return ;
	while (ast)
	{
		printf("[DEBUG] Executing AST node type: %d\n", ast->type);
		if (ast->type == NODE_COMMAND)
			execute_command(ast, data, in_pipeline);
		else if (ast->type == NODE_PIPE)
			execute_pipeline(ast, data);
		else if (ast->type == NODE_AND)
			execute_and(ast, data);
		else if (ast->type == NODE_OR)
			execute_or(ast, data);
		else if (ast->type == NODE_GROUP)
			execute_group(ast, data);
		if (!in_pipeline)
			ast = ast->next;
	}
}
