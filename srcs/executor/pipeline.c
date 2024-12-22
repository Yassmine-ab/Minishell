/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 11:22:57 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/22 14:13:52 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_pid(t_pids *pids, pid_t pid, t_minishell *data)
{
	if (pids->count == pids->capacity)
	{
		pids->capacity *= 2;
		pids->pids = gc_realloc(pids->pids, sizeof(pid_t) * pids->capacity, &data->gc);
	}
	pids->pids[pids->count++] = pid;
}

static void	wait_all_pids(t_pids *pids, t_minishell *data)
{
	int		status;
	pid_t	pid;
	int		i;

	i = -1;
	while (++i < pids->count)
	{
		pid = waitpid(pids->pids[i], &status, 0);
		if (pid == -1)
			error("Waitpid failed", 1, &data->gc);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			data->last_exit_status = 128 + WTERMSIG(status);
			data->child_end_with_signal = true;
		}
	}
	gc_free(pids, &data->gc);
}

void	execute_pipeline(t_node *ast, t_minishell *data)
{
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;

	prev_fd = -1;
	while (ast && ast->type == NODE_PIPE)
	{
		if (pipe(pipe_fd) == -1)
			error("Failed to create pipe", 1, &data->gc);
		pid = fork();
		if (pid == -1)
			error("Fork failed", 1, &data->gc);
		else if (pid == 0)
		{
			data->is_child_process = true;
			signal_child_process();
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
			add_pid(data->pids, pid, data);
			safe_close(&pipe_fd[WRITE_END]);
			if (prev_fd != -1)
				safe_close(&prev_fd);
			prev_fd = pipe_fd[READ_END];
			ast = ast->right;
		}
	}
	execute_ast(ast, data, false);
	wait_all_pids(data->pids, data);
}
