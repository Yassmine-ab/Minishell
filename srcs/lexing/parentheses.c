/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:55:32 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/10 03:25:09 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_matching_parenthesis(char *input, int index)
{
	int	open_paren_count;

	open_paren_count = 1;
	index++;
	while (input[index])
	{
		if (input[index] == '(')
			open_paren_count++;
		else if (input[index] == ')')
		{
			open_paren_count--;
			if (open_paren_count == 0)
				return (index);
		}
		index++;
	}
	return (-1);
}

int	process_parentheses(char *input, int *i, int *count, t_minishell *data)
{
	if (find_matching_parenthesis(input, *i) == -1)
	{
		error("Syntax error: unmatched parentheses", 1, data);
		return (-1);
	}
	if (input[*i] == '(')
	{
		data->tokens[*count] = create_token(PARENTHESIS_OPEN, "(");
		(*count)++;
		(*i)++;
	}
	else if (input[*i] == ')')
	{
		data->tokens[*count] = create_token(PARENTHESIS_CLOSE, ")");
		(*count)++;
		(*i)++;
	}
	return (0);
}
