/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:56:30 by mistery576        #+#    #+#             */
/*   Updated: 2024/12/06 17:56:30 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Copies the command array
 * @param char **cmd
 * @return (char **);
 */
static char	**copy_cmd(char **cmd)
{
	char	**copy;
	int		i;

	i = 0;
	if (!cmd)
		return (NULL);
	while (cmd[i])
		i++;
	copy = malloc(sizeof(char *) * (i));
	i = 0;
	while (cmd[i])
	{
		copy[i] = ft_strdup(cmd[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

/**
 * @brief Copies the ast
 * @param t_ast *original
 * @return (t_ast *);
 */
t_ast	*copy_ast(t_ast *original)
{
	t_ast	*copy;

	if (!original)
		return (NULL);
	copy = malloc(sizeof(t_ast));
	if (!copy)
		return (NULL);
	if (original->token)
	{
		copy->token = malloc(sizeof(t_token));
		if (!copy->token)
			return (free(copy), NULL);
		copy->token->type = original->token->type;
		copy->token->dq = original->token->dq;
		copy->token->expander = original->token->expander;
		copy->token->str = ft_strdup(original->token->str);
		copy->token->path = ft_strdup(original->token->path);
		copy->token->cmd = copy_cmd(original->token->cmd);
		copy->token->next = NULL;
	}
	copy->left = copy_ast(original->left);
	copy->right = copy_ast(original->right);
	return (copy);
}

/**
 * @brief Find commands in no pipe case case
 * @param t_ast *temp_copy, t_ast *orig, t_ast *save_node
 * @return (void);
 */
void	no_pipe(t_ast *temp_copy, t_ast *orig, t_ast *save_node)
{
	if (temp_copy->left)
	{
		if (temp_copy->left->token->dq == 1
			&& ft_count_words(temp_copy->left->token->str) > 1)
			orig->token->dq = 1;
		orig->token->cmd = built_cmd(save_node->left);
	}
	if (temp_copy->right->token->type > 3 && temp_copy->right->right)
		no_pipe_util(orig, temp_copy);
}

/**
 * @brief Find commands in the ast
 * @param t_ast *orig, t_ast *temp_copy, int flag
 * @return (void);
 */
static void	find_commands(t_ast *orig, t_ast *temp_copy, int flag)
{
	t_ast	*save_node;

	save_node = temp_copy;
	if (flag == 0)
		no_pipe(temp_copy, orig, save_node);
	else if (temp_copy->right->token->type > 3
		&& temp_copy->right->right)
	{
		complete_last_redir(temp_copy, orig, save_node);
	}
	else if (temp_copy->left && temp_copy->left->right)
	{
		if (temp_copy->left->right->token->dq == 1)
			orig->token->dq = 1;
		orig->token->cmd = built_cmd(temp_copy->left->right);
	}
}

/**
 * @brief Find files and commands in the ast
 * @param t_ast *orig, t_ast *temp_copy, t_minishell *minishell
 * @return (void);
 */
void	find_files(t_ast *orig, t_ast *temp_copy, t_minishell *minishell)
{
	while (temp_copy && orig)
	{
		if (temp_copy->token->type == PIPE)
			minishell->first = 0;
		if (temp_copy->left)
		{
			find_files(orig->left, temp_copy->left, minishell);
		}
		if (temp_copy->token->type == PIPE)
			minishell->first = 0;
		if (temp_copy->token->type <= 3)
		{
			find_commands(orig, temp_copy, minishell->first);
			if (temp_copy->right->token->type <= 3)
				orig->token->path
					= ft_strdup(temp_copy->right->left->token->str);
			else
				orig->token->path = ft_strdup(temp_copy->right->token->str);
			minishell->first = 1;
		}
		temp_copy = temp_copy->right;
		orig = orig->right;
	}
}
