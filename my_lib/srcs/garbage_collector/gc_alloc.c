/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_alloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 22:16:01 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/03 22:16:41 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	*gc_alloc(t_gc *gc, size_t size)
{
	t_gc_node	*new_node;

	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
		return (NULL);
	new_node->ptr = malloc(size);
	if (!new_node->ptr)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = gc->head;
	gc->head = new_node;
	return (new_node->ptr);
}