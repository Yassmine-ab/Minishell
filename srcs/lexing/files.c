/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besch <besch@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 19:30:54 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/12 22:15:11 by besch            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_file(char *input, int *i, int *count, t_minishell *data)
{
	int	start;

	start = *i;
	if (input[*i] == '\0' || ft_strchr("|<>&()", input[*i]))
	{
		perror("Syntax error: expected filename after redirection");
		return (-1);
	}
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&", input[*i]))
		(*i)++;
	data->current_type = FILENAME;
	data->tokens[*count] = \
	create_token(FILENAME, ft_substr_gc(input, start, *i - start, &data->gc));
	data->is_command = true;
	(*count)++;
	return (0);
}

int	process_wildcard(char *input, int *i, int *count, t_minishell *data)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&", input[*i]))
		(*i)++;
	data->tokens[*count] = \
	create_token(WILDCARD, ft_substr_gc(input, start, *i - start, &data->gc));
	(*count)++;
	return (0);
}

int	process_limiter(char *input, int *i, int *count, t_minishell *data)
{
	int	start;

	start = *i;
	if (input[*i] == '\0' || ft_strchr("|<>&()", input[*i]))
	{
		perror("Syntax error: missing heredoc limiter\n");
		return (-1);
	}
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&", input[*i]))
		(*i)++;
	data->current_type = LIMITER;
	data->tokens[*count] = \
	create_token(LIMITER, ft_substr_gc(input, start, *i - start, &data->gc));
	data->is_command = true;
	(*count)++;
	return (0);
}
