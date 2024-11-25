/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:56:38 by besch             #+#    #+#             */
/*   Updated: 2024/11/25 02:36:21 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_node *cmd_node)
{
	t_node	*arg;
	int		newline;

	arg = cmd_node->left;
	newline = 1;
	while (arg && arg->type == NODE_ARG && arg->value
		&& ft_strncmp(arg->value, "-n", 3) == 0)
	{
		newline = 0;
		arg = arg->next;
	}
	while (arg)
	{
		ft_putstr_fd(arg->value, STDOUT_FILENO);
		if (arg->next)
			ft_putchar_fd(' ', STDOUT_FILENO);
		arg = arg->next;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
