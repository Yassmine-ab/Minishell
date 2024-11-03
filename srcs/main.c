#include "minishell.h"

int	main(int argc, char **argv)
{
	t_shell	data;

	(void)argv;
	(void)data;
	if (argc > 1)
		error("Too many arguments", 1, NULL);
	// data_init(argv, &data);
	return (0);
}
