/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 01:49:39 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/16 02:04:12 by yaabdall         ###   ########.fr       */
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
	node->fd = -1;
	node->left = NULL;
	node->right = NULL;
	node->args = NULL;
	node->redirections = NULL;
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
		if (left == NULL)
			error("Missing command before pipe\n", 1, &data->gc);
		pipe_node = create_node(NODE_PIPE, data->tokens[*i], &data->gc);
		(*i)++;
		right = parse_command(i, data);
		if (right == NULL)
			error("Missing command after pipe\n", 1, &data->gc);
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
	t_node	*fd_node;
	t_node	*last_redir;
	int		fd;
	t_token	token;

	token.value = "";
	token.quoted = 0;
	token.space_after = 1;
	fd = -1;
	if (data->tokens[*i].type == COMMAND && is_number(data->tokens[*i].value))
	{
		fd = ft_atoi(data->tokens[*i].value);
		(*i)++;
	}
	if (data->tokens[*i].type != STDOUT && data->tokens[*i].type != STDIN
		&& data->tokens[*i].type != STDOUT_APPEND)
		error("Invalid redirection operator\n", 1, &data->gc);
	redir_node = create_node(NODE_REDIR, data->tokens[*i], &data->gc);
	(*i)++;
	if (fd != -1)
	{
		fd_node = create_node(NODE_FD, token, &data->gc);
		fd_node->value = ft_itoa(fd);
		redir_node->fd = fd;
	}
	if (data->tokens[*i].type != FILENAME)
		error ("Expected filename after redirection\n", 1, &data->gc);
	file_node = create_node(NODE_FILE, data->tokens[*i], &data->gc);
	(*i)++;
	redir_node->right = file_node;
	last_redir = (*cmd_node)->redirections;
	if (!last_redir)
		(*cmd_node)->redirections = redir_node;
	else
	{
		while (last_redir->next)
			last_redir = last_redir->next;
		last_redir->next = redir_node;
	}
	return (1);
}

int	parse_heredoc(int *i, t_node **cmd_node, t_minishell *data)
{
	t_node	*hd_node;
	t_node	*lim_node;
	t_node	*last_redir;

	hd_node = create_node(NODE_HEREDOC, data->tokens[*i], &data->gc);
	(*i)++;
	if (data->tokens[*i].type != LIMITER)
		error ("Expected Limiter after heredoc\n", 1, &data->gc);
	lim_node = create_node(NODE_LIMITER, data->tokens[*i], &data->gc);
	(*i)++;
	hd_node->right = lim_node;
	if (!(*cmd_node)->redirections)
		(*cmd_node)->redirections = hd_node;
	else
	{
		last_redir = (*cmd_node)->redirections;
		while (last_redir->next)
			last_redir = last_redir->next;
		last_redir->next = hd_node;
	}
	return (1);
}

t_node	*parse_expression(int *i, t_minishell *data)
{
	t_node		*left;
	t_node_type	op_type;
	t_node		*op_node;
	t_node		*right;

	left = parse_pipeline(i, data);
	while (data->tokens[*i].type == AND || data->tokens[*i].type == OR)
	{
		if (left == NULL)
			error ("Expected command before operator\n", 1, &data->gc);
		if (data->tokens[*i].type == AND)
			op_type = NODE_AND;
		else
			op_type = NODE_OR;
		op_node = create_node(op_type, data->tokens[*i], &data->gc);
		(*i)++;
		right = parse_pipeline(i, data);
		if (right == NULL)
			error ("Missing operand after operator\n", 1, &data->gc);
		op_node->left = left;
		op_node->right = right;
		left = op_node;
	}
	return (left);
}
