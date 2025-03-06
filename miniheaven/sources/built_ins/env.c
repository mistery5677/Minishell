/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:39:26 by mistery576        #+#    #+#             */
/*   Updated: 2024/10/29 16:32:33 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Prints all envps
 * @param t_minishell *minishell
 * @return (void);
*/
static void	print_env(t_minishell *minishell)
{
	int	i;

	i = 0;
	while (minishell->envp[i])
	{
		printf("%s\n", minishell->envp[i]);
		i++;
	}
}

/**
 * @brief Executes the env command
 * @param t_minishell *minishell, char **cmd
 * @return (void);
 */
void	ft_env(t_minishell *minishell, char **cmd)
{
	if (count_array(cmd) > 1)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		g_signal = 127;
		return ;
	}
	if (my_getenv(minishell, "PATH") == NULL)
	{
		print_errors("bash: env: No such file or directory\n", NULL, NULL);
		g_signal = 127;
		return ;
	}
	print_env(minishell);
}
