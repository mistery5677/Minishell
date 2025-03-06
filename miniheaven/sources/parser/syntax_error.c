/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by mfrancis          #+#    #+#             */
/*   Updated: 2024/10/22 12:09:44 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks the syntax of the token list to ensure it is valid.
 * 
 * This function iterates through the list of tokens and validates the 
 * syntax based on the type of tokens. It checks for syntax errors related 
 * to pipes (`|`) and redirection operators (`<`, `>`, etc.).
 * 
 * @param tokens Pointer to the first token in the token list.
 * 
 * @return `true` if the syntax is valid, `false` if there is a syntax error.
 */
bool	check_syntax(t_token *tokens)
{
	t_token		*temp;
	t_token		*last_token;
	bool		valid;

	temp = tokens;
	last_token = NULL;
	while (temp)
	{
		valid = true;
		if (temp->type == PIPE)
			valid = check_syntax_pipe(temp, last_token);
		else if (temp->type == REDIR_IN
			|| temp->type == REDIR_HEREDOC
			|| temp->type == REDIR_OUT
			|| temp->type == REDIR_APPEND)
			valid = check_syntax_redir(temp);
		if (!valid)
			return (false);
		last_token = temp;
		temp = temp->next;
	}
	return (true);
}

/**
 * @brief Validates the syntax related to a pipe token (`|`).
 * 
 * A pipe token is only valid if it is preceded by a `WORD` token 
 * and followed by another non-pipe token.
 * If the pipe is incorrectly placed, an error message is printed 
 * and the function returns `false`.
 * 
 * @param temp Pointer to the current token (the pipe token `|`).
 * @param last_token Pointer to the previous token in the token list.
 * 
 * @return `true` if the pipe token is valid, `false` if there 
 * is a syntax error related to the pipe.
 */
bool	check_syntax_pipe(t_token *temp, t_token *last_token)
{
	if (!last_token || last_token->type != WORD)
	{
		ft_putendl_fd("Syntax error near unexpected token `|'", 2);
		return (false);
	}
	if (!temp->next || temp->next->type == PIPE)
	{
		ft_putendl_fd("Syntax error near unexpected token `|'", 2);
		return (false);
	}
	return (true);
}

/**
 * @brief Validates the syntax related to a redirection token.
 * 
 * This function checks that a redirection token is followed by a `WORD` token. 
 * A message is printed and the function returns `false`.
 * 
 * @param temp Pointer to the current redirection token.
 * 
 * @return `true` if the redirection token is followed a word token, or
 * `false`.
 */
bool	check_syntax_redir(t_token *temp)
{
	if (!temp->next || temp->next->type != WORD)
	{
		ft_putendl_fd("Syntax error near unexpected token", 2);
		return (false);
	}
	return (true);
}
