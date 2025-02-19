/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 00:16:15 by mistery576        #+#    #+#             */
/*   Updated: 2024/10/29 16:19:56 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Compares var and env before the '='
 * @param char *var, char *env
 * @return (int);
 */
static int	compare_vars(char *var, char *env)
{
	int	i;

	i = 0;
	while (var[i] && env[i] && env[i] != '=')
	{
		if (var[i] != env[i])
			return (var[i] - env[i]);
		i++;
	}
	if (env[i] == '=' && !var[i])
		return (0);
	return (-1);
}

/**
 * @brief Clears the local var from the start position
 * @param t_minishell *minishell, int start
 * @return (void);
 */
static void	clear_var(t_minishell *minishell, int start)
{
	free(minishell->envp[start]);
	minishell->envp[start] = NULL;
	while (minishell->envp[start + 1])
	{
		minishell->envp[start] = minishell->envp[start + 1];
		start++;
	}
	minishell->envp[start] = NULL;
}

/**
 * @brief Checks if the local exists in the local array
 * @param char *env, char *var
 * @return (int);
 */
static int	check_local(char *env, char *var)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	if (ft_strncmp(var, env, i - 1) == 0)
		return (1);
	return (0);
}

/**
 * @brief Unsets the vars passed as arguments
 * @param char **cmd, t_minishell *minishell
 * @return (void);
 */
void	ft_unset(char **cmd, t_minishell *minishell)
{
	int	i;
	int	x;

	i = 1;
	while (cmd[i])
	{
		x = 0;
		while (minishell->envp[x])
		{
			if (check_local(minishell->envp[x], cmd[i]) == 1)
				clear_local(minishell, cmd[i], ft_strlen(cmd[i]));
			if (compare_vars(cmd[i], minishell->envp[x]) == 0)
				clear_var(minishell, x);
			x++;
		}
		i++;
	}
}
