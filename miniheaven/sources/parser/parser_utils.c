/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:50:57 by mfrancis          #+#    #+#             */
/*   Updated: 2024/11/14 18:10:04 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/minishell.h"

/**
 * @brief Checks if a character is a delimiter:space (' '), tab ('\t'),
 *  or newline ('\n').
 * 
 * @param c The character to check.
 * @return `1` if the character is a delimiter, `0` otherwise.
 */
int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/**
 * @brief Counts the number of words in a string separated by delimiters.
 * 
 * This function iterates through the string, skipping delimiters and 
 * counting sequences of non-delimiter characters as words. 
 * @param s The input string to count words from.
 * @return The number of words in the string.
 */
int	ft_count_words(char const *s)
{
	int	counter;
	int	idx;

	counter = 0;
	idx = 0;
	if (!s)
		return (counter);
	while (s[idx] && is_delimiter(s[idx]))
		idx++;
	while (s[idx])
	{
		if (!is_delimiter(s[idx]))
			counter++;
		while (s[idx] && !is_delimiter(s[idx]))
			idx++;
		while (s[idx] && is_delimiter(s[idx]))
			idx++;
	}
	return (counter);
}
