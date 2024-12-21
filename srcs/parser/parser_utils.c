/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 05:22:40 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/20 11:39:05 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(const char *error_msg, int status, t_gc *gc)
{
	dprintf(2, "%s\n", error_msg);
	gc_cleanup(gc);
	exit(status);
}

bool	is_redir(int current_index, t_minishell *data)
{
	return (data->tokens[current_index].type == STDOUT
		|| data->tokens[current_index].type == STDIN
		|| data->tokens[current_index].type == STDOUT_APPEND);
}

bool	is_operator(t_token_type type)
{
	return (type == PIPE || type == AND || type == OR
		|| type == STDIN || type == STDOUT || type == STDOUT_APPEND);
}
