/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 03:42:11 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/23 05:29:03 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	empty_heredoc(char *limiter, int count_line)
{
	ft_putstr_fd("warning: here-document at line ", STDERR_FILENO);
	ft_putnbr_fd(count_line, STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(limiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

// leaks quand Ctrl+C

static void execute_heredoc(t_node *redir, t_minishell *data)
{
	char	*limiter;
	char	*line;
	pid_t	hd_pid;
	int		status;
	int		count_line;

	pipe(data->here_doc);
	hd_pid = fork();
	if (hd_pid == 0)
	{
		signal_heredoc();
		safe_close(&data->here_doc[READ_END]);
		limiter = redir->right->value;
		count_line = 1;
		while (1)
		{
			if (isatty(STDIN_FILENO))
				line = readline("heredoc > ");
			if (line == NULL)
			{
				empty_heredoc(limiter, count_line);
				break ;
			}
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
				&& line[ft_strlen(limiter)] == '\0')
			{
				gc_free(line, &data->gc);
				break ;
			}
			ft_putstr_fd(line, data->here_doc[WRITE_END]);
			ft_putchar_fd('\n', data->here_doc[WRITE_END]);
			count_line++;
			gc_free(line, &data->gc);
		}
		safe_close(&data->here_doc[WRITE_END]);
		if (data->gc.head)
			gc_cleanup(&data->gc);
		exit(EXIT_SUCCESS);
	}
	safe_close(&data->here_doc[WRITE_END]);
	waitpid(hd_pid, &status, 0);
	handle_child_exit(status, data);
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
