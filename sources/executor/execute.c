/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miafonso <miafonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:41:40 by mistery576        #+#    #+#             */
/*   Updated: 2024/10/30 11:53:12 by miafonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Execute the ast, if it is redirection, pipe, or word.
 * @param t_minishell *minishell, t_ast *ast, int flag
 * @return (void);
 */
void	execute_ast(t_minishell *minishell, t_ast *ast, int flag)
{
	char	**cmd;

	cmd = NULL;
	if (ast->token->type <= 3)
	{
		execute_redir(minishell, ast, flag);
		return ;
	}
	if (ast->token->type == WORD || ast->token->type == STR)
	{
		cmd = collect_commands(minishell, ast);
		if (!cmd)
		{
			g_signal = 0;
			return ;
		}
		if (flag == -1)
			ft_execute(minishell, cmd);
		else
			ft_execute_pipe(minishell, cmd);
	}
	if (ast->token->type == PIPE)
		do_pipeline(minishell, ast);
	rebuild_fileno(minishell);
	close_redir(minishell);
}

/**
 * @brief Checks if it is built_in
 * @param t_minishell *minishell, char **dp
 * @return (int);
 */
int	find_builtin(t_minishell *minishell, char **dp)
{
	if (ft_strcmp(dp[0], "cd") == 0)
	{
		if (count_array(dp) > 2)
		{
			ft_putstr_fd("cd: too many arguments\n", 2);
			g_signal = 1;
			return (1);
		}
		return (ft_cd(dp, minishell), 1);
	}
	else if (ft_strcmp(dp[0], "echo") == 0)
		return (ft_echo(dp), 1);
	else if (ft_strcmp(dp[0], "env") == 0)
		return (ft_env(minishell, dp), 1);
	else if (ft_strcmp(dp[0], "export") == 0)
		return (ft_export(minishell, dp), 1);
	else if (ft_strcmp(dp[0], "unset") == 0)
		return (ft_unset(dp, minishell), 1);
	else if (ft_strcmp(dp[0], "pwd") == 0)
		return (ft_pwd(), 1);
	else if (ft_strcmp(dp[0], "exit") == 0)
		return (ft_exit(minishell, dp), 1);
	return (0);
}

/**
 * @brief Executes the cmd
 * @param t_minishell *minishell, char **cmd
 * @return (void);
 */
void	ft_execute(t_minishell *minishell, char **cmd)
{
	int		child;

	if (redirect_read(minishell) == -1)
		free_exit(minishell, "Something went wrong with dup2\n");
	if (find_builtin(minishell, cmd) == 1)
	{
		minishell->exit_status = WEXITSTATUS(minishell->exit_status);
		return (free_array(cmd));
	}
	if (check_execute(minishell, cmd) == 1)
		return ;
	child = fork();
	if (child == 0)
		execute_cmd(minishell, cmd);
	else
		waitpid(child, &minishell->exit_status, 0);
	minishell->exit_status = WEXITSTATUS(minishell->exit_status);
	rebuild_fileno(minishell);
	free_array(cmd);
}
