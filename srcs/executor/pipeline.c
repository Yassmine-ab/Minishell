/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 11:22:57 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/23 10:42:04 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_all_pids(t_minishell *data)
{
	int		status;
	int		i;

	i = -1;
	while (++i < 2)
	{
		waitpid(data->pids[i], &status, 0);
		handle_child_exit(status, data);
	}
}

static int	safe_fork(t_minishell *data)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		gc_cleanup(&data->gc);
	if (pid == 0)
	{
		data->is_child_process = true;
		signal_child_process();
	}
	return (pid);
}

void	execute_pipeline(t_node *ast, t_minishell *data)
{
	if (pipe(data->pipe_fd) == -1)
		gc_cleanup(&data->gc);
	data->pids[0] = safe_fork(data);
	if (data->pids[0] == 0)
	{
		dup2(data->pipe_fd[WRITE_END], STDOUT_FILENO);
		close(data->pipe_fd[READ_END]);
		close(data->pipe_fd[WRITE_END]);
		execute_ast(ast->left, data, true);
		gc_cleanup(&data->gc);
	}
	data->pids[1] = safe_fork(data);
	if (data->pids[1] == 0)
	{
		dup2(data->pipe_fd[READ_END], STDIN_FILENO);
		close(data->pipe_fd[READ_END]);
		close(data->pipe_fd[WRITE_END]);
		execute_ast(ast->right, data, true);
		gc_cleanup(&data->gc);
	}
	close(data->pipe_fd[READ_END]);
	close(data->pipe_fd[WRITE_END]);
	wait_all_pids(data);
}
