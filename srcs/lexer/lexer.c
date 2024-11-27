/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:42:36 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/27 20:47:48 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	create_token(t_token_type type, char *value)
{
	t_token	new_token;

	new_token.type = type;
	new_token.value = value;
	return (new_token);
}

void	skip_whitespace(char **input, int *index)
{
	while ((*input)[*index] && ft_isspace((*input)[*index]))
		(*index)++;
}

t_token	*tokenize_input(char *input, t_minishell *data)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			skip_whitespace(&input, &i);
		else if (ft_strchr("()", input[i]))
			process_parentheses(input, &i, &count, data);
		else if (ft_strchr("|<>", input[i])
			|| (input[i] == '&' && input[i + 1] == '&'))
			process_operator(input, &i, &count, data);
		else
			process_word(input, &i, &count, data);
	}
	data->tokens[count] = create_token(END, NULL);
	return (data->tokens);
}
