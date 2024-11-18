/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:55:32 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/16 04:00:25 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_matching_parenthesis(char *input, int index)
{
	int	open_parentheses;

	open_parentheses = 1;
	index++;
	while (input[index])
	{
		if (input[index] == '(')
			open_parentheses++;
		else if (input[index] == ')')
		{
			open_parentheses--;
			if (open_parentheses == 0)
				return (index);
		}
		index++;
	}
	return (-1);
}

void	process_parentheses(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i] == '(')
	{
		if (find_matching_parenthesis(input, *i) == -1)
			error("Syntax error: unmatched opening parenthesis", 1, &data->gc);
		data->tokens[*count] = create_token(PARENTHESIS_OPEN, "(");
	}
	else if (input[*i] == ')')
		data->tokens[*count] = create_token(PARENTHESIS_CLOSE, ")");
	(*i)++;
	(*count)++;
}
