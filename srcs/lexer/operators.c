/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:01:45 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/12 22:33:14 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_pipe(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i + 1] == '|')
	{
		if (ft_strchr("|&<>", input[*i + 2]))
			error("Consecutive or invalid operators", 1, &data->gc);
		data->tokens[*count] = create_token(OR, "||");
		(*i)++;
	}
	else
	{
		if (ft_strchr("&<>", input[*i + 1]))
			error("Consecutive or invalid operators", 1, &data->gc);
		data->tokens[*count] = create_token(PIPE, "|");
	}
	(*i)++;
	(*count)++;
	skip_whitespace(&input, i);
	data->current_type = COMMAND;
}

static void	process_stdin(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i + 1] == '<')
	{
		if (ft_strchr("|&<>", input[*i + 2]))
			error("Consecutive or invalid operators", 1, &data->gc);
		data->tokens[*count] = create_token(HEREDOC, "<<");
		(*i) += 2;
		skip_whitespace(&input, i);
		data->current_type = LIMITER;
	}
	else
	{
		if (ft_strchr("|&>", input[*i + 1]))
			error("Consecutive or invalid operators", 1, &data->gc);
		data->tokens[*count] = create_token(STDIN, "<");
		(*i)++;
		skip_whitespace(&input, i);
		data->current_type = FILENAME;
	}
	(*count)++;
}

static void	process_stdout(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i + 1] == '>')
	{
		if (ft_strchr("|&<>", input[*i + 2]))
			error("Consecutive or invalid operators", 1, &data->gc);
		data->tokens[*count] = create_token(STDOUT_APPEND, ">>");
		(*i)++;
	}
	else
	{
		if (ft_strchr("|&<", input[*i + 1]))
			error("Consecutive or invalid operators", 1, &data->gc);
		data->tokens[*count] = create_token(STDOUT, ">");
	}
	(*i)++;
	(*count)++;
	skip_whitespace(&input, i);
	data->current_type = FILENAME;
}

static void	process_and(char *input, int *i, int *count, t_minishell *data)
{
	if (ft_strchr("|&<>", input[*i + 2]))
		error("Consecutive or invalid operators", 1, &data->gc);
	data->tokens[*count] = create_token(AND, "&&");
	*i += 2;
	(*count)++;
	skip_whitespace(&input, i);
	data->current_type = COMMAND;
}

void	process_operator(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i] == '|')
		process_pipe(input, i, count, data);
	else if (input[*i] == '<')
		process_stdin(input, i, count, data);
	else if (input[*i] == '>')
		process_stdout(input, i, count, data);
	else if (input[*i] == '&' && input[*i + 1] == '&')
		process_and(input, i, count, data);
}
