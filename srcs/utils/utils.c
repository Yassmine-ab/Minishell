#include "minishell.h"

void	ft_free(void *ptr)
{
	if (ptr)
		free(ptr);
}

void	error(const char *error_msg, int status, t_gc *gc)
{
	perror(error_msg);
	gc_clear(gc);
	exit(status);
}
