/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:56:38 by besch             #+#    #+#             */
/*   Updated: 2024/12/13 05:13:41 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_node *cmd_node, t_minishell *data)
{
	int		newline;

	(void)data;
	newline = 1;
	while (cmd_node->left && cmd_node->left->type == NODE_ARG
		&& cmd_node->left->value
		&& ft_strncmp(cmd_node->left->value, "-n", 3) == 0)
	{
		newline = 0;
		cmd_node->left = cmd_node->left->next;
	}
	while (cmd_node->left)
	{
		ft_putstr_fd(cmd_node->left->value, STDOUT_FILENO);
		if (cmd_node->left->next)
			ft_putchar_fd(' ', STDOUT_FILENO);
		cmd_node->left = cmd_node->left->next;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	data->last_exit_status = 0;
}
