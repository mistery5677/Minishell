/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 18:23:35 by mfrancis          #+#    #+#             */
/*   Updated: 2024/09/19 18:26:38 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *str, const char *sub_str)
{
	int	i;
	int	j;

	if (!*sub_str)
		return ((char *)sub_str);
	i = 0;
	while (str[i])
	{
		if (str[i] == sub_str[0])
		{
			j = 0;
			while (sub_str[j] && str[i + j] == sub_str[j])
				j++;
			if (!sub_str[j])
				return ((char *)&str[i]);
		}
		i++;
	}
	return (NULL);
}
