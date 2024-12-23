/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:44:17 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/23 13:21:32 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*parse_group(int *i, t_minishell *data)
{
	t_node	*group_node;

	group_node = create_node(NODE_GROUP, data->tokens[*i], &data->gc);
	(*i)++;
	group_node->left = parse_expression(i, data);
	if (group_node->left == NULL)
		return (error("Empty parentheses", 1, data), NULL);
	(*i)++;
	return (group_node);
}

static void	parse_arguments(int *i, t_node *cmd_node, t_minishell *data)
{
	t_node	*arg_node;
	t_node	*last_arg;

	last_arg = NULL;
	while (data->tokens[*i].type == ARGUMENT)
	{
		arg_node = create_node(NODE_ARG, data->tokens[*i], &data->gc);
		if (cmd_node->args == NULL)
			cmd_node->args = arg_node;
		else
			last_arg->next = arg_node;
		last_arg = arg_node;
		(*i)++;
	}
}

static t_node	*parse_simple_command(int *i, t_minishell *data)
{
	t_token	dummy;
	t_node	*cmd_node;
	t_node	*new_cmd;
	t_node	*last_cmd;
	t_node	*redir;
	t_node	*last;

	cmd_node = NULL;
	while (data->tokens[*i].type == COMMAND)
	{
		new_cmd = create_node(NODE_COMMAND, data->tokens[*i], &data->gc);
		if (cmd_node == NULL)
		{
			cmd_node = new_cmd;
			last_cmd = new_cmd;
		}
		else
		{
			last_cmd->next = new_cmd;
			last_cmd = new_cmd;
		}
		(*i)++;
		parse_arguments(i, cmd_node, data);
		redir = parse_redirections(i, data);
		if (redir)
		{
			if (cmd_node == NULL)
			{
				ft_bzero(&dummy, sizeof(t_token));
				dummy.type = COMMAND;
				dummy.value = ft_strdup("");
				dummy.quoted = false;
				dummy.space_after = true;
				cmd_node = create_node(NODE_COMMAND, dummy, &data->gc);
			}
			if (cmd_node->redirections == NULL)
				cmd_node->redirections = redir;
			else
			{
				last = cmd_node->redirections;
				while (last->next)
					last = last->next;
				last->next = redir;
			}
		}
	}
	return (cmd_node);
}

t_node	*parse_command(int *i, t_minishell *data)
{
	t_node	*cmd_node;

	cmd_node = NULL;
	if (data->tokens[*i].type == PARENTHESIS_OPEN)
		cmd_node = parse_group(i, data);
	else if (data->tokens[*i].type == COMMAND)
		cmd_node = parse_simple_command(i, data);
	return (cmd_node);
}
