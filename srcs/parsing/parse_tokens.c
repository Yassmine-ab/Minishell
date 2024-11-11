/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 01:49:39 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/11 02:05:54 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_tokens(t_token *tokens)
{
	t_node	*root;
	t_node	*current_command;
	t_node	*current_arg;
	int			i;

	i = 0;
	root = NULL;
	current_command = NULL;
	current_arg = NULL;
	while (tokens[i].type != END)
	{
		if (tokens[i].type == COMMAND)
		{
			t_node *command_node = create_ast_node(NODE_COMMAND, tokens[i].value);
			if (!root)
				root = command_node;
			else if (current_command)
				current_command->next = command_node;
			current_command = command_node;
			current_arg = NULL;
		}
		else if (tokens[i].type == ARGUMENT)
		{
			t_node *arg_node = create_ast_node(NODE_ARGUMENT, tokens[i].value);
			if (current_arg)
				current_arg->next = arg_node;
			else
				current_command->next = arg_node;
			current_arg = arg_node;
		}
		else if (tokens[i].type == PIPE)
		{
			t_node *pipe_node = create_ast_node(NODE_OPERATOR, "|");
			pipe_node->left = root;
			pipe_node->right = parse_tokens(tokens + i + 1);
			return (pipe_node);
		}
		else if (tokens[i].type == AND)
		{
			t_node *and_node = create_ast_node(NODE_OPERATOR, "&&");
			and_node->left = root;
			and_node->right = parse_tokens(tokens + i + 1);
			return (and_node);
		}
		else if (tokens[i].type == OR)
		{
			t_node *or_node = create_ast_node(NODE_OPERATOR, "||");
			or_node->left = root;
			or_node->right = parse_tokens(tokens + i + 1);
			return (or_node);
		}
		else if (tokens[i].type == STDOUT || tokens[i].type == STDOUT_APPEND)
		{
			t_node *redir_node = create_ast_node(NODE_REDIRECTION, tokens[i].type == STDOUT ? ">" : ">>");
			if (tokens[i + 1].type == FILENAME)
				redir_node->right = create_ast_node(NODE_ARGUMENT, tokens[++i].value);
			if (current_command)
				current_command->next = redir_node;
		}
		else if (tokens[i].type == PARENTHESIS_OPEN)
		{
			t_node *group_node = create_ast_node(NODE_GROUP, NULL);
			group_node->left = parse_tokens(tokens + i + 1);
			root = group_node;
		}
		i++;
	}
	return (root);
}
