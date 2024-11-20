#include "minishell.h"

t_node	*parse_tokens(t_token *tokens, t_gc *gc)
{
	t_node	*root;
	t_node	*current_node;
	t_node	*cmd_node;
	t_node	*arg_node;
	t_node	*pipe_node;
	t_node	*stdin_node;
	t_node	*stdout_node;
	t_node	*stdout_append_node;
	t_node	*file_node;
	t_node	*heredoc_node;
	t_node	*limiter_node;
	t_node	*and_node;
	t_node	*or_node;
	t_node	*group_node;
	t_node	*last;
	int		i;

	i = -1;
	while (tokens[++i].type != END)
	{
		if (tokens[i].type == COMMAND)
		{
			cmd_node = create_ast_node(NODE_COMMAND, tokens[i].value, gc);
			while (tokens[++i].type == ARGUMENT)
			{
				arg_node = create_ast_node(NODE_ARGUMENT, tokens[i].value, gc);
				if (!cmd_node->left)
					cmd_node->left = arg_node;
				else
				{
					last = cmd_node->left;
					while (last->next)
						last = last->next;
					last->next = arg_node;
				}
			}
			if (!root)
				root = cmd_node;
			else
				current_node->right = cmd_node;
			current_node = cmd_node;
		}
		else if (tokens[i].type == PIPE)
		{
			pipe_node = create_operator_node(NODE_PIPE, tokens[i].value, root, NULL, gc);
			pipe_node->right = parse_tokens(tokens + i + 1, gc);
			root = pipe_node;
			break ;
		}
		else if (tokens[i].type == STDIN)
		{
			stdin_node = create_operator_node(NODE_STDIN, tokens[i].value, root, NULL, gc);
			stdin_node->right = parse_tokens(tokens + i + 1, gc);
			if (tokens[++i].type == FILENAME || tokens[i].type == LIMITER)
				stdin_node->left = create_ast_node(tokens[i].type, tokens[i].value, gc);
			{
				file_node = create_ast_node(NODE_FILENAME, tokens[i].value, gc);
				if (!file_node->right)
					stdin_node->right = file_node;
				else
				{
					last = stdin_node->right;
					while (last->next)
						last = last->next;
					last->next = file_node;
				}
			if (!current_node->right)
				current_node->right = stdin_node;
			else
			{
				last = current_node->right;
				while (last->next)
					last = last->next;
				last->next = stdin_node;
			}
			break ;
		}
		else if (tokens[i].type == STDOUT)
			cmd_node = create_operator_node(NODE_STDOUT, tokens[i].value, cmd_node->left, cmd_node->right, gc);
		else if (tokens[i].type == STDOUT_APPEND)
			cmd_node = create_operator_node(NODE_STDOUT_APPEND, tokens[i].value, cmd_node->left, cmd_node->right, gc);
		else if (tokens[i].type == FILENAME)
			cmd_node = create_ast_node(NODE_FILENAME, tokens[i].value, gc);
		else if (tokens[i].type == HEREDOC)
			cmd_node = create_ast_node(NODE_HEREDOC, tokens[i].value, gc);
		else if (tokens[i].type == LIMITER)
			cmd_node = create_ast_node(NODE_LIMITER, tokens[i].value, gc);
		else if (tokens[i].type == AND)
			cmd_node = create_operator_node(NODE_AND, tokens[i].value, gc);
		else if (tokens[i].type == OR)
			cmd_node = create_operator_node(NODE_OR, tokens[i].value, gc);
		//ajouter les parentheses
		i++;
	}
}
