/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:54:14 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/13 02:11:48 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_escaped_char(char *input, int *i, t_minishell *data)
{
	char	*temp;

	temp = NULL;
	if (input[*i] == '\\' && input[*i + 1] == '"')
	{
		*i += 2;
		temp = ft_substr_gc(input, *i - 1, 1, &data->gc);
	}
	else if (input[*i] == '\\')
	{
		*i += 2;
		temp = ft_substr_gc(input, *i - 1, 1, &data->gc);
	}
	return (temp);
}

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

char	*process_single_quotes(char *input, int *i, t_minishell *data)
{
	char	*value;
	int		start;
	int		end;

	value = ft_strdup_gc("", &data->gc);
	end = find_matching_char(input, *i);
	if (end == -1)
		end = handle_unclosed_char(&input, *i, &data->gc);
	(*i)++;
	start = *i;
	while (*i < end)
		(*i)++;
	value = ft_substr_gc(input, start, end - start, &data->gc);
	(*i) = end + 1;
	return (value);
}

char	*process_double_quotes(char *input, int *i, t_minishell *data)
{
	char		*value;
	const int	start = *i + 1;
	int			end;

	value = ft_strdup_gc("", &data->gc);
	end = find_matching_char(input, *i);
	if (end == -1)
		end = handle_unclosed_char(&input, *i, &data->gc);
	(*i)++;
	while (*i < end)
	{
		if (input[*i] == '\\' && (*i + 1 < end))
			value = handle_escaped_char(input, i, data);
		else
		{
			value = ft_substr_gc(input, start, end - start, &data->gc);
			(*i)++;
		}
	}
	(*i)++;
	return (value);
}
