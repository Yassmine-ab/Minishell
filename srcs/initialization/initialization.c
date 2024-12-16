/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:27:24 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/16 03:16:51 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	data_init(int argc, char **argv, char **envp, t_minishell *data)
{
	int	i;

	ft_memset(data, 0, sizeof(t_minishell));
	data->argc = argc;
	data->argv = argv;
	if (envp[0] == NULL)
		data->envp = envp_is_null(data);
	else
	{
		data->envp = gc_malloc
			(sizeof(char *) * (ft_tabstrlen(envp) + 1), &data->gc);
		i = -1;
		while (envp[++i])
			data->envp[i] = ft_strdup_gc(envp[i], &data->gc);
		data->envp[i] = NULL;
	}
	data->tokens = gc_malloc(sizeof(t_token) * (MAX_TOKENS + 1), &data->gc);
	data->current_type = COMMAND;
	data->here_doc[0] = -1;
	data->here_doc[1] = -1;
}

char	**envp_is_null(t_minishell *data)
{
	char	**envp;
	char	*path;

	envp = gc_malloc(sizeof(char *) * 5, &data->gc);
	path = getcwd(NULL, 0);
	envp[0] = ft_strjoin_gc("PWD=", path, &data->gc);
	envp[1] = ft_strdup_gc("SHLVL=1", &data->gc);
	envp[2] = ft_strdup_gc("OLDPWD", &data->gc);
	envp[3] = ft_strdup_gc("_=/usr/bin/env", &data->gc);
	envp[4] = NULL;
	return (envp);
}
