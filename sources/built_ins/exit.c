/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:59:21 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/27 12:59:21 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Checks if the first char is '+' or '-'
 * @param char *str;
 * @return (int);
 */

static int	check_number(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			print_errors("bash: exit: ", str, ": numeric argument required\n");
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Transforms the str into exit code
 * @param char *str;
 * @return (int);)
 */
static int	get_exit_code(char *str)
{
	int	code;

	code = ft_atoi(str);
	if (check_number(str) == 1)
		return (2);
	return (code);
}

/**
 * @brief Exit built_in
 * @param t_minishell *minishell, char **dp
 * @return (void);
 */
void	ft_exit(t_minishell *minishell, char **dp)
{
	int	count;

	count = count_array(dp);
	if (count > 2)
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		g_signal = 2;
		if (check_number(dp[1]) == 0)
			return ;
	}
	else if (dp[1])
		g_signal = get_exit_code(dp[1]);
	ft_putendl_fd("exit", 1);
	free_array(dp);
	free_exit(minishell, "");
}
