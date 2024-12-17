/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 01:49:39 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/17 10:25:51 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_node(t_node_type type, t_token token, t_gc *gc)
{
	t_node	*node;

	node = gc_malloc(sizeof(t_node), gc);
	node->type = type;
	node->value = token.value;
	node->quoted = token.quoted;
	node->space_after = token.space_after;
	node->left = NULL;
	node->right = NULL;
	node->args = NULL;
	node->redirections = NULL;
	node->next = NULL;
	return (node);
}

static t_node	*parse_heredoc(int *i, t_minishell *data)
{
	t_node	*hd_node;
	t_node	*lim_node;

	hd_node = create_node(NODE_HEREDOC, data->tokens[*i], &data->gc);
	(*i)++;
	if (data->tokens[*i].type != LIMITER)
		error ("Expected Limiter after heredoc\n", 1, &data->gc);
	lim_node = create_node(NODE_LIMITER, data->tokens[*i], &data->gc);
	(*i)++;
	hd_node->right = lim_node;
	return (hd_node);
}

t_node	*parse_redirections(int *i, t_minishell *data)
{
	t_node	*redir_node;
	t_node	*redir_list;
	t_node	*last_redir;

	redir_list = NULL;
	while (data->tokens[*i].type == HEREDOC
		|| data->tokens[*i].type == STDIN || data->tokens[*i].type == STDOUT
		|| data->tokens[*i].type == STDOUT_APPEND)
	{
		if (data->tokens[*i].type == HEREDOC)
			redir_node = parse_heredoc(i, data);
		else
		{
			redir_node = create_node(NODE_REDIR, data->tokens[*i], &data->gc);
			(*i)++;
			redir_node->right = create_node(NODE_FILE, data->tokens[(*i)++], \
			&data->gc);
		}
		if (!redir_list)
			redir_list = redir_node;
		else
			last_redir->next = redir_node;
		last_redir = redir_node;
	}
	return (redir_list);
}

static t_node	*parse_pipeline(int *i, t_minishell *data)
{
	t_node	*left;
	t_node	*pipe_node;

	left = parse_command(i, data);
	if (!left)
	{
		left = parse_redirections(i, data);
		if (!left)
			error("Expected command or redirection\n", 1, &data->gc);
	}
	while (data->tokens[*i].type == PIPE)
	{
		if (left == NULL)
			error("Missing command before pipe\n", 1, &data->gc);
		pipe_node = create_node(NODE_PIPE, data->tokens[*i], &data->gc);
		(*i)++;
		pipe_node->right = parse_command(i, data);
		if (pipe_node->right == NULL)
			error("Missing command after pipe\n", 1, &data->gc);
		pipe_node->left = left;
		left = pipe_node;
	}
	return (left);
}

t_node	*parse_expression(int *i, t_minishell *data)
{
	t_node		*left;
	t_node_type	op_type;
	t_node		*op_node;

	left = parse_pipeline(i, data);
	while (data->tokens[*i].type == AND || data->tokens[*i].type == OR)
	{
		if (left == NULL)
			error ("Missing operand before operator\n", 1, &data->gc);
		if (data->tokens[*i].type == AND)
			op_type = NODE_AND;
		else
			op_type = NODE_OR;
		op_node = create_node(op_type, data->tokens[*i], &data->gc);
		(*i)++;
		op_node->right = parse_pipeline(i, data);
		if (op_node->right == NULL)
			error ("Missing operand after operator\n", 1, &data->gc);
		op_node->left = left;
		left = op_node;
	}
	return (left);
}
