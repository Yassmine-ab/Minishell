/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besch <besch@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:27:24 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/13 20:32:12 by besch            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	data_init(char **argv, char **envp, t_minishell *data)
{
	(void)argv;
	data->envp = envp;
	data->line = NULL;
	data->tokens = (t_token *)gc_malloc(sizeof(t_token) * (MAX_TOKENS + 1), &data->gc);
	if (!data->tokens)
		return ;
}
