/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:42:36 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/10 03:24:38 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_whitespace(char **input, int *index)
{
	while ((*input)[*index] == ' ')
		(*index)++;
}

static int	process_env_var(char *input, int *i, int *count, t_minishell *data)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	data->tokens[*count] = create_token(ENV_VARIABLE, \
	ft_substr(input, start, *i - start));
	(*count)++;
	return (0);
}

static int	process_commands(char *input, int *i, int *count, t_minishell *data)
{
	int		start;
	char	*value;

	start = *i;
	(*i)++;
	while (input[*i] && !ft_isspace(input[*i])
		&& !ft_strrchr("|<>&()\"'", input[*i]))
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (data->is_command)
	{
		data->tokens[*count] = create_token(COMMAND, value);
		data->is_command = false;
	}
	else
		data->tokens[*count] = create_token(ARGUMENT, value);
	(*count)++;
	return (0);
}

t_token	*tokenize_input(char *input, t_minishell *data)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (input[i])
	{
		skip_whitespace(&input, &i);
		if (input[i] == '\'' || input[i] == '"')
			process_quotes(input, &i, &count, data);
		else if (input[i] == '(' || input[i] == ')')
			process_parentheses(input, &i, &count, data);
		else if (input[i] == '$')
			process_env_var(input, &i, &count, data);
		else if (ft_strchr("|<>&", input[i]))
			process_operators(input, &i, &count, data);
		else
			process_commands(input, &i, &count, data);
	}
	data->tokens[count].type = END;
	data->tokens[count].value = NULL;
	return (data->tokens);
}
