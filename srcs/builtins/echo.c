/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:56:38 by besch             #+#    #+#             */
/*   Updated: 2024/12/16 02:33:34 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[++i])
		if (arg[i] != 'n')
			return (0);
	return (1);
}

void	ft_echo(t_node *cmd_node, t_minishell *data)
{
	int		newline;

	(void)data;
	newline = 1;
	while (cmd_node->left && cmd_node->left->type == NODE_ARG
		&& cmd_node->left->value
		&& is_valid_n_flag(cmd_node->left->value))
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
