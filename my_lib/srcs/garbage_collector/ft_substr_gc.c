/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr_gc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:29:38 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/10 23:30:25 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

char	*ft_substr_gc(char *s, unsigned int start, size_t len, t_gc *gc)
{
	char	*substr;

	substr = ft_substr(s, start, len);
	if (substr)
		gc_add(gc, substr);
	return (substr);
}
