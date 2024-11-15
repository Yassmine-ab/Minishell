/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:42:36 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/15 10:26:48 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(char **input, int *index)
{
	while ((*input)[*index] && ft_isspace((*input)[*index]))
		(*index)++;
}

void	process_command(char *input, int *i, int *count, t_minishell *data)
{
	char	*value;
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !ft_strchr("'\"()|<>&", input[*i]))
		(*i)++;
	value = ft_substr_gc(input, start, *i - start, &data->gc);
	if (!value || !*value)
		return ;
	if (data->current_type == COMMAND)
	{
		data->tokens[*count] = create_token(COMMAND, value);
		skip_whitespace(&input, i);
		data->current_type = ARGUMENT;
	}
	else if (data->current_type == ARGUMENT)
		data->tokens[*count] = create_token(ARGUMENT, value);
	(*count)++;
}

static void	process_word(char *input, int *i, int *count, t_minishell *data)
{
	if (data->current_type == FILENAME)
		process_file(input, i, count, data);
	else if (data->current_type == LIMITER)
		process_limiter(input, i, count, data);
	else
		process_command(input, i, count, data);
}

t_token	*tokenize_input(char *input, t_minishell *data)
{
	int (i) = 0;
	int (count) = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			skip_whitespace(&input, &i);
		else if (input[i] == '\\' && ft_strchr("'\"()|<>&", input[i + 1]))
			i += 2;
		else if (input[i] == '\'')
			process_single_quotes(input, &i, &count, data);
		else if (input[i] == '"')
			process_double_quotes(input, &i, &count, data);
		else if (ft_strchr("()", input[i]))
			process_parentheses(input, &i, &count, data);
		else if (ft_strchr("|<>&", input[i]))
			process_operator(input, &i, &count, data);
		else if (input[i] == '$')
			process_env_variable(input, &i, &count, data);
		else if (input[i] == '*')
			process_wildcard(input, &i, &count, data);
		else
			process_word(input, &i, &count, data);
	}
	data->tokens[count] = create_token(END, NULL);
	return (data->tokens);
}
