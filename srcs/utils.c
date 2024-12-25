/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 05:22:40 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/25 06:35:19 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(const char *error_msg, int status, t_minishell *data)
{
	data->last_exit_status = status;
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
