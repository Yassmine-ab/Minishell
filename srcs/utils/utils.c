#include "minishell.h"

void	ft_free(void *ptr)
{
	if (ptr)
		free(ptr);
}

void	error(const char *error_msg, int status, t_minishell *data)
{
	(void)data;
	perror(error_msg);
	// if (data)
	// 	clean_up(data);
	exit(status);
}
