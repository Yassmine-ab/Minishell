/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:04:05 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/23 04:08:55 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (pid == 0)
		execute_ast(ast->left, data, false);
	else
	{
		waitpid(pid, &status, 0);
		handle_child_exit(status, data);
	}
}

void	execute_ast(t_node *ast, t_minishell *data, bool in_pipeline)
{
	if (ast == NULL)
		return ;
	init_signal_exec();
	while (ast)
	{
		if (ast->type == NODE_COMMAND)
			execute_command(ast, data, in_pipeline);
		else if (ast->type == NODE_PIPE)
		{
			execute_pipeline(ast, data);
			return ;
		}
		else if (ast->type == NODE_REDIR || ast->type == NODE_HEREDOC)
			execute_redirections(ast, data);
		else if (ast->type == NODE_AND)
			execute_and(ast, data);
		else if (ast->type == NODE_OR)
			execute_or(ast, data);
		else if (ast->type == NODE_GROUP)
			execute_group(ast, data);
		ast = ast->next;
	}
}
