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

static void execute_pipeline(t_node *ast, t_minishell *data)
{
	int pipe_fd[2];
	int prev_fd = -1;
	pid_t pid;
	int status;

	while (ast && ast->type == NODE_PIPE)
	{
		if (pipe(pipe_fd) == -1)
			error("Failed to create pipe", 1, &data->gc);

		pid = fork();
		if (pid == -1)
			error("Fork failed", 1, &data->gc);
		else if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					error("Failed to redirect input from pipe", 1, &data->gc);
				safe_close(&prev_fd);
			}
			if (dup2(pipe_fd[WRITE_END], STDOUT_FILENO) == -1)
				error("Failed to redirect output to pipe", 1, &data->gc);
			safe_close(&pipe_fd[WRITE_END]);
			safe_close(&pipe_fd[READ_END]);

			execute_ast(ast->left, data, true);
			exit(data->last_exit_status);
		}
		else
		{
			safe_close(&pipe_fd[WRITE_END]);
			if (prev_fd != -1)
			{
				safe_close(&prev_fd);
				printf("Parent: Closed previous READ_END (%d)\n", prev_fd);
			}
			prev_fd = pipe_fd[READ_END];
			printf("Parent: Set prev_fd to READ_END (%d)\n", prev_fd);
			ast = ast->right;
		}
	}
	if (ast)
	{
		pid = fork();
		if (pid == -1)
			error("Fork failed", 1, &data->gc);
		else if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					error("Failed to redirect input from pipe", 1, &data->gc);
				safe_close(&prev_fd);
			}

			execute_ast(ast, data, false);
			exit(data->last_exit_status);
		}
		else
		{
			if (prev_fd != -1)
			{
				safe_close(&prev_fd);
				printf("Parent: Closed last READ_END (%d)\n", prev_fd);
			}
		}
	}
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
		printf("Executing node: %s\n", ast->value);
		if (ast->type == NODE_COMMAND)
			execute_command(ast, data, in_pipeline);
		else if (ast->type == NODE_PIPE)
		{
			execute_pipeline(ast, data);
			return ;
		}
		else if (ast->type == NODE_AND)
			execute_and(ast, data);
		else if (ast->type == NODE_OR)
			execute_or(ast, data);
		else if (ast->type == NODE_GROUP)
			execute_group(ast, data);
		ast = ast->next;
	}
}
