/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_gc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 10:01:11 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/15 10:12:27 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

char	*ft_strdup_gc(const char *s1, t_gc *gc)
{
	char	*str;

	str = ft_strdup(s1);
	if (str)
		gc_add(gc, str);
	return (str);
}
