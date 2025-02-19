/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:14:08 by mfrancis          #+#    #+#             */
/*   Updated: 2024/11/14 17:59:33 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/minishell.h"

/**
 * @brief Expands variables and counts heredoc tokens in the token list.
 * Two main tasks:
 * 1. Counts the number of heredoc tokens (`<<`) 
 * 2. Expands environment variables in tokens of type `WORD`
 * 
 * At the end frees the memory allocated for the `expander`.
 * 
 * @param minishell Pointer to the `t_minishell` structure.
 * 
 * @return Void. Modifies the token list.
 */
void	expander(t_minishell *minishell)
{
	t_token	*temp;

	temp = minishell->tokens;
	while (temp)
	{
		if (temp->type == REDIR_HEREDOC)
		{
			minishell->nbr_heredoc++;
			if (temp->next)
				temp = temp->next;
		}
		else if (temp->type == WORD)
		{
			expand_vars(minishell, temp);
		}
		temp = temp->next;
	}
	free(minishell->expander);
	minishell->expander = NULL;
}

/**
 * @brief Appends a character to the end of a given string.
 * 
 * If `str` is `NULL`, it initializes the string to an empty string before
 *  appending the character.
 * The original string is freed, and the new string is returned.
 * 
 * @param c The character to append to the string.
 * @param str The input string to which the character will be appended.
 * 
 * @return A pointer to the newly allocated string or `NULL`.
 */
char	*add_char(int c, char *str)
{
	char	*new_msg;
	size_t	len;

	if (!str)
		str = ft_strdup("");
	len = ft_strlen(str);
	new_msg = malloc(sizeof(char) * (len + 2));
	if (!new_msg)
	{
		if (str)
			free(str);
		return (NULL);
	}
	ft_strlcpy(new_msg, str, len + 1);
	new_msg[len] = (char)c;
	new_msg[len + 1] = '\0';
	free(str);
	return (new_msg);
}

/**
 * @brief Resets the fields of a `t_expand` structure to their default values.
 * 
 * @param expander Pointer to the `t_expand` structure to be reset.
 * 
 * @return Void. Modifies the `expander` structure.
 */
void	reset_expand(t_expand *expander)
{
	expander->var_content = NULL;
	expander->var_name = NULL;
	expander->dq = 0;
	expander->sq = 0;
}
/**
 * @brief Extracts the content of an environment variable based on its name.
 * 
 * This function retrieves the value of an environment variable specified
 * by the `var_name` field. If the variable name is `"?"`, it converts the 
 * `exit_status` field of `minishell` into a string.
 * If the variable exists in the environment, its value is duplicated 
 * and returned. 
 * If the variable name is `NULL` or the variable does not exist, 
 * the function returns `NULL`.
 * 
 * @param minishell Pointer to the `t_minishell` structure
 * 
 * @return A pointer to a newly allocated string or `NULL`.
 */

char	*extrat_var_content(t_minishell *minishell)
{
	char	*str;

	str = NULL;
	if (minishell->expander->var_name == NULL)
		return (NULL);
	if (ft_strcmp(minishell->expander->var_name, "?") == 0)
	{
		str = ft_itoa(minishell->exit_status);
		return (str);
	}
	else if (my_getenv(minishell, minishell->expander->var_name) != NULL)
		str = ft_strdup(my_getenv(minishell, minishell->expander->var_name));
	return (str);
}
