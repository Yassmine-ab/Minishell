/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 01:49:39 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/24 17:18:46 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_node(t_node_type type, char *value, t_gc *gc)
{
	t_node	*node;

	node = gc_malloc(sizeof(t_node), gc);
	node->type = type;
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	node->next = NULL;
	return (node);
}

static t_node	*parse_pipeline(int *i, t_minishell *data)
{
	t_node	*left;
	t_node	*pipe_node;
	t_node	*right;

	left = parse_command(i, data);
	while (data->tokens[*i].type == PIPE)
	{
		pipe_node = create_node(NODE_PIPE, data->tokens[*i].value, &data->gc);
		(*i)++;
		right = parse_command(i, data);
		pipe_node->left = left;
		pipe_node->right = right;
		left = pipe_node;
	}
	return (left);
}

int	parse_redirection(int *i, t_node **cmd_node, t_minishell *data)
{
	t_node	*redir_node;
	t_node	*file_node;
	t_node	*last;

	if (data->tokens[*i].type != STDOUT && data->tokens[*i].type != STDIN
		&& data->tokens[*i].type != STDOUT_APPEND)
		return (fprintf(stderr, "Invalid redirection operator\n"), 0);
	redir_node = create_node(NODE_REDIR, data->tokens[(*i)++].value, &data->gc);
	if (data->tokens[*i].type != FILENAME)
		return (fprintf(stderr, "Expected filename after redirection\n"), 0);
	file_node = create_node(NODE_FILE, data->tokens[(*i)++].value, &data->gc);
	redir_node->right = file_node;
	last = (*cmd_node)->left;
	if (!last)
		(*cmd_node)->left = redir_node;
	else
	{
		while (last->next)
			last = last->next;
		last->next = redir_node;
	}
	return (1);
}

int	parse_heredoc(int *i, t_node **cmd_node, t_minishell *data)
{
	t_node	*hd_node;
	t_node	*lim_node;
	t_node	*last;

	hd_node = create_node(NODE_HEREDOC, data->tokens[(*i)++].value, &data->gc);
	if (data->tokens[*i].type != LIMITER)
		return (fprintf(stderr, "Expected Limiter after heredoc\n"), 0);
	lim_node = create_node(NODE_LIMITER, data->tokens[(*i)++].value, &data->gc);
	hd_node->right = lim_node;
	last = (*cmd_node)->left;
	if (!last)
		(*cmd_node)->left = hd_node;
	else
	{
		while (last->next)
			last = last->next;
		last->next = hd_node;
	}
	return (1);
}

t_node	*parse_expression(int *i, t_minishell *data)
{
	t_node		*left;
	t_node_type	op_type;
	t_node		*operator_node;
	t_node		*right;

	left = parse_pipeline(i, data);
	while (data->tokens[*i].type == AND || data->tokens[*i].type == OR)
	{
		if (data->tokens[*i].type == AND)
			op_type = NODE_AND;
		else
			op_type = NODE_OR;
		operator_node = create_node(op_type, data->tokens[(*i)++].value, &data->gc);
		right = parse_pipeline(i, data);
		operator_node->left = left;
		operator_node->right = right;
		left = operator_node;
	}
	return (left);
}
