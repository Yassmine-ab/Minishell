/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 05:22:40 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/17 05:46:13 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(const char *error_msg, int status, t_gc *gc)
{
	(void)gc;
	dprintf(2, "%s\n", error_msg);
	gc_cleanup(gc);
	exit(status);
}

int	is_number(const char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	is_redir_following(int current_index, t_minishell *data)
{
	int	next;

	next = current_index + 1;
	if (data->tokens[next].type == STDOUT
		|| data->tokens[next].type == STDIN
		|| data->tokens[next].type == STDOUT_APPEND)
		return (1);
	return (0);
}
