/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:04:05 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/25 12:54:23 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_and(t_node *ast, t_minishell *data)
{
	execute_ast(ast->left, data, false);
	if (data->last_exit_status == 0)
		execute_ast(ast->right, data, false);
}

static void	execute_or(t_node *ast, t_minishell *data)
{
	execute_ast(ast->left, data, false);
	if (data->last_exit_status != 0)
		execute_ast(ast->right, data, false);
}

// static int	count_group_nodes(t_node *ast)
// {
// 	int	count;

// 	count = 0;
// 	while (ast)
// 	{
// 		if (ast->value)
// 			count++;
// 		ast = ast->next;
// 	}
// 	return (count);
// }

// static char	**collect_group_values(t_node *ast, t_minishell *data)
// {
// 	int		count;
// 	char	**values;
// 	int		i;

// 	i = 0;
// 	count = count_group_nodes(ast);
// 	values = gc_malloc(sizeof(char *) * (count + 1), &data->gc);
// 	while (ast)
// 	{
// 		values[i++] = ft_strdup_gc(ast->value, &data->gc);
// 		ast = ast->next;
// 	}
// 	values[i] = NULL;
// 	return (values);
// }

// static void	execute_group(t_node *ast, t_minishell *data)
// {
// 	t_minishell	data_group;
// 	char		**args;
// 	t_token		*tokens;
// 	t_node		*ast_root;
// 	int			i;

// 	args = collect_group_values(ast->left, data);
// 	ft_putstr_fd(args[0], 1);
// 	data_init(ft_tabstrlen(args), args, data->envp, &data_group);
// 	if ((tokens = tokenize_input(ast->value, &data_group)))
// 	{
// 		i = 0;
// 		ast_root = parse_expression(&i, &data_group);
// 		execute_ast(ast_root, &data_group, true);
// 		printf("execute_ast_done\n");
// 		gc_cleanup(&data_group.gc);
// 		exit(data_group.last_exit_status);
// 	}
// 	printf("tokenize_input_failed\n");
// }

static void	execute_group(t_node *ast, t_minishell *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		execute_ast(ast->left, data, false);
	else
	{
		waitpid(pid, &status, 0);
		handle_child_exit(status, data);
	}
}

void	execute_ast(t_node *ast, t_minishell *data, bool in_child_process)
{
	struct termios	term;

	if (ast == NULL)
		return ;
	if (in_child_process)
		signal_child_process();
	else
		init_signal_exec();
	ft_memset(&term, 0, sizeof(term));
	tcgetattr(STDIN_FILENO, &term);
	while (ast)
	{
		if (ast->type == NODE_COMMAND)
			execute_command(ast, data, in_child_process);
		else if (ast->type == NODE_REDIR || ast->type == NODE_HEREDOC)
			execute_redirections(ast, data);
		else if (ast->type == NODE_PIPE)
			execute_pipeline(ast, data);
		else if (ast->type == NODE_AND)
			execute_and(ast, data);
		else if (ast->type == NODE_OR)
			execute_or(ast, data);
		else if (ast->type == NODE_GROUP)
			execute_group(ast, data);
		ast = ast->next;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal_to_action(data);
}
