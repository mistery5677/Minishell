/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 01:31:58 by mistery576        #+#    #+#             */
/*   Updated: 2024/12/08 01:41:09 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Helper function to locate a specific symbol in the token list.
 * 
 * This function iterates through the token list, stopping at the first 
 * occurrence of the specified symbol or at the end of the list. It 
 * updates the provided pointers.
 * 
 * @param last Pointer to the last token before the specified symbol.
 * @param temp Pointer to the current token being processed.
 * @param symbol The token type to search for in the list.
 */
static void	split_utils(t_token	**last, t_token	**temp, t_tk_tp symbol)
{
	(void)last;
	while (*temp)
	{
		if ((*temp)->type == symbol)
			break ;
		*last = *temp;
		*temp = (*temp)->next;
	}
}
/**
 * @brief Splits tokens at a specified symbol and builds an AST node.
 * It creates a new AST node for the symbol 
 * and recursively builds the left and right subtrees from the tokens
 * before and after the symbol.
 * 
 * @param tokens Pointer to the list of tokens to be split.
 * @param symbol The token type at which to split the list.
 * 
 * @return Pointer to the root of the newly created AST subtree.
 */

t_ast	*split_tokens_ast(t_token **tokens, t_tk_tp symbol)
{
	t_token	*temp;
	t_token	*last;
	t_token	*next;
	t_ast	*ast;

	temp = *tokens;
	last = NULL;
	split_utils(&last, &temp, symbol);
	ast = new_ast_node(temp);
	if (last)
		last->next = NULL;
	if (temp != NULL)
		next = temp->next;
	else
		next = NULL;
	if (temp->next)
		temp->next = NULL;
	if (temp == *tokens)
		ast->left = create_ast(NULL);
	else
		ast->left = create_ast(*tokens);
	ast->right = create_ast(next);
	return (ast);
}
