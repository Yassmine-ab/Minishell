/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:06:43 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/25 06:36:14 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

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
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;
	char		*prompt;
	t_node		*ast_root;
	int			i;

	if (isatty(STDIN_FILENO) == false || argc > 1)
		return (printf("Minishell does" RED " not " DEFAULT "accept input from "
				"non-interactive mode.\n"), 1);
	if (argc > 1)
		return (printf("Minishell does" RED " not " DEFAULT "accept arguments. "
				"Running in interactive mode only.\n"), 1);
	// data_init(argc, argv, envp, &data);
	ft_memset(&data, 0, sizeof(t_minishell));
	gc_init(&data.gc);
	data.envp = envp;
	rl_outstream = stderr;
	while (1)
	{
		init_signal_interactive_mode();
		data_init(argc, argv, data.envp, &data);
		if (isatty(STDIN_FILENO))
			prompt = create_prompt(&data);
		else
			prompt = "";
		data.line = readline(prompt);
		if (g_signal_received)
			signal_to_action(&data);
		if (data.line == 0)
		{
			data.last_exit_status = 0;
			ft_exit(NULL, &data);
		}
		if (data.line[0])
		{
			add_history(data.line);
			data.current_type = COMMAND;
			if (tokenize_input(data.line, &data))
			{
				i = 0;
				ast_root = parse_expression(&i, &data);
				print_ast(ast_root, 0);
				execute_ast(ast_root, &data, false);
				gc_cleanup_lock(&data.gc);
				ast_root = NULL;
			}
			if (g_signal_received)
				signal_to_action(&data);
		}
		free(data.line);
	}
	rl_clear_history();
	gc_cleanup(&data.gc);
	return (data.last_exit_status);
}
