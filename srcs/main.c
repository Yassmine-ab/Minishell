/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:06:43 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/24 23:23:38 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;
	char		*prompt;
	t_node		*ast_root;
	int			i;

	if (argc > 1)
		return (printf("Minishell does" RED " not " DEFAULT "accept arguments. "
				"Running in interactive mode only.\n"), 1);
	rl_outstream = stderr;
	while (1)
	{
		init_signal_interactive_mode();
		data_init(argc, argv, envp, &data);
		if (isatty(STDIN_FILENO))
			prompt = create_prompt(&data);
		else
			prompt = "";
		data.line = readline(prompt);
		if (g_signal_received)
			signal_to_action(&data);
		if (data.line == 0)
		{
			ft_putendl_fd("exit", STDERR_FILENO);
			break ;
		}
		if (data.line[0])
		{
			add_history(data.line);
			data.current_type = COMMAND;
			if (tokenize_input(data.line, &data))
			{
				i = 0;
				ast_root = parse_expression(&i, &data);
				execute_ast(ast_root, &data, false);
				gc_cleanup(&data.gc);
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
