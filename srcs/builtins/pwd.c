/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:57:04 by besch             #+#    #+#             */
/*   Updated: 2024/11/25 01:41:08 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_node *cmd_node)
{
	char	*path;
	t_node	*arg;

	arg = cmd_node->left;
	if (arg)
	{
		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
		return ;
	}
	path = getcwd(NULL, 0);
	if (!path)
		perror("pwd");
	else
	{
		ft_putstr_fd(path, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		free(path);
	}
}
