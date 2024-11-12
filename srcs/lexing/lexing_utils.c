/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:57:26 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/11 18:12:28 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	create_token(t_token_type type, char *value)
{
	t_token	new_token;

	new_token.type = type;
	new_token.value = value;
	return (new_token);
}

// void	free_tokens(t_token *tokens)
// {
// 	int	i;

// 	i = -1;
// 	while (tokens[++i].type != END)
// 		ft_free(tokens[i].value);
// 	free(tokens);
// }
