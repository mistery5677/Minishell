/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:45:40 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/16 14:45:40 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Rebuilds STDIN_FILENO and STDOUT_FILENO
 * @param t_minishell *minishell.
 * @return (int);
 */
int	rebuild_fileno(t_minishell *minishell)
{
	if (minishell->infile != -1)
	{
		if (dup2(minishell->temp_stdin, STDIN_FILENO) == -1)
			return (-1);
		close(minishell->temp_stdin);
	}
	if (minishell->outfile != -1)
	{
		if (dup2(minishell->temp_stdout, STDOUT_FILENO) == -1)
			return (-1);
		close(minishell->temp_stdout);
	}
	return (0);
}

/**
 * @brief Redirects the read, if exists infile or outfile
 * @param t_minishell *minishell.
 * @return (void);
 */
void	close_redir(t_minishell *minishell)
{
	if (minishell->infile != -1)
	{
		close(minishell->infile);
		minishell->infile = -1;
	}
	if (minishell->outfile != -1)
	{
		close(minishell->outfile);
		minishell->outfile = -1;
	}
}

/**
 * @brief Executes a command and exits the fork
 * @param t_minishell *minishell, char **cmd
 * @return (void);
 */
void	ft_execute_pipe(t_minishell *minishell, char **cmd)
{
	char	*cmd_path;

	redirect_read(minishell);
	if (find_builtin(minishell, cmd) == 1)
	{
		free_array(cmd);
		free_exit(minishell, "");
	}
	if (check_execute(minishell, cmd) == 1)
		free_exit(minishell, "");
	if (access(cmd[0], X_OK) == 0)
	{
		if (execve(cmd[0], cmd, minishell->envp) == -1)
			error_execute(minishell, cmd, NULL);
	}
	cmd_path = find_path(minishell, cmd[0]);
	if (cmd_path == NULL)
		error_execute(minishell, cmd, cmd_path);
	if (execve(cmd_path, cmd, minishell->envp) == -1)
		error_execute(minishell, cmd, cmd_path);
}

/**
 * @brief Checks if the command is a directory
 * @param char *str
 * @return (int);
 */
static int	check_dir(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Executes a command
 * @param t_minishell *minishell, char **split_cmd
 * @return (void);
 */
void	execute_cmd(t_minishell *minishell, char **split_cmd)
{
	char	*cmd_path;

	split_cmd = replace_null_with_empty(split_cmd);
	if (split_cmd[0] && access(split_cmd[0], X_OK) == 0)
		execve(split_cmd[0], split_cmd, minishell->envp);
	if (check_dir(split_cmd[0]) == 1)
	{
		g_signal = 126;
		if (access(split_cmd[0], F_OK) == 0 && access(split_cmd[0], X_OK) != 0)
			print_errors("bash: ", split_cmd[0], ": Permission denied\n");
		else
			print_errors("bash: ", split_cmd[0], ": Is a directory\n");
		free_array(split_cmd);
		free_exit(minishell, " ");
	}
	cmd_path = find_path(minishell, split_cmd[0]);
	if (cmd_path == NULL)
		error_execute(minishell, split_cmd, cmd_path);
	if (execve(cmd_path, split_cmd, minishell->envp) != 0)
	{
		g_signal = 127;
		free(cmd_path);
		free_array(split_cmd);
		free_exit(minishell, "Couldn't find the command\n");
	}
}
