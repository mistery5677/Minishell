/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:37:03 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/16 14:37:03 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/minishell.h"

/**
 * @brief Frees the array
 * @param char **array
 * @return (void);
 */
void	free_array(char **array)
{
	int	idx;

	if (array)
	{
		idx = 0;
		while (array[idx])
		{
			free(array[idx]);
			idx++;
		}
		free(array);
	}
}

/**
 * @brief Frees the tokens
 * @param t_minishell *minishell
 * @return (void);
 */
void	free_tokens(t_minishell *minishell)
{
	t_token	*current;
	t_token	*next;

	current = minishell->tokens;
	while (current)
	{
		if (current->next)
			next = current->next;
		else
			next = NULL;
		if (current->str)
			free(current->str);
		if (current->path)
			free(current->path);
		if (current->cmd)
			free(current->cmd);
		free(current);
		current = next;
	}
	minishell->tokens = NULL;
}

/**
 * @brief Frees the expand
 * @param t_expand *expander
 * @return (void);
 */
void	free_expand(t_expand *expander)
{
	if (expander != NULL)
	{
		if (expander->var_content != NULL)
		{
			free(expander->var_content);
			expander->var_content = NULL;
		}
		if (expander->var_name != NULL)
		{
			free(expander->var_name);
			expander->var_name = NULL;
		}
	}
}

/**
 * @brief Frees the ast
 * @param t_ast *ast
 * @return (void);
 */
void	free_ast(t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->token->type <= 3)
	{
		free(ast->token->path);
		if (ast->token->cmd)
			free_array(ast->token->cmd);
	}
	if (ast->left)
		free_ast(ast->left);
	free(ast->token->str);
	free(ast->token);
	if (ast->right)
		free_ast(ast->right);
	free(ast);
}

/**
 * @brief Frees the heredoc
 * @param t_heredoc *heredoc
 * @return (void);
 */
void	free_heredoc(t_heredoc *heredoc)
{
	t_heredoc	*temp;
	t_heredoc	*next;

	temp = heredoc;
	while (temp)
	{
		if (temp->delimiter != NULL)
		{
			free(temp->delimiter);
			temp->delimiter = NULL;
		}
		if (temp->fd[0] != -1)
		{
			close(temp->fd[0]);
			temp->fd[0] = -1;
		}
		if (temp->fd[1] != -1)
		{
			close(temp->fd[1]);
			temp->fd[1] = -1;
		}
		next = temp->next;
		free(temp);
		temp = next;
	}
}
