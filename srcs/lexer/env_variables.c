/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 21:54:13 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/15 10:57:22 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_env_variable(char *input, int *i, int *count, t_minishell *data)
{
	char		*var_name;
	char		*value;
	const int	start = ++(*i);

	value = NULL;
	if (input[*i] == '?')
	{
		(*i)++;
		value = ft_itoa_gc(data->last_exit_status, &data->gc);
		data->tokens[*count] = create_token(data->current_type, value);
		(*count)++;
		return ;
	}
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	if (*i == start)
		return ;
	var_name = ft_substr_gc(input, start, *i - start, &data->gc);
	value = getenv(var_name);
	if (!value)
		value = "";
	data->tokens[*count] = create_token(data->current_type, value);
	(*count)++;
}
