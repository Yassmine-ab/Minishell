/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 19:30:54 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/15 09:16:42 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_file(char *input, int *i, int *count, t_minishell *data)
{
	char	*value;
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !ft_strchr("()|<>&", input[*i]))
		(*i)++;
	value = ft_substr_gc(input, start, *i - start, &data->gc);
	if (!value || !*value)
		return ;
	data->tokens[*count] = \
	create_token(FILENAME, value);
	(*count)++;
	skip_whitespace(&input, i);
	data->current_type = COMMAND;
}

void	process_wildcard(char *input, int *i, int *count, t_minishell *data)
{
	char	*value;
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !ft_strchr("()|<>&", input[*i]))
		(*i)++;
	value = ft_substr_gc(input, start, *i - start, &data->gc);
	if (!value || !*value)
		return ;
	data->tokens[*count] = create_token(WILDCARD, value);
	(*count)++;
	skip_whitespace(&input, i);
	data->current_type = COMMAND;
}

void	process_limiter(char *input, int *i, int *count, t_minishell *data)
{
	char	*value;
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !ft_strchr("()|<>&", input[*i]))
		(*i)++;
	value = ft_substr_gc(input, start, *i - start, &data->gc);
	if (!value || !*value)
		return ;
	data->tokens[*count] = \
	create_token(LIMITER, value);
	(*count)++;
	skip_whitespace(&input, i);
	data->current_type = COMMAND;
}
