/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:42:36 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/11 01:08:43 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(char **input, int *index)
{
	while ((*input)[*index] && isspace((*input)[*index]))
		(*index)++;
}

static int	process_env_var(char *input, int *i, int *count, t_minishell *data)
{
	int	start;

	start = ++(*i);
	if (input[*i] == '?')
	{
		data->tokens[*count] = create_token(ENV_VARIABLE, ft_substr(input, start, 1));
		(*i)++;
	}
	else if (!ft_isalnum(input[*i]) && input[*i] != '_')
	{
		perror("Syntax error: invalid or missing environment variable name\n");
		return (-1);
	}
	else
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
		data->tokens[*count] = create_token(ENV_VARIABLE, ft_substr(input, start, *i - start));
	}
	(*count)++;
	return (0);
}

static int	process_commands(char *input, int *i, int *count, t_minishell *data)
{
	int		start;
	char	*value;

	start = *i;
	(*i)++;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&()\"'", input[*i]))
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
			process_operator(input, &i, &count, data);
		else if (input[i] == '*')
			process_wildcard(input, &i, &count, data);
		else
			process_commands(input, &i, &count, data);
	}
	data->tokens[count].type = END;
	data->tokens[count].value = NULL;
	return (data->tokens);
}
