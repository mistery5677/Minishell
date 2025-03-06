/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:00:18 by mfrancis          #+#    #+#             */
/*   Updated: 2024/11/12 17:08:08 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/minishell.h"

/**
 * @brief Removes single quotes from a token's string.
 * Updates the token and adjusts the index to account for the removal.
 * 
 * @param token The token whose string will be modified.
 * @param minishell Pointer to the `t_minishell`.
 * @param idx Pointer to the current index in the token string.
 * 
 * @note This function allocates memory for a new string and frees the old.
 * If memory allocation fails, the program exits with an error message.
 */
void	rm_single_quotes(t_token *token, t_minishell *minishell, int *idx)
{
	char	*new_str;
	int		i;
	int		j;
	int		pair;

	i = *idx + 1;
	while (token->str[i] && token->str[i] != '\'')
		i++;
	pair = i;
	new_str = malloc(sizeof(char) * (ft_strlen(token->str) - 1));
	if (!new_str)
		free_exit(minishell, "Error - Fail allocating memory for new str\n");
	i = -1;
	j = -1;
	while (token->str[++i])
	{
		if (i != *idx && i != pair)
			new_str[++j] = token->str[i];
	}
	new_str[++j] = '\0';
	free(token->str);
	token->str = new_str;
	*idx = pair - 2;
}

/**
 * @brief Removes a double quote from a token's string.
 * Updates the token and adjusts the index to account for the removal.
 * 
 * @param token The token whose string will be modified.
 * @param minishell Pointer to the `t_minishell`
 * @param idx Pointer to the current index in the token string.
 * 
 * @note This function allocates memory for a new string and frees the old.
 * If memory allocation fails, the program exits with an error message.
 */
void	rm_double_quote(t_token *token, t_minishell *minishell, int *idx)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(token->str)));
	if (!new_str)
		free_exit(minishell, "Error - Fail allocating memory for new str\n");
	while (token->str[i])
	{
		if (i != *idx)
		{
			new_str[j] = token->str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	free(token->str);
	token->str = new_str;
	*(idx) = *(idx) - 1;
}

/**
 * @brief Removes a dollar sign from a token's string.
 * Updates the token and adjusts the index to account for the removal.
 * 
 * @param token The token whose string will be modified.
 * @param minishell Pointer to the `t_minishell`
 * @param idx Pointer to the current index in the token string.
 * 
 * @note This function allocates memory for a new string and frees the old.
 * If memory allocation fails, the program exits with an error message.
 */
void	rm_dollar(t_token *token, t_minishell *minishell, int *idx)
{
	int		i;
	char	*new_str;
	int		j;

	(void)minishell;
	i = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(token->str)));
	if (!new_str)
		free_exit(minishell, "Error - Fail allocating memory for new str\n");
	while (i < *idx)
	{
		new_str[i] = token->str[i];
		i++;
	}
	j = i;
	i++;
	while (token->str[i])
	{
		new_str[j] = token->str[i];
		j++;
		i++;
	}
	new_str[j] = '\0';
	free(token->str);
	token->str = new_str;
}
