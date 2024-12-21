/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcantin <jcantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:06:43 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/21 16:47:52 by jcantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static void	print_tokens(t_token *tokens)
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

static void	print_ast(t_node *node, int depth)
{
	const char	*colors[] = {
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		DEFAULT
	};
	const char	*color;
	int			num_colors;
	int			i;

	num_colors = sizeof(colors) / sizeof(colors[0]);
	color = colors[depth % num_colors];
	if (!node)
		return ;
	i = -1;
	while (++i < depth)
		printf("  ");
	printf("%s", color);
	if (node->value)
		printf("%s\n", node->value);
	else
		printf("(group)\n");
	printf(DEFAULT);
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
	if (node->next)
		print_ast(node->next, depth);
	if (node->args)
		print_ast(node->args, depth + 1);
	if (node->redirections)
		print_ast(node->redirections, depth + 1);
	printf(DEFAULT);
}*/

static void	free_ast(t_node *node, t_gc *gc)
{
	if (!node)
		return ;
	free_ast(node->left, gc);
	free_ast(node->right, gc);
	free_ast(node->next, gc);
	free_ast(node->args, gc);
	free_ast(node->redirections, gc);
	if (node->value)
		gc_free(node->value, gc);
	gc_free(node, gc);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;
	const char	*prompt;
	// t_token		*tokens;
	t_node		*ast_root;
	int			i;

	if (argc > 1)
		return (printf("Minishell does" RED " not " DEFAULT "accept arguments. "
				"Running in interactive mode only.\n"), 1);
	data_init(argc, argv, envp, &data);
	init_signal();
	while (1)
	{
		prompt = create_prompt(&data);
		data.line = readline(prompt);
		if (data.line == 0)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			break ;
		}
		if (data.line[0] != '\0')
		{
			add_history(data.line);
			data.current_type = COMMAND;
			tokenize_input(data.line, &data);
			// print_tokens(tokens);
			i = 0;
			ast_root = parse_expression(&i, &data);
			// print_ast(ast_root, 0);
			execute_ast(ast_root, &data, false);
			free_ast(ast_root, &data.gc);
            ast_root = NULL;
		}
		gc_free(&data.line, &data.gc);
		if (g_signal_received)
		{
			printf("Signal received. Cleaning up and exiting.\n");
			break ;
		}
	}
	rl_clear_history();
	// gc_cleanup(&data.gc);
	return (data.last_exit_status);
}
