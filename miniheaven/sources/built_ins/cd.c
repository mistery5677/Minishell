/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:36:34 by mistery576        #+#    #+#             */
/*   Updated: 2024/10/29 16:28:56 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief "I will split my PWD by '/', remove the last 
 * directory, and then reconstruct the PATH and return it"
 * @param t_minishell *minishell.
 * @return (char *);
 */
static char	*go_back(t_minishell *minishell)
{
	char	**split_path;
	char	*new_pwd;
	int		i;

	i = 0;
	if (ft_strcmp(my_getenv(minishell, "PWD"), "/") == 0)
		return (NULL);
	split_path = ft_split(my_getenv(minishell, "PWD"), '/');
	if (!split_path)
		return (NULL);
	while (split_path[i + 1])
		i++;
	free(split_path[i]);
	split_path[i] = NULL;
	i = 0;
	new_pwd = rebuild_pwd(split_path);
	if (!new_pwd)
		return (NULL);
	free_array(split_path);
	return (new_pwd);
}

/**
 * @brief Will enter in the new_path folder
 * @param t_minishell *minishell, char *new_path.
 * @return (char *full_path);
 */
static char	*go_to_path(t_minishell *minishell, char *new_path)
{
	char	*full_path;
	char	*pwd;

	pwd = my_getenv(minishell, "PWD");
	full_path = NULL;
	if (new_path[0] == '/')
		full_path = ft_strjoin("", new_path);
	else
	{
		full_path = ft_strjoin("", pwd);
		if (ft_strcmp("/", pwd) != 0)
			full_path = ft_strjoin_gnl(full_path, "/");
		full_path = ft_strjoin_gnl(full_path, new_path);
	}
	if (full_path[ft_strlen(full_path) - 1] == '/')
		full_path[ft_strlen(full_path) - 1] = '\0';
	return (full_path);
}

void	cd_expand(t_minishell *minishell, char **split_cmd, char **new_pwd)
{
	if (ft_strcmp(split_cmd[1], "..") == 0)
		*new_pwd = go_back(minishell);
	else if (ft_strcmp(split_cmd[1], ".") == 0)
		*new_pwd = ft_strdup(my_getenv(minishell, "PWD"));
	else if (ft_strcmp(split_cmd[1], "-") == 0)
		*new_pwd = go_to_path(minishell, my_getenv(minishell, "OLDPWD"));
	else if (ft_strlen(split_cmd[1]) > 1 && split_cmd[1][0] == '~')
	{
		if (my_getenv(minishell, "HOME") == NULL)
			return (ft_putstr_fd("bash: cd: HOME not set\n", 2));
		*new_pwd = ft_strjoin(my_getenv(minishell, "HOME"), split_cmd[1] + 1);
	}
}

/**
 * @brief I will enter in the new_path folder, and 
 * change the value of the OLDPWD and PWD
 * @param char **split_cmd, t_minishell *minishell.
 * @return (void);
 */
void	ft_cd(char **split_cmd, t_minishell *minishell)
{
	char	*new_pwd;
	char	*temp_pwd;

	temp_pwd = my_getenv(minishell, "PWD");
	new_pwd = NULL;
	if (!split_cmd[1])
		new_pwd = my_getenv(minishell, "HOME");
	else if (count_array(split_cmd) > 1)
		cd_expand(minishell, split_cmd, &new_pwd);
	if (new_pwd == NULL)
		new_pwd = go_to_path(minishell, split_cmd[1]);
	if (chdir(new_pwd) == 0)
	{
		change_old_path(minishell, temp_pwd);
		change_pwd(minishell, new_pwd);
	}
	else
	{
		g_signal = 1;
		print_errors("bash: cd: ", split_cmd[1],
			": No such file or directory\n");
	}
	if (split_cmd[1])
		free(new_pwd);
}
