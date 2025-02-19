/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 19:26:45 by mistery576        #+#    #+#             */
/*   Updated: 2024/12/06 19:26:45 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Wait for all the pipes to finish
 * @param t_minishell *minishell, pid_t *pids, int num_commands
 * @return (void)
 */
static void	wait_pipes(t_minishell *minishell, pid_t *pids, int num_commands)
{
	int	i;

	i = 0;
	while (i < num_commands)
	{
		waitpid(pids[i], &minishell->exit_status, 0);
		minishell->exit_status = WEXITSTATUS(minishell->exit_status);
		i++;
	}
}

/**
 * @brief Count the number of pipes in the ast
 * @param t_ast *ast
 * @return (int)
 */
static int	count_pipes(t_ast *ast)
{
	t_ast	*temp;
	int		commands;

	temp = ast;
	commands = 1;
	while (temp->token->type == PIPE)
	{
		commands++;
		temp = temp->right;
	}
	return (commands);
}

/**
 * @brief Create a pipeline process
 * @param t_minishell *minishell, t_ast *ast, pid_t *pids, int *i
 * @return (void)
 */
static void	create_pipeline_process(t_minishell *minishell,
	t_ast *ast, pid_t *pids, int *i)
{
	pipe(minishell->fd);
	pids[*i] = fork();
	if (pids[*i] == 0)
	{
		dup2(minishell->prev_fd, STDIN_FILENO);
		dup2(minishell->fd[1], STDOUT_FILENO);
		close(minishell->fd[0]);
		close(minishell->fd[1]);
		free(pids);
		execute_ast(minishell, ast->left, 1);
	}
	close(minishell->fd[1]);
	if (minishell->prev_fd != STDIN_FILENO)
		close(minishell->prev_fd);
	minishell->prev_fd = minishell->fd[0];
	(*i)++;
}

/**
 * @brief Handle the last command in the pipeline
 * @param t_minishell *minishell, t_ast *ast, pid_t *pids, int i
 * @return (void)
 */
static void	handle_last_command(t_minishell *minishell,
	t_ast *ast, pid_t *pids, int i)
{
	pids[i] = fork();
	if (pids[i] == 0)
	{
		dup2(minishell->prev_fd, STDIN_FILENO);
		free(pids);
		execute_ast(minishell, ast, 1);
		exit(EXIT_SUCCESS);
	}
	if (minishell->prev_fd != STDIN_FILENO)
		close(minishell->prev_fd);
}

/**
 * @brief Execute a pipeline
 * @param t_minishell *minishell, t_ast *ast
 * @return (void)
 */
void	do_pipeline(t_minishell *minishell, t_ast *ast)
{
	pid_t	*pids;
	int		i;

	i = 0;
	minishell->prev_fd = STDIN_FILENO;
	minishell->commands = 0;
	minishell->_pipe_ = 1;
	minishell->commands = count_pipes(ast);
	pids = malloc(sizeof(pid_t) * minishell->commands);
	while (ast->token->type == PIPE)
	{
		create_pipeline_process(minishell, ast, pids, &i);
		ast = ast->right;
	}
	handle_last_command(minishell, ast, pids, i);
	wait_pipes(minishell, pids, minishell->commands);
	free(pids);
	minishell->commands = 0;
	minishell->_pipe_ = 0;
}
