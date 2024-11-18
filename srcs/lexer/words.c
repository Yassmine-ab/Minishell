/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 19:30:54 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/17 21:36:09 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_file(char *value, int *count, t_minishell *data)
{
	data->tokens[*count] = create_token(FILENAME, value);
}

static void	process_limiter(char *value, int *count, t_minishell *data)
{
	data->tokens[*count] = create_token(LIMITER, value);
}

static void	process_command(char *value, int *count, t_minishell *data)
{
	if (data->current_type == COMMAND)
	{
		data->tokens[*count] = create_token(COMMAND, value);
		data->current_type = ARGUMENT;
	}
	else if (data->current_type == ARGUMENT)
		data->tokens[*count] = create_token(ARGUMENT, value);
}

void	process_word(char *input, int *i, int *count, t_minishell *data)
{
	char		*value;
	const int	start = *i;

	while (input[*i] && !ft_isspace(input[*i])
		&& !ft_strchr("()|<>&", input[*i]))
		(*i)++;
	value = ft_substr_gc(input, start, *i - start, &data->gc);
	if (!*value)
		return ;
	if (data->current_type == FILENAME)
		process_file(value, count, data);
	else if (data->current_type == LIMITER)
		process_limiter(value, count, data);
	else
		process_command(value, count, data);
	(*count)++;
	skip_whitespace(&input, i);
	if (data->current_type == FILENAME || data->current_type == LIMITER)
		data->current_type = COMMAND;
}
