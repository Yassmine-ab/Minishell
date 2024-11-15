/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:54:14 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/15 19:51:56 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_matching_char(char *input, int char_index)
{
	char	match_char;

	match_char = input[char_index];
	char_index++;
	while (input[char_index])
	{
		if (match_char == '"' && input[char_index] == '\\'
			&& input[char_index + 1] == match_char)
			char_index++;
		else if (input[char_index] == match_char)
			return (char_index);
		char_index++;
	}
	return (-1);
}

static int	handle_unclosed_char(char **input, int start, t_minishell *data)
{
	char	*additional_input;
	char	*temp;
	int		end;

	while (1)
	{
		write(1, "> ", 2);
		additional_input = get_next_line(0);
		if (!additional_input)
			break ;
		temp = *input;
		*input = ft_strjoin_gc(temp, additional_input, &data->gc);
		ft_free(&temp);
		ft_free(&additional_input);
		end = find_matching_char(*input, start);
		if (end != -1)
			return (end);
	}
	return (-1);
}

void	process_single_quotes(char *input, int *i, int *count, t_minishell *data)
{
	const int	start = *i;
	int			end;

	end = find_matching_char(input, start);
	(*i)++;
	if (end == -1)
		end = handle_unclosed_char(&input, start, data);
	while (*i < end)
		(*i)++;
	data->tokens[*count] = create_token(data->current_type, \
	ft_substr_gc(input, start + 1, end - start - 1, &data->gc));
	(*i)++;
	(*count)++;
}

void	process_backquotes(char *input, int *i, int *count, t_minishell *data)
{
	const int	start = *i;
	int			end;

	end = find_matching_char(input, start);
	if (end == -1)
		return ;
	(*i)++;
	while (*i < end)
		(*i)++;
	data->tokens[*count] = create_token(COMMAND, \
	ft_substr_gc(input, start + 1, end - start - 1, &data->gc));
	(*i)++;
	(*count)++;
}

void	process_double_quotes(char *input, int *i, int *count, t_minishell *data)
{
	const int	start = *i;
	int			end;

	end = find_matching_char(input, start);
	if (end == -1)
		end = handle_unclosed_char(&input, start, data);
	(*i)++;
	while (*i < end)
	{
		if (input[*i] == '$')
			process_env_variable(input, i, count, data);
		else if (input[*i] == '\\' && (*i + 1 < end) && (input[*i + 1] == '$'
				|| input[*i + 1] == '\\' || input[*i + 1] == '`'
				|| input[*i + 1] == '"'))
			(*i)++;
		else if (input[*i] == '`')
			process_backquotes(input, i, count, data);
		(*i)++;
	}
	data->tokens[*count] = create_token(data->current_type, \
	ft_substr_gc(input, start + 1, end - start - 1, &data->gc));
	(*i)++;
	(*count)++;
}
