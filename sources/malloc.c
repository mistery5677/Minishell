/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:31:31 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/16 14:31:31 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include <stdlib.h>

extern void	*__real_malloc(size_t size);

void	*__wrap_malloc(size_t size)
{
	srand(time(NULL));
	if (rand() % 2)
		return (NULL);
	return (__real_malloc(size));
}
