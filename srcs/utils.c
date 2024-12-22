/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 05:22:40 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/22 16:38:21 by yaabdall         ###   ########.fr       */
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
			perror("Error closing file descriptor");
		*fd = -1;
	}
}
