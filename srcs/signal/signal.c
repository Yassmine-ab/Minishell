/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 04:56:07 by petitcoeur        #+#    #+#             */
/*   Updated: 2024/12/19 11:19:58 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

static int	sig_exit(void)
{
	return (EXIT_SUCCESS);
}

void	child_signal_to_action(t_minishell *data)
{
	if (g_signal_received == 128 + SIGINT && data->child_end_with_signal)
		ft_putchar_fd('\n', STDERR_FILENO);
	else if (g_signal_received == 128 + SIGQUIT && data->child_end_with_signal)
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
	data->child_end_with_signal = false;
	data->last_exit_status = g_signal_received;
	g_signal_received = 0;
}

static void	handle_signal_exec(int sig)
{
	g_signal_received = 128 + sig;
}

void	signal_child_process(void)
{
	rl_event_hook = sig_exit;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_IGN);
}

void	init_signal_exec(void)
{
	struct sigaction	sa;

	rl_event_hook = sig_exit;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = &handle_signal_exec;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGTSTP, &sa, NULL);
}

static void	sigint_interactive_mode(int sig)
{
	g_signal_received = 128 + sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	rl_done = 1;
}

void	init_signal_interactive_mode(void)
{
	struct sigaction	sa;

	rl_event_hook = sig_exit;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = &sigint_interactive_mode;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
}
