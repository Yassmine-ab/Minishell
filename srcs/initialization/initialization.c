/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:27:24 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/10 03:27:25 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	data_init(char **argv, char **envp, t_minishell *data)
{
	(void)argv;
	data->envp = envp;
	data->line = NULL;
	data->tokens = (t_token *)malloc(sizeof(t_token) * (MAX_TOKENS + 1));
	if (!data->tokens)
		return ;
	data->is_command = true;
}
