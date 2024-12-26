#include "minishell.h"

void	signal_heredoc(void)
{
	rl_event_hook = sig_exit;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	signal_child_process(void)
{
	rl_event_hook = sig_exit;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_IGN);
}

void	signal_to_action(t_minishell *data)
{
	if (g_signal_received == 128 + SIGINT && data->child_end_with_signal)
		ft_putchar_fd('\n', STDERR_FILENO);
	else if (g_signal_received == 128 + SIGQUIT && data->child_end_with_signal)
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
	data->child_end_with_signal = false;
	if (g_signal_received)
		data->last_exit_status = g_signal_received;
	g_signal_received = 0;
}