/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:04:05 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/13 21:50:59 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipe(t_node *ast, t_minishell *data)
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

void	execute_and(t_node *ast, t_minishell *data)
{
	execute_ast(ast->left, data);
	if (data->last_exit_status == 0)
		execute_ast(ast->right, data);
}

void	execute_or(t_node *ast, t_minishell *data)
{
	execute_ast(ast->left, data);
	if (data->last_exit_status != 0)
		execute_ast(ast->right, data);
}

void	execute_builtins(t_node *current, t_minishell *data)
{
	if (!ft_strncmp(current->value, "cd", 3))
		ft_cd(current, data);
	else if (!ft_strncmp(current->value, "echo", 5))
		ft_echo(current, data);
	else if (!ft_strncmp(current->value, "env", 4))
		ft_env(current, data);
	else if (!ft_strncmp(current->value, "exit", 5))
		ft_exit(current, data);
	else if (!ft_strncmp(current->value, "export", 7))
		ft_export(current, data);
	else if (!ft_strncmp(current->value, "pwd", 4))
		ft_pwd(current, data);
	else if (!ft_strncmp(current->value, "unset", 6))
		ft_unset(current, data);
	else
	{
		ft_putstr_fd(current->value, STDERR_FILENO);
		ft_putstr_fd(": Commande non reconnue\n", STDERR_FILENO);
		data->last_exit_status = 127;
	}
}

static void	concatenate_adjacent_nodes(t_node *node, t_minishell *data)
{
	t_node	*current;
	t_node	*temp;
	char	*combined;

	current = node;
	while (current && current->next)
	{
		if (!current->space_after)
		{
			combined = ft_strjoin_gc(current->value, current->next->value, &data->gc);
			gc_free(current->value, &data->gc);
			current->value = combined;
			temp = current->next;
			current->next = temp->next;
			gc_free(temp->value, &data->gc);
			gc_free(temp, &data->gc);
		}
		else
			current = current->next;
	}
}
void	execute_ast(t_node *ast, t_minishell *data)
{
	if (!ast)
		return ;

	while (ast)
	{
		if (ast->type == NODE_COMMAND)
		{
			expand_variables(ast->left, data);
			expand_variables(ast, data);
			concatenate_adjacent_nodes(ast, data);
			concatenate_adjacent_nodes(ast->left, data);
			execute_builtins(ast, data);
		}
		else if (ast->left && ast->left->type == NODE_HEREDOC)
		{
			process_here_doc(ast->left, data);
			if (dup2(data->here_doc[READ_END], STDIN_FILENO) == -1)
				error("Failed to redirect stdin for heredoc", 1, &data->gc);
			close_fd(&data->here_doc[READ_END]);
		}
		else if (ast->type == NODE_PIPE)
			execute_pipe(ast, data);
		// else if (ast->type == NODE_REDIR)
		// 	execute_redir(ast, data);
		else if (ast->type == NODE_AND)
			execute_and(ast, data);
		else if (ast->type == NODE_OR)
			execute_or(ast, data);
		// else if (ast->type == NODE_GROUP)
		// 	execute_group(ast, data);
		// else if (ast->type == NODE_LIMITER)
		// 	execute_limiter(ast, data);
		// else if (ast->type == NODE_HEREDOC)
		// 	execute_heredoc(ast, data);
		// else if (ast->type == NODE_FD)
		// 	printf("Executing fd\n");
		// else if (ast->type == NODE_FILE)
		// 	printf("Executing file\n");
		ast = ast->next;
	}
}
