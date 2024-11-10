/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:54:14 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/10 03:27:53 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_matching_quote(char *input, int index)
{
	char	quote_char;

	quote_char = input[index];
	index++;
	while (input[index])
	{
		if (input[index] == '\\' && (input[index + 1] == quote_char
				|| input[index + 1] == '\\'))
			index += 2;
		else if (input[index] == quote_char)
			return (index);
		index++;
	}
	return (-1);
}

int	process_quotes(char *input, int *i, int *count, t_minishell *data)
{
	char	quote_char;
	int		start;

	quote_char = input[*i];
	start = ++(*i);
	if (find_matching_quote(input, *i) == -1)
	{
		error("Syntax error: unmatched quote", 1, data);
		return (-1);
	}
	while (input[*i] && input[*i] != quote_char)
	{
		if (input[*i] == '\\' && input[*i + 1] == quote_char)
			(*i)++;
		(*i)++;
	}
	data->tokens[*count] = create_token(QUOTE, \
	ft_substr(input, start, *i - start));
	(*count)++;
	(*i)++;
	return (0);
}
