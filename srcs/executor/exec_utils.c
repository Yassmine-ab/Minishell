/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 03:42:30 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/17 02:18:22 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	strncat_realloc(char **result, char *append, size_t *size, t_gc *gc)
{
	size_t	new_len;

	new_len = ft_strlen(*result) + ft_strlen(append) + 1;
	if (new_len > *size)
	{
		*size = new_len + 16;
		*result = gc_realloc(*result, *size, gc);
	}
	ft_strlcat(*result, append, *size);
}

void	close_fd(int *fd)
{
	if (fd && *fd != -1)
	{
		if (close(*fd) == -1)
			perror("Error closing file descriptor");
		else
			*fd = -1;
	}
}

void	free_split(char **strs, t_gc *gc)
{
	int	i;

	i = 0;
	if (strs == NULL)
		return ;
	while (strs[i])
	{
		gc_free(strs[i], gc);
		i++;
	}
	gc_free(strs, gc);
}

void	free_args(char **args, t_minishell *data)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		gc_free(args[i], &data->gc);
		i++;
	}
	gc_free(args, &data->gc);
}

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
