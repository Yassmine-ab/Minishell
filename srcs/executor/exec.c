/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:04:05 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/17 02:30:23 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_pipe(t_node *ast, t_minishell *data)
{
	int	fd[2];
	int	pid[2];
	int	status;

	if (pipe(fd) == -1)
		exit(-1);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		(close(fd[0]), close(fd[1]));
		execute_ast(ast->left, data);
		exit(-1);
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		(close(fd[0]), close(fd[1]));
		execute_ast(ast->right, data);
		exit(-1);
	}
	(close(fd[0]), close(fd[1]));
	waitpid(pid[0], &status, 0);
	waitpid(pid[1], &status, 0);
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
		ast = ast->next;
	}
}
