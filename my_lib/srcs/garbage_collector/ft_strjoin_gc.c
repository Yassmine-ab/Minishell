/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_gc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 03:20:52 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/14 03:23:26 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

char	*ft_strjoin_gc(char const *s1, char const *s2, t_gc *gc)
{
	size_t	len1;
	size_t	len2;
	char	*joined_str;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined_str = gc_malloc(len1 + len2 + 1, gc);
	if (!joined_str)
		return (NULL);
	ft_strlcpy(joined_str, s1, len1 + 1);
	ft_strlcat(joined_str, s2, len1 + len2 + 1);
	return (joined_str);
}
