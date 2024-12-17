/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 03:42:11 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/17 02:31:12 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	execute_redir(t_node *ast, t_minishell *data)
// {
// 	int		fd;
// 	pid_t	pid;
// 	int		status;

// 	fd = open(ast->right->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		ft_putstr_fd("open error\n", STDERR_FILENO);
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		ft_putstr_fd("fork error\n", STDERR_FILENO);
// 		return ;
// 	}
// 	if (pid == 0)
// 	{
// 		dup2(fd, STDOUT_FILENO);
// 		close(fd);
// 		execute_ast(ast->left, data);
// 		exit(0);
// 	}
// 	else
// 		waitpid(pid, &status, 0);
// }

static void	process_heredoc(t_node *node, t_minishell *data)
{
	char	*line;
	int		status;
	pid_t	hd_pid;
	char	*limiter;
	char	*msg;

	limiter = node->right->value;
	hd_pid = fork();
	if (hd_pid == -1)
		error("Fork failed", 1, &data->gc);
	if (!hd_pid)
	{
		close_fd(&data->here_doc[READ_END]);
		while (1)
		{
			ft_putstr_fd("heredoc > ", STDOUT_FILENO);
			line = get_next_line(STDIN_FILENO);
			if (!line)
			{
				msg = ft_strjoin_gc("minishell: warning: here-document delimited by end-of-file (wanted \"", limiter, &data->gc);
				msg = ft_strjoin_gc(msg, "\")\n", &data->gc);
				ft_putstr_fd(msg, STDERR_FILENO);
				break ;
			}
			if (!ft_strncmp(line, limiter, ft_strlen(limiter))
				&& line[ft_strlen(limiter)] == '\n')
			{
				free(line);
				break ;
			}
			if (!node->quoted)
				expand_variables(line, data);
			ft_putstr_fd(line, data->here_doc[WRITE_END]);
			free(line);
		}
		close_fd(&data->here_doc[WRITE_END]);
		exit(EXIT_SUCCESS);
	}
	close_fd(&data->here_doc[WRITE_END]);
	waitpid(hd_pid, &status, 0);
	if (WIFEXITED(status))
		data->heredoc_status = WEXITSTATUS(status);
}

void	execute_heredoc(t_node *ast, t_minishell *data)
{
	if (pipe(data->here_doc) == -1)
		error("Failed to create here_doc pipe", 1, &data->gc);
	process_heredoc(ast->left, data);
	if (dup2(data->here_doc[READ_END], STDIN_FILENO) == -1)
		error("Failed to redirect stdin for heredoc", 1, &data->gc);
	close_fd(&data->here_doc[READ_END]);
}

void	execute_redirections(t_node *redir_node, t_minishell *data)
{
	int	fd;

	while (redir_node)
	{
		if (redir_node->type == NODE_REDIR)
		{
			if (ft_strncmp(redir_node->value, ">", 2) == 0)
				fd = open(redir_node->right->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else if (ft_strncmp(redir_node->value, ">>", 3) == 0)
				fd = open(redir_node->right->value, O_CREAT | O_WRONLY | O_APPEND, 0644);
			else if (ft_strncmp(redir_node->value, "<", 2) == 0)
				fd = open(redir_node->right->value, O_RDONLY);
			if (fd == -1)
				error("Failed to open file for redirection", 1, &data->gc);
			if (dup2(fd, (redir_node->fd == -1) ? ((redir_node->value[0] == '<') ? STDIN_FILENO : STDOUT_FILENO) : redir_node->fd) == -1)
				error("Failed to duplicate file descriptor", 1, &data->gc);
			close(fd);
		}
		else if (redir_node->type == NODE_HEREDOC)
			execute_heredoc(redir_node, data);
		redir_node = redir_node->next;
	}
}
