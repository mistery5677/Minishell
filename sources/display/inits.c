/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:56:42 by mfrancis          #+#    #+#             */
/*   Updated: 2025/01/05 20:59:56 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief It takes the SHLVL value, and increment it by 1
 * @param t_minishell *minishell, char **envp, int start;
 * @return (void);
 */
static void	update_lvl(t_minishell *minishell, char *envp, int start)
{
	char	*new_lvl;
	int		lvl;

	minishell->envp[start] = NULL;
	lvl = ft_atoi(envp + 6) + 1;
	new_lvl = ft_itoa(lvl);
	minishell->envp[start] = ft_strjoin("SHLVL=", new_lvl);
	free(new_lvl);
}

/**
 * @brief  It whill create the envp array
 * @param t_minishell *minishell, char *envp[], int envp_counter
 * @return (int);
 */
static int	create_env(t_minishell *minishell, char *envp[], int envp_counter)
{
	int	i;

	i = 0;
	while (i < envp_counter)
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			update_lvl(minishell, envp[i], i);
		else
			minishell->envp[i] = ft_strdup(envp[i]);
		if (!minishell->envp[i])
			free_exit(minishell, "Error - Memory envp fail\n");
		i++;
	}
	if (envp_counter < 3)
	{
		i = 3;
	}
	return (i);
}

/**
 * @brief It takes the new_string and adds it to array
 * @param char **array, char *new_string
 * @return (char **);
 */
static char	**add_string_to_array(char **array, char *new_string)
{
	char	**new_array;
	int		i;

	i = 0;
	while (array && array[i] != NULL)
		i++;
	new_array = ft_realloc(array, i * sizeof(char *), (i + 2) * sizeof(char *));
	if (!new_array)
	{
		ft_putendl_fd("Failed to allocate memory", 2);
		return (NULL);
	}
	new_array[i] = strdup(new_string);
	if (!new_array[i])
	{
		ft_putendl_fd("Failed to duplicate string", 2);
		return (NULL);
	}
	new_array[i + 1] = NULL;
	return (new_array);
}

/**
 * @brief Sets the mandatory env variables (OLDPWD, PWD, SHLVL)
 * @param t_minishell *minishell;
 * @return (char **);
 */
static void	mandatory_env(t_minishell *minishell)
{
	char	*pwd;
	char	*shlvl;
	char	*oldpwd;

	if (my_getenv(minishell, "PWD") == NULL)
	{
		pwd = ft_strjoin("PWD=", getcwd(NULL, 0));
		minishell->envp = add_string_to_array(minishell->envp, pwd);
		free(pwd);
	}
	if (my_getenv(minishell, "OLDPWD") == NULL)
	{
		oldpwd = ft_strdup("OLDPWD");
		minishell->envp = add_string_to_array(minishell->envp, oldpwd);
		free(oldpwd);
	}
	if (my_getenv(minishell, "SHLVL") == NULL)
	{
		shlvl = ft_strdup("SHLVL=1");
		minishell->envp = add_string_to_array(minishell->envp, shlvl);
		free(shlvl);
	}
}

/**
 * @brief Set all the values in the minishell structure
 * @param char *envp[], t_minishell *minishell
 * @return (void);
 */
void	initialization(char *envp[], t_minishell *minishell)
{
	int	envp_counter;
	int	i;

	envp_counter = 0;
	i = 0;
	ft_bzero(minishell, sizeof(t_minishell));
	minishell->local = malloc(sizeof(char *) * 1);
	if (!minishell->local)
		free_exit(minishell, "Error - Memory allocation fail\n");
	minishell->local[0] = NULL;
	minishell->_str_ = 0;
	minishell->_pipe_ = 0;
	minishell->temp_stdin = -1;
	minishell->temp_stdout = -1;
	minishell->infile = -1;
	minishell->outfile = -1;
	minishell->tokens = NULL;
	while (envp[envp_counter])
		envp_counter++;
	minishell->envp = malloc(sizeof(char *) * (envp_counter + 1));
	if (!minishell->envp)
		free_exit(minishell, "Error - Memory allocation fail\n");
	i = create_env(minishell, envp, envp_counter);
	mandatory_env(minishell);
	minishell->envp[i] = NULL;
}
