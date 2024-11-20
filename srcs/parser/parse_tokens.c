/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 01:49:39 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/19 09:53:21 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_tokens(t_token *tokens, t_gc *gc)
{
	t_node	*root;
	t_node	*current_command;
	int		i;

	i = 0;
	root = NULL;
	current_command = NULL;
	while (tokens[i].type != END)
	{
		if (tokens[i].type == COMMAND)
		{
			t_node *command_node = create_ast_node(NODE_COMMAND, tokens[i].value, gc);
			if (!root)
				root = command_node;
			else if (current_command)
				current_command->next = command_node;
			current_command = command_node;
		}
		else if (tokens[i].type == PIPE)
		{
			t_node *pipe_node = create_ast_node(NODE_OPERATOR, "|", gc);
			pipe_node->left = root;
			pipe_node->right = parse_tokens(tokens + i + 1, gc);
			return (pipe_node);
		}
		else if (tokens[i].type == AND)
		{
			t_node *and_node = create_ast_node(NODE_OPERATOR, "&&", gc);
			and_node->left = root;
			and_node->right = parse_tokens(tokens + i + 1, gc);
			return (and_node);
		}
		else if (tokens[i].type == OR)
		{
			t_node *or_node = create_ast_node(NODE_OPERATOR, "||", gc);
			or_node->left = root;
			or_node->right = parse_tokens(tokens + i + 1, gc);
			return (or_node);
		}
		else if (tokens[i].type == STDOUT || tokens[i].type == STDOUT_APPEND)
		{
			t_node *redir_node = create_ast_node(NODE_REDIRECTION, tokens[i].type == STDOUT ? ">" : ">>", gc);
			if (tokens[i + 1].type == FILENAME)
				redir_node->right = create_ast_node(NODE_ARGUMENT, tokens[++i].value, gc);
			if (current_command)
				current_command->next = redir_node;
		}
		else if (tokens[i].type == PARENTHESIS_OPEN)
		{
			t_node *group_node = create_ast_node(NODE_GROUP, NULL, gc);
			group_node->left = parse_tokens(tokens + i + 1, gc);
			root = group_node;
		}
		i++;
	}
	return (root);
}
