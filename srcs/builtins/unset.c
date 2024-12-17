/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:57:42 by besch             #+#    #+#             */
/*   Updated: 2024/12/17 03:22:34 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_unset_error(char *key, t_minishell *data)
{
	ft_putstr_fd("unset: ", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putstr_fd(": not found\n", STDERR_FILENO);
	data->last_exit_status = 1;
}

void	ft_unset(t_node *cmd_args, t_minishell *data)
{
	int		index;
	char	**new_envp;
	int		i;
	int		j;

	while (cmd_args)
	{
		index = get_env_index(cmd_args->value, data);
		if (index == -1)
			print_unset_error(cmd_args->value, data);
		else
		{
			i = 0;
			j = -1;
			new_envp = gc_malloc
				(sizeof(char *) * ft_tabstrlen(data->envp), &data->gc);
			while (data->envp[++j])
				if (j != index)
					new_envp[i++] = ft_strdup_gc(data->envp[j], &data->gc);
			new_envp[i] = NULL;
			free_envp(data);
			data->envp = new_envp;
		}
		cmd_args = cmd_args->next;
	}
}
