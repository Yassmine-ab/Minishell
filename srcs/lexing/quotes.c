/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:54:14 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/11 20:39:32 by yaabdall         ###   ########.fr       */
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
	char	quote_char;
	int		start;

	quote_char = input[*i];
	start = ++(*i);
	if (find_matching_quote(input, start - 1) == -1)
	{
		perror("Syntax error: unmatched quote");
		return (-1);
	}
	while (*i < find_matching_quote(input, start - 1))
	{
		if (input[*i] == '\\' && input[*i + 1] == quote_char)
			(*i)++;
		(*i)++;
	}
	data->tokens[*count] = \
	create_token(QUOTE, ft_substr_gc(input, start, *i - start, &data->gc));
	(*count)++;
	(*i)++;
	return (0);
}
