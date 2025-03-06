/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:38:11 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/16 16:38:11 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks the permissions of the file that im trying to open
 * @param t_ast *ast, char *file
 * @return (int);
 */
static int	check_permissions(t_ast *ast, char *file)
{
	if (ast->token->type == REDIR_IN)
	{
		if (access(file, F_OK) == 0 && access(file, R_OK) != 0)
			return (print_errors("bash: ", file, ": permissions denied\n"), -1);
	}
	if (ast->token->type == REDIR_OUT || ast->token->type == REDIR_APPEND)
	{
		if (access(file, F_OK) == 0 && access(file, W_OK) != 0)
			return (print_errors("bash: ", file, ": permissions denied\n"), -1);
	}
	return (0);
}

/**
 * @brief Prints the error if doesnt find the file
 * @param char *file
 * @return (int);
 */
static int	print_error_opening(char *file)
{
	print_errors("bash: ", file, ": No such file or directory\n");
	return (-1);
}

/**
 * @brief Utils for open_file function
 * @param t_minishell *minishell, t_ast *ast, char *file
 * @return (int)
 */
static int	open_file_util(t_minishell *minishell, t_ast *ast, char *file)
{
	if (check_permissions(ast, file) == -1)
		return (-1);
	else if (ft_strlen(file) == 0)
		return (print_error_opening(file));
	else if (ast->token->type == REDIR_IN)
	{
		if (access(file, F_OK) != 0)
			return (print_error_opening(file));
		if (minishell->infile != -1)
			close(minishell->infile);
		minishell->infile = open(file, O_RDONLY, 0644);
	}
	else if (ast->token->type == REDIR_OUT || ast->token->type == REDIR_APPEND)
	{
		if (minishell->outfile != -1)
			close(minishell->outfile);
		if (ast->token->type == REDIR_OUT)
			minishell->outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		else
			minishell->outfile = open(file, O_WRONLY | O_CREAT | O_APPEND,
					0644);
	}
	return (0);
}

/**
 * @brief Set the file descriptor for the redirection 
 * in infile or outfile var in minishell struct
 * @param t_minishell *minishell, t_ast *ast, char *file
 * @return (int)
 */
int	open_file(t_minishell *minishell, t_ast *ast)
{
	char		*file;
	t_heredoc	*temp_here;

	temp_here = minishell->heredoc;
	file = ast->token->path;
	if (open_file_util(minishell, ast, file) == -1)
		return (-1);
	else if (ast->token->type == REDIR_HEREDOC)
	{
		if (minishell->infile != -1)
			close(minishell->infile);
		minishell->infile = dup(minishell->heredoc->fd[0]);
		close(minishell->heredoc->fd[0]);
		free(minishell->heredoc->delimiter);
		temp_here = minishell->heredoc->next;
		free(minishell->heredoc);
		minishell->heredoc = temp_here;
	}
	return (0);
}

/**
 * @brief Redirect the input and output file descriptors
 * @param t_minishell *minishell
 * @return (int)
 */
int	redirect_read(t_minishell *minishell)
{
	if (minishell->infile != -1)
	{
		if (minishell->_pipe_ == 0)
			minishell->temp_stdin = dup(STDIN_FILENO);
		if (dup2(minishell->infile, STDIN_FILENO) == -1)
			return (-1);
		close(minishell->infile);
	}
	if (minishell->outfile != -1)
	{
		minishell->temp_stdout = dup(STDOUT_FILENO);
		if (dup2(minishell->outfile, STDOUT_FILENO) == -1)
			return (-1);
		close(minishell->outfile);
	}
	return (0);
}
