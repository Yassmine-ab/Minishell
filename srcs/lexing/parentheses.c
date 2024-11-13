/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:55:32 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/13 03:03:20 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_matching_parenthesis(char *input, int start_index)
{
	int	open_parentheses;

	open_parentheses = 1;
	start_index++;
	while (input[start_index])
	{
		if (input[start_index] == '(')
			open_parentheses++;
		else if (input[start_index] == ')')
		{
			open_parentheses--;
			if (open_parentheses == 0)
				return (start_index);
		}
		start_index++;
	}
	return (-1);
}

int	process_parentheses(char *input, int *i, int *count, t_minishell *data)
{
	if (find_matching_parenthesis(input, *i) == -1)
	{
		perror("Syntax error: unmatched parentheses");
		return (-1);
	}
	if (input[*i] == '(')
	{
		data->tokens[*count] = create_token(PARENTHESIS_OPEN, "(");
		(*i)++;
		(*count)++;
	}
	else if (input[*i] == ')')
	{
		data->tokens[*count] = create_token(PARENTHESIS_CLOSE, ")");
		(*i)++;
		(*count)++;
	}
	return (0);
}
