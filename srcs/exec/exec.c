/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:04:05 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/11/25 03:04:52 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_ast(t_node *ast, t_minishell *data)
{
	(void)data;
	if (!ast)
		return ;
	if (ast->type == NODE_COMMAND)
	{
		if (ft_strncmp(ast->value, "echo", 5) == 0)
		{
			printf("Executing ft_echo\n");
			ft_echo(ast);
		}
		else if (ft_strncmp(ast->value, "cd", 3) == 0)
		{
			printf("Executing ft_cd\n");
			int status = ft_cd(ast);
			if (status != 0)
				ft_putstr_fd("Erreur lors de l'exécution de cd\n", STDERR_FILENO);
		}
		else if (ft_strncmp(ast->value, "pwd", 4) == 0)
		{
			printf("Executing ft_pwd\n");
			ft_pwd(ast);
		}
		else
			ft_putstr_fd("Commande non reconnue\n", STDERR_FILENO);
	}
}
