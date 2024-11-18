/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 03:02:44 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/17 03:17:34 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	*gc_realloc(void *ptr, size_t new_size, t_gc *gc)
{
	void	*new_ptr;
	size_t	old_size;

	new_ptr = gc_malloc(new_size, gc);
	if (ptr)
	{
		old_size = gc_malloc_size(ptr, gc);
		if (old_size < new_size)
			ft_memcpy(new_ptr, ptr, old_size);
		else
			ft_memcpy(new_ptr, ptr, new_size);
		gc_remove(gc, ptr);
	}
	return (new_ptr);
}
