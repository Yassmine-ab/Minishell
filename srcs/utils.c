/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 05:22:40 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/13 02:17:25 by yaabdall         ###   ########.fr       */
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

void	close_fd(int *fd)
{
	if (fd && *fd != -1)
	{
		if (close(*fd) == -1)
			perror("Error closing file descriptor");
		else
			*fd = -1;
	}
}
