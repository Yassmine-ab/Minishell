/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:44:17 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/13 03:22:43 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*parse_group(int *i, t_minishell *data)
{
	t_node	*group_node;
	t_token	token;

	token.value = NULL;
	token.quoted = 0;
	token.space_after = 1;
	(*i)++;
	group_node = create_node(NODE_GROUP, token, &data->gc);
	group_node->left = parse_expression(i, data);
	if (!group_node->left)
	{
		gc_free(group_node, &data->gc);
		error("Empty parentheses", 1, &data->gc);
	}
	(*i)++;
	return (group_node);
}

static void	parse_arguments(int *i, t_node *cmd_node, t_minishell *data)
{
	t_node	*arg_node;
	t_node	*last;

	while (data->tokens[*i].type == ARGUMENT)
	{
		arg_node = create_node(NODE_ARG, data->tokens[*i], &data->gc);
		if (!cmd_node->left)
			cmd_node->left = arg_node;
		else
		{
			last = cmd_node->left;
			while (last->next)
				last = last->next;
			last->next = arg_node;
		}
		(*i)++;
	}
}

static void	parse_redirections(int *i, t_node **cmd_node, t_minishell *data)
{
	while (data->tokens[*i].type == STDOUT || data->tokens[*i].type == STDIN
		|| data->tokens[*i].type == STDOUT_APPEND
		|| data->tokens[*i].type == HEREDOC
		|| (data->tokens[*i].type == COMMAND
			&& is_number(data->tokens[*i].value)
			&& is_redir_following(*i, data)))
	{
		if (data->tokens[*i].type == STDOUT || data->tokens[*i].type == STDIN
			|| data->tokens[*i].type == STDOUT_APPEND
			|| (data->tokens[*i].type == COMMAND
				&& is_number(data->tokens[*i].value)
				&& is_redir_following(*i, data)))
			parse_redirection(i, cmd_node, data);
		else if (data->tokens[*i].type == HEREDOC)
			parse_heredoc(i, cmd_node, data);
	}
}

static t_node	*parse_simple_command(int *i, t_minishell *data)
{
	t_node	*cmd_node;
	t_token	token;
	t_node	*new_cmd;
	t_node	*last_cmd = NULL;

	token.value = "";
	token.quoted = 0;
	token.space_after = 1;
	cmd_node = create_node(NODE_COMMAND, token, &data->gc);
	parse_redirections(i, &cmd_node, data);
	while (data->tokens[*i].type == COMMAND)
	{
		new_cmd = create_node(NODE_COMMAND, data->tokens[*i], &data->gc);
		if (!*cmd_node->value)
			cmd_node = new_cmd;
		else
			last_cmd->next = new_cmd;
		last_cmd = new_cmd;
		(*i)++;
		parse_arguments(i, cmd_node, data);
	}
	parse_redirections(i, &cmd_node, data);
	if (!cmd_node)
		error("Expected command\n", 1, &data->gc);
	return (cmd_node);
}

t_node	*parse_command(int *i, t_minishell *data)
{
	t_node	*cmd_node;
	t_node	*current;

	if (data->tokens[*i].type == PARENTHESIS_OPEN)
		cmd_node = parse_group(i, data);
	else
		cmd_node = parse_simple_command(i, data);
	current = cmd_node;
	while (current->next)
		current = current->next;
	if (data->tokens[*i].type == AND || data->tokens[*i].type == OR)
	{
		(*i)++;
		if (data->tokens[*i].type == AND || data->tokens[*i].type == OR)
			error ("Consecutive operators are not allowed", 1, &data->gc);
		(*i)--;
	}
	return (cmd_node);
}
