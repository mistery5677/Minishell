/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:59:26 by mfrancis          #+#    #+#             */
/*   Updated: 2024/10/29 16:17:42 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief If a symbol exists the function call another function to separete
 * the symbol from the previouse word if exists. Update the idx;
 * @param minishel->display
 * @param int *start
 * @param int *idx
 * @return void;
 */
void	process_op_tokens(t_minishell *minishell, int *start, int *idx)
{
	if (minishell->display[*idx] == '>' && minishell->display[*idx + 1] == '>')
	{
		add_operator_word(minishell, start, *idx, REDIR_APPEND);
		(*idx)++;
	}
	else if (minishell->display[*idx] == '<'
		&& minishell->display[*idx + 1] == '<')
	{
		add_operator_word(minishell, start, *idx, REDIR_HEREDOC);
		(*idx)++;
	}
	else
	{
		add_word(minishell, start, idx);
		add_simple_token(minishell, start, idx);
	}
}

/**
 * @brief Auxiliary function to add the previouse word (if exits) with the
 * type WORD, and the symbol of operation with the respective type;
 * Update de idx;
 * @return void;
 */
void	add_operator_word(t_minishell *minishell, int *start, int idx,
		int token_type)
{
	int	range;

	range = idx - *start;
	if (range > 0)
		add_token_back(minishell, ft_substr(minishell->display, *start, range),
			WORD);
	add_token_back(minishell, ft_substr(minishell->display, idx, 2),
		token_type);
	*start = idx + 2;
}

/**
 * @brief Auxiliary function to add the previouse word (if exits) with the
 * type WORD;
 * Update de idx;
 * @return void;
 */
void	add_word(t_minishell *minishell, int *start, int *idx)
{
	int	range;

	range = *idx - *start;
	if (range > 0)
		add_token_back(minishell, ft_substr(minishell->display, *start, range),
			WORD);
	*start = *idx + 1;
}

/**
 * @brief Auxiliary function to add the symbol with the designeted type; 
 * Update de idx;
 * @return void;
 */
void	add_simple_token(t_minishell *minishell, int *start, int *idx)
{
	if (minishell->display[*idx] == '|')
		add_token_back(minishell, ft_substr(minishell->display, *idx, 1), PIPE);
	else if (minishell->display[*idx] == '>')
		add_token_back(minishell, ft_substr(minishell->display, *idx, 1),
			REDIR_OUT);
	else if (minishell->display[*idx] == '<')
		add_token_back(minishell, ft_substr(minishell->display, *idx, 1),
			REDIR_IN);
	*start = *idx + 1;
}

/**
 * @brief Auxiliary function that allocates memory for a new token and 
 * add that token to end of the list of tokens;
 * This token it will be initialized only with the string and type of token;
 * @param minishel->display
 * @param int *start
 * @param int *idx
 * @return void;
 */
void	add_token_back(t_minishell *minishell, char *token, t_tk_tp type)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		free_exit(minishell, "Error - Fail allocating token");
	new_token->str = token;
	new_token->type = type;
	new_token->path = NULL;
	new_token->next = NULL;
	new_token->cmd = NULL;
	new_token->dq = 0;
	if (!minishell->tokens)
	{
		minishell->tokens = new_token;
		return ;
	}
	else
	{
		temp = minishell->tokens;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_token;
	}
}
