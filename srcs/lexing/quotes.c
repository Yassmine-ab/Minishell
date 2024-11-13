/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:54:14 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/13 05:22:04 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_matching_quote(char *input, int quote_index)
{
	char	quote_char;

	quote_char = input[quote_index];
	quote_index++;
	while (input[quote_index])
	{
		if (input[quote_index] == '\\' && input[quote_index + 1] == quote_char)
			quote_index++;
		else if (input[quote_index] == quote_char)
			return (quote_index);
		quote_index++;
	}
	return (-1);
}

int	process_quotes(char *input, int *i, int *count, t_minishell *data)
{
	char			quote_char;
	int				start;
	int				end;

	quote_char = input[*i];
	start = *i;
	end = find_matching_quote(input, start);
	(*i)++;
	if (end == -1)
		return (perror("Syntax error: unmatched quote"), -1);
	while (*i < end)
	{
		if (input[*i] == '\\' && input[*i + 1] == quote_char)
			(*i)++;
		(*i)++;
	}
	data->tokens[*count] = create_token(data->current_type, \
	ft_substr_gc(input, start + 1, end - start - 1, &data->gc));
	data->single_quote = (quote_char == '\'');
	data->double_quote = (quote_char == '"');
	(*i)++;
	(*count)++;
	return (0);
}
