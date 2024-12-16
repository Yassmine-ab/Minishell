/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:58:01 by besch             #+#    #+#             */
/*   Updated: 2024/12/16 02:51:33 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_node *cmd_node, t_minishell *data)
{
	char	*equal_sign;
	int		i;

	if (cmd_node->left)
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		data->last_exit_status = 1;
		return ;
	}
	i = -1;
	while (data->envp[++i])
	{
		equal_sign = ft_strchr(data->envp[i], '=');
		if (equal_sign)
		{
			ft_putstr_fd(data->envp[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
	}
	data->last_exit_status = 0;
}
