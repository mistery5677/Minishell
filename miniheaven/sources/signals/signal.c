/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 14:40:01 by mfrancis          #+#    #+#             */
/*   Updated: 2024/10/23 13:00:10 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles the SIGINT signal (Ctrl+C) in the shell.
 * 
 * When the `SIGINT` signal (pressing `Ctrl+C`) is received it sets the global
 * `g_signal` variable to 130, simulates a new line, and prepares the readline 
 * library to handle the prompt appropriately.
 * It clears the current line and redisplays the prompt.
 * 
 * @param signal The signal received is always `SIGINT` for this handler.
 * 
 * @return Void. Alters the shell's state to handle the signal.
 */
static void	sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = 130;
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
/**
 * @brief Handles the SIGINT signal (Ctrl+C) during heredoc processing.
 * 
 * When the `SIGINT` signal is received during a heredoc operation.
 * It sets the global `g_signal` variable to 130, outputs a new line, and closes 
 * the `STDIN_FILENO` to terminate the heredoc input.
 * 
 * @param signal The signal received is always `SIGINT` for this handler.
 * 
 * @return Void. Modifies the input handling during heredoc.
 */

static void	here_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = 130;
		ft_putchar_fd('\n', 1);
		close(STDIN_FILENO);
	}
}

/**
 * @brief Sets up signal handlers for the main shell process.
 * 
 * This function configures the shell to handle `SIGINT` and `SIGQUIT` signals.
 * - `SIGINT`: Handled by the `sig_handler`, which updates the `g_signal`
 * variable and manages the prompt.
 * - `SIGQUIT`: Ignored by the shell.
 * 
 * It also sets the `SA_RESTART` flag on `SIGINT` to automatically restart 
 * system calls interrupted by `SIGINT`.
 * 
 * @param None
 * 
 * @return Void. Configures signal handlers.
 */
void	setup_signals(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sigemptyset(&sig_int.sa_mask);
	sigemptyset(&sig_quit.sa_mask);
	sig_int.sa_handler = sig_handler;
	sig_quit.sa_handler = SIG_IGN;
	sig_int.sa_flags = SA_RESTART;
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sigaction(SIGQUIT, &sig_quit, NULL);
}

/**
 * @brief Temporarily disables signal handling for SIGINT and SIGQUIT.
 * 
 * This function modifies the signal handlers for `SIGINT` and `SIGQUIT` to
 * ignore these signals wheem the parsing is ongoing.
 * 
 * @param None
 * 
 * @return Void. Changes the signal handling behavior.
 */
void	stop_signals(void)
{
	struct sigaction	sig_ignore;

	sigemptyset(&sig_ignore.sa_mask);
	sig_ignore.sa_handler = SIG_IGN;
	sig_ignore.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_ignore, NULL);
	sigaction(SIGQUIT, &sig_ignore, NULL);
}
/**
 * @brief Sets up signal handlers for heredoc processing.
 * 
 * - `SIGINT`: Handled by the `here_handler` to properly manage heredoc
 * input interruption.
 * - `SIGQUIT`: Ignored by the shell during heredoc.
 * 
 * It also sets the `SA_RESTART` flag for `SIGINT` to ensure system calls 
 * are restarted after the signal is handled.
 * 
 * @param None
 * 
 * @return Void. Configures signal handlers specifically for heredoc.
 */

void	setup_signals_here(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sigemptyset(&sig_int.sa_mask);
	sigemptyset(&sig_quit.sa_mask);
	sig_int.sa_handler = here_handler;
	sig_quit.sa_handler = SIG_IGN;
	sig_int.sa_flags = SA_RESTART;
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sigaction(SIGQUIT, &sig_quit, NULL);
}
