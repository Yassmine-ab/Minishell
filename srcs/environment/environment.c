/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:08:19 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/13 08:32:59 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(t_minishell *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		gc_free(data->envp[i], &data->gc);
		i++;
	}
	gc_free(data->envp, &data->gc);
}

char	*get_env_value(char *env_key, t_minishell *data)
{
	int		i;
	char	*value;

	i = 0;
	value = NULL;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], env_key, ft_strlen(env_key)) == 0)
		{
			value = ft_strdup_gc(ft_strchr(data->envp[i], '=') + 1, &data->gc);
			break ;
		}
		i++;
	}
	return (value);
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
	gc_free(data->envp[i], &data->gc);
	data->envp[i] = ft_strjoin_gc(key, "=", &data->gc);
	data->envp[i] = ft_strjoin_gc(data->envp[i], new_value, &data->gc);
}

void	add_env(char *key, char *value, t_minishell *data)
{
	char	**new_envp;
	char	*new_key_value;
	int		i;

	i = ft_tabstrlen(data->envp);
	new_envp = gc_malloc(sizeof(char *) * (i + 2), &data->gc);
	i = -1;
	while (data->envp[++i])
		new_envp[i] = ft_strdup_gc(data->envp[i], &data->gc);
	if (value == NULL)
		new_key_value = ft_strdup_gc(key, &data->gc);
	else
	{
		new_key_value = ft_strjoin_gc(key, "=", &data->gc);
		if (value)
			new_key_value = ft_strjoin_gc(new_key_value, value, &data->gc);
	}
	new_envp[i] = new_key_value;
	new_envp[i + 1] = NULL;
	free_envp(data);
	data->envp = new_envp;
}
