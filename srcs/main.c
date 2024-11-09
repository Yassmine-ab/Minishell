#include "minishell.h"

int	main(int argc, char **argv)
{
	t_minishell	data;
	const char	*prompt;

	(void)argc;
	(void)argv;

	prompt = create_prompt();
	while (1)
	{
		data.line = readline(prompt);
		if (!data.line)
			break ;
		else if (data.line[0] == EOF || data.line[0] == '\0')
		{
			free(data.line);
			break ;
		}
		add_history(data.line);
	}
	free((void *)prompt);
	rl_clear_history();
	return (0);
}
