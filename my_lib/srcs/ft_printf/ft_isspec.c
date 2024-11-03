/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:37:46 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/03 21:35:08 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_isspec(int c)
{
	return (ft_isconv(c) || ft_isdigit(c) || ft_isflag(c));
}
/*
This function checks if the character is a flag.
*/
