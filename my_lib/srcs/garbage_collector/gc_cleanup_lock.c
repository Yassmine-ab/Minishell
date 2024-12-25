#include "gc.h"

void	gc_cleanup_lock(t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*prev;
	t_gc_node	*next;

	current = gc->head;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (!current->locked)
		{
			free(current->ptr);
			free(current);
			if (prev)
				prev->next = next;
			else
				gc->head = next;
		}
		else
			prev = current;
		current = next;
	}
}
