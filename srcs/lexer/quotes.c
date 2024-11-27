/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:54:14 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/27 20:45:33 by yaabdall         ###   ########.fr       */
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
			&& input[char_index + 1] && input[char_index + 1] == '"')
			char_index++;
		else if (input[char_index] == match_char)
			return (char_index);
		char_index++;
	}
	return (-1);
}

static int	handle_unclosed_char(char **input, int start, t_gc *gc)
{
	char	*additional_input;
	int		end;

	while (1)
	{
		write(1, "Unclosed quote detected. Continue input > ", 42);
		additional_input = get_next_line(0);
		if (!additional_input)
			return (-1);
		if (*additional_input)
			*input = ft_strjoin_gc(*input, additional_input, gc);
		ft_free(&additional_input);
		end = find_matching_char(*input, start);
		if (end != -1)
			return (end);
	}
	return (-1);
}

void	process_single_quotes(char *input, int *i, char **value, t_minishell *data)
{
	int		start;
	int		end;
	char	*temp;

	while (input[*i] == '\'')
	{
		end = find_matching_char(input, *i);
		if (end == -1)
			end = handle_unclosed_char(&input, *i, &data->gc);
		(*i)++;
		start = *i;
		while (*i < end)
			(*i)++;
		temp = ft_substr_gc(input, start, end - start, &data->gc);
		*value = ft_strjoin_gc(*value, temp, &data->gc);
		(*i) = end + 1;
	}
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

void	process_double_quotes(char *input, int *i, char **value, int *count, t_minishell *data)
{
	int			start;
	int			end;
	char		*temp;

	while (input[*i] == '"')
	{
		end = find_matching_char(input, *i);
		if (end == -1)
			end = handle_unclosed_char(&input, *i, &data->gc);
		(*i)++;
		start = *i;
		while ((*i)++ < end)
		{
			if (input[*i] == '\\' && (*i + 1 < end) && input[*i + 1] == '`')
				(*i)++;
			else if (input[*i] == '`')
				process_backquotes(input, i, count, data);
		}
		temp = ft_substr_gc(input, start, end - start, &data->gc);
		*value = ft_strjoin_gc(*value, temp, &data->gc);
		(*i) = end + 1;
	}
}
