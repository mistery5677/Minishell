/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miafonso <miafonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:00:44 by mfrancis          #+#    #+#             */
/*   Updated: 2024/10/29 20:06:34 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief  Process imput from display and add that input to tokens;
 * Considers if the first token is a symbol, word, and the whitespaces.
 * Auxliary functions: process_op_tokens, add_op_word, add_word
 * After the loop ends, if there is any remaining unprocessed segment 
 * (from start to the end of the string), it is added as a token
 * using add_token_back.
 * @param minishel->display;
 * @return (void);
 */
void	tokenization(t_minishell *minishell)
{
	int	start;
	int	idx;
	int	len;

	start = 0;
	idx = -1;
	len = ft_strlen(minishell->display);
	while (minishell->display[++idx])
	{
		if (minishell->display[idx] == '"' || minishell->display[idx] == '\'')
		{
			if (check_quotes(minishell->display[idx], minishell, &idx) == -1)
				return ;
		}
		else if (minishell->display[idx] == '>'
			|| minishell->display[idx] == '<' || minishell->display[idx] == '|')
			process_op_tokens(minishell, &start, &idx);
		else if (minishell->display[idx] == 32 || (minishell->display[idx] >= 9
				&& minishell->display[idx] <= 13))
			add_word(minishell, &start, &idx);
	}
	if (start < len)
		add_token_back(minishell, ft_substr(minishell->display, start, len
				- start), WORD);
}

/**
 * @brief If a quote exists check if they are closed. If the quotes arent closed 
 * a message is printed in stderror and the g_signal change to 2;
 * Update the idx if the quotes are close;
 * @param char quote
 * @param minishel->display
 * @param int *idx
 * @return 0 or -1 if que quotes arent closed;
 */
int	check_quotes(char quote, t_minishell *minishell, int *idx)
{
	if (quote == '"')
	{
		if (process_quotes(minishell, idx, '"') == -1)
		{
			ft_putstr_fd("Error - Only one \" \n", 2);
			g_signal = 2;
			return (-1);
		}
	}
	else if (quote == '\'')
	{
		if (process_quotes(minishell, idx, '\'') == -1)
		{
			ft_putstr_fd("Error - Only one ' \n", 2);
			g_signal = 2;
			return (-1);
		}
	}
	return (0);
}

/**
 * @brief auxiliary function to update the idx if the quotes are closed.
 * @param char quote
 * @param minishel->display
 * @param int *idx
 * @return 0 or -1 if que quotes arent closed;
 */
int	process_quotes(t_minishell *minishell, int *idx, char symb)
{
	int	flag;

	flag = 0;
	(*idx)++;
	while (minishell->display[*idx] && flag == 0)
	{
		if (minishell->display[*idx] == symb)
			flag = 1;
		(*idx)++;
	}
	if (flag == 0)
		return (-1);
	while (minishell->display[*idx] && (minishell->display[*idx] <= 9
			&& minishell->display[*idx] >= 13
			&& minishell->display[*idx] != 32))
	{
		(*idx)++;
	}
	(*idx)--;
	return (*idx);
}
