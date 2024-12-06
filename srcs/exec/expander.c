/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 21:54:13 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/05 17:08:57 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_env_variable(char **result, size_t *size, int i, t_minishell *data)
{
	char	*var_name;
	char	*value;
	int		start;

	if (data->line[++i] == '?')
		value = ft_itoa_gc(data->last_exit_status, &data->gc);
	else if ((ft_isdigit(data->line[i]) && (data->line[i] - '0') < data->argc)
		|| data->line[i] == '_')
		value = data->argv[0];
	else if (ft_isalnum(data->line[i]))
	{
		start = i;
		while (data->line[i] && (ft_isalnum(data->line[i])
				|| data->line[i] == '_'))
			i++;
		var_name = ft_substr_gc(data->line, start, i - start, &data->gc);
		value = getenv(var_name);
		if (!value)
			value = "";
		i--;
	}
	else
		return (strncat_realloc(result, "$", size, &data->gc), i);
	return (strncat_realloc(result, value, size, &data->gc), i + 1);
}

// static int	match_pattern(const char *name, const char *pattern)
// {
// 	if (*pattern == '*')
// 	{
// 		while (*pattern == '*')
// 			pattern++;
// 		if (!*pattern)
// 			return (1);
// 		while (*name)
// 		{
// 			if (match_pattern(name, pattern))
// 				return (1);
// 			name++;
// 		}
// 		return (0);
// 	}
// 	else if (*pattern == *name)
// 		return (match_pattern(name + 1, pattern + 1));
// 	return (0);
// }

static char	**find_wildcard_matches(char *pattern, t_minishell *data)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				count;

	matches = gc_malloc(sizeof(char *) * (MAX_MATCHES + 1), &data->gc);
	count = 0;
	dir = opendir(".");
	if (!dir)
		return (perror("Unable to open directory"), NULL);
	while ((entry = readdir(dir)))
	{
		if ((entry->d_name[0] != '.' && pattern[0] != '.')
			// && match_pattern(entry->d_name, pattern))
			&& fnmatch(pattern, entry->d_name, 0) == 0)
		{
			matches[count++] = ft_strdup_gc(entry->d_name, &data->gc);
			if (count >= MAX_MATCHES)
				break ;
		}
	}
	closedir(dir);
	if (count == 0)
		matches[count++] = ft_strdup_gc(ft_strchr(pattern, '*'), &data->gc);
	matches[count] = NULL;
	return (matches);
}

int	expand_wildcard(char **result, size_t *size, int i, t_minishell *data)
{
	char	**matches;
	int		match_index;
	char	*pattern;
	int		start;

	while (i > 0 && !ft_isspace(data->line[i - 1])
		&& !ft_strchr("\"'()|<>&", data->line[i - 1]))
		i--;
	start = i;
	while (data->line[i]
		&& !ft_isspace(data->line[i]) && !ft_strchr("\"'()|<>&", data->line[i]))
		i++;
	pattern = ft_substr_gc(data->line, start, i - start, &data->gc);
	matches = find_wildcard_matches(pattern, data);
	if (matches)
	{
		match_index = -1;
		while (matches[++match_index])
		{
			if (match_index > 0)
				strncat_realloc(result, " ", size, &data->gc);
			strncat_realloc(result, matches[match_index], size, &data->gc);
		}
	}
	return (i);
}

static char	*process_expansion(char *value, t_minishell *data, \
int (*expander)(char **, size_t *, int, t_minishell *), char expansion_char)
{
	char	*result;
	size_t	size;
	int		i;
	char	append[2];

	ft_memset(append, 0, 2);
	size = ft_strlen(value) + 1;
	result = gc_malloc((sizeof(char) * size), &data->gc);
	result[0] = '\0';
	i = 0;
	while (value[i])
	{
		if (value[i] == expansion_char)
			i = expander(&result, &size, i, data);
		else
		{
			while (value[i] && value[i] != expansion_char)
			{
				append[0] = value[i++];
				strncat_realloc(&result, append, &size, &data->gc);
			}
		}
	}
	return (result);
}

void	expand_variables(t_node *node, t_minishell *data)
{
	char	*expanded;

	if (!node || !node->value)
		return ;
	expanded = process_expansion(node->value, data, expand_env_variable, '$');
	node->value = expanded;
	expanded = process_expansion(node->value, data, expand_wildcard, '*');
	node->value = expanded;
}

