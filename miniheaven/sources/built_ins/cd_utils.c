/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:36:31 by mistery576        #+#    #+#             */
/*   Updated: 2024/10/29 16:29:44 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Rebuilds the split_pwd, and returns the new string
 * @param char **split_pwd.
 * @return (char *new_pwd);
 */
char	*rebuild_pwd(char **split_pwd)
{
	char	*new_pwd;
	int		i;

	i = 0;
	new_pwd = ft_strjoin("", "/");
	while (split_pwd[i])
	{
		new_pwd = ft_strjoin_gnl(new_pwd, split_pwd[i]);
		if (split_pwd[i + 1])
			new_pwd = ft_strjoin_gnl(new_pwd, "/");
		if (!new_pwd)
			return (NULL);
		i++;
	}
	return (new_pwd);
}

/**
 * @brief It will change the OLDPWD, with the new_path
 * no minishell->envp.
 * @param t_minishell *minishell, char *new_path.
 * @return (int);
 */
int	change_old_path(t_minishell *minishell, char *new_path)
{
	char	*new_old_path;
	int		i;

	i = 0;
	new_old_path = NULL;
	while (minishell->envp[i])
	{
		if (strncmp(minishell->envp[i], "OLDPWD", 6) == 0
			&& minishell->envp[i][6] == '=')
		{
			new_old_path = ft_strjoin("OLDPWD=", new_path);
			free(minishell->envp[i]);
			minishell->envp[i] = new_old_path;
			return (0);
		}
		i++;
	}
	return (-1);
}

/**
 * @brief It will change the PWD with the new_path
 * @param t_minishell *minishell, char *new_path.
 * @return (int);
 */

int	change_pwd(t_minishell *minishell, char *new_path)
{
	char	*new_pwd;
	int		i;

	i = 0;
	new_pwd = NULL;
	while (minishell->envp[i])
	{
		if (strncmp(minishell->envp[i], "PWD", 3) == 0
			&& minishell->envp[i][3] == '=')
		{
			new_pwd = ft_strjoin("PWD=", new_path);
			free(minishell->envp[i]);
			minishell->envp[i] = new_pwd;
			return (0);
		}
		i++;
	}
	return (-1);
}
