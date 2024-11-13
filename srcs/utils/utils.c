/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besch <besch@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 05:22:40 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/13 18:36:35 by besch            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	error(const char *error_msg, int status, t_gc *gc)
{
	perror(error_msg);
	gc_clear(gc);
	exit(status);
}
