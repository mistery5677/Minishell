/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miafonso <miafonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:41:54 by mistery576        #+#    #+#             */
/*   Updated: 2024/10/30 11:53:32 by miafonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Returns the var name before the '='
 * @param char *path
 * @return (char *);
 */
static char	*remove_equal(char	*path)
{
	int	i;

	i = 0;
	while (path[i] && path[i] != '=')
		i++;
	return (ft_substr(path, 0, i));
}
/**
 * @brief Copy the getenv() function
 * @param t_minishell *minishell, char *path
 * @return (char *before'=');
 */

char	*my_getenv(t_minishell *minishell, char *path)
{
	char	*get_value;
	int		i;
	size_t	len;

	i = 0;
	get_value = remove_equal(path);
	while (minishell->envp[i])
	{
		len = ft_strlen(get_value);
		if (ft_strncmp(minishell->envp[i], get_value, len) == 0
			&& minishell->envp[i][len] == '=')
		{
			free(get_value);
			return (minishell->envp[i] + (len) + 1);
		}
		i++;
	}
	free(get_value);
	return (NULL);
}
