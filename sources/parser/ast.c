/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:53:44 by mfrancis          #+#    #+#             */
/*   Updated: 2024/10/29 20:25:08 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/minishell.h"

/**
 * @brief Creates an Abstract Syntax Tree (AST) from a list of tokens.
 * 
 * This function builds an AST based on the given tokens. Depending on the
 * type of token encountered, it recursively splits tokens into subtrees.
 * 
 * @param tokens Pointer to the list of tokens to be parsed into an AST.
 * 
 * @return Pointer to the root node of the generated AST, or NULL.
 */

t_ast	*create_ast(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	if (check_symbol(tokens, PIPE) == 1)
		return (split_tokens_ast(&tokens, PIPE));
	else if (check_redir(tokens) != NO_TYPE)
		return (split_tokens_ast(&tokens, check_redir(tokens)));
	else
		return (add_right(&tokens));
	return (NULL);
}

/**
 * @brief Checks for the presence of a redirection operator in the tokens list.
 *
 * @param tokens Pointer to the list of tokens to be checked.
 * 
 * @return The type of the first redirection operator found, or `NO_TYPE`
 */
t_tk_tp	check_redir(t_token *tokens)
{
	t_token	*temp;

	temp = tokens;
	while (temp)
	{
		if (temp->type < WORD)
			return (temp->type);
		temp = temp->next;
	}
	return (NO_TYPE);
}

/**
 * @brief Checks if a specific symbol is present in the token list.
 * 
 * @param tokens Pointer to the list of tokens to be checked.
 * @param symbol The type of symbol to look for.
 * 
 * @return 1 if the symbol is found, 0 otherwise.
 */
int	check_symbol(t_token *tokens, t_tk_tp symbol)
{
	t_token	*temp;

	temp = tokens;
	while (temp)
	{
		if (temp->type == symbol)
			return (1);
		temp = temp->next;
	}
	return (0);
}

/**
 * @brief Creates an AST node for the current token and processes the rest
 * as its right subtree.
 * Recursively constructs the right subtree with the remaining tokens.
 * 
 * @param tokens Pointer to the token list to be parsed.
 * 
 * @return Pointer to the new AST node, or NULL on failure.
 */
t_ast	*add_right(t_token **tokens)
{
	t_token	*temp;
	t_token	*next;
	t_ast	*ast;

	temp = *tokens;
	ast = new_ast_node(temp);
	if (temp != NULL)
		next = temp->next;
	else
		next = NULL;
	ast->right = create_ast(next);
	return (ast);
}

/**
 * @brief Allocates and initializes a new AST node for the given token.
 *	Iitializes its left and right children as NULL.
 * 
 * @param tokens Pointer to the token to be stored in the new AST node.
 * 
 * @return Pointer to the newly created AST node, or NULL.
 */
t_ast	*new_ast_node(t_token *tokens)
{
	t_ast	*new_node;

	if (!tokens)
		return (NULL);
	new_node = malloc(sizeof (t_ast));
	if (!new_node)
		return (NULL);
	new_node->token = tokens;
	new_node->right = NULL;
	new_node->left = NULL;
	return (new_node);
}
