/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 19:30:54 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/05 16:20:45 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_word(char *input, int *i, int *count, t_minishell *data)
{
	char		*value;
	char		*temp;

	value = ft_strdup_gc("", &data->gc);
	while (input[*i] && !ft_isspace(input[*i])
		&& !ft_strchr("()|<>", input[*i]))
	{
		if (input[*i] == '&' && input[*i + 1] && input[*i + 1] == '&')
			break ;
		else if (input[*i] == '\'')
		{
			process_single_quotes(input, i, &value, data);
		}
		else if (input[*i] == '"')
		{
			process_double_quotes(input, i, &value, count, data);
		}
		else
		{
			temp = ft_substr_gc(input, *i, 1, &data->gc);
			value = ft_strjoin_gc(value, temp, &data->gc);
			(*i)++;
		}
	}
	data->tokens[*count] = create_token(data->current_type, value);
	(*count)++;
	skip_whitespace(&input, i);
	if (data->current_type == COMMAND)
		data->current_type = ARGUMENT;
	else if (data->current_type == FILENAME || data->current_type == LIMITER)
		data->current_type = COMMAND;
}
