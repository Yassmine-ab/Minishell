#include "minishell.h"

char	*create_prompt(void)
{
	char	*prompt;
	char	*user;
	char	*tmp;
	char	*tmp2;
	char	cwd[4096];

	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);
	user = getenv("USER");
	if (!user)
		user = "user";
	tmp = ft_strjoin(ROSE "\n", user);
	tmp2 = ft_strjoin(tmp, DEFAULT " in ");
	ft_free(tmp);
	tmp = ft_strjoin(tmp2, GREEN);
	ft_free(tmp2);
	tmp2 = ft_strjoin(tmp, cwd);
	ft_free(tmp);
	prompt = ft_strjoin(tmp2, DEFAULT " 🌺 ⋙  ");
	ft_free(tmp2);
	return (prompt);
}
