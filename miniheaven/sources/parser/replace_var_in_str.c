/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var_in_str.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 22:10:16 by mistery576        #+#    #+#             */
/*   Updated: 2024/12/07 22:20:00 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Initializes variables and finds the position of a variable in the str.
 * 
 * @param minishell Pointer to the `t_minishell` 
 * @param var_pos Pointer to a character pointer to store the position of 
 * the variable
 * @param idx Pointer to the current index
 * @param str The original string being processed.
 */

static void	ft_initialize(t_minishell *minishell,
	char	**var_pos, int *idx, char *str)
{
	char	*temp;

	*var_pos = NULL;
	if (!minishell->expander->var_name)
		*var_pos = &str[(*idx)];
	else
	{
		temp = ft_strjoin("$", minishell->expander->var_name);
		*var_pos = (ft_strnstr(str + *idx, temp, ft_strlen(str))) + 1;
		free(temp);
	}
}
/**
 * @brief Replaces the placeholder of an environment variable in a string 
 * with its value.
 * 
 * This function locates a variable in the input string starting with `$`,
 * extracts its position, and creates a new string with the variable replaced by 
 * its corresponding value. It calculates the lengths of substrings before and
 * after the variable and combines them with the variable's content into a 
 * new allocated string. 
 * Updates the index.
 * 
 * @param str The original string containing the variable to be replaced.
 * @param minishell Pointer to the `t_minishell`.
 * @param idx Pointer to the current index in the string.
 * @return A new string with the variable replaced by its value.
 * Returns `NULL` if memory allocation fails.
 */

char	*replace_var_in_str(char *str, t_minishell *minishell, int *idx)
{
	char	*new_str;
	char	*var_pos;
	size_t	before_var;
	size_t	after_var;
	size_t	var_content_len;

	ft_initialize(minishell, &var_pos, idx, str);
	before_var = var_pos - str - 1;
	if (!minishell->expander->var_content)
		var_content_len = 0;
	else
		var_content_len = ft_strlen(minishell->expander->var_content);
	after_var = ft_strlen(var_pos + ft_strlen(minishell->expander->var_name));
	new_str = malloc(before_var + var_content_len + after_var + 1);
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, str, before_var);
	ft_memcpy(new_str + before_var, minishell->expander->var_content,
		var_content_len);
	ft_memcpy(new_str + before_var + var_content_len,
		var_pos + ft_strlen(minishell->expander->var_name), after_var);
	new_str[before_var + var_content_len + after_var] = '\0';
	(*idx) = before_var + var_content_len - 1;
	return (new_str);
}
