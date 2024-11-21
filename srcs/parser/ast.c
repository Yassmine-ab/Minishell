#include "minishell.h"



t_node	*parse_tokens(t_token *tokens, int *i, t_gc *gc)
{
	t_node	*root;
	t_node	*current_node;
	t_node	*cmd_node;
	t_node	*arg_node;
	t_node	*pipe_node;
	t_node	*redir_node;
	t_node	*file_node;
	t_node	*heredoc_node;
	t_node	*limiter_node;
	t_node	*and_node;
	t_node	*or_node;
	t_node	*group_node;
	t_node	*last;
	int		i;

	root = NULL;
	current_node = NULL;
	i = -1;
	while (tokens[++(*i)].type != END && tokens[*i].type != PARENTHESIS_CLOSE)
	{
		if (tokens[*i].type == COMMAND)
		{
			cmd_node = create_node(NODE_COMMAND, tokens[*i].value, gc);
			while (tokens[++(*i)].type == ARGUMENT)
			{
				arg_node = create_node(NODE_ARGUMENT, tokens[*i].value, gc);
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
		else if (tokens[*i].type == PIPE)
		{
			pipe_node = create_operator_node(NODE_PIPE, tokens[*i].value, NULL, NULL, gc);
			pipe_node->left = root;
			pipe_node->right = parse_tokens(tokens, i + 1, gc);
			root = pipe_node;
			current_node = pipe_node;
			break ;
		}
		else if (tokens[*i].type == STDIN || tokens[*i].type == STDOUT || tokens[*i].type == STDOUT_APPEND)
		{
			redir_node = create_operator_node(NODE_REDIRECTION, tokens[*i].value, NULL, NULL, gc);
			redir_node->left = root;
			if (tokens[++(*i)].type == FILENAME)
				redir_node->right = create_node(NODE_FILENAME, tokens[*i].value, gc);
			else
			{
				perror("expected filename after redirection");
				return (NULL);
			}
			if (!root)
				root = redir_node;
			else
				current_node->right = redir_node;
			current_node = redir_node;
		}
		else if (tokens[*i].type == HEREDOC)
		{
			heredoc_node = create_node(NODE_HEREDOC, tokens[*i].value, gc);
			heredoc_node->left = root;
			if (tokens[++(*i)].type == LIMITER)
				heredoc_node->right = create_node(NODE_LIMITER, tokens[*i].value, gc);
			else
			{
				perror("expected limiter after heredoc");
				return (NULL);
			}
		}
		else if (tokens[*i].type == AND)
		{
			and_node = create_operator_node(NODE_AND, tokens[*i].value, root, NULL, gc);
			if (!root)
				root = and_node;
			else
				current_node->right = and_node;
			current_node = and_node;
		}
		else if (tokens[*i].type == OR)
		{
			or_node = create_operator_node(NODE_OR, tokens[*i].value, root, NULL, gc);
			if (!root)
				root = or_node;
			else
				current_node->right = or_node;
		}
		else if (tokens[*i].type == PARENTHESIS_OPEN)
		{
			group_node = create_operator_node(NODE_GROUP, tokens[*i].value, root, NULL, gc);
			while (tokens[++(*i)].type != PARENTHESIS_CLOSE)
				group_node->left = parse_tokens(tokens + i + 1, gc);
			if (!root)
				root = group_node;
			else
				current_node->right = group_node;
			current_node = group_node;
		}
	}
	return (root);
}
