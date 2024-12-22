/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:27:24 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/22 16:27:30 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pids_init(t_pids *pids, t_minishell *data)
{
	pids->count = 0;
	pids->capacity = 10;
	pids->pids = gc_malloc(sizeof(pid_t) * pids->capacity, &data->gc);
}

static char	**envp_is_null(t_minishell *data)
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

static void	update_shlvl(t_minishell *data)
{
	int	shlvl_value;

	if (get_env_index("SHLVL", data) == -1)
		add_env("SHLVL", "1", data);
	else
	{
		shlvl_value = ft_atoi(get_env_value("SHLVL", data));
		if (shlvl_value < 0)
			shlvl_value = -1;
		update_env
			("SHLVL", ft_itoa_gc(shlvl_value + 1, &data->gc), data);
	}
}

void	data_init(int argc, char **argv, char **envp, t_minishell *data)
{
	int	i;

	ft_memset(data, 0, sizeof(t_minishell));
	gc_init(&data->gc);
	data->pids = gc_malloc(sizeof(t_pids), &data->gc);
	pids_init(data->pids, data);
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
		update_shlvl(data);
	}
	data->tokens = gc_malloc(sizeof(t_token) * (MAX_TOKENS + 1), &data->gc);
	data->current_type = COMMAND;
	data->here_doc[0] = -1;
	data->here_doc[1] = -1;
	data->last_exec_error = EXEC_NO_FILE;
}
