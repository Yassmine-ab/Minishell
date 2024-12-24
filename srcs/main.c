/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:06:43 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/23 13:22:04 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

// static void	print_tokens(t_token *tokens)
// {
// 	int	i;
// 	const char *token_names[] =
// 	{
// 		"COMMAND", "ARGUMENT", "PIPE", "STDIN", "STDOUT", "STDOUT_APPEND",
// 		"FILENAME", "HEREDOC", "LIMITER", "AND", "OR", "PARENTHESIS_OPEN",
// 		"PARENTHESIS_CLOSE", "END"
// 	};

// 	i = -1;
// 	while (tokens[++i].type != END)
// 		dprintf(2, "%s [%s]\n", token_names[tokens[i].type], tokens[i].value);
// }

// static void	print_ast(t_node *node, int depth)
// {
// 	const char	*colors[] = {
// 		RED,
// 		GREEN,
// 		YELLOW,
// 		BLUE,
// 		MAGENTA,
// 		CYAN,
// 		DEFAULT
// 	};
// 	const char	*color;
// 	int			num_colors;
// 	int			i;

// 	num_colors = sizeof(colors) / sizeof(colors[0]);
// 	color = colors[depth % num_colors];
// 	if (!node)
// 		return ;
// 	i = -1;
// 	while (++i < depth)
// 		printf("  ");
// 	printf("%s", color);
// 	if (node->value)
// 		printf("%s\n", node->value);
// 	else
// 		printf("(group)\n");
// 	printf(DEFAULT);
// 	print_ast(node->left, depth + 1);
// 	print_ast(node->right, depth + 1);
// 	if (node->next)
// 		print_ast(node->next, depth);
// 	if (node->args)
// 		print_ast(node->args, depth + 1);
// 	if (node->redirections)
// 		print_ast(node->redirections, depth + 1);
// 	printf(DEFAULT);
// }

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
	char		*prompt;
	t_token		*tokens;
	t_node		*ast_root;
	int			i;

	if (argc > 1)
		return (printf("Minishell does" RED " not " DEFAULT "accept arguments. "
				"Running in interactive mode only.\n"), 1);
	data_init(argc, argv, envp, &data);
	rl_outstream = stderr;
	while (1)
	{
		init_signal_interactive_mode();
		if (isatty(STDIN_FILENO))
			prompt = create_prompt(&data);
		else
			prompt = "";
		data.line = readline(prompt);
		if (g_signal_received)
			signal_to_action(&data);
		if (data.line == 0)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			break ;
		}
		if (data.line[0] != '\0')
		{
			add_history(data.line);
			data.current_type = COMMAND;
			if ((tokens = tokenize_input(data.line, &data)))
			{
				// print_tokens(tokens);
				i = 0;
				ast_root = parse_expression(&i, &data);
				// print_ast(ast_root, 0);
				execute_ast(ast_root, &data, false);
				free_ast(ast_root, &data.gc);
				ast_root = NULL;
			}
			if (g_signal_received)
				signal_to_action(&data);
		}
		gc_free(&data.line, &data.gc);
	}
	rl_clear_history();
	gc_cleanup(&data.gc);
	return (data.last_exit_status);
}

// c'est normal qu'il y ait 2 fois if (g_signal_received)

// associer redir avant comande a commande
// rajouter un boolen pour executer les redir non associees a une commande
