/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:04:05 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/13 03:13:03 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			free(current->value);
			current->value = combined;
			temp = current->next;
			current->next = temp->next;
			free(temp->value);
			free(temp);
		}
		else
			current = current->next;
	}
}

void execute_ast(t_node *cmd_node, t_minishell *data)
{
	if (!cmd_node)
		return ;
	while (cmd_node)
	{
		if (cmd_node->type == NODE_COMMAND)
		{
			expand_variables(cmd_node->left, data);
			expand_variables(cmd_node, data);
			concatenate_adjacent_nodes(cmd_node, data);
			concatenate_adjacent_nodes(cmd_node->left, data);
			if (!ft_strncmp(cmd_node->value, "echo", 5))
				ft_echo(cmd_node);
			else if (!ft_strncmp(cmd_node->value, "cd", 3))
				ft_cd(cmd_node);
			else if (!ft_strncmp(cmd_node->value, "pwd", 4))
				ft_pwd(cmd_node);
		}
		if (cmd_node->left && cmd_node->left->type == NODE_HEREDOC)
		{
			process_here_doc(cmd_node->left, data);
			if (dup2(data->here_doc[READ_END], STDIN_FILENO) == -1)
				error("Failed to redirect stdin for heredoc", 1, &data->gc);
			close_fd(&data->here_doc[READ_END]);
		}
		cmd_node = cmd_node->next;
	}
}