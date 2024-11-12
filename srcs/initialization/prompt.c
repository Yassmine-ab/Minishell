/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:27:27 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/11 19:10:24 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_prompt(void)
{
	char	*prompt;
	char	*user;
	char	*tmp;
	char	*tmp2;
	char	cwd[MAX_PATHLENGTH];

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
