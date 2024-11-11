#include "minishell.h"

static void	print_tokens(t_token *tokens)
{
	int	i;

	i = -1;
	while (tokens[++i].type != END)
		ft_printf("type: %d, value: %s\n", tokens[i].type, tokens[i].value);
}

static void	print_ast(t_node *node, int depth)
{
	int	i;

	i = -1;
	if (!node)
		return ;
	while (++i < depth)
		printf("  ");
	if (node->value)
		printf("%s\n", node->value);
	else
		printf("(group)\n");
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
	print_ast(node->next, depth);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;
	const char	*prompt;
	t_token		*tokens;
	t_node		*ast_root;

	(void)argc;
	data_init(argv, envp, &data);
	prompt = create_prompt();
	while (1)
	{
		data.line = readline(prompt);
		if (!data.line)
			break ;
		else if (data.line[0] == EOF || data.line[0] == '\0')
		{
			free(data.line);
			break ;
		}
		add_history(data.line);
		tokens = tokenize_input(data.line, &data);
		print_tokens(tokens);
		ast_root = parse_tokens(tokens);
		print_ast(ast_root, 0);
		free_tokens(tokens);
	}
	free((void *)prompt);
	rl_clear_history();
	return (0);
}
