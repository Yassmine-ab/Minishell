/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:08:19 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/18 11:45:35 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(t_minishell *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		gc_free(data->envp[i], &data->gc_env);
		i++;
	}
	gc_free(data->envp, &data->gc_env);
}

char	*get_env_value(char *env_key, t_minishell *data)
{
	int		i;
	char	*env_var;
	char	*equal_sign;

	i = 0;
	while (data->envp[i])
	{
		env_var = data->envp[i];
		if (ft_strncmp(env_var, env_key, ft_strlen(env_key)) == 0
			&& (env_var[ft_strlen(env_key)] == '='))
		{
			equal_sign = ft_strchr(env_var, '=');
			if (equal_sign)
				return (ft_strdup_gc(equal_sign + 1, &data->gc_env));
		}
		i++;
	}
	return (NULL);
}

int	get_env_index(char *env_key, t_minishell *data)
{
	int		i;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], env_key, ft_strlen(env_key)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void	update_env(char *key, char *new_value, t_minishell *data)
{
	int		i;

	i = get_env_index(key, data);
	gc_free(data->envp[i], &data->gc_env);
	data->envp[i] = ft_strjoin_gc(key, "=", &data->gc_env);
	data->envp[i] = ft_strjoin_gc(data->envp[i], new_value, &data->gc_env);
}

void	add_env(char *key, char *value, t_minishell *data)
{
	char	**new_envp;
	char	*new_key_value;
	int		i;

	i = ft_tabstrlen(data->envp);
	new_envp = gc_malloc(sizeof(char *) * (i + 2), &data->gc_env);
	i = -1;
	while (data->envp[++i])
		new_envp[i] = ft_strdup_gc(data->envp[i], &data->gc_env);
	if (value == NULL)
		new_key_value = ft_strdup_gc(key, &data->gc_env);
	else
	{
		new_key_value = ft_strjoin_gc(key, "=", &data->gc_env);
		if (value)
			new_key_value = ft_strjoin_gc(new_key_value, value, &data->gc_env);
	}
	new_envp[i] = new_key_value;
	new_envp[i + 1] = NULL;
	free_envp(data);
	data->envp = new_envp;
}
