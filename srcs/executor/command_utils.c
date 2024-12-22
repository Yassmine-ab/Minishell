/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 10:49:41 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/22 14:32:40 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	concatenate_adjacent_nodes(t_node *node, t_minishell *data)
{
	t_node	*current;
	t_node	*temp;
	char	*combined;

	current = node;
	while (current && current->next)
	{
		if (current->space_after == false)
		{
			combined = ft_strjoin_gc(current->value, \
			current->next->value, &data->gc);
			gc_free(current->value, &data->gc);
			current->value = combined;
			temp = current->next;
			current->next = temp->next;
			gc_free(temp->value, &data->gc);
			gc_free(temp, &data->gc);
		}
		else
			current = current->next;
	}
}

char	**get_command_args(t_node *cmd_node, t_minishell *data)
{
	int		arg_count;
	t_node	*arg;
	char	**args;
	int		i;

	arg_count = 1;
	arg = cmd_node->args;
	while (arg)
	{
		arg_count++;
		arg = arg->next;
	}
	args = gc_malloc(sizeof(char *) * (arg_count + 1), &data->gc);
	args[0] = cmd_node->value;
	arg = cmd_node->args;
	i = 1;
	while (arg)
	{
		args[i++] = arg->value;
		arg = arg->next;
	}
	args[i] = NULL;
	return (args);
}

static bool	is_executable(const char *path, t_minishell *data)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		printf("Stat échoué pour: %s, errno: %d\n", path, errno);
		if (errno == ENOENT)
			error("No such file or directory", 127, &data->gc);
		else
			error(strerror(errno), 1, &data->gc);
	}
	if (!S_ISREG(st.st_mode))
		error("Not a regular file", 126, &data->gc);
	if (access(path, X_OK) == -1)
	{
		if (errno == EACCES)
			error("Permission denied", 126, &data->gc);
		else
			error(strerror(errno), 1, &data->gc);
	}
	return (true);
}

char	*get_command_path(char *command, t_minishell *data)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	if (is_executable(command, data) == true)
		return (command);
	path_env = getenv("PATH");
	if (path_env == NULL)
		return (NULL);
	paths = ft_split_gc(path_env, ':', &data->gc);
	if (paths == NULL)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin_gc(paths[i], "/", &data->gc);
		full_path = ft_strjoin_gc(full_path, command, &data->gc);
		if (is_executable(full_path, data) == true)
			return (full_path);
	}
	return (NULL);
}
