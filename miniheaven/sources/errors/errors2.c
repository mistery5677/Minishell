/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:08:09 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/16 17:26:32 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* void	free_exit(t_minishell *minishell, char *str)
{
	if (minishell->local)
	{
		free_array(minishell->local);
		minishell->local = NULL;
	}
	if (str && str[0] != '\0')
		ft_putstr_fd(str, 2);
	if (minishell->envp)
		free_array(minishell->envp);
	if (minishell->temp_stdin != -1)
		close (minishell->temp_stdin);
	if (minishell->temp_stdout != -1)
		close (minishell->temp_stdout);
	if (minishell->_pipe_ == 1)
	{
		close(minishell->fd[0]);
		close(minishell->fd[1]);
	}
	if (minishell->fd[1] > 0)
		close(1);
	if (minishell->fd[0] > 0)
		close(0);
	if (minishell->cmd)
	{
		free(minishell->cmd);
		minishell->cmd = NULL;
	}
	if (minishell->infile != -1)
		close(minishell->infile);
	if (minishell->temp_stdin != -1)
		close(0);
	if (minishell->temp_stdout != -1)
		close(1);
	free_utils(minishell);
	fd_clean();
	exit(g_signal);
} */

/**
 * @brief Closse the fd
 * @param int fd
 * @return void
 */
void	close_pipe(int fd)
{
	if (fd >= 0)
		close(fd);
}

/**
 * @brief Free the pointer
 * @param void *ptr
 * @return void
 */
void	free_pointer(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

/**
 * @brief If some file fails to open.
 * @param t_minishell *minishell, char **split_cmd, char *cmd_path
 * @return void
 */
void	error_execute(t_minishell *minishell,
		char **split_cmd, char *cmd_path)
{
	if (g_signal == 1)
	{
		free_array(split_cmd);
		free_exit(minishell, "");
	}
	g_signal = 127;
	ft_putstr_fd(split_cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	if (cmd_path != NULL)
		free(cmd_path);
	free_array(split_cmd);
	free_exit(minishell, "");
}
