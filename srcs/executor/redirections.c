/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 03:42:11 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/22 19:47:01 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_heredoc(t_node *redir, t_minishell *data)
{
	char	*limiter;
	char	*line;
	pid_t	hd_pid;
	int		status;

	pipe(data->here_doc);
	hd_pid = fork();
	if (hd_pid == 0)
	{
		data->is_child_process = 1;
		safe_close(&data->here_doc[READ_END]);
		limiter = redir->right->value;
		while (1)
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("heredoc > ", STDOUT_FILENO);
			line = get_next_line(STDIN_FILENO);
			if (line == NULL
				|| (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
					&& line[ft_strlen(limiter)] == '\n'))
				break ;
			ft_putstr_fd(line, data->here_doc[WRITE_END]);
			gc_free(line, &data->gc);
		}
		safe_close(&data->here_doc[WRITE_END]);
		exit(EXIT_SUCCESS);
	}
	safe_close(&data->here_doc[WRITE_END]);
	waitpid(hd_pid, &status, 0);
	if (WIFEXITED(status))
		data->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		data->last_exit_status = 128 + WTERMSIG(status);
		data->child_end_with_signal = true;
	}
}

static void	redirect_fd(t_node *redir, t_minishell *data)
{
	int	fd;

	if (ft_strncmp(redir->value, ">", 2) == 0)
	{
		fd = open(redir->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (dup2(fd, STDOUT_FILENO) == -1)
			error("Failed to redirect output to file", 1, data);
		safe_close(&fd);
	}
	else if (ft_strncmp(redir->value, ">>", 3) == 0)
	{
		fd = open(redir->right->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (dup2(fd, STDOUT_FILENO) == -1)
			error("Failed to redirect output to file", 1, data);
		safe_close(&fd);
	}
	else if (ft_strncmp(redir->value, "<", 2) == 0)
	{
		fd = open(redir->right->value, O_RDONLY, 0644);
		if (dup2(fd, STDIN_FILENO) == -1)
			error("Failed to redirect input from file", 1, data);
		safe_close(&fd);
	}
}

void	execute_redirections(t_node *redir, t_minishell *data)
{
	while (redir)
	{
		if (redir->type == NODE_REDIR)
			redirect_fd(redir, data);
		else if (redir->type == NODE_HEREDOC)
		{
			execute_heredoc(redir, data);
			if (data->in_command)
			{
				if (dup2(data->here_doc[READ_END], STDIN_FILENO) == -1)
					error("Failed to redirect heredoc to stdin", 1, data);
				safe_close(&data->here_doc[READ_END]);
			}
		}
		redir = redir->next;
	}
}
