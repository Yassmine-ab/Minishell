/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*    gc_clear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 22:13:54 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/03 22:14:06 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	gc_clear(t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*next;

	current = gc->head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	gc->head = NULL;
}
