/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 05:22:40 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/23 04:45:34 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(const char *error_msg, int status, t_minishell *data)
{
	dprintf(2, "%s\n", error_msg);
	data->last_exit_status = status;
	if (data->is_child_process)
	{
		gc_cleanup(&data->gc);
		exit(status);
	}
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
		|| type == STDOUT || type == STDOUT_APPEND);
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

void	safe_close(int *fd)
{
	if (fd && *fd != -1)
	{
		if (close(*fd) == -1)
			dprintf(2, "Error closing file descriptor\n");
		*fd = -1;
	}
}
