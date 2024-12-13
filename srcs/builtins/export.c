/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:57:24 by besch             #+#    #+#             */
/*   Updated: 2024/12/13 09:06:50 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	var_key_checks(char *key, t_minishell *data)
{
	int	i;

	if (!ft_isalpha(key[0]) && key[0] != '_')
	{
		ft_putstr_fd("export: '", STDERR_FILENO);
		ft_putstr_fd(key, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		data->last_exit_status = 1;
		return (1);
	}
	i = 0;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			ft_putstr_fd("export: '", STDERR_FILENO);
			ft_putstr_fd(key, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			data->last_exit_status = 1;
			return (1);
		}
	}
	return (0);
}

static void	display_export_vars(char **sorted_envp)
{
	char	*equal_sign;
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
			ft_putstr_fd(sorted_envp[i], STDOUT_FILENO);
			if (*(equal_sign + 1) == '\0')
				ft_putstr_fd("\"\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

static void	print_export(t_minishell *data)
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
	display_export_vars(sorted_envp);
	i = -1;
	while (sorted_envp[++i])
		gc_free(sorted_envp[i], &data->gc);
	gc_free(sorted_envp, &data->gc);
}

static void	handle_export_with_value(char *arg, t_minishell *data)
{
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	key = ft_substr_gc(arg, 0, equal_sign - arg, &data->gc);
	if (!var_key_checks(key, data))
	{
		value = ft_strdup_gc(equal_sign + 1, &data->gc);
		if (!value)
			value = ft_strdup_gc("", &data->gc);
		if (get_env_index(key, data) == -1)
			add_env(key, value, data);
		else
			update_env(key, value, data);
	}
	(gc_free(key, &data->gc), gc_free(value, &data->gc));
}

void	ft_export(t_node *cmd_node, t_minishell *data)
{
	char	*arg;
	char	*equal_sign;
	char	*key;

	if (!cmd_node->left)
		print_export(data);
	while (cmd_node->left)
	{
		arg = cmd_node->left->value;
		equal_sign = ft_strchr(arg, '=');
		if (!equal_sign)
		{
			if (!var_key_checks(arg, data))
			{
				key = ft_strdup_gc(arg, &data->gc);
				if (get_env_index(key, data) == -1)
					add_env(key, NULL, data);
				gc_free(key, &data->gc);
			}
		}
		else
			handle_export_with_value(arg, data);
		cmd_node->left = cmd_node->left->next;
	}
}
