/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:36:05 by mistery576        #+#    #+#             */
/*   Updated: 2024/10/29 17:05:46 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Copy the content from envp, to envp_dup 
 * @param minishell, char **envp, char **envp_dup, int envp_counter;
 * @return (void);
 */
static void	copy_content(t_minishell *minishell, char **envp,
		char **envp_dup, int envp_counter)
{
	int	i;
	int	x;

	x = 0;
	i = 0;
	while (envp[i] && i < envp_counter)
	{
		envp_dup[i] = ft_strdup(envp[i]);
		i++;
	}
	while (minishell->local[x])
	{
		envp_dup[i] = ft_strdup(minishell->local[x]);
		i++;
		x++;
	}
	envp_dup[i] = NULL;
}

/**
 * @brief Used to dup the envp, and returns the duped_env
 * @param minishell, char **envp;
 * @return (char **);
 */
char	**dup_envp(t_minishell *minishell, char **envp)
{
	char	**envp_dup;
	int		envp_counter;
	int		i;

	i = 0;
	envp_counter = 0;
	while (envp[envp_counter])
		envp_counter++;
	while (minishell->local[i])
	{
		envp_counter++;
		i++;
	}
	envp_dup = malloc(sizeof (char *) * (envp_counter + 1));
	i = 0;
	if (!envp_dup)
	{
		printf("Failed to allocate the env_dup\n");
		free_array(envp_dup);
		return (NULL);
	}
	copy_content(minishell, envp, envp_dup, envp_counter);
	return (envp_dup);
}

/**
 * @brief Util function
 * @param char *full_path, char **split_cmd, char **dir
 * @return (char **full_path);
 */
static char	*find_path_util(char *full_path, char **split_cmd, char **split_dir)
{
	int	i;

	i = 0;
	while (split_cmd && split_dir[i] != NULL)
	{
		full_path = ft_strjoin(split_dir[i], "/");
		full_path = ft_strjoin_gnl(full_path, split_cmd[0]);
		if (access(full_path, X_OK) == 0)
		{
			free_array(split_dir);
			free_array(split_cmd);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(split_dir);
	free_array(split_cmd);
	return (NULL);
}

/**
 * @brief Finds the split_cmd path
 * @param char *cmd.
 * @return (char **full_path);
 */
char	*find_path(t_minishell *minishell, char *cmd)
{
	char	**split_env;
	char	**split_cmd;
	char	*full_path;
	char	*path_envp;

	if (!cmd[0] || ft_strncmp(cmd, ".", 1) == 0 || ft_strncmp(cmd, " ", 1) == 0)
		return (NULL);
	path_envp = my_getenv(minishell, "PATH");
	if (path_envp == NULL)
		exit(0);
	split_env = ft_split(path_envp, ':');
	split_cmd = ft_split(cmd, ' ');
	full_path = NULL;
	if (cmd && access(cmd, X_OK) == 0)
	{
		return (cmd);
	}
	else
		full_path = find_path_util(full_path, split_cmd, split_env);
	return (full_path);
}

/**
 * @brief Checks ir the original is bigger or lower then the step_ahead
 * @param char *original, char *step_ahead.
 * @return (int);
 */
int	bigger_var_name(char *original, char *step_ahead)
{
	int	i;

	i = 0;
	while ((original[i] || step_ahead[i])
		&& original[i] != '='
		&& step_ahead[i] != '=')
	{
		if (original[i] < step_ahead[i])
			return (0);
		if (original[i] > step_ahead[i])
			return (1);
		i++;
	}
	return (0);
}
