/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:06:43 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/19 15:41:51 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_tokens(t_token *tokens)
{
	int	i;
	const char *token_names[] =
	{
		"COMMAND", "ARGUMENT", "PIPE", "STDIN", "STDOUT", "STDOUT_APPEND",
		"FILENAME", "HEREDOC", "LIMITER", "AND", "OR", "PARENTHESIS_OPEN",
		"PARENTHESIS_CLOSE", "END"
	};

	i = -1;
	while (tokens[++i].type != END)
		dprintf(2, "%s [%s]\n", token_names[tokens[i].type], tokens[i].value);
}

// static void	print_ast(t_node *node, int depth)
// {
// 	int	i;

// 	i = -1;
// 	if (!node)
// 		return ;
// 	while (++i < depth)
// 		printf("  ");
// 	if (node->value)
// 		printf("%s\n", node->value);
// 	else
// 		printf("(group)\n");
// 	print_ast(node->left, depth + 1);
// 	print_ast(node->right, depth + 1);
// 	print_ast(node->next, depth);
// }

int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;
	const char	*prompt;
	t_token		*tokens;
	t_node		*ast_root;

	if (argc > 1)
		return (printf("Minishell does" RED " not " DEFAULT "accept arguments. "
				"Running in interactive mode only.\n"), 1);
	data_init(argc, argv, envp, &data);
	prompt = create_prompt(&data.gc);
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
		data.current_type = COMMAND;
		expand_variables(&data);
		tokens = tokenize_input(data.line, &data);
		print_tokens(tokens);
		ast_root = parse_tokens(tokens, &data.gc);
		print_ast(ast_root, 0);
	}
	rl_clear_history();
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_minishell	data;
// 	t_token		*tokens;
// 	int			i;

// 	(void)argc;
// 	data_init(argc, argv, envp, &data);
// 	char *test_inputs[] =
// 	{
// 		"ls -la /home/user",
// 		"cat file.txt | grep \"hello\" | sort",
// 		"echo \"Hello, world\" > output.txt",
// 		"echo $HOME",
// 		"echo \"Outer 'inner single quotes' outer\"",
// 		"(echo \"start\" && ls) || echo \"failed\"",
// 		"ls *.txt",
// 		NULL
// 	};
// 	i = -1;
// 	while (test_inputs[++i])
// 	{
// 		printf("Input: %s\n", test_inputs[i]);
// 		data.current_type = COMMAND;
// 		// expand_variables(&data);
// 		tokens = tokenize_input(test_inputs[i], &data);
// 		print_tokens(tokens);
// 	}
// 	return (0);
// }
