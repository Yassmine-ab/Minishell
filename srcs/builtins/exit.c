/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: petitcoeur <petitcoeur@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:58:18 by besch             #+#    #+#             */
/*   Updated: 2024/12/16 03:46:37 by petitcoeur       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_number(char *nb)
{
	long long int	result;
	int				sign;
	int				i;

	i = 0;
	sign = 1;
	result = 0;
	if (ft_strlen(nb) > 20)
		return (1);
	if (nb[i] == '+' || nb[i] == '-')
		if (nb[i++] == '-')
			sign = -1;
	while (nb[i] >= '0' && nb[i] <= '9')
	{
		if (result > 922337203685477580
			|| (sign == 1 && result == 922337203685477580 && nb[i] > '7')
			|| (sign == -1 && result == 922337203685477580 && nb[i] > '8'))
			return (1);
		result *= 10;
		result += (nb[i++] - '0');
	}
	return (0);
}

static int	is_numeric(char *nb)
{
	int	i;

	i = 0;
	if (!nb)
		return (0);
	if (nb[i] == '+' || nb[i] == '-')
		i++;
	while (nb[i])
	{
		if (!ft_isdigit(nb[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	check_arg_value(char *arg, t_minishell *data)
{
	if (is_numeric(arg) || is_valid_number(arg))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		data->last_exit_status = 2;
		gc_cleanup(&data->gc);
		exit(data->last_exit_status);
	}
	data->last_exit_status = ft_atoi(arg);
	return ;
}

void	ft_exit(t_node *cmd_node, t_minishell *data)
{
	if (cmd_node->left)
	{
		if (cmd_node->left->next)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			ft_putstr_fd
				("minishell: exit: too many arguments\n", STDERR_FILENO);
			data->last_exit_status = 1;
			return ;
		}
		check_arg_value(cmd_node->left->value, data);
	}
	ft_putstr_fd("exit\n", STDERR_FILENO);
	gc_cleanup(&data->gc);
	exit(data->last_exit_status);
}
