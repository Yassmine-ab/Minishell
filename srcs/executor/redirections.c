/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 03:42:11 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/24 22:10:37 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	empty_heredoc(char *limiter, int count_line)
{
	ft_putstr_fd("warning: here-document at line ", STDERR_FILENO);
	ft_putnbr_fd(count_line, STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(limiter, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
}

static void	execute_heredoc(t_node *redir, t_minishell *data)
{
	char	*limiter;
	char	*line;
	int		count_line;

	signal_heredoc();
	data->tmp_file = "/tmp/.heredoc";
	data->tmp_fd = open(data->tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
		ft_putendl_fd(line, data->tmp_fd);
		count_line++;
		gc_free(line, &data->gc);
	}
	safe_close(&data->tmp_fd);
}

static void	redirect_fd(t_node *redir, t_minishell *data)
{
	int	fd;

	if (redir->right->is_single_quoted == false)
		redir->right->value = expand_variables(redir->right->value, data);
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
	else if (ft_strncmp(redir->value, "<", 2) == 0 && data->in_command == true)
	{
		fd = open(redir->right->value, O_RDONLY, 0644);
		if (dup2(fd, STDIN_FILENO) == -1)
			error("Failed to redirect input from file", 1, data);
		safe_close(&fd);
	}
}

void	execute_redirections(t_node *redir, t_minishell *data)
{
	if (redir->type == NODE_REDIR)
		redirect_fd(redir, data);
	else if (redir->type == NODE_HEREDOC)
		execute_heredoc(redir, data);
}
