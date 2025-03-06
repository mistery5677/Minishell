/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_files_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:29:31 by mistery576        #+#    #+#             */
/*   Updated: 2024/12/18 11:51:23 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Copies the array of commands
 * @param t_ast *ast, int count
 * @return (char **);
 */
static char	**copy_array(t_ast *ast, int count)
{
	char	**cmd_array;
	int		i;

	cmd_array = malloc((count + 1) * sizeof(char *));
	if (!cmd_array)
		return (NULL);
	i = 0;
	while (ast && (ast->token->type == WORD || ast->token->type == STR))
	{
		if (!copy_token_to_array(ast, cmd_array, &i))
			return (NULL);
		ast = ast->right;
	}
	cmd_array[i] = NULL;
	return (cmd_array);
}

/**
 * @brief Builds the command array
 * @param t_ast *ast
 * @return (char **);
 */
char	**built_cmd(t_ast *ast)
{
	t_ast	*temp;
	char	**cmd_array;
	int		count;
	char	**split_cmd;

	count = 0;
	temp = ast;
	while (temp && (temp->token->type == WORD || temp->token->type == STR))
	{
		if (temp->token->expander == 1)
		{
			split_cmd = ft_split(temp->token->str, ' ');
			count += count_array(split_cmd);
			free_array(split_cmd);
		}
		else
			count++;
		temp = temp->right;
	}
	cmd_array = copy_array(ast, count);
	return (cmd_array);
}

/**
 * @brief Set the redirections, to the symbols
 * @param t_minishell *minishell, t_ast *ast
 * @return (void);
 */
void	set_redirs(t_minishell *minishell, t_ast *ast)
{
	t_ast	*copy;
	t_ast	*orig;
	t_ast	*temp_copy;

	copy = copy_ast(ast);
	if (!copy)
		free_exit(minishell, "Failed to create a copy\n");
	orig = ast;
	temp_copy = copy;
	find_files(orig, temp_copy, minishell);
	free_ast(copy);
}

/**
 * @brief Utils for the no pipe case in set_redirs
 * @param t_ast *orig, t_ast *temp_copy
 * @return (void);
 */
void	no_pipe_util(t_ast *orig, t_ast *temp_copy)
{
	if (orig->token->cmd)
	{
		temp_copy = temp_copy->right->right;
		while (temp_copy)
		{
			orig->token->cmd
				= join_array(orig->token->cmd, temp_copy->token->str);
			temp_copy = temp_copy->right;
		}
	}
	else
	{
		if (orig->right->right->token->dq == 1
			&& ft_count_words(temp_copy->right->right->token->str) > 1)
		{
			orig->token->dq = 1;
		}
		orig->token->cmd = built_cmd(temp_copy->right->right);
	}
}

/**
 * @brief Set the information for the last redirection
 * @param _ast *temp_copy, t_ast *orig, t_ast *save_node
 * @return (void);
 */

void	complete_last_redir(t_ast *temp_copy, t_ast *orig, t_ast *save_node)
{
	if (temp_copy->left && temp_copy->left->right)
	{
		if (orig->left->right->token->dq == 1)
			orig->left->right->token->dq = 1;
		orig->token->cmd = built_cmd(save_node->left->right);
	}
	if (orig->token->cmd && temp_copy->right->right)
	{
		temp_copy = temp_copy->right->right;
		if (temp_copy->token->dq == 1)
			orig->token->dq = 1;
		while (temp_copy)
		{
			orig->token->cmd
				= join_array(orig->token->cmd, temp_copy->token->str);
			temp_copy = temp_copy->right;
		}
	}
	else
	{
		if (temp_copy->right->right->token->dq == 1)
			orig->token->dq = 1;
		orig->token->cmd = built_cmd(temp_copy->right->right);
	}
}
