/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:39:29 by mistery576        #+#    #+#             */
/*   Updated: 2024/10/29 16:31:36 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Sorts envp, and returns it
 * @param t_minishell *minishell, char **envp
 * @return (char **);
 */
static char	**sort_envp(t_minishell *minishell, char **envp)
{
	char	**copy_envp;
	char	*temp;
	int		i;
	int		x;

	copy_envp = dup_envp(minishell, envp);
	if (!copy_envp)
		return (NULL);
	i = 0;
	while (copy_envp[i])
	{
		x = i + 1;
		while (copy_envp[x])
		{
			if (bigger_var_name(copy_envp[i], copy_envp[x]) > 0)
			{
				temp = copy_envp[i];
				copy_envp[i] = copy_envp[x];
				copy_envp[x] = temp;
			}
			x++;
		}
		i++;
	}
	return (copy_envp);
}

/**
 * @brief Prints envp, by order
 * @param t_minishell *minishell, char **envp
 * @return (void);
 */
static void	print_export(t_minishell *minishell, char **envp)
{
	char	**copy_envp;
	int		i;
	int		x;

	i = 0;
	copy_envp = sort_envp(minishell, envp);
	while (copy_envp[i])
	{
		printf("declare -x ");
		x = 0;
		while (copy_envp[i][x] && copy_envp[i][x] != '=')
		{
			printf("%c", copy_envp[i][x]);
			x++;
		}
		if (copy_envp[i][x] == '=')
			printf("=%c%s%c", '"', my_getenv(minishell, copy_envp[i]), '"');
		printf("\n");
		i++;
	}
	free_array(copy_envp);
}

/**
 * @brief Adds var to envp
 * @param t_minishell *minishell, char *var
 * @return (void);
 */
static void	add_var(t_minishell *minishell, char *var)
{
	int		i;
	char	**copy_envp;

	i = 0;
	while (minishell->envp[i])
		i++;
	copy_envp = malloc((i + 2) * sizeof(char *));
	if (!copy_envp)
		return ;
	i = 0;
	while (minishell->envp[i])
	{
		copy_envp[i] = ft_strdup(minishell->envp[i]);
		i++;
	}
	copy_envp[i] = ft_strdup(var);
	i++;
	copy_envp[i] = NULL;
	free_array(minishell->envp);
	minishell->envp = copy_envp;
}

/**
 * @brief Checks if the value already exists in envp
 * @param char *var, t_minishell *minishell
 * @return (int);
 */
static int	check_var(char *var, t_minishell *minishell)
{
	int	i;
	int	x;

	i = 0;
	if (ft_isalpha(var[i]) == 0)
	{
		g_signal = 1;
		print_errors("bash: export: \'", var, "\': not a valid identifier\n");
		return (-1);
	}
	while (minishell->envp[++i])
	{
		x = 0;
		while (var[x] && var[x] != '=')
			x++;
		if (ft_strncmp(minishell->envp[i], var, x) == 0)
		{
			free(minishell->envp[i]);
			minishell->envp[i] = ft_strdup(var);
			return (1);
		}
	}
	clear_local(minishell, var, x);
	return (0);
}

/**
 * @brief Exports the var to envp or "local"
 * @param t_minishell *minishell, char **cmd
 * @return (void);
 */
void	ft_export(t_minishell *minishell, char **cmd)
{
	int	i;

	i = 1;
	if (!cmd[i])
		print_export(minishell, minishell->envp);
	else
	{
		while (cmd[i])
		{
			if (find_equal(cmd[i]) == -1
				&& check_local_env(minishell, cmd[i]) == 0
				&& ft_isalpha(cmd[i][0]) == 1)
				add_local(minishell, cmd[i]);
			else if (check_var(cmd[i], minishell) == 0
				&& find_equal(cmd[i]) == 0)
				add_var(minishell, cmd[i]);
			i++;
		}
	}
}
