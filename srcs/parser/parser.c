/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:44:17 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/27 23:20:09 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*parse_group(int *i, t_minishell *data)
{
	t_node	*group_node;

	(*i)++;
	group_node = create_node(NODE_GROUP, NULL, &data->gc);
	group_node->left = parse_expression(i, data);
	if (!group_node->left)
		return (gc_free(group_node, &data->gc), NULL);
	if (data->tokens[*i].type != PARENTHESIS_CLOSE)
		error("Expected close parenthesis\n", 1, &data->gc);
	(*i)++;
	return (group_node);
}

static void	parse_arguments(int *i, t_node *cmd_node, t_minishell *data)
{
	t_node	*arg_node;
	t_node	*last;

	while (data->tokens[*i].type == ARGUMENT)
	{
		arg_node = create_node(NODE_ARG, data->tokens[*i].value, &data->gc);
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

static t_node	*parse_simple_command(int *i, t_minishell *data)
{
	t_node	*cmd_node;

	cmd_node = create_node(NODE_COMMAND, "", &data->gc);
	while (data->tokens[*i].type == STDOUT || data->tokens[*i].type == STDIN
		|| data->tokens[*i].type == STDOUT_APPEND
		|| data->tokens[*i].type == HEREDOC || (data->tokens[*i].type == COMMAND
		&& is_number(data->tokens[*i].value) && is_redir_following(*i, data)))
	{
		if (data->tokens[*i].type == STDOUT || data->tokens[*i].type == STDIN
			|| data->tokens[*i].type == STDOUT_APPEND || (data->tokens[*i].type == COMMAND
		&& is_number(data->tokens[*i].value) && is_redir_following(*i, data)))
			parse_redirection(i, &cmd_node, data);
		else if (data->tokens[*i].type == HEREDOC)
			parse_heredoc(i, &cmd_node, data);
	}
	if (data->tokens[*i].type == COMMAND)
	{
		cmd_node->value = data->tokens[(*i)++].value;
		parse_arguments(i, cmd_node, data);
	}
	else
		error("Expected command\n", 1, &data->gc);
	while (data->tokens[*i].type == STDOUT || data->tokens[*i].type == STDIN
		|| data->tokens[*i].type == STDOUT_APPEND
		|| data->tokens[*i].type == HEREDOC || (data->tokens[*i].type == COMMAND
		&& is_number(data->tokens[*i].value) && is_redir_following(*i, data)))
	{
		if (data->tokens[*i].type == STDOUT || data->tokens[*i].type == STDIN
			|| data->tokens[*i].type == STDOUT_APPEND || (data->tokens[*i].type == COMMAND
		&& is_number(data->tokens[*i].value) && is_redir_following(*i, data)))
			parse_redirection(i, &cmd_node, data);
		else if (data->tokens[*i].type == HEREDOC)
			parse_heredoc(i, &cmd_node, data);
	}
	return (cmd_node);
}

t_node	*parse_command(int *i, t_minishell *data)
{
	t_node			*cmd_node;

	if (data->tokens[*i].type == PARENTHESIS_OPEN)
		cmd_node = parse_group(i, data);
	else
		cmd_node = parse_simple_command(i, data);
	if (data->tokens[*i].type == AND || data->tokens[*i].type == OR)
	{
		(*i)++;
		if (data->tokens[*i].type == AND || data->tokens[*i].type == OR)
			error ("Consecutive operators are not allowed", 1, &data->gc);
		(*i)--;
	}
	return (cmd_node);
}
