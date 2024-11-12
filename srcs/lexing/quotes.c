/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besch <besch@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:54:14 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/12 23:00:43 by besch            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_matching_quote(char *input, int start_index)
{
	char	quote_char;

	quote_char = input[start_index];
	start_index++;
	while (input[start_index])
	{
		if (input[start_index] == '\\' && input[start_index + 1] == quote_char)
			start_index += 2;
		else if (input[start_index] == quote_char)
			return (start_index);
		start_index++;
	}
	return (-1);
}

int	process_quotes(char *input, int *i, int *count, t_minishell *data)
{
	char			quote_char;
	int				start;
	int				end;

	dprintf(2, "process_quotes\n");
	quote_char = input[*i];
	start = ++(*i);
	end = find_matching_quote(input, start - 1);
	if (end == -1)
	{
		perror("Syntax error: unmatched quote");
		return (-1);
	}
	while (*i < end)
	{
		if (input[*i] == '\\' && input[*i + 1] == quote_char)
			(*i)++;
		(*i)++;
	}
	data->tokens[*count] = create_token(data->current_type, ft_substr_gc(input, start, end - start, &data->gc));
	data->tokens[*count].single_quote = (quote_char == '\'');
	data->tokens[*count].double_quote = (quote_char == '"');
	(*count)++;
	(*i)++;
	return (0);
}
