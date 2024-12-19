/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:04:05 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/19 11:36:56 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipe(t_node *ast, t_minishell *data)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	int		status;

	in_fd = STDIN_FILENO;
	while (ast && ast->type == NODE_PIPE)
	{
		if (pipe(pipe_fd) == -1)
			error("Failed to create pipe", 1, &data->gc);
		pid = fork();
		if (pid == -1)
			error("Fork failed", 1, &data->gc);
		if (pid == 0)
		{
			dup2(in_fd, STDIN_FILENO);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close_fd(&pipe_fd[0]);
			execute_ast(ast->left, data);
			exit(EXIT_FAILURE);
		}
		else
		{
			close_fd(&pipe_fd[1]);
			if (in_fd != STDIN_FILENO)
				close_fd(&in_fd);
			in_fd = pipe_fd[0];
			ast = ast->right;
		}
	}
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close_fd(&in_fd);
	}
	pid = fork();
	if (pid == -1)
		error("Fork failed", 1, &data->gc);
	if (pid == 0)
	{
		execute_ast(ast, data);
		exit(EXIT_FAILURE);
	}
	close_fd(&in_fd);
	while (wait(&status) > 0)
		;
	data->last_exit_status = WEXITSTATUS(status);
}

static void	execute_and(t_node *ast, t_minishell *data)
{
	execute_ast(ast->left, data);
	if (data->last_exit_status == 0)
		execute_ast(ast->right, data);
}

static void	execute_or(t_node *ast, t_minishell *data)
{
	execute_ast(ast->left, data);
	if (data->last_exit_status != 0)
		execute_ast(ast->right, data);
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
		execute_ast(ast->left, data);
		exit(data->last_exit_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		data->last_exit_status = WEXITSTATUS(status);
	}
}

void	execute_ast(t_node *ast, t_minishell *data)
{
	if (!ast)
		return ;
	while (ast)
	{
		if (ast->type == NODE_COMMAND)
			execute_command(ast, data);
		else if (ast->type == NODE_PIPE)
			execute_pipe(ast, data);
		else if (ast->type == NODE_AND)
			execute_and(ast, data);
		else if (ast->type == NODE_OR)
			execute_or(ast, data);
		else if (ast->type == NODE_GROUP)
			execute_group(ast, data);
		if (ast->type != NODE_PIPE)
			ast = ast->next;
		else
			break ;
	}
}
