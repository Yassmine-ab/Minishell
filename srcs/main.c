#include "minishell.h"

static void	print_tokens(t_token *tokens)
{
	int	i;

	i = -1;
	while (tokens[++i].type != END)
		ft_printf("type: %d, value: %s\n", tokens[i].type, tokens[i].value);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;
	const char	*prompt;

	(void)argc;
	data_init(argv, envp, &data);
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
		print_tokens(tokenize_input(data.line, &data));
	}
	free((void *)prompt);
	rl_clear_history();
	return (0);
}
