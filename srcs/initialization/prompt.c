/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:27:27 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/25 03:24:48 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_prompt(t_gc *gc)
{
	char	*prompt;
	char	*user;
	char	cwd[MAX_PATHLENGTH];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("Failed to get current working directory (getcwd)");
		cwd[0] = '\0';
	}
	user = getenv("USER");
	if (!user)
	{
		perror("Failed to retrieve USER environment variable");
		user = "user";
	}
	prompt = ft_strjoin_gc(YELLOW "\n", user, gc);
	prompt = ft_strjoin_gc(prompt, DEFAULT " in ", gc);
	prompt = ft_strjoin_gc(prompt, GREEN, gc);
	prompt = ft_strjoin_gc(prompt, cwd, gc);
	prompt = ft_strjoin_gc(prompt, DEFAULT " 🥚👶🕺 ⋙  ", gc);
	return (prompt);
}
