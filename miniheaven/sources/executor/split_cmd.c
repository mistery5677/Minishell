/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:23:41 by mistery576        #+#    #+#             */
/*   Updated: 2024/12/27 01:36:07 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Utils for process_tokens
 * @param char **result, int *result_size, int new_size
 * @return char **
 */
static char	**allocate_result_space(char **result,
	int *result_size, int new_size)
{
	result = ft_realloc(result, (*result_size) * sizeof(char *),
			new_size * sizeof(char *));
	if (!result)
		return (NULL);
	*result_size = new_size;
	return (result);
}

/**
 * @brief Utils for ft_split_cmd
 * @param char **tokens, char ***result, int *result_count, int *result_size
 * @return (void)
 */
static void	process_tokens(char **tokens, char ***result,
			int *result_count, int *result_size) {
	int	j;
	int	new_size;

	new_size = 0;
	j = 0;
	while (tokens[j] != NULL)
	{
		if (*result_count >= *result_size)
		{
			if (*result_size == 0)
				new_size = 4;
			else
				new_size = (*result_size) * 2;
			*result = allocate_result_space(*result, result_size, new_size);
		}
		(*result)[*result_count] = tokens[j];
		(*result_count)++;
		j++;
	}
	free(tokens);
}

/**
 * @brief Utils for ft_split_cmd
 * @param char ***result, int *result_count, int *result_size
 * @return (void)
 */
static void	finalize_result(char ***result, int *result_count, int *result_size)
{
	if (*result_count >= *result_size)
		*result = allocate_result_space(*result, result_size,
				(*result_count) + 1);
	(*result)[*result_count] = NULL;
}

/**
 * @brief Split the command into tokens, $a 
 * @param char **cmd, int cmd_count
 * @return char **
 */
char	**ft_split_cmd(char **cmd, int cmd_count)
{
	char	**result;
	char	**tokens;
	int		result_count;
	int		result_size;
	int		i;

	tokens = NULL;
	result = NULL;
	result_count = 0;
	result_size = 0;
	i = 0;
	while (i < cmd_count)
	{
		tokens = ft_split(cmd[i], ' ');
		if (tokens)
			process_tokens(tokens, &result, &result_count, &result_size);
		i++;
	}
	finalize_result(&result, &result_count, &result_size);
	free_array(cmd);
	return (result);
}
