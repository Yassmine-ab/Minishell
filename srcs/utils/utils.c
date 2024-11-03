#include "minishell.h"

void	error(const char *error_msg, int status, t_shell *data)
{
	perror(error_msg);
	if (data)
		clean_up(data);
	exit(status);
}
