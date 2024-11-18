/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:27:24 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/17 23:03:05 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	data_init(int argc, char **argv, char **envp, t_minishell *data)
{
	ft_memset(data, 0, sizeof(t_minishell));
	data->argc = argc;
	data->argv = argv;
	data->envp = envp;
	data->tokens = gc_malloc(sizeof(t_token) * (MAX_TOKENS + 1), &data->gc);
	data->current_type = COMMAND;
}
