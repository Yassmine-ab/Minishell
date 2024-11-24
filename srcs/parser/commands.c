/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:44:17 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/24 17:19:32 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*parse_group(int *i, t_minishell *data)
{
	t_node	*group_node;

	(*i)++;
	group_node = create_node(NODE_GROUP, NULL, &data->gc);
	group_node->left = parse_expression(i, data);
	if (data->tokens[*i].type != PARENTHESIS_CLOSE)
	{
		fprintf(stderr, "Expected close parenthesis\n");
		gc_free(group_node, &data->gc);
		return (NULL);
	}
	(*i)++;
	return (group_node);
}

static int	parse_arguments(int *i, t_node *cmd_node, t_minishell *data)
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
	return (1);
}

static t_node	*parse_simple_command(int *i, t_minishell *data)
{
	t_node	*cmd_node;

	if (data->tokens[*i].type != COMMAND)
		return (fprintf(stderr, "Expected command\n"), NULL);
	cmd_node = create_node(NODE_COMMAND, data->tokens[(*i)++].value, &data->gc);
	if (!parse_arguments(i, cmd_node, data))
		return (NULL);
	while (data->tokens[*i].type == STDOUT || data->tokens[*i].type == STDIN
		|| data->tokens[*i].type == STDOUT_APPEND
		|| data->tokens[*i].type == HEREDOC)
	{
		if (data->tokens[*i].type == STDOUT || data->tokens[*i].type == STDIN
			|| data->tokens[*i].type == STDOUT_APPEND)
		{
			if (!parse_redirection(i, &cmd_node, data))
				return (NULL);
		}
		else if (data->tokens[*i].type == HEREDOC)
		{
			if (!parse_heredoc(i, &cmd_node, data))
				return (NULL);
		}
	}
	return (cmd_node);
}

t_node	*parse_command(int *i, t_minishell *data)
{
	t_node	*cmd_node;

	if (data->tokens[*i].type == PARENTHESIS_OPEN)
		cmd_node = parse_group(i, data);
	else
		cmd_node = parse_simple_command(i, data);
	return (cmd_node);
}
