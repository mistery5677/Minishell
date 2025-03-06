/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 00:37:59 by mistery576        #+#    #+#             */
/*   Updated: 2024/12/10 10:21:59 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles the SIGINT and SIGQUIT signals during command execution.
 * 
 * - Sets `g_signal` to 130 when `SIGINT` is received (`Ctrl+C`).
 * - Sets `g_signal` to 131 when `SIGQUIT` is received (`Ctrl+\`).
 * 
 * @param signal The signal received.
 * 
 * @return Void. Modifies the global `g_signal` variable.
 */

static void	exec_handler(int signal)
{
	if (signal == SIGQUIT)
		g_signal = 131;
	if (signal == SIGINT)
		g_signal = 130;
}

/**
 * @brief Configures signal handlers for the execution phase of the shell.
 * 
 * - `SIGINT` or `SIGQUIT`: Handled by the `exec_handler`, which updates 
 * the `g_signal` to 130  or 131.
 * 
 * It also sets the `SA_RESTART` flag for `SIGINT`, allowing certain system 
 * calls to be automatically restarted after the signal is handled.
 * 
 * @param None
 * 
 * @return Void. Configures the signal handlers for the execution phase.
 */
void	setup_signals_executer(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sigemptyset(&sig_int.sa_mask);
	sigemptyset(&sig_quit.sa_mask);
	sig_int.sa_handler = exec_handler;
	sig_quit.sa_handler = exec_handler;
	sig_int.sa_flags = SA_RESTART;
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sigaction(SIGQUIT, &sig_quit, NULL);
}
