/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:01:45 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/10 03:19:26 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_pipe(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i + 1] == '|')
	{
		data->tokens[*count] = create_token(OR, "||");
		(*i) += 2;
	}
	else
	{
		data->tokens[*count] = create_token(PIPE, "|");
		(*i)++;
	}
	data->is_command = true;
	return (0);
}

static int	process_stdin(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i + 1] == '<')
	{
		data->tokens[*count] = create_token(HEREDOC, "<<");
		(*i) += 2;
	}
	else
	{
		data->tokens[*count] = create_token(STDIN, "<");
		(*i)++;
	}
	data->is_command = false;
	return (0);
}

static int	process_stdout(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i + 1] == '>')
	{
		data->tokens[*count] = create_token(STDOUT_APPEND, ">>");
		(*i) += 2;
	}
	else
	{
		data->tokens[*count] = create_token(STDOUT, ">");
		(*i)++;
	}
	data->is_command = false;
	return (0);
}

static int	process_and(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i + 1] == '&')
	{
		data->tokens[*count] = create_token(AND, "&&");
		(*i) += 2;
	}
	else
	{
		data->tokens[*count] = create_token(SIGNAL, "&");
		(*i)++;
	}
	data->is_command = true;
	return (0);
}

int	process_operators(char *input, int *i, int *count, t_minishell *data)
{
	if (input[*i] == '|')
		return (process_pipe(input, i, count, data));
	else if (input[*i] == '<')
		return (process_stdin(input, i, count, data));
	else if (input[*i] == '>')
		return (process_stdout(input, i, count, data));
	else if (input[*i] == '&')
		return (process_and(input, i, count, data));
	return (0);
}
