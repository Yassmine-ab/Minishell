/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:57:04 by besch             #+#    #+#             */
/*   Updated: 2024/12/13 09:08:51 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_node *cmd_node, t_minishell *data)
{
	char	*path;

	(void)data;
	(void)cmd_node;
	path = getcwd(NULL, 0);
	if (path)
	{
		ft_putstr_fd(path, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		free(path);
		data->last_exit_status = 0;
		return ;
	}
	ft_putstr_fd("pwd: can't get current path\n", STDERR_FILENO);
	data->last_exit_status = 1;
}
