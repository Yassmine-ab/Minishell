/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:53:50 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/11 22:17:51 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_sigint_heredoc(int signal)
{
	(void)signal;
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(130);
}

static void	execute_here_doc(t_node *node, t_minishell *data)
{
	char	*line;
	int		status;
	pid_t	hd_pid;
	char	*limiter;

	limiter = node->right->value;
	hd_pid = fork();
	if (hd_pid == -1)
		error("Fork failed", 1, &data->gc);
	if (!hd_pid)
	{
		signal(SIGINT, handle_sigint_heredoc);
		signal(SIGQUIT, SIG_IGN);
		close_fd(&data->here_doc[READ_END]);
		while (1)
		{
			write(1, "heredoc > ", 10);
			line = get_next_line(STDIN_FILENO);
			if (!line || (!ft_strncmp(line, limiter, ft_strlen(limiter))
					&& line[ft_strlen(limiter)] == '\n'))
				break ;
			if (node->quoted)
				node->value = ft_strdup_gc(line, &data->gc);
			else
				expand_variables(node, data);
			ft_putstr_fd(node->value, data->here_doc[WRITE_END]);
			free(line);
		}
		close_fd(&data->here_doc[WRITE_END]);
		exit(EXIT_SUCCESS);
	}
	close_fd(&data->here_doc[WRITE_END]);
	waitpid(hd_pid, &status, 0);
	if (WIFEXITED(status))
		WEXITSTATUS(status);
}

void	process_here_doc(t_node *node, t_minishell *data)
{
	struct sigaction	sa_old;
	struct sigaction	sa_new;

	if (pipe(data->here_doc) == -1)
		error("Failed to create here_doc pipe", 1, &data->gc);
	sa_new.sa_handler = SIG_IGN;
	sigemptyset(&sa_new.sa_mask);
	sa_new.sa_flags = 0;
	sigaction(SIGINT, &sa_new, &sa_old);
	execute_here_doc(node, data);
	sigaction(SIGINT, &sa_old, NULL);
	signal(SIGQUIT, SIG_IGN);
}
