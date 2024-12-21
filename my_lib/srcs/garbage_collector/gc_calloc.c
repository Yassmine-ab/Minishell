/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcantin <jcantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 15:14:46 by jcantin           #+#    #+#             */
/*   Updated: 2024/12/21 15:15:24 by jcantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	*gc_calloc(size_t nmemb, size_t size, t_gc *gc)
{
	void	*ptr;

	ptr = calloc(nmemb, size);
	if (!ptr)
	{
		perror("Memory allocation failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	gc_add(gc, ptr, nmemb * size);
	return (ptr);
}
