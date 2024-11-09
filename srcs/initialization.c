#include "minishell.h"

void	data_init(int argc, char *argv, char **envp, t_minishell *data)
{
	(void)argc;
	(void)argv;
	data->envp = envp;
	data->line = NULL;
	data->tokens = NULL;
	data->tokens = malloc(sizeof(char *) * 1024);
	if (!data->tokens)
		exit(1);
	for (int i = 0; i < 1024; i++)
	{
		data->tokens[i] = malloc(sizeof(char) * 1024);
		if (!data->tokens[i])
			exit(1);
	}
}
