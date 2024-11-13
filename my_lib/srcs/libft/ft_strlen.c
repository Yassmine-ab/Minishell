/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besch <besch@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 19:11:09 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/13 21:02:16 by besch            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strlen libc function is used to calculate the length (i) of a string (*s).
*/
#include "../../includes/libft.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}
