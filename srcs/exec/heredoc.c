/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:53:50 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/02 17:40:18 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_exit(char **line, t_minishell *data, int status)
{
	while (*line)
	{
		free(*line);
		*line = get_next_line(data->here_doc[WRITE_END]);
	}
	close_fd(&data->here_doc[WRITE_END]);
	exit(status);
}

static void	process_here_doc(t_node *heredoc_node, pid_t *hd_pid, t_minishell *data, int *status)
{
	char	*line;
	char	*processed_line;
	char	*limiter;
	bool	is_quoted;

	limiter = heredoc_node->right->value;
	is_quoted = heredoc_node->is_quoted;
	if (pipe(data->here_doc) == -1)
		error("Failed to create here_doc pipe", 1, &data->gc);
	*hd_pid = fork();
	if (*hd_pid == -1)
		error("Fork failed", 1, &data->gc);
	if (!*hd_pid)
	{
		close_fd(&data->here_doc[READ_END]);
		while (1)
		{
			write(1, "heredoc > ", 10);
			line = get_next_line(STDIN_FILENO);
			if (!line || (!ft_strncmp(line, limiter, ft_strlen(limiter))
					&& line[ft_strlen(limiter)] == '\n'))
				break ;
			if (is_quoted)
				processed_line = ft_strdup_gc(line, &data->gc);
			else
				processed_line = expand_variable(line, data);
			ft_putstr_fd(processed_line, data->here_doc[WRITE_END]);
			free(line);
		}
		clean_exit(&line, data, EXIT_SUCCESS);
	}
	close_fd(&data->here_doc[WRITE_END]);
	waitpid(*hd_pid, status, 0);
}

void	execute_command(t_node *cmd_node, t_minishell *data)
{
	pid_t	hd_pid;
	int		status;
	t_node	*node;

	node = cmd_node->left;
	while (node)
	{
		if (node->type == NODE_ARG)
			node->value = expand_variable(node->value, data);
		else if (node->type == NODE_HEREDOC)
		{
			process_here_doc(node, &hd_pid, data, &status);
			dup2(data->here_doc[READ_END], STDIN_FILENO);
			close(data->here_doc[READ_END]);
		}
		node = node->next;
	}
}

