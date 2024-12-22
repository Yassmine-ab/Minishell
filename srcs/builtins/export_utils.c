/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 02:48:46 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/22 00:43:26 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	bubble_sort(char **envp, int size)
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

static void	display_export_vars(char **sorted_envp, t_minishell *data)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	int		i;

	i = -1;
	while (sorted_envp[++i])
	{
		ft_putstr_fd("export ", STDOUT_FILENO);
		equal_sign = ft_strchr(sorted_envp[i], '=');
		if (!equal_sign)
			ft_putstr_fd(sorted_envp[i], STDOUT_FILENO);
		else
		{
			key = ft_substr_gc
				(sorted_envp[i], 0, equal_sign - sorted_envp[i], &data->gc);
			value = get_env_value(key, data);
			ft_putstr_fd(key, STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(value, STDOUT_FILENO);
			ft_putchar_fd('"', STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

void	print_export(t_minishell *data)
{
	char	**sorted_envp;
	int		env_count;
	int		i;

	env_count = ft_tabstrlen(data->envp);
	sorted_envp = gc_malloc(sizeof(char *) * (env_count + 1), &data->gc);
	i = -1;
	while (++i < env_count)
		sorted_envp[i] = ft_strdup_gc(data->envp[i], &data->gc);
	sorted_envp[i] = NULL;
	bubble_sort(sorted_envp, env_count);
	display_export_vars(sorted_envp, data);
	i = -1;
	while (sorted_envp[++i])
		gc_free(sorted_envp[i], &data->gc);
	gc_free(sorted_envp, &data->gc);
}
