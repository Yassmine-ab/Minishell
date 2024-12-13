/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:56:51 by besch             #+#    #+#             */
/*   Updated: 2024/12/13 21:34:16 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd_vars(char *new_path, t_minishell *data)
{
	char	*old_path;
	int		var_pwd_idx;
	int		var_old_pwd_idx;

	old_path = get_env_value("PWD", data);
	if (old_path)
	{
		var_pwd_idx = get_env_index("PWD", data);
		gc_free(data->envp[var_pwd_idx], &data->gc);
		data->envp[var_pwd_idx] = ft_strjoin_gc("PWD=", new_path, &data->gc);
	}
	var_old_pwd_idx = get_env_index("OLDPWD", data);
	if (var_old_pwd_idx != -1 && old_path)
	{
		gc_free(data->envp[var_old_pwd_idx], &data->gc);
		data->envp[var_old_pwd_idx] = \
			ft_strjoin_gc("OLDPWD=", old_path, &data->gc);
		gc_free(old_path, &data->gc);
	}
}

static void	update_pwd(t_minishell *data)
{
	char	*new_path;

	new_path = getcwd(NULL, 0);
	if (new_path)
	{
		update_pwd_vars(new_path, data);
		free(new_path);
		data->last_exit_status = 0;
		return ;
	}
	ft_putstr_fd("pwd: can't get current path\n", STDERR_FILENO);
	data->last_exit_status = 1;
}

static int	change_directory(char *path, t_minishell *data)
{
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("cd: no such file or directory: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		data->last_exit_status = 1;
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: permission denied: ", STDERR_FILENO);
		data->last_exit_status = 1;
		return (1);
	}
	return (0);
}

void	ft_cd(t_node *cmd_node, t_minishell *data)
{
	char	*path;

	if (!cmd_node->left || !cmd_node->left->value)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			data->last_exit_status = 1;
			return ;
		}
	}
	else
		path = cmd_node->left->value;
	if (!change_directory(path, data))
		update_pwd(data);
}
