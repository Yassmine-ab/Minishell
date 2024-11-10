/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 19:30:54 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/10 20:27:34 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_file(char *input, int *i, int *count, t_minishell *data)
{
	int	start;

	start = *i;
	if (input[*i] == '\0' || ft_strchr("|<>&()", input[*i]))
	{
		perror("Syntax error: expected filename after redirection");
		return (-1);
	}
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&", input[*i]))
		(*i)++;
	data->tokens[*count] = create_token(FILENAME, ft_substr(input, start, *i - start));
	(*count)++;
	return (0);
}

int	process_wildcard(char *input, int *i, int *count, t_minishell *data)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&", input[*i]))
		(*i)++;
	data->tokens[*count] = create_token(WILDCARD, ft_substr(input, start, *i - start));
	(*count)++;
	return (0);
}

int	process_limiter(char *input, int *i, int *count, t_minishell *data)
{
	int	start;

	start = *i;
	if (input[*i] == '\0' || ft_strchr("|<>&()", input[*i]))
	{
		perror("Syntax error: missing heredoc limiter\n");
		return (-1);
	}
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&", input[*i]))
		(*i)++;
	data->tokens[*count] = create_token(LIMITER, ft_substr(input, start, *i - start));
	(*count)++;
	return (0);
}
