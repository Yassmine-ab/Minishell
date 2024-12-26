/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 05:22:40 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/26 01:45:08 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_gc_node_locked(t_gc *gc, void *ptr, bool lock)
{
	t_gc_node	*current;

	current = gc->head;
	while (current)
	{
		if (current->ptr == ptr)
		{
			current->locked = lock;
			break ;
		}
		current = current->next;
	}
}

void	error(char *context, char *error_msg, int status, t_minishell *data)
{
	if (context)
		ft_putstr_fd(context, 2);
	ft_putendl_fd(error_msg, STDERR_FILENO);
	if (data->is_child_process)
	{
		gc_cleanup(&data->gc);
		exit(status);
	}
}

bool	is_redir(t_token_type type)
{
	return (type == STDIN || type == STDOUT || type == STDOUT_APPEND
		|| type == HEREDOC);
}

bool	is_operator(t_token_type type)
{
	return (type == PIPE || type == AND || type == OR);
}

void	strncat_realloc(char **result, char *append, size_t *size, t_gc *gc)
{
	size_t	new_len;

	new_len = ft_strlen(*result) + ft_strlen(append) + 1;
	if (new_len > *size)
	{
		*size = new_len + 16;
		*result = gc_realloc(*result, *size, gc);
	}
	ft_strlcat(*result, append, *size);
}

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
// 	(print_ast(node->left, depth + 1), print_ast(node->right, depth + 1));
// 	if (node->next)
// 		print_ast(node->next, depth);
// 	if (node->args)
// 		print_ast(node->args, depth + 1);
// 	if (node->redirections)
// 		print_ast(node->redirections, depth + 1);
// 	if (depth == 0)
// 		(printf(DEFAULT), fflush(stdout));
// }
