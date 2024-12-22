/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:42:36 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/22 19:40:18 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	create_token(t_token_type type, char *value)
{
	t_token	token;

	token.type = type;
	token.value = value;
	token.quoted = 0;
	token.space_after = 1;
	return (token);
}

void	skip_whitespace(char **input, int *index)
{
	while ((*input)[*index] && ft_isspace((*input)[*index]))
		(*index)++;
}

static bool	validate_tokens(t_minishell *data)
{
	int	i;

	i = -1;
	while (data->tokens[++i].type != END)
	{
		if (i == 0 && is_operator(data->tokens[i].type))
			return (error("Unexpected operator at start", 1, data), false);
		if (is_operator(data->tokens[i].type)
			|| data->tokens[i].type == HEREDOC
			|| data->tokens[i].type == STDIN)
		{
			if (data->tokens[i + 1].type == END)
				return (error("Unexpected end of command after operator", \
				1, data), false);
			if (is_operator(data->tokens[i + 1].type)
				|| data->tokens[i + 1].type == HEREDOC
				|| data->tokens[i + 1].type == STDIN)
				return (error("Consecutive operators", 1, data), false);
		}
		else if (data->tokens[i].type == COMMAND
			&& data->tokens[i + 1].type == PARENTHESIS_OPEN)
			return (error("Unexpected opening parenthesis after command", \
			1, data), false);
	}
	return (true);
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
		{
			if (process_parentheses(input, &i, &count, data) == false)
				return (NULL);
		}
		else if (ft_strchr("|<>", input[i])
			|| (input[i] == '&' && input[i + 1] == '&'))
			process_operator(input, &i, &count, data);
		else
			process_word(input, &i, &count, data);
	}
	data->tokens[count] = create_token(END, NULL);
	if (validate_tokens(data) == false)
		return (NULL);
	return (data->tokens);
}
