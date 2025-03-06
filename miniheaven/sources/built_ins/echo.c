/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miafonso <miafonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:39:13 by mistery576        #+#    #+#             */
/*   Updated: 2024/09/26 17:08:39 by miafonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Reads str to the end, until is equal to c.
 * @param char *str, char c.
 * @return (int);
 */
int	find_char(char *str, char c)
{
	int	i;
	int	flag;

	i = 0;
	if (!str || !str[i])
		return (0);
	while (str[i] && str[i] == c)
	{
		flag = 1;
		i++;
	}
	if (!str[i] && flag)
		return (1);
	return (0);
}

/**
 * @brief Skips the flags of the echo command.
 * @param char *str, int *flag.
 * @return (int);
 */
static int	skip(char **cmd, int *flag)
{
	int	i;

	i = 1;
	if (ft_strlen(cmd[i]) >= 2)
	{
		while (ft_strlen(cmd[i]) > 0
			&& cmd[i][0] == '-'
			&& find_char(cmd[i] + 1, 'n') == 1)
		{
			*flag = 1;
			i++;
		}
	}
	if (*flag == 1)
		return (i);
	return (i);
}

/**
 * @brief Prints the string passed as argument.
 * @param char *str
 * @return (void);
 */
void	ft_echo(char **cmd)
{
	int	flag;
	int	i;

	flag = 0;
	if (cmd[1])
	{
		i = skip(cmd, &flag);
		while (cmd[i])
		{
			printf("%s", cmd[i]);
			i++;
			if (cmd[i])
				printf(" ");
		}
	}
	if (flag == 0)
		printf("\n");
}
