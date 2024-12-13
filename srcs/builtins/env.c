/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:58:01 by besch             #+#    #+#             */
/*   Updated: 2024/12/13 16:18:42 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Je l'ai foutu la mais il sert pour export
j'voulais juste pas creer un fichier que pour ca */
void	bubble_sort(char **envp, int size)
{
	char	*temp;
	int		i;
	int		j;

	i = -1;
	while (++i < size - 1)
	{
		j = -1;
		while (++j < size - 1 - i)
		{
			if (ft_strncmp(envp[j], envp[j + 1], ft_strlen(envp[j]) + 1) > 0)
			{
				temp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = temp;
			}
		}
	}
}

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
